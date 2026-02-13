# Code::Blocks IDE Development Guide

Code::Blocks is a cross-platform C/C++ IDE built with wxWidgets. This guide helps Copilot sessions work effectively in this codebase.

## Build Commands

### Unix/Linux
```bash
# First-time setup (from SVN)
./bootstrap

# Configure with all contrib plugins
./configure --with-contrib-plugins=all

# Build
make

# Install (requires root)
sudo make install
```

### Configure Options
```bash
# Custom installation prefix
./configure --prefix=/usr --with-contrib-plugins=all

# Exclude specific plugins (example: exclude help plugin)
./configure --with-contrib-plugins=all,-help

# Specify boost library directory (if NassiShneiderman plugin fails)
./configure --with-boost-libdir=/usr/lib64 --with-contrib-plugins=all
```

### Windows
1. Open `src/CodeBlocks.cbp` in Code::Blocks and build (core + plugins)
2. Open `src/ContribPlugins.workspace` and build (contrib plugins)
3. Run `src/update.bat` to set up `devel` and `output` directories

### Development Workflow
- `devel/` - Development build output (incremental changes)
- `output/` - Production build (run `make update` or `update.bat` to sync)
- Work in `output/CodeBlocks.exe` while developing, test incremental builds in `devel/`

### Testing
- Script tests are in `src/scripts/tests/`
- Code completion tests: `src/plugins/codecompletion/cctest`
- No centralized test runner; tests are plugin-specific

## Architecture

### Manager Pattern
Code::Blocks uses a centralized manager system. All managers are singletons accessed via `Manager::Get()`:

- **ProjectManager** - Workspace and project management
- **EditorManager** - Editor windows and file management
- **PluginManager** - Plugin loading/unloading lifecycle
- **ConfigManager** - Configuration persistence
- **LogManager** - Logging infrastructure
- **MacrosManager** - Variable expansion system
- **DebuggerManager** - Debugger interface coordination
- **ScriptingManager** - Squirrel script integration
- **FileManager** - Async file operations
- **ToolsManager** - External tools integration
- **UserVariableManager** - User-defined variables
- **CCManager** - Code completion coordination

Access pattern: `Manager::Get()->GetProjectManager()` returns the ProjectManager instance.

### Plugin System
Code::Blocks is heavily plugin-based. SDK version is defined in `cbplugin.h`:
```cpp
#define PLUGIN_SDK_VERSION_MAJOR   2
#define PLUGIN_SDK_VERSION_MINOR   25
#define PLUGIN_SDK_VERSION_RELEASE 0
```

Plugin types:
- **Core plugins** (`src/plugins/`) - Essential IDE functionality
- **Contrib plugins** (`src/plugins/contrib/`) - Optional features

All plugins inherit from `cbPlugin` and must provide a `manifest.xml` file in their resource zip.

### Project Structure
```
src/
├── sdk/              # SDK implementation (libcodeblocks.so/.dll)
├── include/          # SDK public headers
├── src/              # Main IDE application
├── plugins/          # Core plugins
│   ├── codecompletion/
│   ├── compilergcc/
│   ├── debuggergdb/
│   └── ...
├── plugins/contrib/  # Contributed plugins
├── scripts/          # Squirrel scripts
└── tools/            # Utilities (cbp2make, etc)
```

### Key Components
- **cbProject** - Represents a Code::Blocks project (.cbp file)
- **cbWorkspace** - Container for multiple projects (.workspace file)
- **cbEditor** - Editor window wrapping wxStyledTextCtrl
- **Compiler** - Compiler abstraction (GCC, MSVC, Borland, etc.)
- **ProjectBuildTarget** - Individual build target within a project

## Conventions

### File Organization
- Header files: `src/include/*.h` (public SDK) or alongside `.cpp` files (private)
- All SDK files use LGPL v3 license header
- Precompiled headers: `sdk_precomp.h` for SDK, `sdk.h` for convenience includes

### Scripting
- Code::Blocks uses Squirrel for scripting extensions
- Script bindings in `src/include/scripting/bindings/`
- Scripts can access managers via registered bindings

### Build System
- **Autotools** for Unix/Linux (`configure.ac`, `Makefile.am`)
- **Code::Blocks projects** for Windows (`.cbp`, `.workspace` files)
- Multiple workspace files for different wxWidgets versions (wx30, wx31, wx32)

### wxWidgets Dependency
- Code::Blocks requires wxWidgets built as a **monolithic DLL**
- Different workspaces for wx 3.0, 3.1, and 3.2 (including 64-bit variants)
- Uses wxStyledTextCtrl (Scintilla wrapper) for editing

### Naming Conventions
- Classes: CamelCase with `cb` prefix for core SDK classes (`cbProject`, `cbEditor`)
- Managers: `*Manager` suffix
- Plugins: Inherit from `cbPlugin`, prefix varies by type
- Members: `m_` prefix for member variables (common in older code)

### Plugin Manifest
Every plugin needs a `manifest.xml` with metadata (name, version, author, description, etc.). The manifest is packaged in a zip file alongside the plugin.

## Available Contrib Plugins
AutoVersioning, BrowseTracker, byogames, Cccc, CppCheck, cb_koders, clangd_client, codesnippets, codestat, copystrings, Cscope, DoxyBlocks, dragscroll, EditorConfig, EditorTweaks, envvars, FileManager, headerfixup, help, hexeditor, incsearch, keybinder, libfinder, MouseSap, NassiShneiderman, ProjectOptionsManipulator, profiler, regex, ReopenEditor, rndgen, exporter, symtab, ThreadSearch, ToolsPlus, Valgrind, wxsmith, wxsmithcontrib, wxsmithaui

## Compilers Supported
- GNU GCC (primary)
- Microsoft Visual C++ Free Toolkit
- Borland C++ Compiler 5.5
- Auto-detection on first launch (configurable in Settings)

## Platform-Specific Notes

### Windows
- DLL export/import macros: `PLUGIN_EXPORT`, `DLLIMPORT`
- Batch scripts for building: `batch_build_all.bat`, `batch_build_core.bat`, etc.

### Unix/Linux
- Uses standard GNU toolchain
- `.la` files are libtool artifacts
- Install paths follow FHS (filesystem hierarchy standard)

### macOS
- Bundle creation via `bundle.sh`
- Special handling in `Makefile.am` for `DARWIN` platform
- Icon files in ICNS format

# Code::Blocks 25.03 - Custom Build for WSLg

This is a customized build of Code::Blocks 25.03 with enhancements for WSLg and improved debugging features.

## Table of Contents

- [Building from Source](#building-from-source)
- [Quick Rebuild](#quick-rebuild)
- [Custom Features](#custom-features)
- [Documentation](#documentation)
- [Git Repository](#git-repository)

## Building from Source

### Prerequisites

Before building Code::Blocks, ensure you have the required dependencies installed:

**On Ubuntu/Debian (WSL2/WSLg):**
```bash
# Install build tools
sudo apt-get update
sudo apt-get install -y build-essential automake autoconf libtool pkg-config

# Install wxWidgets 3.2 development libraries
sudo apt-get install -y libwxgtk3.2-gtk3-dev

# Install additional dependencies
sudo apt-get install -y libgtk-3-dev libglib2.0-dev zip
```

**On Fedora/RHEL:**
```bash
# Install build tools
sudo dnf groupinstall "Development Tools"
sudo dnf install automake autoconf libtool pkg-config

# Install wxWidgets development libraries
sudo dnf install wxGTK-devel gtk3-devel

# Additional dependencies
sudo dnf install glib2-devel zip
```

### Step-by-Step Build Process

#### 1. Clone or Download the Repository

```bash
# Clone from GitHub
git clone https://github.com/bshewan/codeblocks-wslg-custom.git
cd codeblocks-wslg-custom

# Or if you already have it locally
cd /path/to/codeblocks_25.03
```

#### 2. Bootstrap the Build System

The bootstrap script generates the configure script and other autotools files:

```bash
# Make bootstrap script executable (if needed)
chmod +x bootstrap update_revision.sh

# Run bootstrap
./bootstrap
```

**Expected output:**
```
Running aclocal...
Running autoheader...
Running automake...
Running autoconf...
Bootstrap complete.
```

**Note:** The `egrep is obsolete` warnings can be safely ignored - they don't affect the build.

#### 3. Configure the Build

Run the configure script to check dependencies and prepare the build:

```bash
./configure --prefix=/usr/local
```

**Common configure options:**
```bash
# Minimal build (just the IDE, no contrib plugins)
./configure --prefix=/usr/local --with-contrib-plugins=no

# Custom installation prefix
./configure --prefix=$HOME/.local

# Enable debug symbols
./configure --prefix=/usr/local CXXFLAGS="-g -O0"
```

**Expected output (final lines):**
```
checking for wxWidgets version >= 3.0.0... yes (version 3.2.x)
configure: creating ./config.status
config.status: creating Makefile
...
Configuration complete. You may now run 'make'.
```

#### 4. Build Code::Blocks

Compile the source code using make:

```bash
# Full build (uses all CPU cores)
make -j$(nproc)

# Or build with specific number of threads
make -j4

# Single-threaded build (better for debugging build errors)
make
```

**Build time:** Approximately 5-15 minutes depending on your system.

**Expected output (final lines):**
```
make[2]: Leaving directory '.../src/src'
make[1]: Leaving directory '.../src/src'
Making all in plugins
...
Build complete.
```

#### 5. Install Code::Blocks (Optional)

Install to the system (requires sudo):

```bash
sudo make install
```

**Or run from build directory without installing:**
```bash
# Run directly from source tree
./src/src/.libs/codeblocks

# Or use the wrapper script
./src/src/codeblocks
```

#### 6. Verify Installation

```bash
# Check version
codeblocks --version

# Or if running from build directory
./src/src/.libs/codeblocks --version
```

**Expected output:**
```
Code::Blocks 25.03
```

### Build Troubleshooting

**Problem: "configure: error: wxWidgets not found"**
```bash
# Install wxWidgets development package
sudo apt-get install libwxgtk3.2-gtk3-dev  # Ubuntu/Debian
sudo dnf install wxGTK-devel              # Fedora/RHEL
```

**Problem: "aclocal: command not found" or "egrep is obsolete"**
```bash
# Install autotools
sudo apt-get install automake autoconf  # Ubuntu/Debian
sudo dnf install automake autoconf      # Fedora/RHEL

# Make scripts executable
chmod +x bootstrap update_revision.sh

# The "egrep is obsolete" warnings can be ignored - they don't affect the build
```

**Problem: Build fails with "undefined reference"**
```bash
# Clean and rebuild
make clean
./configure --prefix=/usr/local
make -j$(nproc)
```

**Problem: Precompiled headers error**
```bash
# Disable precompiled headers
./configure --prefix=/usr/local --disable-pch
make -j$(nproc)
```

## Quick Rebuild

After making changes to the source code:

```bash
# Rebuild just the main application
make -C src/src

# Or use the provided script
./rebuild_debug_windows.sh

# View custom changes
./show_custom_changes.sh

# Check git status
git status
git log --oneline --name-only
```

## Custom Features

### 1. **Debug Windows Dock by Default** (WSLg Fix)
Windows no longer float - they dock automatically in sensible positions.
- See: `DEBUG_WINDOWS_LAYOUT_GUIDE.md`

### 2. **Memory Change Highlighting**
Red text shows which memory values changed during debugging.
- See: `MEMORY_CHANGE_HIGHLIGHTING.md`
- Config: `~/.config/codeblocks/default.conf`
- Word size: 4 bytes default (configurable: 1, 2, 4, or 8)

### 3. **AI Assistant Instructions**
GitHub Copilot instructions for working with this codebase.
- See: `.github/copilot-instructions.md`

## Documentation

| File | Description |
|------|-------------|
| `CUSTOM_CHANGES.md` | Summary of all modifications |
| `MEMORY_CHANGE_HIGHLIGHTING.md` | Memory highlighting feature guide |
| `DEBUG_WINDOWS_LAYOUT_GUIDE.md` | Debug window docking info |
| `BUILD_TEST_RESULTS.md` | Build verification |
| `SAVE_LAYOUT_INSTRUCTIONS.md` | How to save/load layouts |

## Modified Source Files

- `src/src/debugger_interface_creator.cpp` - Debug window docking (lines 55, 77, 99, 121, 142, 164, 185)
- `src/src/resources/memdump.xrc` - Rich text for memory window (line 70: added wxTE_RICH2)
- `src/src/examinememorydlg.h` - Change tracking declarations (added tracking members)
- `src/src/examinememorydlg.cpp` - Highlighting implementation (4-byte word alignment logic)

## Build Artifacts Location

After building, the binaries are located at:
```
src/src/.libs/codeblocks          # Main executable
src/src/.libs/libcodeblocks.so    # Core library
```

**Note:** The `.libs` directory is created by libtool during the build process.

## Git Repository

```bash
# View commits
git log --oneline

# View changes to a specific file
git log -p src/src/examinememorydlg.cpp

# Create a branch for new work
git checkout -b feature/my-feature

# Save your work
git add <files>
git commit -m "Description of changes"
```

## Backup/Distribution

To create a distributable archive:

```bash
# Create tarball (excludes build artifacts via .gitignore patterns)
git archive --format=tar.gz --output=codeblocks-25.03-custom.tar.gz HEAD

# Or include all files
tar czf codeblocks-25.03-custom-full.tar.gz \
  --exclude='.git' \
  --exclude='*.o' \
  --exclude='.libs' \
  .
```

## Reverting Changes

If you need to revert custom changes:

```bash
# Revert specific file to original
git checkout 4c4f7e5 -- src/src/examinememorydlg.cpp

# View original file without reverting
git show 4c4f7e5:src/src/examinememorydlg.cpp
```

## Environment

- **Platform:** 64-bit Linux with WSLg
- **wxWidgets:** 3.2
- **Build system:** Autotools (./configure && make)
- **IDE:** Code::Blocks itself

## Questions?

See the detailed documentation files listed above.

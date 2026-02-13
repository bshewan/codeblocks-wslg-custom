# Code::Blocks 25.03 - Custom Build for WSLg

This is a customized build of Code::Blocks 25.03 with enhancements for WSLg and improved debugging features.

## Quick Start

```bash
# Rebuild after changes
make -C src/src
# or
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

- `src/src/debugger_interface_creator.cpp` - Debug window docking
- `src/src/resources/memdump.xrc` - Rich text for memory window
- `src/src/examinememorydlg.h` - Change tracking declarations
- `src/src/examinememorydlg.cpp` - Highlighting implementation

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

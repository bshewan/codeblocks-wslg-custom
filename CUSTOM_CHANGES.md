# Custom Changes to Code::Blocks 25.03

This document tracks the custom modifications made to Code::Blocks for WSLg compatibility and enhanced debugging features.

## Changes Made

### 1. Debug Window Docking (WSLg Fix)
**Files:** `src/src/debugger_interface_creator.cpp`

Changed debug windows from floating (`dsFloating`) to docked by default:
- Watches, Call Stack, Breakpoints, Threads → `dsBottom`
- CPU Registers, Disassembly, Memory → `dsRight`

**Reason:** Manual docking via drag-and-drop doesn't work in WSLg environment.

**Documentation:** `DEBUG_WINDOWS_LAYOUT_GUIDE.md`

### 2. Memory Change Highlighting
**Files:** 
- `src/src/resources/memdump.xrc` - Added `wxTE_RICH2` style
- `src/src/examinememorydlg.h` - Added change tracking members
- `src/src/examinememorydlg.cpp` - Implemented highlighting logic

**Features:**
- Red text highlighting for changed memory values
- 4-byte word alignment (default) - perfect for int, float
- Configurable word sizes (1, 2, 4, or 8 bytes)
- Automatic reset when debugger continues
- Per-address byte tracking

**Documentation:** `MEMORY_CHANGE_HIGHLIGHTING.md`

### 3. Documentation & Build Tools

Created:
- `.github/copilot-instructions.md` - AI assistant instructions
- `BUILD_TEST_RESULTS.md` - Build verification
- `SAVE_LAYOUT_INSTRUCTIONS.md` - Layout save/load guide
- `rebuild_debug_windows.sh` - Quick rebuild script

## Configuration

All changes respect existing Code::Blocks configuration system:
- Layout perspectives stored in `~/.config/codeblocks/default.conf`
- Memory highlight word size: `/common/examine_memory/highlight_word_size`

## Build Instructions

```bash
# Quick rebuild after changes
make -C src/src

# Or use provided script
./rebuild_debug_windows.sh
```

## Testing

See `BUILD_TEST_RESULTS.md` and `MEMORY_CHANGE_HIGHLIGHTING.md` for test scenarios.

## Compatibility

- Tested on: 64-bit Linux with WSLg
- Code::Blocks version: 25.03
- No changes to plugin API or SDK version
- Backward compatible with existing configurations

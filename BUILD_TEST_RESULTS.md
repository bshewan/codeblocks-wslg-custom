# Debug Windows Docking - Build and Test Results

## Build Status: ✅ SUCCESS

**Build Date**: February 12, 2026 at 18:24 UTC
**Binary Location**: `src/src/.libs/codeblocks`
**Binary Size**: 2.8 MB

## Changes Applied

Modified `src/src/debugger_interface_creator.cpp` to change default docking behavior:

### Before (all floating):
```cpp
evt.dockSide = CodeBlocksDockEvent::dsFloating;
```

### After (docked):
```cpp
// Bottom pane (with build messages)
evt.dockSide = CodeBlocksDockEvent::dsBottom;  // Watches, Call Stack, Breakpoints, Threads

// Right pane
evt.dockSide = CodeBlocksDockEvent::dsRight;   // CPU Registers, Disassembly, Memory
```

## Verified Changes

✅ Source code modified correctly (7 windows updated)
✅ Object file recompiled (debugger_interface_creator.o)
✅ Binary linked successfully
✅ Application starts and loads plugins
✅ Config backup created (~/.config/codeblocks/default.conf.backup)

## Testing Instructions

### Test in WSLg

1. **Run Code::Blocks** from the source directory:
   ```bash
   cd /home/bshewan/codeblocks_25.03/codeblocks_25.03/src/src
   ./.libs/codeblocks
   ```

2. **Open or create a C/C++ project** with a debuggable executable

3. **Start debugging** (Debug → Start/Continue or F8)

4. **Verify debug windows**:
   - Open View → Debugger windows menu
   - Enable: Watches, Call stack, Breakpoints
   - **Expected**: These windows should appear DOCKED at the bottom, not floating
   - **Expected**: CPU Registers, Disassembly, Memory (if enabled) should dock on the right

5. **Save your layout** if you like it:
   - View → Perspectives → Save current
   - Name it "WSLg Debug Layout" or similar

### Compare with Backup

If you want to compare with the old behavior:

1. Restore old config:
   ```bash
   mv ~/.config/codeblocks/default.conf.backup ~/.config/codeblocks/default.conf
   ```

2. Run Code::Blocks - windows will float (old behavior)

3. Switch back to new config:
   ```bash
   rm ~/.config/codeblocks/default.conf
   ```

## Installation (Optional)

To install system-wide:
```bash
cd /home/bshewan/codeblocks_25.03/codeblocks_25.03
sudo make install
```

This will install to `/usr/local/bin/codeblocks` by default.

## Customizing Dock Positions

If you want to change where specific windows dock, edit the values in:
`src/src/debugger_interface_creator.cpp`

Available positions:
- `dsLeft` - Dock on left side
- `dsRight` - Dock on right side
- `dsTop` - Dock at top
- `dsBottom` - Dock at bottom
- `dsFloating` - Float (original behavior)

Then rebuild:
```bash
cd /home/bshewan/codeblocks_25.03/codeblocks_25.03
./rebuild_debug_windows.sh
```

## Troubleshooting

### Windows still float
- Make sure you deleted or backed up `~/.config/codeblocks/default.conf`
- The config file overrides the default docking positions

### Can't see debug windows
- They only appear when debugging is active
- Or manually enable via: View → Debugger windows

### Docking doesn't work in WSLg
- This is expected - the changes make windows PRE-DOCKED
- You won't be able to drag-and-drop to dock in WSLg
- Use View → Perspectives to save different layouts

## Next Steps

1. Test the changes by running a debug session
2. If you like the layout, save it as a custom perspective
3. Optionally install system-wide with `sudo make install`
4. Consider creating multiple perspectives for different workflows

## Reverting Changes

To revert to original floating behavior:
```bash
cd /home/bshewan/codeblocks_25.03/codeblocks_25.03/src/src
git checkout debugger_interface_creator.cpp
make
```

Or manually edit the file and change all `dsBottom`/`dsRight` back to `dsFloating`.

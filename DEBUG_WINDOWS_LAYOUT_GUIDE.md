# Code::Blocks Debug Windows Layout Guide for WSLg

This guide explains how to work with debug window layouts in Code::Blocks, especially when running via WSLg where docking doesn't work properly.

## Changes Made to Source Code

The debug windows were originally hardcoded to float by default. I've modified `src/src/debugger_interface_creator.cpp` to dock them by default instead:

### Default Dock Positions

- **Watches** - Bottom (with Build Messages, Logs)
- **Call Stack (Backtrace)** - Bottom
- **Breakpoints** - Bottom
- **Running Threads** - Bottom
- **CPU Registers** - Right
- **Disassembly** - Right
- **Memory** - Right

This arrangement provides:
- Primary debug info (watches, call stack, breakpoints, threads) at the bottom
- Advanced debug info (registers, disassembly, memory) on the right side
- Editor area remains unobstructed in the center/left

## Rebuilding After Changes

After modifying the source, rebuild the application:

```bash
cd /home/bshewan/codeblocks_25.03/codeblocks_25.03

# If you've already built once, just rebuild the main app
make -C src/src

# Or rebuild everything
make

# Install (if desired)
sudo make install
```

## Using Custom Layouts (Alternative to Code Changes)

Even with the code changes, you can create and save custom layouts for different debugging scenarios.

### Creating a Custom Layout

1. **Arrange Windows** - Start Code::Blocks and arrange all windows (debug windows, toolbars, panels) as you want them
   
2. **Save the Layout**:
   - Menu: `View` → `Perspectives` → `Save current`
   - Enter a name like "Debug Layout" or "WSLg Debug"
   
3. **Switch Between Layouts**:
   - Menu: `View` → `Perspectives` → [Select your layout name]

### Layout Storage

Layouts are stored in Code::Blocks configuration:
- **Config Location**: `~/.config/codeblocks/default.conf` (or similar)
- **Config Section**: `/main_frame/layout/`

Each layout stores:
- Window positions and sizes (wxAUI perspective string)
- Docking relationships
- Tab orders for message pane

### Predefined Layouts

Code::Blocks comes with two hardcoded layouts:
- **default** - Standard IDE layout
- **&Minimal** - Minimal layout with fewer windows

### Exporting/Importing Layouts

To share a layout across machines or backup:

1. **Find the config file**:
   ```bash
   # Usually at:
   ~/.config/codeblocks/default.conf
   # or
   ~/.codeblocks/default.conf
   ```

2. **Extract layout section**:
   ```bash
   # View layouts in config
   grep -A50 "main_frame/layout" ~/.config/codeblocks/default.conf
   ```

3. **Copy to another machine** - Copy the entire `[main_frame/layout]` section to the target machine's config file

## Creating a Debugging Perspective

Here's a suggested workflow for creating a good debug layout:

1. **Start debugging** to make all debug windows visible
2. **Arrange windows**:
   - Keep editor in the center/left
   - Dock Watches, Call Stack, and Breakpoints at bottom (tabbed together)
   - Dock CPU Registers and Disassembly on right side (if you use them)
   - Hide windows you rarely use (they'll still be in View menu)
3. **Save as "Debug Layout"**
4. **Create another layout** for normal coding (without debug windows)
5. **Switch** between them via View → Perspectives

## Reverting to Original Floating Behavior

If you want to revert the changes:

```bash
cd /home/bshewan/codeblocks_25.03/codeblocks_25.03/src/src
git checkout debugger_interface_creator.cpp
# Or manually change all dsBottom/dsRight back to dsFloating
```

## Layout System Architecture

For developers wanting to understand or extend the layout system:

- **Layout Manager**: `wxAuiManager` (`m_LayoutManager` in `MainFrame`)
- **Save/Load**: `SaveViewLayout()` / `LoadViewLayout()` in `src/src/main.cpp`
- **Storage**: Perspectives are stored as wxAUI perspective strings
- **Debug Windows**: Created by `DebugInterfaceFactory` in `src/src/debugger_interface_creator.cpp`
- **Dock Events**: `CodeBlocksDockEvent` with `cbEVT_ADD_DOCK_WINDOW`

### Dock Side Options

From `src/include/sdk_events.h`:
```cpp
enum DockSide
{
    dsLeft = 0,
    dsRight,
    dsTop,
    dsBottom,
    dsFloating,
    dsUndefined
};
```

## Troubleshooting

**Issue**: Debug windows still float after rebuild
- **Solution**: Delete saved layout config to force using new defaults:
  ```bash
  # Backup first
  cp ~/.config/codeblocks/default.conf ~/.config/codeblocks/default.conf.backup
  
  # Remove layout section or delete entire config
  rm ~/.config/codeblocks/default.conf
  ```

**Issue**: Can't dock windows manually in WSLg
- **Solution**: Use the code changes in this guide or create layouts on a native Linux/Windows installation, then copy the config file

**Issue**: Lost my custom layout
- **Solution**: Check `~/.config/codeblocks/default.conf.backup` or use View → Perspectives → Revert to get back to hardcoded defaults

## Additional Tips

- **Keyboard Shortcut**: You can assign shortcuts to switch perspectives in Settings → Editor → Keyboard shortcuts
- **Toggle Layout**: View → Perspectives → `Toggle to last` (Shift+F2) switches between your last two layouts
- **Delete Layout**: View → Perspectives → `Delete current` removes unwanted layouts
- **Reset Default**: Deleting the "default" layout resets it to the hardcoded version

## Reference

- Main frame implementation: `src/src/main.cpp` and `src/src/main.h`
- Debug interface factory: `src/src/debugger_interface_creator.cpp`
- Debugger manager: `src/sdk/debuggermanager.cpp`
- SDK events: `src/include/sdk_events.h`

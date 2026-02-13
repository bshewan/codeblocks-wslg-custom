# How to Save Your Custom Debug Layout in Code::Blocks

## Quick Save (In the UI)

1. **Arrange your windows** exactly as you want them (you've already done this!)

2. **Save the layout**:
   - Menu: `View` → `Perspectives` → `Save current`
   - Or press the keyboard shortcut if configured
   
3. **Name your layout**:
   - Enter a descriptive name like:
     - "WSLg Debug"
     - "My Debug Layout"
     - "Bottom Debug Panels"
   - Click OK

4. **Your layout is now saved!**
   - It will appear in: `View` → `Perspectives` → [Your Layout Name]
   - A checkmark will show next to the active layout

## Switching Between Layouts

- **View → Perspectives** → Select any saved layout
- **Toggle**: View → Perspectives → `Toggle to last` (Shift+F2)
  - Quickly switch between your last two used layouts

## Verify Layout Was Saved

After saving, check the config file:

```bash
# View saved layouts
grep -A5 "layout.*name" ~/.config/codeblocks/default.conf | head -30
```

You should see your layout name listed.

## Making Your Layout the Default

If you want Code::Blocks to always open with your custom layout:

1. **Save your layout** (as described above)
2. **Select it** from View → Perspectives
3. Code::Blocks will remember it as the default for next time

Or manually edit the config:
```bash
# Edit the config file
nano ~/.config/codeblocks/default.conf

# Find the line:
[main_frame/layout]
default=...

# Change it to your layout name:
default=WSLg Debug
```

## Exporting Your Layout to Share or Backup

To save your layout as a standalone file:

```bash
# Extract your layout section from the config
cd ~/.config/codeblocks

# Create a backup of just your layout
grep -A100 "\[main_frame/layout\]" default.conf > my-debug-layout-backup.txt
```

To restore or use on another machine:
1. Copy the `my-debug-layout-backup.txt` to the new machine
2. Merge the `[main_frame/layout]` section into the new machine's `default.conf`

## Advanced: Viewing the Layout Data

Your layout is stored as a wxAUI perspective string. To see it:

```bash
# View all layouts
grep "main_frame/layout" ~/.config/codeblocks/default.conf -A3
```

The `data=` line contains the actual window positions (it's a long encoded string).

## Tips

- **Create multiple layouts** for different scenarios:
  - "Coding" - No debug windows, maximized editor
  - "Debugging" - Your custom debug layout
  - "Review" - Split editor views with file browser

- **Quick switch** - Use Shift+F2 to toggle between your two most-used layouts

- **Reset if needed** - View → Perspectives → `Delete current` removes unwanted layouts

- **Backup regularly** - Copy `~/.config/codeblocks/default.conf` to keep your layouts safe

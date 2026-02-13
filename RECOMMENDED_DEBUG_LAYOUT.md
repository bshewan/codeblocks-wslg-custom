# Recommended Debug Window Layout for WSLg

This document contains the recommended debug window layout configuration for Code::Blocks running under WSLg.

## Problem

Under WSLg, debug windows float by default and docking doesn't work reliably. This layout pre-configures all debug windows to be docked in sensible positions.

## Layout Configuration

The following layout string should be placed in `~/.config/codeblocks/default.conf` within the `<aui>` section, replacing the existing `layout2` CDATA section:

```xml
<![CDATA[layout2|name=ManagementPane;caption=Management;state=2099196;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=200;besth=980;minw=100;minh=100;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=MessagesPane;caption=Logs & others;state=2099196;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=1820;besth=150;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=CallStackPane;caption=Call stack;state=2099196;dir=3;layer=0;row=0;pos=1;prop=100000;bestw=450;besth=150;minw=150;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=BreakpointsPane;caption=Breakpoints;state=2099199;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=350;besth=250;minw=150;minh=150;maxw=-1;maxh=-1;floatx=200;floaty=150;floatw=350;floath=250|name=CPURegistersPane;caption=CPU Registers;state=2099199;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=350;besth=250;minw=150;minh=150;maxw=-1;maxh=-1;floatx=200;floaty=150;floatw=350;floath=250|name=DisassemblyPane;caption=Disassembly;state=2099199;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=350;besth=250;minw=150;minh=150;maxw=-1;maxh=-1;floatx=200;floaty=150;floatw=350;floath=250|name=ExamineMemoryPane;caption=Memory;state=2099196;dir=2;layer=0;row=0;pos=0;prop=50000;bestw=450;besth=400;minw=350;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=ThreadsPane;caption=Running threads;state=2099199;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=450;besth=75;minw=250;minh=75;maxw=-1;maxh=-1;floatx=200;floaty=150;floatw=450;floath=75|name=WatchesPane;caption=Watches;state=2099196;dir=2;layer=0;row=1;pos=0;prop=50000;bestw=450;besth=400;minw=150;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=MainPane;caption=;state=768;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=ScriptConsole;caption=Scripting console;state=2099199;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=328;besth=100;minw=100;minh=100;maxw=-1;maxh=-1;floatx=300;floaty=200;floatw=-1;floath=-1|name=HighlightedOccurrences;caption=Highlighted Occurrences;state=2099198;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=150;besth=100;minw=50;minh=50;maxw=-1;maxh=-1;floatx=200;floaty=150;floatw=100;floath=150|name=OpenFilesPane;caption=Open files list;state=2099198;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=150;besth=100;minw=50;minh=50;maxw=-1;maxh=-1;floatx=200;floaty=150;floatw=100;floath=150|name=DefMimeHandler_HTMLViewer;caption=HTML viewer;state=2099199;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=350;besth=250;minw=150;minh=150;maxw=-1;maxh=-1;floatx=200;floaty=150;floatw=350;floath=250|name=TodoListPanev2.0.0;caption=Todo list;state=2099199;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=352;besth=94;minw=252;minh=94;maxw=-1;maxh=-1;floatx=200;floaty=150;floatw=352;floath=94|name=MainToolbar;caption=Main Toolbar;state=2108156;dir=1;layer=10;row=0;pos=0;prop=100000;bestw=456;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=CompilerToolbar;caption=Compiler Toolbar;state=2108156;dir=1;layer=10;row=0;pos=467;prop=100000;bestw=365;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=DebuggerToolbar;caption=Debugger Toolbar;state=2108156;dir=1;layer=10;row=0;pos=843;prop=100000;bestw=428;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=CodeCompletionToolbar;caption=Code completion Toolbar;state=2108156;dir=1;layer=10;row=1;pos=0;prop=100000;bestw=947;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(4,1,0)=202|dock_size(3,0,0)=332|dock_size(5,0,0)=22|dock_size(2,0,0)=460|dock_size(2,0,1)=460|dock_size(1,10,0)=44|dock_size(1,10,1)=44|]]>
```

## Panel Positions

### Docked Panels
- **Memory (ExamineMemoryPane)**: Upper right side (50% height)
- **Watches (WatchesPane)**: Lower right side (50% height)
- **Call Stack (CallStackPane)**: Bottom panel (right side)
- **Messages/Logs**: Bottom panel (left side)
- **Management**: Left side

### Floating Panels (on-demand)
- Breakpoints
- CPU Registers
- Disassembly
- Running Threads
- Script Console
- Todo List

## Applying the Layout

### Method 1: Automated Script
```bash
# Backup current config
cp ~/.config/codeblocks/default.conf ~/.config/codeblocks/default.conf.backup

# Use the Python script to update
python3 << 'EOF'
import re
with open('/home/USERNAME/.config/codeblocks/default.conf', 'r') as f:
    config = f.read()
new_layout = "layout2|name=ManagementPane;caption=Management;state=2099196;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=200;besth=980;minw=100;minh=100;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=MessagesPane;caption=Logs & others;state=2099196;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=1820;besth=150;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=CallStackPane;caption=Call stack;state=2099196;dir=3;layer=0;row=0;pos=1;prop=100000;bestw=450;besth=150;minw=150;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=BreakpointsPane;caption=Breakpoints;state=2099199;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=350;besth=250;minw=150;minh=150;maxw=-1;maxh=-1;floatx=200;floaty=150;floatw=350;floath=250|name=CPURegistersPane;caption=CPU Registers;state=2099199;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=350;besth=250;minw=150;minh=150;maxw=-1;maxh=-1;floatx=200;floaty=150;floatw=350;floath=250|name=DisassemblyPane;caption=Disassembly;state=2099199;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=350;besth=250;minw=150;minh=150;maxw=-1;maxh=-1;floatx=200;floaty=150;floatw=350;floath=250|name=ExamineMemoryPane;caption=Memory;state=2099196;dir=2;layer=0;row=0;pos=0;prop=50000;bestw=450;besth=400;minw=350;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=ThreadsPane;caption=Running threads;state=2099199;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=450;besth=75;minw=250;minh=75;maxw=-1;maxh=-1;floatx=200;floaty=150;floatw=450;floath=75|name=WatchesPane;caption=Watches;state=2099196;dir=2;layer=0;row=1;pos=0;prop=50000;bestw=450;besth=400;minw=150;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=MainPane;caption=;state=768;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=ScriptConsole;caption=Scripting console;state=2099199;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=328;besth=100;minw=100;minh=100;maxw=-1;maxh=-1;floatx=300;floaty=200;floatw=-1;floath=-1|name=HighlightedOccurrences;caption=Highlighted Occurrences;state=2099198;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=150;besth=100;minw=50;minh=50;maxw=-1;maxh=-1;floatx=200;floaty=150;floatw=100;floath=150|name=OpenFilesPane;caption=Open files list;state=2099198;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=150;besth=100;minw=50;minh=50;maxw=-1;maxh=-1;floatx=200;floaty=150;floatw=100;floath=150|name=DefMimeHandler_HTMLViewer;caption=HTML viewer;state=2099199;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=350;besth=250;minw=150;minh=150;maxw=-1;maxh=-1;floatx=200;floaty=150;floatw=350;floath=250|name=TodoListPanev2.0.0;caption=Todo list;state=2099199;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=352;besth=94;minw=252;minh=94;maxw=-1;maxh=-1;floatx=200;floaty=150;floatw=352;floath=94|name=MainToolbar;caption=Main Toolbar;state=2108156;dir=1;layer=10;row=0;pos=0;prop=100000;bestw=456;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=CompilerToolbar;caption=Compiler Toolbar;state=2108156;dir=1;layer=10;row=0;pos=467;prop=100000;bestw=365;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=DebuggerToolbar;caption=Debugger Toolbar;state=2108156;dir=1;layer=10;row=0;pos=843;prop=100000;bestw=428;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=CodeCompletionToolbar;caption=Code completion Toolbar;state=2108156;dir=1;layer=10;row=1;pos=0;prop=100000;bestw=947;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(4,1,0)=202|dock_size(3,0,0)=332|dock_size(5,0,0)=22|dock_size(2,0,0)=460|dock_size(2,0,1)=460|dock_size(1,10,0)=44|dock_size(1,10,1)=44|"
pattern = r'(CDATA\[)layout2\|[^\]]+\|(\]\]>)'
replacement = r'\1' + new_layout + r'\2'
new_config = re.sub(pattern, replacement, config)
with open('/home/USERNAME/.config/codeblocks/default.conf', 'w') as f:
    f.write(new_config)
EOF

# Restart Code::Blocks
killall codeblocks
codeblocks
```

### Method 2: Manual
1. Close Code::Blocks
2. Edit `~/.config/codeblocks/default.conf`
3. Find the `<aui>` section
4. Replace the entire `<![CDATA[layout2|...]]>` section with the one above
5. Save and restart Code::Blocks

## See Also
- `DEBUG_WINDOWS_LAYOUT_GUIDE.md` - Original debug window docking documentation
- `SAVE_LAYOUT_INSTRUCTIONS.md` - How to save custom layouts

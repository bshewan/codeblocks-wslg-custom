# Freeze Address Feature - Memory Dump Window

## Overview

The Memory Dump window now supports "freezing" register and variable expressions to fixed hex addresses using the `#` prefix syntax.

## Problem Solved

When examining memory around registers like `$rsp` (stack pointer), the address changes as you step through code. This makes it difficult to watch a fixed region of memory (like the initial stack frame) as it changes during execution.

## Solution: `#` Prefix Syntax

Prefix any expression with `#` to **freeze** it to the current evaluated value.

### How It Works

1. **Type** `#$rsp` (or any expression) in the Address field
2. **Click "Go"** or press Enter
3. The expression is evaluated **once**
4. The address field is automatically **replaced** with the resolved hex address (e.g., `0x7fffffffdbd0`)
5. **Subsequent refreshes** view that fixed address, even as `$rsp` changes

## Examples

### Freeze Stack Pointer

```
Address: #$rsp
[Click Go]
Address: 0x7fffffffdbd0   ← Automatically replaced with actual address
```

Now as you step through code:
- `$rsp` may change to `0x7fffffffdb80`, `0x7fffffffdbc0`, etc.
- Memory window continues showing `0x7fffffffdbd0` (the original snapshot)
- You can see how memory at that fixed location changes

### Freeze Frame Pointer

```
Address: #$rbp
[Click Go]
Address: 0x7fffffffdbe0
```

### Freeze Variable Address

```
Address: #&&myVariable
[Click Go]
Address: 0x7fffffffdc10
```

### Freeze Any Register

```
Address: #$rax
Address: #$rdi
Address: #$r15
```

## Use Cases

### 1. Watch Initial Stack Frame

```c
void function() {
    int local1 = 5;      // Breakpoint here
    int local2 = 10;
    // Step through code...
}
```

1. Break at first line
2. Type `#$rsp` in Memory window
3. Click Go → Address becomes `0x7fffffffdbd0`
4. Step through function
5. Memory window stays at that address, showing how local variables change

### 2. Monitor Fixed Memory Region

```c
char buffer[256];
processData(buffer);  // Breakpoint before
// Step through processData
```

1. Break before function call
2. Type `#&&buffer` in Memory window
3. Click Go → Address becomes buffer's hex address
4. Step into/through `processData()`
5. Watch buffer contents change at fixed address

### 3. Compare Multiple Memory Regions

Open multiple Memory windows:
- Window 1: `#$rsp` (original stack)
- Window 2: `$rsp` (current stack)
- Window 3: `#&&globalVar` (fixed global variable)

## Syntax Details

### With `#` Prefix (Freeze)

| Input | After "Go" | Behavior |
|-------|-----------|----------|
| `#$rsp` | `0x7fffffffdbd0` | Fixed address |
| `#$rbp` | `0x7fffffffdbe0` | Fixed address |
| `#&&variable` | `0x7fffffffdc10` | Fixed address |

### Without `#` Prefix (Normal)

| Input | After "Go" | Behavior |
|-------|-----------|----------|
| `$rsp` | `$rsp` | Re-evaluates each time |
| `$rbp` | `$rbp` | Re-evaluates each time |
| `&&variable` | `&&variable` | Re-evaluates each time |
| `0x401060` | `0x401060` | Always fixed |

## Supported Expressions

The `#` prefix works with **any expression** that GDB can evaluate:

**Registers:**
- `#$rsp`, `#$rbp`, `#$rip`, `#$rax`, `#$rbx`, etc.
- `#$eax`, `#$ebx` (32-bit)

**Variables:**
- `#&&localVar`
- `#&&globalVar`
- `#&&structPtr->field`

**Arithmetic:**
- `#$rsp-64` (64 bytes before stack pointer)
- `#$rbp+16` (16 bytes after frame pointer)

**Dereferencing:**
- `#*ptr` (address that ptr points to)

## Implementation Details

### Code Flow

1. User types `#expression` and clicks "Go"
2. `OnGo()` detects `#` prefix
3. Strips `#`, sets address to `expression`, sets `m_FreezeAddress = true`
4. Debugger evaluates `expression` and sends memory dump
5. `AddHexByte()` receives first byte with resolved address
6. If `m_FreezeAddress == true`, replaces address field with hex value
7. Sets `m_FreezeAddress = false`

### Modified Files

- `src/src/resources/memdump.xrc`
  - Updated help text to mention `#` syntax

- `src/src/examinememorydlg.h`
  - Added `bool m_FreezeAddress` member

- `src/src/examinememorydlg.cpp`
  - `OnGo()`: Detects `#` prefix and strips it
  - `AddHexByte()`: Replaces address with hex value when `m_FreezeAddress` is true

## Tips

**Tip 1:** Use meaningful addresses
```
Good: #$rsp-256   (view 256 bytes of stack)
Good: #&&buffer   (monitor specific buffer)
```

**Tip 2:** Open multiple memory windows for comparison
```
Window 1: #$rsp (initial stack)
Window 2: $rsp  (current stack - follows as it changes)
```

**Tip 3:** Combine with change highlighting
- Changes at the fixed address show in red
- Great for seeing exactly what modifies memory at a specific location

**Tip 4:** Re-freeze at any time
- Just type `#$rsp` again to capture a new snapshot

## Future Enhancements

Potential improvements:
- Add "Freeze" button as an alternative to `#` syntax
- Add "Snapshot History" to keep multiple frozen addresses
- Allow expressions like `#($rsp-256)..#($rsp+256)` for range
- Add visual indicator when viewing frozen address

## See Also

- MEMORY_CHANGE_HIGHLIGHTING.md - Memory change tracking feature
- DEBUG_WINDOWS_LAYOUT_GUIDE.md - Debug window configuration

# Memory Window Change Highlighting Feature

## Overview

The Memory Dump window in Code::Blocks now highlights memory values that have changed since the last debugger pause. Changed values are displayed in **red text** using **4-byte word alignment** by default, which works perfectly for most common data types including 32-bit integers, floats, and more.

## How It Works

### Visual Feedback

When you examine memory during debugging:
1. **First time** viewing memory - all bytes display in normal (black) text
2. **Step through code or continue** - the debugger runs
3. **Stop at next breakpoint** - refresh the memory view
4. **Changed values appear in RED** - entire 4-byte words highlighted in both hex and ASCII columns
5. **Unchanged values** remain in normal text color

### Word-Aligned Highlighting

The highlighting uses **4-byte (32-bit) word boundaries** by default:
- Bytes 0-3, 4-7, 8-11, 12-15 are treated as separate 4-byte words
- If **any byte** in a 4-byte word changes, the **entire word** is highlighted in red
- This is perfect for the most common data types: `int`, `uint32_t`, `float`, 32-bit pointers

**Why 4 bytes?**
- Most common data type size in C/C++
- Matches `int`, `float`, `uint32_t`, 32-bit pointers
- Works well on both 32-bit and 64-bit systems
- Doesn't over-highlight like 8-byte words would for small types
- Not too granular like byte-by-byte highlighting

### Configurable for Other Sizes

You can configure the word size for specific debugging scenarios:
- **1 byte**: Individual byte highlighting (debugging `char`, `uint8_t`)
- **2 bytes**: Short highlighting (debugging `short`, `uint16_t`)
- **4 bytes**: Word highlighting - **DEFAULT** (`int`, `float`, 32-bit pointers)
- **8 bytes**: Double word highlighting (debugging `long`, 64-bit pointers, `double`)

### Example (All Systems)

```
Before (at address 0x1000):
0x1000: 01 00 00 00  05 00 00 00  09 00 00 00  0D 00 00 00 | ................

After step (first int changed from 1 to 2):
0x1000: 02 00 00 00  05 00 00 00  09 00 00 00  0D 00 00 00 | ................
        ^^^^^^^^^^^                                          ^^^^
        (all 4 bytes red)                                   (all 4 bytes red in ASCII)
```

Even though only one byte changed (0x01 → 0x02), all 4 bytes of that integer are highlighted.

**Multiple changes:**
```
Before:
0x1000: 01 00 00 00  02 00 00 00  03 00 00 00  04 00 00 00

After changing first and third integers:
0x1000: FF 00 00 00  02 00 00 00  EE 00 00 00  04 00 00 00
        ^^^^^^^^^^^              ^^^^^^^^^^^
        (red)                    (red)
```

## Usage

### Viewing Memory

1. **Start debugging** your program (F8)
2. **Open Memory window**: `View → Debugger windows → Memory`
3. **Enter address** to examine (e.g., `0x401060`, `&myVariable`, `$esp`)
4. **Click "Go"** to display memory

### Seeing Changes

1. **Note the initial values**
2. **Step over** (F7), **Step into** (Shift+F7), or **Continue** (F8)
3. **Refresh memory** by clicking "Go" again (or it may auto-refresh)
4. **Changed 4-byte words appear in red**

### Resetting Highlighting

The red highlighting is automatically reset when:
- You **continue execution** (F8) - highlighting is disabled until next pause
- You **clear the memory window** - change history is reset
- You **examine a different address** - new tracking starts

## Implementation Details

### What Changed

**Modified Files:**
1. `src/src/resources/memdump.xrc` - Added `wxTE_RICH2` style to text control
2. `src/src/examinememorydlg.h` - Added change tracking members
3. `src/src/examinememorydlg.cpp` - Implemented word-aligned comparison and highlighting logic

**Key Features:**
- Stores previous memory values in a `std::map<uint64_t, unsigned char>`
- Compares each byte to its previous value
- **Highlights entire 4-byte aligned word** when any byte in that word changes
- Uses `wxTextAttr` with red color for changed words
- Automatically manages highlighting state based on debugger events

### Technical Notes

#### Default Word Size

The highlight word size defaults to 4 bytes for optimal debugging experience:
```cpp
// Default to 4 bytes which works well for most common types
m_HighlightWordSize = c->ReadInt(
    wxT("/common/examine_memory/highlight_word_size"), 
    4);  // int, float, uint32_t, 32-bit pointers
```

#### Word Alignment Logic

When a byte changes, the entire word containing it is highlighted:
```cpp
// Calculate word boundary based on m_HighlightWordSize
int wordStart = (bcmod / m_HighlightWordSize) * m_HighlightWordSize;
int wordEnd = wordStart + m_HighlightWordSize;

// Mark all bytes in this word as changed
for (int i = wordStart; i < wordEnd && i < 16; ++i)
{
    m_ChangedBytes[i] = true;
}
```

#### Supported Word Sizes

The implementation supports: **1, 2, 4, or 8 bytes**
- 1 byte: Individual byte highlighting (like `char`, `uint8_t`)
- 2 bytes: Short word highlighting (`short`, `uint16_t`)
- 4 bytes: Word highlighting - **DEFAULT** (`int`, `uint32_t`, `float`, 32-bit pointers)
- 8 bytes: Double word highlighting (`long`, `uint64_t`, `double`, 64-bit pointers)

#### Memory Storage

Individual bytes are tracked per address:
```cpp
std::map<uint64_t, unsigned char> m_PreviousValues;
```

This map persists across memory refreshes, allowing byte-level comparison while displaying word-level highlighting.

#### Color Application

Colors are applied character-by-character using `wxTextCtrl::SetDefaultStyle()`:
```cpp
if (hasChanged)
{
    wxTextAttr redAttr(*wxRED);
    m_pText->SetDefaultStyle(redAttr);
}
```

#### Event Handling

Listens for `cbEVT_DEBUGGER_CONTINUED` to reset highlighting when debugging resumes.

## Why 4-Byte Default?

The 4-byte default word size works best for most debugging scenarios because:

**Most common data types are 4 bytes:**
- `int` - 4 bytes on all modern systems
- `float` - 4 bytes (IEEE 754 single precision)
- `uint32_t` - 4 bytes (explicitly sized)
- 32-bit pointers on 32-bit systems

**Works well on both 32-bit and 64-bit systems:**
- On 32-bit: Perfect match for most types
- On 64-bit: Still matches most types (int, float) even though pointers are 8 bytes

**Doesn't over-highlight:**
- 8-byte default would highlight too much for int/float changes
- You'd see adjacent unrelated data highlighted in red

**Examples of the problem with 8-byte default:**
```c
struct Data {
    int x;      // 4 bytes
    int y;      // 4 bytes (adjacent to x)
};

// With 8-byte highlighting, changing x would also highlight y!
// With 4-byte highlighting (correct), only x is highlighted.
```

### Data Type Sizes on 64-bit Systems

| Type | Size | 4-byte highlight | 8-byte highlight |
|------|------|-----------------|-----------------|
| `char` | 1 byte | 4 bytes shown | 8 bytes shown |
| `short` | 2 bytes | 4 bytes shown | 8 bytes shown |
| `int` | **4 bytes** | **4 bytes shown** ✓ | 8 bytes shown ✗ |
| `float` | **4 bytes** | **4 bytes shown** ✓ | 8 bytes shown ✗ |
| `long` | 8 bytes | 8 bytes shown* | 8 bytes shown ✓ |
| `void*` | 8 bytes | 8 bytes shown* | 8 bytes shown ✓ |
| `double` | 8 bytes | 8 bytes shown* | 8 bytes shown ✓ |

*With 4-byte default, 8-byte types span 2 words, so both words get highlighted - still correct, just shows as 2 adjacent 4-byte regions.

## Configuration

### Viewing Current Settings

The word size is stored in the configuration:
```bash
# Location: ~/.codeblocks/default.conf or ~/.config/codeblocks/default.conf
# Section: [debugger_common]
# Key: /common/examine_memory/highlight_word_size
```

### Changing Highlight Word Size

You can customize the word size by editing the configuration file:

**For byte-level highlighting (debugging char arrays):**
```ini
[debugger_common]
/common/examine_memory/highlight_word_size=1
```

**For 2-byte (short) highlighting:**
```ini
[debugger_common]
/common/examine_memory/highlight_word_size=2
```

**For 4-byte (int/float) highlighting - DEFAULT:**
```ini
[debugger_common]
/common/examine_memory/highlight_word_size=4
```

**For 8-byte (long/pointer on 64-bit) highlighting:**
```ini
[debugger_common]
/common/examine_memory/highlight_word_size=8
```

### When to Use Different Word Sizes

**Use 4-byte (default)** for most debugging:
- General purpose code
- 32-bit integers (`int`, `uint32_t`)
- Floating point (`float`)
- Mixed variable types

**Use 8-byte** when debugging:
- Pointer-heavy code on 64-bit systems
- 64-bit integers (`long`, `uint64_t`)
- Double-precision floats (`double`)
- Large structures with 8-byte aligned members

**Use 1-byte** when debugging:
- String/character operations
- Byte arrays
- Communication protocols (byte-by-byte)
- Memory corruption issues

**Use 2-byte** when debugging:
- 16-bit values (`short`, `uint16_t`)
- Network protocols (many use 16-bit fields)
- Legacy 16-bit code

## Limitations and Future Enhancements

### Current Limitation: No Type Information

The memory window doesn't have access to variable type information, so it cannot automatically distinguish between:
- 32-bit integers (4 bytes)
- 64-bit pointers (8 bytes)  
- Other data types

**Current solution:** Uses a fixed 4-byte word size by default, which works correctly for most common types (int, float) and acceptably for larger types (pointers show as two adjacent highlighted regions when bytes in both halves change).

### Workaround for Specific Scenarios

If you're debugging specific types of code, you can adjust the word size:

**For stack viewing** (mixed ints and pointers):
- Keep 4-byte default
- Ints highlight correctly (4 bytes)
- Pointers show as adjacent regions (still identifies changes)

**For pointer-heavy code** (many 64-bit addresses):
- Switch to 8-byte mode via config
- All values highlight as 8-byte chunks
- Better for pointer operations, may over-highlight ints

### Future Enhancement: Watch Pane Integration

A potential future improvement would be to integrate with the Watch pane:
- When a watched variable's memory is displayed, use its type information
- Parse type strings: `int` → 4 bytes, `long` → 8 bytes, `void*` → 8 bytes
- Automatically highlight the correct number of bytes for each watched variable
- Would require inter-window communication and type parsing logic

This would provide perfect highlighting for all types without manual configuration or compromise.

**Implementation requirements:**
1. Communication channel between Watch pane and Memory window
2. Type string parser (handle `int`, `long`, `void*`, `struct`, arrays, etc.)
3. Address-to-size mapping registry
4. Dynamic updates when watches change
5. Handling of complex types (structs, unions, arrays)

### Future Enhancements

Potential UI improvements:
- **GUI configuration** - Add word size selector to memory window
- **Context menu option** - Right-click to change highlight granularity
- **Per-address mode** - Different highlight sizes for different memory regions
- **Type-aware highlighting** - Highlight based on variable type if known

## Troubleshooting

### Red text not appearing

**Possible causes:**
1. **First view** - No previous values to compare against
   - **Solution**: Step through code, then refresh memory
   
2. **Memory cleared** - Previous values were reset
   - **Solution**: Normal behavior after clearing

3. **Different address** - Viewing different memory region
   - **Solution**: Navigate back to original address

### More than expected is highlighted

**Cause**: Word-aligned highlighting highlights entire 4-byte words
- **Example**: Changing a `char` at offset 1 highlights bytes 0-3
- **Solution**: This is intentional - helps visualize complete values

### All text appears red

**Cause**: Comparing against uninitialized or zero memory
- **Solution**: This is normal if comparing against initial state

## Testing

### Test Scenario 1: 32-bit Integer Change (All Systems)

```c
int main() {
    int x = 1;           // Set breakpoint here
    x = 100;             // Step here - x's 4 bytes will be red
    return 0;
}
```

1. Break at first line
2. Examine memory at `&x`
3. Step to assignment
4. Refresh memory - **exactly 4 bytes** highlighted (perfect!)

### Test Scenario 2: 64-bit Pointer on 64-bit System

```c
int main() {
    void *ptr = (void*)0x123456;  // Set breakpoint here
    ptr = (void*)0xABCDEF;        // Step here
    return 0;
}
```

1. Break at first line
2. Examine memory at `&ptr`
3. Step to assignment
4. Refresh memory - **8 bytes highlighted** (spans 2 adjacent 4-byte words)

### Test Scenario 3: Array of Integers

```c
int arr[4] = {1, 2, 3, 4};
arr[1] = 99;  // Break here
```

1. Break at assignment
2. Examine `&arr`  
3. Step
4. Second 4-byte word (bytes 4-7) highlighted in red - **not the first or third integer**

## Rebuilding

After making changes to the memory window code:

```bash
cd /home/bshewan/codeblocks_25.03/codeblocks_25.03
make -C src/src
```

Or use the provided script:
```bash
./rebuild_debug_windows.sh
```

## See Also

- DEBUG_WINDOWS_LAYOUT_GUIDE.md - Debug window docking configuration
- BUILD_TEST_RESULTS.md - Build verification and testing

## Credits

Enhanced memory window with word-aligned change tracking implemented for Code::Blocks 25.03.
Highlighting uses 4-byte (32-bit) word boundaries for optimal debugging of common data types.

## Usage

### Viewing Memory

1. **Start debugging** your program (F8)
2. **Open Memory window**: `View → Debugger windows → Memory`
3. **Enter address** to examine (e.g., `0x401060`, `&myVariable`, `$esp`)
4. **Click "Go"** to display memory

### Seeing Changes

1. **Note the initial values**
2. **Step over** (F7), **Step into** (Shift+F7), or **Continue** (F8)
3. **Refresh memory** by clicking "Go" again (or it may auto-refresh)
4. **Changed bytes appear in red**

### Resetting Highlighting

The red highlighting is automatically reset when:
- You **continue execution** (F8) - highlighting is disabled until next pause
- You **clear the memory window** - change history is reset
- You **examine a different address** - new tracking starts

## Implementation Details

### What Changed

**Modified Files:**
1. `src/src/resources/memdump.xrc` - Added `wxTE_RICH2` style to text control
2. `src/src/examinememorydlg.h` - Added change tracking members
3. `src/src/examinememorydlg.cpp` - Implemented comparison and highlighting logic

**Key Features:**
- Stores previous memory values in a `std::map<uint64_t, unsigned char>`
- Compares each byte to its previous value
- Uses `wxTextAttr` with red color for changed bytes
- Automatically manages highlighting state based on debugger events

### Technical Notes

#### Memory Storage

Changed bytes are tracked per address:
```cpp
std::map<uint64_t, unsigned char> m_PreviousValues;
```

This map persists across memory refreshes, allowing comparison.

#### Color Application

Colors are applied character-by-character using `wxTextCtrl::SetDefaultStyle()`:
```cpp
if (hasChanged)
{
    wxTextAttr redAttr(*wxRED);
    m_pText->SetDefaultStyle(redAttr);
}
```

#### Event Handling

Listens for `cbEVT_DEBUGGER_CONTINUED` to reset highlighting when debugging resumes.

## Configuration

No configuration options are currently provided. The feature is always enabled.

### Future Enhancements

Potential improvements:
- **Configurable colors** - user-specified highlight color
- **Multiple highlight modes** - different colors for reads vs writes
- **Persistence control** - option to keep or clear history
- **Address range tracking** - highlight only specific memory ranges
- **Export changes** - save list of changed addresses

## Troubleshooting

### Red text not appearing

**Possible causes:**
1. **First view** - No previous values to compare against
   - **Solution**: Step through code, then refresh memory
   
2. **Memory cleared** - Previous values were reset
   - **Solution**: Normal behavior after clearing

3. **Different address** - Viewing different memory region
   - **Solution**: Navigate back to original address

### All text appears red

**Cause**: Comparing against unitialized or zero memory
- **Solution**: This is normal if comparing against initial state

### Text control looks different

**Cause**: `wxTE_RICH2` style enables rich text formatting
- **Effect**: Slightly different rendering, but same functionality

## Testing

### Test Scenario 1: Local Variable Change

```c
int main() {
    int x = 0x12345678;    // Set breakpoint here
    x = 0x87654321;        // Step to here
    return 0;
}
```

1. Break at first line
2. Examine memory at `&x`
3. Step to next line
4. Refresh memory - all 4 bytes should be red

### Test Scenario 2: Array Modification

```c
int arr[10] = {0};
for (int i = 0; i < 10; i++) {
    arr[i] = i * 2;  // Break here
}
```

1. Break in loop
2. Examine `&arr`
3. Continue/step through iterations
4. Each iteration highlights newly changed elements

## Rebuilding

After making changes to the memory window code:

```bash
cd /home/bshewan/codeblocks_25.03/codeblocks_25.03
make -C src/src
```

Or use the provided script:
```bash
./rebuild_debug_windows.sh
```

## See Also

- DEBUG_WINDOWS_LAYOUT_GUIDE.md - Debug window docking configuration
- BUILD_TEST_RESULTS.md - Build verification and testing

## Credits

Enhanced memory window with change tracking implemented for Code::Blocks 25.03.

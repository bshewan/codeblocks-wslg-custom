#!/bin/bash
# Quick rebuild script for Code::Blocks after debug window layout changes

set -e

echo "=== Rebuilding Code::Blocks with docked debug windows ==="
echo ""

# Check if we're in the right directory
if [ ! -f "src/src/debugger_interface_creator.cpp" ]; then
    echo "Error: Must run from Code::Blocks source root directory"
    exit 1
fi

# Rebuild just the main application (faster than full rebuild)
echo "Building main application..."
make -C src/src -j$(nproc)

echo ""
echo "=== Build complete! ==="
echo ""
echo "To test the changes:"
echo "  1. Clear your old layout config (optional, but recommended):"
echo "     mv ~/.config/codeblocks/default.conf ~/.config/codeblocks/default.conf.backup"
echo ""
echo "  2. Run Code::Blocks from the devel directory:"
echo "     cd src/devel && ./codeblocks"
echo ""
echo "  3. Start debugging a project to see debug windows docked"
echo ""
echo "To install system-wide:"
echo "     sudo make install"
echo ""

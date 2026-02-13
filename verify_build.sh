#!/bin/bash
# Build verification script for Code::Blocks

echo "=== Code::Blocks Build Verification ==="
echo ""

# Check if we're in the right directory
if [ ! -f "configure.ac" ]; then
    echo "❌ ERROR: Not in Code::Blocks source directory"
    echo "   Please cd to the codeblocks source directory first"
    exit 1
fi

echo "✓ In Code::Blocks source directory"
echo ""

# Check for required files
echo "Checking required files..."
if [ -f "m4/acinclude.m4" ]; then
    echo "  ✓ m4/acinclude.m4 exists"
else
    echo "  ❌ m4/acinclude.m4 MISSING"
    exit 1
fi

if [ -x "bootstrap" ]; then
    echo "  ✓ bootstrap is executable"
else
    echo "  ⚠ bootstrap not executable (fixing...)"
    chmod +x bootstrap
fi

if [ -x "update_revision.sh" ]; then
    echo "  ✓ update_revision.sh is executable"
else
    echo "  ⚠ update_revision.sh not executable (fixing...)"
    chmod +x update_revision.sh
fi

echo ""

# Check for required tools
echo "Checking required tools..."
for tool in autoconf automake libtool pkg-config; do
    if command -v $tool &> /dev/null; then
        echo "  ✓ $tool found"
    else
        echo "  ❌ $tool NOT FOUND"
        echo "     Install with: sudo apt-get install $tool"
        exit 1
    fi
done

echo ""

# Check for wxWidgets
echo "Checking wxWidgets..."
if pkg-config --exists wxwidgets; then
    WX_VERSION=$(pkg-config --modversion wxwidgets)
    echo "  ✓ wxWidgets $WX_VERSION found"
elif wx-config --version &> /dev/null; then
    WX_VERSION=$(wx-config --version)
    echo "  ✓ wxWidgets $WX_VERSION found (via wx-config)"
else
    echo "  ❌ wxWidgets NOT FOUND"
    echo "     Install with: sudo apt-get install libwxgtk3.2-gtk3-dev"
    exit 1
fi

echo ""
echo "=== All checks passed! ==="
echo ""
echo "You can now run:"
echo "  1. ./bootstrap"
echo "  2. ./configure --prefix=/usr/local"
echo "  3. make -j\$(nproc)"
echo "  4. sudo make install"
echo ""

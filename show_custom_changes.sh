#!/bin/bash
# Show the custom changes we made to Code::Blocks

echo "=== Custom Changes to Code::Blocks 25.03 ==="
echo ""
echo "Modified source files:"
git log --all --format=%H | xargs git show --name-only --format="" | grep -E "(debugger_interface_creator|memdump\.xrc|examinememorydlg)" | sort -u

echo ""
echo "Documentation added:"
ls -1 *.md 2>/dev/null

echo ""
echo "Build scripts:"
ls -1 *.sh 2>/dev/null

echo ""
echo "To view specific changes:"
echo "  git show HEAD:src/src/debugger_interface_creator.cpp"
echo "  git show HEAD:src/src/examinememorydlg.cpp"
echo "  git diff <commit> src/src/examinememorydlg.cpp"

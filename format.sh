#!/bin/sh

clang-format --version

if command -v dos2unix ; then
    find trantor -name *.h -o -name *.cc -exec dos2unix {} \;
fi
find trantor -name *.h -o -name *.cc|xargs clang-format -i -style=file

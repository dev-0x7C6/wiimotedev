#!/bin/bash
#git reset --hard
find -type f \( -name '*.h' -o -name '*.cpp' \) -printf "\"%p\" " | xargs clang-format -i

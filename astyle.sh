#!/bin/bash
find -type f -name '*.h' -printf "\"%p\" " | xargs astyle
find -type f -name '*.cpp' -printf "\"%p\" " | xargs astyle
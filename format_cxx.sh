#! /usr/bin/env bash

find . -regex '.*\.\(cpp\|hpp\|cc\|cxx\)' -exec clang-format -style=.clang_format -i {} \;

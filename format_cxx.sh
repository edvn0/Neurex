#! /usr/bin/env bash

find ./{Neurex,NXSandbox}/src -iname "*.h" -o -iname "*.cpp" | xargs clang-format -style=file -i


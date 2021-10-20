#! /usr/bin/env bash

find ./{Neurex,NXSandbox}/src -name '*.cpp' -exec clang-format --style=file -i {} \;


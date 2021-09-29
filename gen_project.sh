#! /usr/bin/env bash

go() {
	make clean
	premake5 --os=macosx --cc=clang gmake2
	make -j4
}

go

#! /usr/bin/env bash

go() {
	vendor/premake5 --os=macosx --cc=clang gmake2
}

go

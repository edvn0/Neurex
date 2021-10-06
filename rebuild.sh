#! /usr/bin/env bash

go() {
	make -j4
	source ./run.sh
}

go

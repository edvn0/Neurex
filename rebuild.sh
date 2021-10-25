#! /usr/bin/env bash

set -e

go() {
	make -j4
	source ./run.sh
}

go

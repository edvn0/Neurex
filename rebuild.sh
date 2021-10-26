#! /usr/bin/env bash

set -e

go() {
	make -j8
	source ./run.sh
}

go

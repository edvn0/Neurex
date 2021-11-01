#! /usr/bin/env bash

start() {
	cd /Volumes/1TBSSD/Programmering/Neurex/Neutralise
	../bin/Debug-macosx-x86_64/Neutralise/Neutralise
	cd - > /dev/null
}

start

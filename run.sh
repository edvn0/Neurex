#! /usr/bin/env bash

start() {
	cd /Volumes/1TBSSD/Programmering/Neurex/NXSandbox
	../bin/Debug-macosx-x86_64/NXSandbox/NXSandbox
	cd - > /dev/null 
}

start

#!/usr/bin/env bash

qemu-system-i386 -d int -no-reboot -no-shutdown -hda ./Build/LittleOS.bin

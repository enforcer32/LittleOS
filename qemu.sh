#!/usr/bin/env bash

qemu-system-i386 -d int,cpu_reset -no-reboot -no-shutdown -hda ./Build/LittleOS.bin

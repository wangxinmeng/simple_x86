#!/bin/sh
qemu-system-x86_64 -S -s -m 4G -machine pc -serial mon:stdio -hda ucore.img -nographic

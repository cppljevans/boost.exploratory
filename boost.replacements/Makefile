SHELL := /bin/bash
############PATH := $(USER)/bin:$(PATH)

diff:
	diff --recursive --text --exclude *.hpp~ $(DIR0) $(DIR1)>diff.out
include $(HOME)/prog_dev/root.imk
DIR0=$(boost.replacements)/debug
DIR1=$(boost.replacements)/unfold_left/debug

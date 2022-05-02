SHELL := /bin/bash
############
PATH := $(USER)/bin:$(PATH)
MAIN=trans2rhs

HOW=clangxx11_0_0
#HOW=gxx10_2_0

DIR.root := $(shell dirup_dir_file.pl $(PWD) root.imk)
git.root := $(shell dirup_dir_file.pl $(PWD) .git)
include $(DIR.root)/root.imk
boost.replacements=$(HOME)/prog_dev/boost.org/boost.replacements
INC.transform_attribute=-I$(boost.replacements)/transform_attribute
INC.boost_additions=-I$(boost.additions)
INCS:=\
  $(INC.transform_attribute)\
  $(INC.boost_additions)\
  -I$(boost.root)\
  $(INCS)

compare_collapse:
	make --always-make DO_COLLAPSE=not run
	make --always-make DO_COLLAPSE=yes run

show:
	echo "git.root=$(git.root)"

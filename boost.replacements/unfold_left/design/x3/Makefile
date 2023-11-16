SHELL := /bin/bash
############
PATH := $(USER)/bin:$(PATH)
MAIN=attribute_of
MAIN=auto_def_tag
#MAIN=parse_unused
#MAIN=attribute_of-simple_seq
#MAIN=attribute_of-simple_alt

HOW=clangxx14_0_3
#HOW=gxx10_2_0

DIR.root := $(shell dirup_dir_file.pl $(PWD) root.imk)
include $(DIR.root)/root.imk
INC.common=-I$(boost.replacements)/common
DEF.USE_UNFOLD_LEFT?=D
INC.USE_UNFOLD_LEFT=-I$(boost.replacements)/unfold_left
DEF.USE_STD_VARIANT?=$(DEF.USE_UNFOLD_LEFT)
AVOID_HASH_HPP_WARNS=-DBOOST_NO_CXX98_FUNCTION_BASE

INCS:=\
  $(INC.common)\
  $(INC.USE_UNFOLD_LEFT)\
  -$(DEF.USE_UNFOLD_LEFT)USE_UNFOLD_LEFT\
  -$(DEF.USE_STD_VARIANT)USE_STD_VARIANT\
  -I$(boost.additions)\
  -I$(boost.root)\
  $(AVOID_HASH_HPP_WARNS)\
  $(INCS)

show:
	echo "USE_UNFOLD_LEFT=$(USE_UNFOLD_LEFT)"
	echo "INC.USE_UNFOLD_LEFT=$(INC.USE_UNFOLD_LEFT)"

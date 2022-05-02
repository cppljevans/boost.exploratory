SHELL := /bin/bash
############
PATH := $(USER)/bin:$(PATH)
MAIN=attribute_of
#MAIN=parse_unused
MAIN=attribute_of-simple_seq
#MAIN=attribute_of-simple_alt

HOW=clangxx11_0_0
#HOW=gxx10_2_0

DIR.root := $(shell dirup_dir_file.pl $(PWD) root.imk)
include $(DIR.root)/root.imk
INC.transform_attribute=-I$(boost.replacements)/transform_attribute
INC.attribute_of=-I$(boost.replacements)/attribute_of
INC.common=-I$(boost.replacements)/common
INC.fusion=-I$(boost.replacements)/fusion#provides deque copy assignment operator
DO_COLLAPSE?=not
#^=not|yes
DEF.attribute_of.not_collapse:=-DUSE_ATTRIBUTE_OF_NO_COLLAPSE 
DEF.attribute_of.yes_collapse:=-UUSE_ATTRIBUTE_OF_NO_COLLAPSE
DEF.attribute_of.which=$(DEF.attribute_of.$(DO_COLLAPSE)_collapse)
DO.transform_rule2rhs?=yes
DEF.transform_rule2rhs=-$(if $(DO.transform_rule2rhs),D,U)TRANSFORM_ATTRIBUTE_RULE2RHS
DO.USE_UNFOLD_LEFT?=yes
DEF.USE_UNFOLD_LEFT=-$(if $(DO.USE_UNFOLD_LEFT),D,U)USE_UNFOLD_LEFT
INC.USE_UNFOLD_LEFT=$(if $(DO.USE_UNFOLD_LEFT),-I$(boost.replacements)/unfold_left)
INCS:=\
  $(DEF.transform_rule2rhs)\
  $(DEF.attribute_of.which)\
  $(INC.transform_attribute)\
  $(INC.fusion)\
  $(INC.USE_UNFOLD_LEFT)\
  $(DEF.USE_UNFOLD_LEFT)\
  $(INC.attribute_of)\
  $(INC.common)\
  -I$(boost.additions)\
  -I$(boost.root)\
  $(INCS)

compare_collapse:
	make --always-make DO_COLLAPSE=not run
	make --always-make DO_COLLAPSE=yes run

show:
	echo "boost.additions=$(boost.additions)"

SHELL := /bin/bash
############PATH := $(USER)/bin:$(PATH)
BOOST_ROOT_VERSION=76
BOOST_ROOT_VERSION=78

boost.root=$(boost.root.1_$(BOOST_ROOT_VERSION)_0)
MAIN=1.78breakage
#MAIN=1.78breakage-fmt
#MAIN=1.78breakage-original
#MAIN=1.78breakage-simplified
#MAIN=1.78breakage-simplified2
#MAIN=1.78breakage-simplified3
MAIN=1.78breakage-simplified4
#MAIN=is_same_enough
#MAIN=1.78breakage-is_substitute-variants
#MAIN=alternative
#MAIN=alternative-debug
#MAIN=alternative-debug-transform
#MAIN=variant_io
#MAIN=ArithExpr-orginal
boost.git.spirit=$(boost.top)/git_download/spirit/spirit/include
USE_CPP_FMT?=U
USE_DEBUG?=D
USE_UNFOLD_LEFT?=U
USE_IS_SAME_ENOUGH?=D
TRANSFORM_ATTRIBUTE_RULE2RHS?=$(USE_UNFOLD_LEFT)
USE_DEMANGLE_FMT?=D
SHOW_ISSUE707_ERROR?=D
MOVE_TO_SRC_LOGDATA=$(SHOW_ISSUE707_ERROR)
DEFINES.alternative=\
 -fno-diagnostics-show-option\
  ###
DEFINES.alternative-debug=\
 -DUSE_VARIANT_SUB_1_78_0\
 -DUSE_IS_SUBSTITUTE_1_78_0\
 -UUSE_ISSUE707_VAR2\
 -UUSE_UNFOLD_LEFT\
 -UTRANSFORM_ATTRIBUTE_RULE2RHS\
 -UUSE_SEQ_PARSE_STD_VECTOR_SPECIALIZATION\
 -fno-diagnostics-show-option\
  ###
DEFINES.alternative-debug-transform=\
 -DUSE_VARIANT_SUB_1_78_0\
 -DUSE_IS_SUBSTITUTE_1_78_0\
 -UUSE_ISSUE707_VAR2\
 -DUSE_UNFOLD_LEFT\
 -DTRANSFORM_ATTRIBUTE_RULE2RHS\
 -UUSE_SEQ_PARSE_STD_VECTOR_SPECIALIZATION\
 -fno-diagnostics-show-option\
  ###
DEFINES.1.78breakage-original=\
 -DUSE_VARIANT_SUB_1_78_0\
 -DUSE_IS_SUBSTITUTE_1_78_0\
 -fno-diagnostics-show-option\
  ###
DEFINES.1.78breakage=\
 -DUSE_UNFOLD_LEFT\
  -$(USE_CPP_FMT)CPP_FMT\
 -fno-diagnostics-show-option\
  ###
DEFINES.1.78breakage-fmt=\
  $(DEFINES.1.78breakage)\
  ###
DEFINES.1.78breakage-simplified=\
 -UUSE_VARIANT_SUB_1_78_0\
 -DUSE_IS_SUBSTITUTE_1_78_0\
 -UMOVE_TO_SRC_LOGDATA\
 -UUSE_UNFOLD_LEFT\
 -fno-diagnostics-show-option\
  ###
DEFINES.1.78breakage-simplified2=\
 -DUSE_VARIANT_SUB_1_78_0\
 -DUSE_IS_SUBSTITUTE_1_78_0\
 -UMOVE_TO_SRC_LOGDATA\
 -UUSE_UNFOLD_LEFT\
 -fno-diagnostics-show-option\
  ###
DEFINES.1.78breakage-simplified3=\
 -DUSE_VARIANT_SUB_1_78_0\
 -DUSE_IS_SUBSTITUTE_1_78_0\
 -$(USE_UNFOLD_LEFT)USE_UNFOLD_LEFT\
 -$(TRANSFORM_ATTRIBUTE_RULE2RHS)TRANSFORM_ATTRIBUTE_RULE2RHS\
 -fno-diagnostics-show-option\
  ###
DEFINES.1.78breakage-simplified4=\
 -DUSE_VARIANT_SUB_1_78_0\
 -DUSE_IS_SUBSTITUTE_1_78_0\
 -$(MOVE_TO_SRC_LOGDATA)MOVE_TO_SRC_LOGDATA\
 -$(USE_UNFOLD_LEFT)USE_UNFOLD_LEFT\
 -$(SHOW_ISSUE707_ERROR)SHOW_ISSUE707_ERROR\
 -$(USE_IS_SAME_ENOUGH)USE_IS_SAME_ENOUGH\
 -$(TRANSFORM_ATTRIBUTE_RULE2RHS)TRANSFORM_ATTRIBUTE_RULE2RHS\
 -fno-diagnostics-show-option\
  ###
DEFINES.1.78breakage-is_substitute-variants=\
 -DUSE_VARIANT_SUB_1_78_0\
 -DUSE_IS_SUBSTITUTE_1_78_0\
 -UUSE_ISSUE707_VAR2\
 -$(USE_DEBUG)USE_DEBUG\
 -fno-diagnostics-show-option\
  ###
INC.transform_attribute.D=-I$(boost.replacements)/transform_attribute
INC.transform_attribute.U=
INC.unfold_left.D=-I$(boost.replacements)/unfold_left
INC.unfold_left.U=
INC.unfold_left.D=-I$(boost.replacements)/unfold_left
INC.unfold_left.U=
INC.unfold_left=$(INC.unfold_left.D)
INC.debug.D=-I$(boost.replacements)/debug
INC.debug.U=
INC.debug=$(INC.debug.$(USE_DEBUG))
HOW=clangxx14_0_3
#HOW=gxx10_2_0

DIR.root := $(shell dirup_dir_file.pl $(PWD) root.imk)
include $(DIR.root)/root.imk
#HOW=gxx10_2_0

#include $(HOME)/prog_dev/root.imk
INC.common=-I$(boost.additions) -I$(boost.replacements)/common
#INC.seqsize1=-I$(boost.replacements)/FUSION_ADAPT_seqsize1
INC.magic_enum=-I$(magic_enum.github) -I$(magic_enum.io)

INC.issue707=-I$(boost.replacements)/issue707
INC.1.78breakage-original=$(INC.issue707)
INC.1.78breakage=$(INC.issue707)
INC.1.78breakage=$(INC.common) $(INC.unfold_left.D) $(INC.unfold_left.D)/debug
INC.1.78breakage-fmt=$(INC.1.78breakage)
INC.alternative=$(INC.issue707)
INC.alternative-debug=$(INC.debug) -I$(boost.replacements)/fusion $(INC.unfold_left.D) $(INC.common)
INC.alternative-debug-transform=$(INC.debug) -I$(boost.replacements)/fusion $(INC.unfold_left) $(INC.common)
INC.1.78breakage-simplified=$(INC.debug) $(INC.unfold_left.D) $(INC.common)
INC.1.78breakage-simplified2=$(INC.debug) $(INC.unfold_left.D) $(INC.common)
#INC.1.78breakage-simplified2=$(INC.debug) $(INC.unfold_left.D) $(INC.common)
#INC.1.78breakage-simplified3=-I$(boost.additions)
#INC.1.78breakage-simplified3=$(INC.debug) $(INC.unfold_left.$(USE_UNFOLD_LEFT)) $(INC.transform_attribute.$(TRANSFORM_ATTRIBUTE_RULE2RHS)) $(INC.common)
INC.1.78breakage-simplified4=$(INC.debug) $(INC.unfold_left.$(USE_UNFOLD_LEFT)) $(INC.transform_attribute.$(TRANSFORM_ATTRIBUTE_RULE2RHS)) $(INC.common)
INC.1.78breakage-is_substitute-variants=$(INC.common)
INC.1.78breakage-is_substitute-variants=$(INC.magic_enum) -I$(magic_enum.io) $(INC.debug) $(INC.common)
#INC.ArithExpr-original=$(INC.issue707)
INC.is_same_enough=$(INC.common)
#INCS:=$(INC.$(MAIN)) $(DEFINES.$(MAIN)) -I$(boost.root) $(INCS)
INCS:=$(DEFINES.$(MAIN)) $(INC.$(MAIN)) $(boost.spirit.git) -I$(boost.root) $(INCS)
FLAGS_AS_PATH=-$(USE_UNFOLD_LEFT)USE_UNFOLD_LEFT-$(USE_IS_SAME_ENOUGH)USE_IS_SAME_ENOUGH-$(SHOW_ISSUE707_ERROR)SHOW_ISSUE707_ERROR

showx:
	ls -l $(boost.replacements)/unfold_left
	echo "USE_UNFOLD_LEFT=$(USE_UNFOLD_LEFT)"
	echo "INC.unfold_left.D=$(INC.unfold_left.D)"
	echo $(INC.unfold_left.$(USE_UNFOLD_LEFT))
show:
	@echo "DIR.root=$(DIR.root)"
	@echo "WHAT.dir=$(WHAT.dir)"
	@echo "BUILD_HOW_WHAT.dir=$(BUILD_HOW_WHAT.dir)"
show-diff:
	diff $(boost.replacements)/issue707/boost/spirit/home/x3/support/traits/is_substitute.hpp $(boost.root.1_78_0)/boost/spirit/home/x3/support/traits/is_substitute.hpp
show-diff.out:
	diff breakage-is_substitute-variants-DUSE_ISSUE707_VAR2.out breakage-is_substitute-variants-UUSE_ISSUE707_VAR2.out>breakage-is_substitute-variants-DU-USE_ISSUE707_VAR2.diff
find-hidden:
	find $(boost.replacements)/debug/ -name \*.hpp~hide
nl:
	#nl --number-format=rz --number-separator="]: " $(boost.additions)/example/utility/templ_expr/fmt-compiler-out.scratch.out
	nl --number-format=rz --number-separator="]: " 1.78breakage-simplified4.error_not.out>1.78breakage-simplified4.error_not.nl

templ_expr:
	ls -lrt $(boost.additions)/example/utility/templ_expr

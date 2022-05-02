SHELL := /bin/bash
############PATH := $(USER)/bin:$(PATH)
BOOST_ROOT_VERSION=76
BOOST_ROOT_VERSION=78
boost.root=$(boost.root.1_$(BOOST_ROOT_VERSION)_0)
MAIN=1.78breakage
MAIN=1.78breakage-original
#MAIN=1.78breakage-simplified
MAIN=1.78breakage-simplified2
#MAIN=1.78breakage-simplified3
MAIN=1.78breakage-is_substitute-variants
#MAIN=alternative
#MAIN=alternative-debug
#MAIN=alternative-debug-transform
#MAIN=variant_io
MAIN=ArithExpr-original
boost.git.spirit=$(boost.top)/git_download/spirit/spirit/include
USE_DEBUG?=D
USE_UNFOLD_LEFT?=U
TRANSFORM_ATTRIBUTE_RULE2RHS?=$(USE_UNFOLD_LEFT)
USE_DEMANGLE_FMT=U
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
 -fno-diagnostics-show-option\
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
HOW=clangxx11_0_0
HOW=gxx10_2_0

include $(HOME)/prog_dev/root.imk
boost.replacements=$(HOME)/prog_dev/boost.org/boost.replacements
INC.common=-I$(boost.replacements)/common -I$(boost.additions)
#INC.seqsize1=-I$(boost.replacements)/FUSION_ADAPT_seqsize1
INC.magic_enum=-I$(magic_enum.github) -I$(magic_enum.io)

INC.issue707=-I$(boost.replacements)/issue707
INC.1.78breakage-original=$(INC.issue707)
INC.1.78breakage=$(INC.issue707)
#INC.1.78breakage=$(INC.debug)
INC.alternative=$(INC.issue707)
INC.alternative-debug=$(INC.debug) -I$(boost.replacements)/fusion $(INC.unfold_left.D) $(INC.common)
INC.alternative-debug-transform=$(INC.debug) -I$(boost.replacements)/fusion $(INC.unfold_left) $(INC.common)
INC.1.78breakage-simplified=$(INC.debug) $(INC.unfold_left.D) $(INC.common)
INC.1.78breakage-simplified2=$(INC.debug) $(INC.unfold_left.D) $(INC.common)
#INC.1.78breakage-simplified2=$(INC.debug) $(INC.unfold_left.D) $(INC.common)
#INC.1.78breakage-simplified3=-I$(boost.additions)
#INC.1.78breakage-simplified3=$(INC.debug) $(INC.unfold_left.$(USE_UNFOLD_LEFT)) $(INC.transform_attribute.$(TRANSFORM_ATTRIBUTE_RULE2RHS)) $(INC.common)
INC.1.78breakage-is_substitute-variants=$(INC.common)
INC.1.78breakage-is_substitute-variants=$(INC.magic_enum) -I$(magic_enum.io) $(INC.debug) $(INC.common)
#INC.ArithExpr-original=$(INC.issue707)
#INCS:=$(INC.$(MAIN)) $(DEFINES.$(MAIN)) -I$(boost.root) $(INCS)
INCS:=$(DEFINES.$(MAIN)) $(INC.$(MAIN)) -I$(boost.root) $(INCS)

showx:
	ls -l $(boost.replacements)/unfold_left
	echo "USE_UNFOLD_LEFT=$(USE_UNFOLD_LEFT)"
	echo "INC.unfold_left.D=$(INC.unfold_left.D)"
	echo $(INC.unfold_left.$(USE_UNFOLD_LEFT))
show:
	pushd $(boost.git.spirit)
	popd
show-diff:
	diff $(boost.replacements)/issue707/boost/spirit/home/x3/support/traits/is_substitute.hpp $(boost.root.1_78_0)/boost/spirit/home/x3/support/traits/is_substitute.hpp
show-diff.out:
	diff breakage-is_substitute-variants-DUSE_ISSUE707_VAR2.out breakage-is_substitute-variants-UUSE_ISSUE707_VAR2.out>breakage-is_substitute-variants-DU-USE_ISSUE707_VAR2.diff
find-hidden:
	find $(boost.replacements)/debug/ -name \*.hpp~hide

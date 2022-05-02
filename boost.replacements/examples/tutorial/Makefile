SHELL := /bin/bash
############PATH := $(USER)/bin:$(PATH)
BOOST_ROOT_VERSION=76
BOOST_ROOT_VERSION=78
boost.root=$(boost.root.1_$(BOOST_ROOT_VERSION)_0)
MAIN=calc1
#MAIN=calc1-transform_attribute_recur
USE_DEBUG?=U
USE_UNFOLD_LEFT?=U
USE_TRANSFORM_ATTRIBUTE_RULE2RHS?=$(USE_UNFOLD_LEFT)
USE_DEMANGLE_FMT=U
DEFINES.calc1=\
 -DUSE_VARIANT_SUB_1_78_0\
 -DUSE_IS_SUBSTITUTE_1_78_0\
 -UUSE_ISSUE707_VAR2\
 -UUSE_UNFOLD_LEFT\
 -UTRANSFORM_ATTRIBUTE_RULE2RHS\
 -fno-diagnostics-show-option\
  ###
DEFINES.calc1-transform_attribute_recur=\
 -DUSE_VARIANT_SUB_1_78_0\
 -DUSE_IS_SUBSTITUTE_1_78_0\
 -UUSE_ISSUE707_VAR2\
 -$(USE_UNFOLD_LEFT)USE_UNFOLD_LEFT\
 -$(USE_TRANSFORM_ATTRIBUTE_RULE2RHS)TRANSFORM_ATTRIBUTE_RULE2RHS\
 -fno-diagnostics-show-option\
  ###
INC.unfold_left.D=-I$(boost.replacements)/unfold_left
INC.unfold_left.U=
INC.unfold_left=$(INC.unfold_left.$(USE_UNFOLD_LEFT))
INC.transform_attribute.D=-I$(boost.replacements)/transform_attribute
INC.transform_attribute.U=
INC.transform_attribute=$(INC.transform_attribute.$(USE_TRANSFORM_ATTRIBUTE_RULE2RHS))
INC.debug.D=-I$(boost.replacements)/debug
INC.debug.U=
INC.debug=$(INC.debug.$(USE_DEBUG))
HOW=clangxx11_0_0
#HOW=gxx10_2_0

include $(HOME)/prog_dev/root.imk
boost.replacements=$(HOME)/prog_dev/boost.org/boost.replacements
INC.common=-I$(boost.replacements)/common -I$(boost.additions)

INC.issue707=-I$(boost.replacements)/issue707

#INC.calc1=$(INC.debug) -I$(boost.replacements)/fusion $(INC.unfold_left.D) $(INC.common)

INCS:=$(DEFINES.$(MAIN)) $(INC.$(MAIN)) -I$(boost.root) $(INCS)

calc1.run: $(BUILD_HOW_WHAT.dir)/calc1.exe
	cat calc1.inp|$(BUILD_HOW_WHAT.dir)/calc1.exe>calc1.out

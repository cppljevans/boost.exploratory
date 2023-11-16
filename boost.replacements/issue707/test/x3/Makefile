SHELL := /bin/bash
############PATH := $(USER)/bin:$(PATH)
BOOST_ROOT_VERSION=80
boost.root=$(boost.root.1_$(BOOST_ROOT_VERSION)_0)
MAIN=issue707
INC.spirit.test=-I$(boost.root)/libs/spirit/test/x3
USE_ISSUE707:=D
INC.issue707.D=-I$(boost.replacements)/issue707/include
INC.issue707.U=
INC.issue707=$(INC.issue707.$(USE_ISSUE707)) $(INC.spirit.test)
DEFINE.issue707=-DBOOST_NO_CXX98_FUNCTION_BASE

include $(HOME)/prog_dev/root.imk
INCS:=$(DEFINE.$(MAIN)) $(INC.$(MAIN)) -I$(boost.root) $(INCS)

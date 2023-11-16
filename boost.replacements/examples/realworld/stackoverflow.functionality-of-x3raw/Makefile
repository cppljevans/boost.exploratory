SHELL := /bin/bash
############PATH := $(USER)/bin:$(PATH)
OUT?=run
BOOST_ROOT_VERSION=76
#BOOST_ROOT_VERSION=78
#BOOST_ROOT_VERSION=80
boost.root=$(boost.root.1_$(BOOST_ROOT_VERSION)_0)
MAIN=example

HOW=clangxx14_0_3
#HOW=gxx10_2_0

include $(HOME)/prog_dev/root.imk
AVOID_HASH_HPP_WARNS=-DBOOST_NO_CXX98_FUNCTION_BASE

INCS := $(AVOID_HASH_HPP_WARNS) -I$(boost.additions) -I$(boost.replacements)/move_to -I$(boost.root) $(INCS)

show:
	ls -l $(boost.root)

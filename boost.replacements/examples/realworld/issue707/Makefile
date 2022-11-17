SHELL := /bin/bash
############PATH := $(USER)/bin:$(PATH)
OUT?=run
BOOST_ROOT_VERSION=76
#BOOST_ROOT_VERSION=78
#BOOST_ROOT_VERSION=80
boost.root=$(boost.root.1_$(BOOST_ROOT_VERSION)_0)
#MAIN=stackoverflow_73673899
#MAIN=stackoverflow_73673899-1
#MAIN=1.78breakage-original
#MAIN=1.78breakage
#MAIN=1.78breakage-simplified
#MAIN=1.78breakage-simplified2
#MAIN=1.78breakage-simplified3
#MAIN=1.78breakage-simplified4
#MAIN=1.78breakage-simplified5
#MAIN=ast_container_value
#MAIN=attribute_of
#MAIN=1.78breakage-is_substitute-variants
#MAIN=alternative
MAIN=issue707-test
#MAIN=unique
#MAIN=alternative
#MAIN=alternative-debug
#MAIN=alternative-debug-transform
#MAIN=variant_io
#MAIN=is_variant
#MAIN=ArithExpr-original
#MAIN=ArithExpr-apply_visitor
#MAIN=ArithExpr-hanaLess
#MAIN=equal_types_variant_sequence
#MAIN=component_types_substitutable
#MAIN=fusion_result_of_at_c
boost.git.spirit=$(boost.top)/git_download/spirit/spirit/include
USE_SANDBOX?=U
USE_IS_SUBSTITUTE_IMPL_VAR2?=D
USING_IS_SUBSTITUTE_IMPL_VAR2=$(findstring D,$(USE_IS_SUBSTITUTE_IMPL_VAR2))
MOVE_TO_SRC_LOGDATA=$(if $(USING_IS_SUBSTITUTE_IMPL_VAR2),U,D)
USE_STD_VARIANT?=U
USE_DEBUG?=D
USING_DEBUG:=$(filter D,$(USE_DEBUG))
USE_ISSUE707:=D
USE_UNFOLD_LEFT?=U
USE_COLLAPSE_LEFT?=U
USE_IS_SAME_ENOUGH?=U
TRANSFORM_ATTRIBUTE_RULE2RHS?=$(USE_UNFOLD_LEFT)
USE_DEMANGLE_FMT=U
SHOW_ISSUE707_ERROR?=D

DEFINE.component_types_substitutable=\
 -DUSE_VARIANT_SUB_1_78_0\
 -DUSE_IS_SUBSTITUTE_1_78_0\
 -$(USE_IS_SUBSTITUTE_IMPL_VAR2)USE_IS_SUBSTITUTE_IMPL_VAR2\
  ###
DEFINE.stackoverflow_73673899=\
 -fno-diagnostics-show-option\
  ###
DEFINE.stackoverflow_73673899-1=\
 -fno-diagnostics-show-option\
  ###
DEFINE.1.78breakage-original=\
 -DUSE_VARIANT_SUB_1_78_0\
 -DUSE_IS_SUBSTITUTE_1_78_0\
 -fno-diagnostics-show-option\
  ###
DEFINE.1.78breakage=\
 -DUSE_VARIANT_SUB_1_78_0\
 -DUSE_IS_SUBSTITUTE_1_78_0\
 -$(MOVE_TO_SRC_LOGDATA)MOVE_TO_SRC_LOGDATA\
 -$(USE_UNFOLD_LEFT)USE_UNFOLD_LEFT\
 -$(SHOW_ISSUE707_ERROR)SHOW_ISSUE707_ERROR\
 -$(USE_IS_SAME_ENOUGH)USE_IS_SAME_ENOUGH\
 -$(TRANSFORM_ATTRIBUTE_RULE2RHS)TRANSFORM_ATTRIBUTE_RULE2RHS\
  -fno-diagnostics-show-option\
  ###
DEFINE.1.78breakage-simplified=\
 -UUSE_VARIANT_SUB_1_78_0\
 -DUSE_IS_SUBSTITUTE_1_78_0\
 -UMOVE_TO_SRC_LOGDATA\
 -UUSE_UNFOLD_LEFT\
 -fno-diagnostics-show-option\
  ###
DEFINE.1.78breakage-simplified2=\
 -DUSE_VARIANT_SUB_1_78_0\
 -DUSE_IS_SUBSTITUTE_1_78_0\
 -UMOVE_TO_SRC_LOGDATA\
 -UUSE_UNFOLD_LEFT\
 -fno-diagnostics-show-option\
  ###
DEFINE.1.78breakage-simplified3=\
 -DUSE_VARIANT_SUB_1_78_0\
 -DUSE_IS_SUBSTITUTE_1_78_0\
 -$(USE_UNFOLD_LEFT)USE_UNFOLD_LEFT\
 -$(TRANSFORM_ATTRIBUTE_RULE2RHS)TRANSFORM_ATTRIBUTE_RULE2RHS\
 -fno-diagnostics-show-option\
  ###
DEFINE.1.78breakage-simplified4=\
 -DUSE_VARIANT_SUB_1_78_0\
 -DUSE_IS_SUBSTITUTE_1_78_0\
 -$(SHOW_ISSUE707_ERROR)SHOW_ISSUE707_ERROR\
 -$(USE_IS_SUBSTITUTE_IMPL_VAR2)USE_IS_SUBSTITUTE_IMPL_VAR2\
 -$(USE_STD_VARIANT)USE_STD_VARIANT\
 -$(MOVE_TO_SRC_LOGDATA)MOVE_TO_SRC_LOGDATA\
 -$(USE_UNFOLD_LEFT)USE_UNFOLD_LEFT\
 -$(USE_COLLAPSE_LEFT)USE_COLLAPSE_LEFT\
 -$(USE_IS_SAME_ENOUGH)USE_IS_SAME_ENOUGH\
 -$(TRANSFORM_ATTRIBUTE_RULE2RHS)TRANSFORM_ATTRIBUTE_RULE2RHS\
 -fno-diagnostics-show-option\
  ###
DEFINE.ast_container_value=$(DEFINE.1.78breakage-simplified4)
DEFINE.1.78breakage-simplified5=\
  $(DEFINE.1.78breakage-simplified4)\
  ###
DEFINE.alternative=\
 $(DEFINE.1.78breakage-simplified4)\
  ###
DEFINE.issue707-test=\
  $(AVOID_HASH_HPP_WARNS)\
  ###
# $(DEFINE.1.78breakage-simplified4)\
DEFINE.ArithExpr-original=\
  ###
DEFINE.ArithExpr-apply_visitor=\
  -UUSE_TRACE_SCOPE
  ###
DEFINE.ArithExpr-hanaLess=\
  ###
DEFINE.alternative-debug=\
 -DUSE_VARIANT_SUB_1_78_0\
 -DUSE_IS_SUBSTITUTE_1_78_0\
 -UUSE_UNFOLD_LEFT\
 -UTRANSFORM_ATTRIBUTE_RULE2RHS\
 -UUSE_SEQ_PARSE_STD_VECTOR_SPECIALIZATION\
 -fno-diagnostics-show-option\
  ###
DEFINE.alternative-debug-transform=\
 -DUSE_VARIANT_SUB_1_78_0\
 -DUSE_IS_SUBSTITUTE_1_78_0\
 -DUSE_UNFOLD_LEFT\
 -DTRANSFORM_ATTRIBUTE_RULE2RHS\
 -UUSE_SEQ_PARSE_STD_VECTOR_SPECIALIZATION\
 -fno-diagnostics-show-option\
  ###
DEFINE.attribute_of=\
 -DUSE_VARIANT_SUB_1_78_0\
 -DUSE_IS_SUBSTITUTE_1_78_0\
 -$(USE_IS_SUBSTITUTE_IMPL_VAR2)USE_IS_SUBSTITUTE_IMPL_VAR2\
 -$(USE_DEBUG)USE_DEBUG\
 -$(USE_UNFOLD_LEFT)USE_UNFOLD_LEFT\
 -$(USE_IS_SAME_ENOUGH)USE_IS_SAME_ENOUGH\
  ###
DEFINE.1.78breakage-is_substitute-variants=\
 -DUSE_VARIANT_SUB_1_78_0\
 -DUSE_IS_SUBSTITUTE_1_78_0\
 -$(USE_DEBUG)USE_DEBUG\
 -fno-diagnostics-show-option\
  ###

INC.transform_attribute.D=-I$(boost.replacements)/transform_attribute
INC.transform_attribute.U=
INC.unfold_left.D=-I$(boost.replacements)/unfold_left
INC.unfold_left.U=
INC.unfold_left=$(INC.unfold_left.D)
INC.debug.D=-I$(boost.replacements)/debug
INC.debug.U=
INC.debug=$(INC.debug.$(USE_DEBUG))
INC.boost.sandbox.D=-I$(boost.root.1_80_0)/sandbox/lje/spirit-experiments/include
INC.boost.sandbox.U=
INC.boost.sandbox=$(INC.boost.sandbox.$(USE_SANDBOX))
INC.issue707.D=-I$(boost.replacements)/issue707/include
INC.issue707.U=
INC.issue707=$(INC.issue707.$(USE_ISSUE707))

HOW=clangxx14_0_3
#HOW=gxx10_2_0

include $(HOME)/prog_dev/root.imk
INC.unique=-I$(boost.additions)
INC.ignore_warnings=-I$(boost.replacements)/ignore_warnings
AVOID_HASH_HPP_WARNS=$(INC.ignore_warnings)
AVOID_HASH_HPP_WARNS=-DBOOST_NO_CXX98_FUNCTION_BASE

INC.common=$(AVOID_HASH_HPP_WARNS) -I$(boost.replacements)/common -I$(boost.additions)
#INC.seqsize1=-I$(boost.replacements)/FUSION_ADAPT_seqsize1
INC.magic_enum=-I$(magic_enum.github) -I$(magic_enum.io)

INC.is_variant=$(INC.common)
INC.stackoverflow_73673899=$(INC.boost.sandbox)
INC.stackoverflow_73673899-1=$(INC.boost.sandbox)
#INC.stackoverflow_73673899=$(INC.boost.sandbox) -I$(boost.replacements)/common 
INC.1.78breakage-original=$(INC.boost.sandbox) -I$(boost.replacements)/common 
INC.1.78breakage=$(INC.boost.sandbox) $(INC.common)
INC.1.78breakage-simplified=$(INC.debug) $(INC.unfold_left.D) $(INC.common)
INC.1.78breakage-simplified2=$(INC.debug) $(INC.unfold_left.D) $(INC.common)
#INC.1.78breakage-simplified2=$(INC.debug) $(INC.unfold_left.D) $(INC.common)
#INC.1.78breakage-simplified3=-I$(boost.additions)
#INC.1.78breakage-simplified3=$(INC.debug) $(INC.unfold_left.$(USE_UNFOLD_LEFT)) $(INC.transform_attribute.$(TRANSFORM_ATTRIBUTE_RULE2RHS)) $(INC.common)
INC.1.78breakage-is_substitute-variants=$(INC.common)
INC.1.78breakage-is_substitute-variants=$(INC.magic_enum) -I$(magic_enum.io) $(INC.debug) $(INC.common)

INC.1.78breakage-simplified4=\
  $(INC.unfold_left)\
  $(INC.debug)\
  $(INC.common)\
  ###

#  $(INC.issue707)\
#  $(INC.transform_attribute.$(TRANSFORM_ATTRIBUTE_RULE2RHS))\

INC.component_types_substitutable=\
  $(INC.debug)\
  $(INC.common)\
  ###

#  $(INC.issue707)\

INC.ast_container_value=$(INC.1.78breakage-simplified4)
INC.fusion=-I$(boost.replacements)/fusion
INC.1.78breakage-simplified5=$(INC.1.78breakage-simplified4)
#INC.alternative=$(INC.issue707) $(INC.fusion) $(INC.unfold_left.D) $(INC.debug) $(INC.common)
INC.alternative=$(INC.fusion) $(INC.1.78breakage-simplified4)
INC.issue707-test=$(INC.issue707)
INC.ArithExpr-original=\
  $(INC.ignore_warnings)\
  ###
INC.ArithExpr-apply_visitor=\
  -I$(boost.replacements)/spirit_variant\
  $(INC.common)\
  ###
INC.ArithExpr-hanaLess=\
  $(INC.ignore_warnings)\
  $(INC.issue707)\
  ###
#-I$(boost.replacements)/fusion
INC.alternative-debug=$(INC.debug) $(INC.fusion) $(INC.unfold_left.D) $(INC.common)
INC.alternative-debug-transform=$(INC.debug) -I$(boost.replacements)/fusion $(INC.unfold_left) $(INC.common)
INC.expectation_operator=$(INC.common)
INC.unique=$(INC.common)
INC.equal_types_variant_sequence=$(INC.common)
INC.fusion_result_of_at_c=$(INC.common)
#INCS:=$(INC.$(MAIN)) $(DEFINE.$(MAIN)) -I$(boost.root) $(INCS)
INCS:=$(DEFINE.$(MAIN)) $(INC.$(MAIN)) -I$(boost.root) $(INCS)
#FLAGS_AS_PATH=-$(USE_UNFOLD_LEFT)USE_UNFOLD_LEFT-$(MOVE_TO_SRC_LOGDATA)MOVE_TO_SRC_LOGDATA-$(USE_IS_SAME_ENOUGH)USE_IS_SAME_ENOUGH-$(SHOW_ISSUE707_ERROR)SHOW_ISSUE707_ERROR
FLAGS_AS_PATH=-$(MOVE_TO_SRC_LOGDATA)MOVE_TO_SRC_LOGDATA-$(USE_IS_SUBSTITUTE_IMPL_VAR2)USE_IS_SUBSTITUTE_IMPL_VAR2

showx:
	ls -l $(boost.replacements)/unfold_left
	echo "USE_UNFOLD_LEFT=$(USE_UNFOLD_LEFT)"
	echo "INC.unfold_left.D=$(INC.unfold_left.D)"
	echo $(INC.unfold_left.$(USE_UNFOLD_LEFT))
show-diff:
	diff $(boost.replacements)/issue707/boost/spirit/home/x3/support/traits/is_substitute.hpp $(boost.root.1_78_0)/boost/spirit/home/x3/support/traits/is_substitute.hpp
show-diff.out:
	diff breakage-is_substitute-variants-DUSE_ISSUE707_VAR2.out breakage-is_substitute-variants-UUSE_ISSUE707_VAR2.out>breakage-is_substitute-variants-DU-USE_ISSUE707_VAR2.diff
find-hidden:
	find $(boost.replacements)/debug/ -name \*.hpp~hide
make_out:
	-make --always-make $(OUT)_out > $(MAIN).$(FLAGS_AS_PATH).make_$(OUT) 2>&1
compile-version:
	$(COMPILER.$(HOW)) --version
	$(COMPILER.$(HOW)) -I$(boost.root.1_80_0) -std=c++17 -c $(MAIN).cpp
show:
	@echo "USING_IS_SUBSTITUTE_IMPL_VAR2=$(USING_IS_SUBSTITUTE_IMPL_VAR2)"
	@echo "MOVE_TO_SRC_LOGDATA=$(MOVE_TO_SRC_LOGDATA)"

preprocessor_quoter:
	$(COMPILER.$(HOW)) -E preprocessor_quoter.cpp

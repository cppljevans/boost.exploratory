SHELL := /bin/bash
############
BOOST_ROOT_VERSION=80

boost.root=$(boost.root.1_$(BOOST_ROOT_VERSION)_0)
PATH := $(USER)/bin:$(PATH)
MAIN=Edit_2021-05-30.original
#MAIN=Edit_2021-05-30.adapted_variant
#MAIN=Edit_2021-05-30.use_boost_variant
#MAIN=Edit_2021-05-30.rule_attr_rhs
#MAIN=Edit_2021-05-30.rule_attr_rhs.xform2rhs
#MAIN=templ_expr_demangle_fmt
#MAIN=std_variant_sout
#MAIN=macros
#MAIN=Edit_2021-05-30.start_aof_soln
WITHOUT_WHITE?=U
WHITE_AOF?=U
BRACED_AOF?=U
START_AOF?=D
USE_START_ACTION?=U
START_TRANSFORM_ATTRIBUTE=D

TRANSFORM_ATTRIBUTE_RULE2RHS?=D
USE_IS_SUBSTITUTE_IMPL_VAR2?=U
USE_STD_VARIANT?=U
USE_DEBUG?=D
USE_UNFOLD_LEFT?=U
USE_ATTRIBUTE_OF_NO_COLLAPSE?=U
USE_DEMANGLE_FMT=D
USE_TRANSFORM_ATTRIBUTE_SPECIALIZATION=U
USE_UNFOLD_LEFT?=U
USE_COLLAPSE_LEFT?=U
USE_IS_SAME_ENOUGH?=U
TRANSFORM_ATTRIBUTE_RULE2RHS?=$(USE_UNFOLD_LEFT)
DEFINE.Edit_2021-05-30.original=\
 -fno-diagnostics-show-option\
  ##DEFINE.*
DEFINE.Edit_2021-05-30.adapted_variant=\
 -DUSE_VARIANT_SUB_1_78_0\
 -DUSE_IS_SUBSTITUTE_1_78_0\
 -$(USE_IS_SUBSTITUTE_IMPL_VAR2)USE_IS_SUBSTITUTE_IMPL_VAR2\
 -$(USE_STD_VARIANT)USE_STD_VARIANT\
 -$(USE_UNFOLD_LEFT)USE_UNFOLD_LEFT\
 -$(USE_COLLAPSE_LEFT)USE_COLLAPSE_LEFT\
 -fno-diagnostics-show-option\
  ##DEFINE.*
DEFINE.Edit_2021-05-30.use_boost_variant=\
  $(DEFINE.Edit_2021-05-30.adapted_variant)\
  ##DEFINE.*
DEFINE.Edit_2021-05-30.rule_attr_rhs=\
 -$(WITHOUT_WHITE)WITHOUT_WHITE\
 -$(START_AOF)START_AOF\
 -$(USE_START_ACTION)USE_START_ACTION\
 -$(USE_ATTRIBUTE_OF_NO_COLLAPSE)USE_ATTRIBUTE_OF_NO_COLLAPSE\
 -$(USE_UNFOLD_LEFT)USE_UNFOLD_LEFT\
 -fno-diagnostics-show-option\
 ##DEFINE.*
DEFINE.Edit_2021-05-30.rule_attr_rhs.xform2rhs=\
 -$(WITHOUT_WHITE)WITHOUT_WHITE\
 -$(WHITE_AOF)WHITE_AOF\
 -$(BRACED_AOF)BRACED_AOF\
 -$(START_AOF)START_AOF\
 -$(USE_DEMANGLE_FMT)USE_DEMANGLE_FMT\
 -$(TRANSFORM_ATTRIBUTE_RULE2RHS)TRANSFORM_ATTRIBUTE_RULE2RHS\
 -$(USE_UNFOLD_LEFT)USE_UNFOLD_LEFT\
 -$(USE_TRANSFORM_ATTRIBUTE_SPECIALIZATION)USE_TRANSFORM_ATTRIBUTE_SPECIALIZATION\
 -$(START_TRANSFORM_ATTRIBUTE)START_TRANSFORM_ATTRIBUTE\
 -DTRANSFORM_TRACE\
 -fno-diagnostics-show-option\
 ##DEFINE.*
DEFINE.macros=\
 -$(WITHOUT_WHITE)WITHOUT_WHITE\
 -$(START_AOF)START_AOF\
 ##DEFINE.*
DEFINES=$(DEFINE.$(MAIN))

HOW=clangxx14_0_3
#HOW=gxx10_2_0

DIR.git := $(shell dirup_dir_file.pl $(PWD) .git)
include $(HOME)/prog_dev/root.imk
DIR.algebra= $(HOME)/prog_dev/c++/ljeLocal/libs/gram_algebra
INCS.algebra.D=\
    -I$(boost.replacements)/char_set_constexpr\
    -I$(bitset2.include)\
    -I$(DIR.algebra)\
    -I$(magic_enum.github)\
    -I$(magic_enum.io)\
    -I$(boost.additions)\
    -I$(compstore.root)\
    ##INC.*
INCS.algebra.U=
INC.boost.additions=-I$(boost.additions)
#INC.move_to=-I$(boost.replacements)/FUSION_ADAPT_seqsize1
INC.common=-I$(boost.replacements)/common
INC.transform_attribute=-I$(boost.replacements)/transform_attribute
INC.unfold_left.D=-I$(boost.replacements)/unfold_left
INC.unfold_left.U=
INC.unfold_left=$(INC.unfold_left.D)
INC.debug.D=-I$(boost.replacements)/debug
INC.debug.U=
INC.debug=$(INC.debug.D)
INC.attribute_of.D=-I$(boost.replacements)/attribute_of
INC.attribute_of.U=
INC.Edit_2021-05-30.original=\
  ##$(INC.common)\
  ##INC.*
INC.Edit_2021-05-30.adapted_variant=\
  $(INC.boost.additions)\
  $(INC.debug)\
  $(INC.common)\
  $(INC.unfold_left)\
  $(INC.transform_attribute.$(TRANSFORM_ATTRIBUTE_RULE2RHS))\
  ##INC.*
INC.Edit_2021-05-30.use_boost_variant=\
  $(INC.Edit_2021-05-30.adapted_variant)\
  ##INC.*
#INC.Edit_2021-05-30.rule_attr_rhs.xform2rhs=$(INCS.algebra.$(USE_DEMANGLE_FMT)) $(INC.common) $(INC.unfold_left.D) $(INC.transform_attribute) $(INC.boost.additions)
INC.Edit_2021-05-30.rule_attr_rhs.xform2rhs=$(INC.common) $(INC.unfold_left.D) $(INC.transform_attribute)  $(INC.boost.additions)
INC.templ_expr_demangle_fmt=$(INC.boost.additions) $(INCS.algebra.D) 
INC.std_variant_sout=$(INC.unfold_left) $(INC.transform_attribute) $(INC.boost.additions)
INC.std_variant_sout=$(INC.common)
INCS:=$(INC.$(MAIN)) -I$(boost.root) $(INCS)

templ_expr_fmtx3-stdin.exe=c:/msys64/tmp/build/clangxx11_0_0/c++/ljeLocal/libs/gram_algebra/gram_algebra/examples/realworld/templ_expr_fmt/templ_expr_fmtx3-stdin.exe
compare_collapse:
	make --always-make USE_ATTRIBUTE_OF_NO_COLLAPSE=D run
	make --always-make USE_ATTRIBUTE_OF_NO_COLLAPSE=U run
macros:
	-$(MAKE) MAIN=macros --always-make WITHOUT_WHITE=D START_AOF=U o
	-$(MAKE) MAIN=macros --always-make WITHOUT_WHITE=U START_AOF=U o
show:
	@echo "COMPILE.HOW=$(COMPILE.$(HOW))"
	@echo "MAIN=$(MAIN)"
	@echo "INC.$(MAIN)=$(INC.$(MAIN))"
camomilla:
	cat compilation.out|$@ -d4 >compilation.$@
fmtx3-stdin:
	cat errmsg.out|$(templ_expr_fmtx3-stdin.exe)
diff:
	diff --text Exposed.cpp Transformed.cpp

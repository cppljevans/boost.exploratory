SHELL := /bin/bash
############
PATH := $(USER)/bin:$(PATH)
MAIN=Edit_2021-05-30
MAIN=Edit_2021-05-30.rule_attr_rhs
MAIN=Edit_2021-05-30.rule_attr_rhs.xform2rhs
#MAIN=templ_expr_demangle_fmt
#MAIN=std_variant_sout
#MAIN=macros
#MAIN=Edit_2021-05-30.start_aof_soln
WITHOUT_WHITE?=U
WHITE_AOF?=U
BRACED_AOF?=U
START_AOF?=D
USE_START_ACTION?=U
TRANSFORM_ATTRIBUTE_RULE2RHS?=D
USE_UNFOLD_LEFT?=D
USE_ATTRIBUTE_OF_NO_COLLAPSE?=U
USE_DEMANGLE_FMT=D
USE_TRANSFORM_ATTRIBUTE_SPECIALIZATION=D
START_TRANSFORM_ATTRIBUTE=D
DEFINES.Edit_2021-05-30.rule_attr_rhs=\
 -$(WITHOUT_WHITE)WITHOUT_WHITE\
 -$(START_AOF)START_AOF\
 -$(USE_START_ACTION)USE_START_ACTION\
 -$(USE_ATTRIBUTE_OF_NO_COLLAPSE)USE_ATTRIBUTE_OF_NO_COLLAPSE\
 -$(USE_UNFOLD_LEFT)USE_UNFOLD_LEFT\
 -fno-diagnostics-show-option\
 #DEFINES
DEFINES.Edit_2021-05-30.rule_attr_rhs.xform2rhs=\
 -$(WITHOUT_WHITE)WITHOUT_WHITE\
 -$(WHITE_AOF)WHITE_AOF\
 -$(BRACED_AOF)BRACED_AOF\
 -$(START_AOF)START_AOF\
 -$(USE_DEMANGLE_FMT)USE_DEMANGLE_FMT\
 -$(TRANSFORM_ATTRIBUTE_RULE2RHS)TRANSFORM_ATTRIBUTE_RULE2RHS\
 -$(USE_UNFOLD_LEFT)USE_UNFOLD_LEFT\
 -$(USE_TRANSFORM_ATTRIBUTE_SPECIALIZATION)USE_TRANSFORM_ATTRIBUTE_SPECIALIZATION\
 -$(START_TRANSFORM_ATTRIBUTE)START_TRANSFORM_ATTRIBUTE\
 -fno-diagnostics-show-option\
 #DEFINES
DEFINES.macros=\
 -$(WITHOUT_WHITE)WITHOUT_WHITE\
 -$(START_AOF)START_AOF\
 #DEFINES
DEFINES=$(DEFINES.$(MAIN))

HOW=clangxx11_0_0
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
    ###
INCS.algebra.U=
INC.boost.additions=-I$(boost.additions)
#INC.move_to=-I$(boost.replacements)/FUSION_ADAPT_seqsize1
INC.common=-I$(boost.replacements)/common
INC.transform_attribute=-I$(boost.replacements)/transform_attribute
INC.unfold_left.D=-I$(boost.replacements)/unfold_left
INC.unfold_left.U=
INC.attribute_of.D=-I$(boost.replacements)/attribute_of
INC.attribute_of.U=
INC.Edit_2021-05-30.rule_attr_rhs=$(INC.move_to) $(INC.attribute_of.$(USE_ATTRIBUTE_OF_NO_COLLAPSE)) $(INC.boost.additions)
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
	@echo "INC.$(MAIN)=$(INC.$(MAIN))"
camomilla:
	cat compilation.out|$@ -d4 >compilation.$@
fmtx3-stdin:
	cat errmsg.out|$(templ_expr_fmtx3-stdin.exe)
diff:
	diff --text Exposed.cpp Transformed.cpp

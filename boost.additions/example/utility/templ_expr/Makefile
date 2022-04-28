SHELL := /bin/bash
############
PATH := $(USER)/bin:$(PATH)
MAIN=fmt-insource
MAIN=fmt-stdin
MAIN=fmt-compiler-out
#MAIN=xpr-insource
HOW=clangxx11_0_0
#HOW=gxx10_2_0

DIR.root := $(shell dirup_dir_file.pl $(PWD) root.imk)
include $(DIR.root)/root.imk
INC.boost.additions=-I$(boost.additions)
INCS:=$(INC.$(MAIN)) $(INC.boost.additions) -I$(boost.root) $(INCS)

fmt-stdin=spaces
fmt-stdin=comments
#fmt-stdin=const_char_ptr
#fmt-stdin=std_string
#fmt-stdin=scratch
#fmt-stdin=Edit_2021-05-30.rule_attr_rhs.xform2rhs
#fmt-stdin=Edit_2021-05-30.rule_attr_rhs.xform2rhs.gap
#fmt-stdin=Edit_2021-05-30.rule_attr_rhs.xform2rhs.simplified
#fmt-stdin=Edit_2021-05-30.rule_attr_rhs.xform2rhs.simplified-most
#fmt-stdin=unfold_left-sequence.hpp_79_55
#fmt-stdin=transform_parser_attribute
fmt-stdin=plus_32_26
fmt-stdin: $(BUILD_HOW_WHAT.dir)/fmt-stdin.exe
	cat fmt-stdin.$(fmt-stdin).inp|$(BUILD_HOW_WHAT.dir)/fmt-stdin.exe
#>fmt-stdin.$(fmt-stdin).out

fmt-compiler-out=scratch
fmt-compiler-out: $(BUILD_HOW_WHAT.dir)/fmt-compiler-out.exe
	cat fmt-compiler-out.$(fmt-compiler-out).inp|$(BUILD_HOW_WHAT.dir)/$@.exe
xxx: $@.txt
	echo $@
show:
	echo "BUILD_HOW_WHAT.dir=$(BUILD_HOW_WHAT.dir)"
find-x3:
	grep -e '/x3' *.cpp

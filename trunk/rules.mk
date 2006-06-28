ADON_SRC:=$(strip $(shell pwd))

# select between /bin, /usr/bin, etc..
# I first thought about sticking /usr in $(DESTDIR) if needed
# but then changed my mind
BIN:=/bin
LIB:=/lib
SBIN:=/sbin

include Rules-$(ADON_BUILD).mk
include adon-files.mk
# initialize accumulators
ADON_LIBDIRS:=
adon_TARGETS:=
INSTALL_HOOKS:=
adon_VARS:=
adon_INSTALL_TARGETS:=
adon_CHECKS:=
adon_DIST_FILES:=$(adon_MAINTAINERCLEAN_FILES) \
    Makefile Makefile.real rules.mk vardefs.mk \
    $(wildcard Rules-*.mk) \
    $(wildcard env-tests/*.c) $(wildcard env-tests/*.mk) \
    $(wildcard env-tests/*.cpp) $(wildcard env-tests/*.sh)
adon_CLEAN_FILES:=log
adon_DISTCLEAN_FILES:=

# variables in list below will be reset before loading
# each makefile
adon_RESETVARS:=ADON_SUBDIRS LOCAL_CPPFLAGS LOCAL_LDFLAGS \
    LOCAL_CFLAGS LOCAL_CXXFLAGS TESTS
adon_CXXEXTS:=cpp C cc
adon_CEXTS:=c
adon_IN_SUBST:=PACKAGE_VERSION VERSION_MAJOR \
    VERSION_MINOR VERSION_MICRO GENERATED_FILE \
    PACKAGE VERSION PACKAGE_VERSION_TR
GENERATED_FILE:=/* This is a generated file. Do not edit */
adon_DIST_DIR:=$(PACKAGE)-$(PACKAGE_VERSION)
adon_DIST_ARCHIVE:=$(PACKAGE)-$(PACKAGE_VERSION).tar.bz2

###################################################
# PACKAGE_VERSION can contain letters numbers and dots
ifdef PACKAGE_VERSION
PACKAGE_VERSION_TR:=$(subst .,_,$(PACKAGE_VERSION))
TOP_CPPFLAGS+=-DPACKAGE_VERSION=$(PACKAGE_VERSION) -DPACKAGE_VERSION_TR=$(PACKAGE_VERSION_TR)
endif

###################################################
$(if $(ADON_NODEPS),$(eval include $(wildcard $(ADON_BUILD)/env-tests/*.cache)),\
 $(eval include env-tests/env-tests.mk))

###################################################
adon_IN_SUBST_SCRIPT:=$(foreach subst,$(adon_IN_SUBST), -e 's|\@$(subst)\@|$($(subst))|')
%: %.in rules.mk
	sed $(adon_IN_SUBST_SCRIPT) $< >$@
%.h: %.h.in rules.mk
	sed $(adon_IN_SUBST_SCRIPT) $< >$@

###################################################
#
### WARNING: watch out for subtelities
### since this will be evalueated differently at
### different time (see recursive vs. simple make
### variables in GNU make info pages)
all_CXXFLAGS=$(CXXFLAGS) $(CPPFLAGS) $(TOP_CPPFLAGS) \
    $(TOP_CXXFLAGS) $(LOCAL_CXXFLAGS) $(LOCAL_CPPFLAGS) \
    $(TARGET_INCLUDES) \
    $($(1)_CXXFLAGS) $($(1)_CPPFLAGS)

all_CFLAGS=$(CFLAGS) $(CPPFLAGS) $(TOP_CFLAGS) \
    $(TOP_CPPFLAGS) $(LOCAL_CFLAGS) $(LOCAL_CPPFLAGS) \
    $(TARGET_INCLUDES) \
    $($(1)_CFLAGS) $($(1)_CPPFLAGS)

ADON_LIBFLAGS=$(addprefix -L,$(ADON_LIBDIRS))

# funky conditional evaluation below means that
# target will be considered if either
#  1. target_CONDITIONAL is not set at all
#        or
#  2. variable set in target_CONDITIONAL expands to 1
define do_conditional
$(eval $(if $($(1)_CONDITIONAL),tmp_CONDITIONAL:=$($($(1)_CONDITIONAL)),tmp_CONDITIONAL:=1))
$(eval tmp_CONDITIONAL:=$(filter 1,$(tmp_CONDITIONAL)))
endef

define obj_list
	$(foreach ext,$(adon_CEXTS) $(adon_CXXEXTS),$(patsubst %.$(ext), %.o, $(filter %.$(ext), $($(1)_SOURCES))))
endef

define dep_list
	$(patsubst %, %.dep, $($(1)_SOURCES))
endef

tmp_CXXPAT:=$(addprefix %.,$(adon_CXXEXTS))
tmp_CPAT:=$(addprefix %.,$(adon_CEXTS))

# called with:
# $(call obj_deps, targetname, objectlist, path)
define objdep_template
$(ADON_BUILD)/$(2).dep: $(2) .predep
	mkdir -p $$(dir $$@)
	$(foreach ext,$(filter $(tmp_CXXPAT),$(2)),\
	    $(CXX) $(all_CXXFLAGS) -MM -MF $$@ $$< \
		-MT $$(basename $$(basename $$@)).o -MT $$@ \
	)
	$(foreach ext,$(filter $(tmp_CPAT),$(2)),\
	    $(CC) $(all_CFLAGS) -MM -MF $$@ $$< \
		-MT $$(basename $$(basename $$@)).o -MT $$@\
	)
endef

define record_distfiles
$(foreach source,$(addprefix $(2)/,$($(1)_SOURCES)),$(eval adon_DIST_FILES+=$(source)))
endef

define obj_deps_cond
$(foreach source,$(addprefix $(2)/,$($(1)_SOURCES)),$(eval $(call objdep_template,$(1),$(source))))
$(shell test -z "$($(1)_OBJDEPS)" && echo "empty object list for $(2):$(1)">>log)
$(if $(ADON_NODEPS),,include $(addprefix $(ADON_BUILD)/$(2)/,$($(1)_OBJDEPS)))

$(foreach ext,$(adon_CXXEXTS),$(eval \
$(addprefix $(ADON_BUILD)/$(2)/,$(patsubst %.$(ext),%.o,$(filter %.$(ext),$($(1)_SOURCES)))):
	$(CXX) $(all_CXXFLAGS) -c -o $$@ $$<
))
$(foreach ext,$(adon_CEXTS),$(eval \
$(addprefix $(ADON_BUILD)/$(2)/,$(patsubst %.$(ext),%.o,$(filter %.$(ext),$($(1)_SOURCES)))):
	$(CC) $(all_CFLAGS) -c -o $$@ $$<
))
endef
define obj_deps
$(eval $(1)_OBJDEPS:=$(call dep_list,$(1)))
$(eval adon_CLEAN_FILES+=$(addprefix $(ADON_BUILD)/$(2)/,$($(1)_OBJDEPS)))
$(call record_distfiles,$(1),$(2))
$(if $(tmp_CONDITIONAL),$(call obj_deps_cond,$(1),$(2)))
endef

#FIXME: what about other languages, besides C & C++
define set_LINK
    LINK:=$(shell test -z "$(filter $(tmp_CXXPAT),$($(1)_SOURCES))" && echo "$(CC)" || echo "$(CXX)")
endef

define adon_expandobjs
$(eval $(2)_OBJS:=$(addprefix $(ADON_BUILD)/$(1)/, $(call obj_list,$(2))))
$(eval adon_CLEAN_FILES+=$($(2)_OBJS))
$(eval $(call set_LINK,$(2)))
$(eval $(call obj_deps,$(2),$(1)))
endef

define adon_shlibtarget
adon_CLEAN_FILES+= $(ADON_BUILD)/$(2)-dep $(ADON_BUILD)/$(1)/$(2).so
$(ADON_BUILD)/$(2)-dep: $(1)/Makefile.adon
	touch $$@
$(ADON_BUILD)/$(1)/$(2).so: $(ADON_BUILD)/$(2)-dep $(3)
	$(LINK) $$^ $$(SHLIB_FLAGS) $$(ADON_LIBFLAGS) $($(2)_SHLIB_FLAGS) \
	    $(LOCAL_LDADD) -shared -o $$@
endef

define adon_shlibtemplate
$(eval $(call do_conditional,$(2)))
$(if $(tmp_CONDITIONAL),$(eval adon_TARGETS+=$(2)))
$(call adon_expandobjs,$(1),$(2))
$(eval $(2): $(ADON_BUILD)/$(1)/$(2).so)
$(eval $(call adon_shlibtarget,$(1),$(2),$($(2)_OBJS)))
$(eval $(3)+=$(ADON_BUILD)/$(1)/$(2).so)
endef

define adon_stlibtarget
adon_CLEAN_FILES+= $(ADON_BUILD)/$(2)-dep $(ADON_BUILD)/$(1)/$(2).a
$(ADON_BUILD)/$(2)-dep: $(1)/Makefile.adon
	@echo "deps for $(2): $$^"
	touch $$@
$(ADON_BUILD)/$(1)/$(2).a: $(ADON_BUILD)/$(2)-dep $(3)
	$(AR) -cr $$@ $$^
	$(RANLIB) $$@
endef

define adon_stlibtemplate
$(eval $(call do_conditional,$(2)))
$(if $(tmp_CONDITIONAL),$(eval adon_TARGETS+=$(2)))
$(call adon_expandobjs,$(1),$(2))
$(eval $(2): $(ADON_BUILD)/$(1)/$(2).a)
$(eval $(call adon_stlibtarget,$(1),$(2),$($(2)_OBJS)))
endef

define adon_progtarget
adon_CLEAN_FILES+= $(ADON_BUILD)/$(2)-dep $(ADON_BUILD)/$(1)/$(2)
$(ADON_BUILD)/$(2)-dep: $(1)/Makefile.adon
	@echo "deps for $(2): $$^"
	touch $$@
$(ADON_BUILD)/$(1)/$(2): $(ADON_BUILD)/$(2)-dep $(3)
	$(LINK) $$^ $$(LDFLAGS) $$(TARGET_LDFLAGS) \
	    $$(TOP_LDFLAGS) $$(ADON_LIBFLAGS) \
	    $(LOCAL_LDFLAGS) $($(2)_LDFLAGS) -o $$@
endef

# curdir, progname, instvar
define adon_progtemplate
$(eval $(call do_conditional,$(2)))
$(if $(tmp_CONDITIONAL),$(eval adon_TARGETS+=$(2)))
$(call adon_expandobjs,$(1),$(2))
$(eval $(2): $(ADON_BUILD)/$(1)/$(2))
$(eval $(call adon_progtarget,$(1),$(2),$($(2)_OBJS)))
$(eval $(3)+=$(ADON_BUILD)/$(1)/$(2))
endef

# Limitations
# - Library names must be globally unique
#   (This simplifies inter-module dep tracking)
# - specify base name of libary only
define adon_doshlibs
ADON_LIBDIRS+=$(ADON_BUILD)/$(strip $(1))
$(foreach shlib,$(2),$(call adon_shlibtemplate,$(1),$(shlib),$(3)))
endef

define adon_dostlibs
ADON_LIBDIRS+=$(ADON_BUILD)/$(strip $(1))
$(foreach stlib,$(2),$(call adon_stlibtemplate,$(1),$(stlib),$(3)))
endef

define adon_doprogs
$(foreach prog,$(2),$(call adon_progtemplate,$(1),$(prog),$(3)))
endef

define adon_dodist
$(foreach distfile,$(2),$(eval adon_DIST_FILES+=$(1)/$(distfile)))
endef

define adon_doscripts
$(foreach script,$(2),$(eval $(3)+=$(1)/$(script)))
$(call adon_dodist,$(1),$(2))
endef

ifeq ($(strip $(ADON_CHECKS)),1)
define adon_dochecks
$(shell echo "adding checks: $(2)">>log)
$(call adon_doprogs,$(1),$(2),adon_INST_IGNORE)
$(foreach check,$(2),$(eval $(call do_conditional,$(check)))\
 $(if $(tmp_CONDITIONAL),$(eval adon_CHECKS+=$(ADON_BUILD)/$(1)/$(check))))
endef
else
define adon_dochecks
$(foreach check,$(2),$(eval $(call record_distfiles,$(check),$(1))))
endef
endif

# args:
#  name
#  path
#  mode
#  action - name of macro to call. macros take three args - current
#           path, list of targets, var to save install targets in
#  noinst - set to non-empty value to prevent files from installing

define new_adonvar
adon_VARS += $(1)
adon_$(strip $(1))_path := $(2)
adon_$(strip $(1))_mode := $(3)
adon_$(strip $(1))_act := $(4)
$(if $(filter noinst,$(5)),$(eval\
adon_$(strip $(1))_INSTALL :=
adon_$(strip $(1))_INSTVAR := adon_$(strip $(1))_INSTALL
),$(eval \
adon_$(strip $(1))_INSTVAR := adon_INST_IGNORE
))
endef

include vardefs.mk

define expand_var
$(if $($(2)),\
$(eval tmp_ACT:=$(adon_$(2)_act))\
$(eval tmp_INSTVAR:=adon_$(2)_INSTALL)\
$(shell echo 'Processing var $(2) value $($(2)); tmp_ACT=$(tmp_ACT), tmp_INSTVAR=$(tmp_INSTVAR)'>>log)\
$(call $(tmp_ACT),$(1),$($(2)),$(tmp_INSTVAR),$(tmp_CONDITIONAL))
)
endef

define adon_expandvars
$(foreach var,$(adon_VARS),$(eval $(call expand_var,$(1),$(strip $(var)))))
endef

# this function recurses into each subdir, loads Makefile.adon
# and processes other variables
# called with $(call adon_dodir, .) from top level
define adon_dodir
$(shell echo Starting $(1)>>log)
$(foreach var,$(adon_VARS),$(eval $(strip $(var)):=))
$(foreach var,$(adon_RESETVARS),$(eval $(strip $(var)):=))
$(eval ADON_CURDIR:=$(1))
$(eval ADON_FULLPATH:=$(ADON_SRC)/$(1))
$(eval include $(1)/Makefile.adon)
$(eval adon_DIST_FILES+=$(1)/Makefile.adon)
$(eval adon_CLEAN_FILES+=$(addprefix $(1)/,$(CLEANFILES)))
$(eval adon_MAINTAINERCLEAN_FILES+=$(addprefix $(1)/,$(MAINTAINERCLEANFILES)))
$(shell mkdir -p $(ADON_BUILD)/$(strip $(1))>>log)
$(call adon_expandvars,$(1))
$(foreach subdir,$(ADON_SUBDIRS),$(call adon_dodir,$(strip $(1)/$(subdir))))
$(shell echo Done with $(1)>>log)
endef

$(eval $(call adon_dodir, .))

# target
define do_target_deps
$(ADON_BUILD)/$(1)-dep:$($(1)_DEPS)
endef
$(foreach target,$(adon_TARGETS),$(eval $(call do_target_deps,$(target))))

define install_target_template
$$(DESTDIR)/$(2):$(1)
	install -D -m $(3) $$< $$@
adon_INSTALL_TARGETS+=$$(DESTDIR)/$(2)
endef

define install_dir_target_template
$(foreach file,$(1),$(eval $(call install_target_template,$(file),$(2)/$(notdir $(file)),$(3))))
endef

install-hook: $(INSTALL_HOOKS)
	echo install hooks: $(INSTALL_HOOKS)

$(foreach var,$(adon_VARS),\
 $(eval $(call install_dir_target_template,$(adon_$(var)_INSTALL),$(adon_$(var)_path),$(adon_$(var)_mode))))

doinstall: $(adon_INSTALL_TARGETS) install-hook
	@echo "done installing"

.PHONY: install-hook doinstall

########## make check stuff
adon_CHECK_RESULTS:=$(addsuffix .check.result,$(adon_CHECKS))
adon_CLEAN_FILES+=$(adon_CHECK_RESULTS)

always_do:

%.check.result: % always_do
	@echo running $<
	@echo -n "">$@
	@if ! ./$< >> $@ 2>&1 ; then \
	    if [ "$(TEST_SUCCESS_REQUIRED)" = "1" ]; then \
		exit 1; \
	    fi; \
	fi

checkwarn: $(adon_CHECKS)
	@echo "********************************************************************"
	@echo "* Running tests. This may take a while. Please be patient..........*"
	@echo "********************************************************************"

check: doall checkwarn $(adon_CHECK_RESULTS)
	@if test -z "$(adon_CHECK_RESULTS)"; then \
	    echo "No tests defined"; \
	else \
	    echo "-=Testing results=-"; \
	    cat $(adon_CHECK_RESULTS); \
	fi

.PHONY: check checkwarn always_do

.predep:
	touch $@

clean:
	test -z "$(adon_CLEAN_FILES)" || rm -rf $(adon_CLEAN_FILES)

distclean: clean
	test -z "$(adon_DISTCLEAN_FILES)" || rm -f $(adon_DISTCLEAN_FILES)
	rm -rf $(adon_DIST_DIR) .adon_dist_file_list log libebt-1.3.0.tar.bz2 .predep
	rm -rf $(ADON_BUILD)/

.PHONY: clean distclean

dist: $(adon_DIST_ARCHIVE)
	@echo "Package is ready, your randomness"

$(adon_DIST_ARCHIVE): .adon_dist_file_list
	tar -cjvf $@ --files-from=$<

define add2distlist
	@echo "$(adon_DIST_DIR)/$(1)" >> $(2)
	@mkdir -p "$(dir $(adon_DIST_DIR)/$(1))"
	@cp -f "$(1)" "$(dir $(adon_DIST_DIR)/$(1))"
	
endef
.adon_dist_file_list: $(adon_DIST_FILES) rules.mk
	echo -n "">$@
	$(foreach distfile,$^,$(call add2distlist,$(distfile),$@))

.PHONY: dist distcheck

distcheck: dist
	mkdir -p distcheck_dir
	tar -xjvf $(adon_DIST_ARCHIVE) -C distcheck_dir
	$(MAKE) check -C distcheck_dir/$(adon_DIST_DIR)
	rm -rf distcheck_dir

maintainer-clean: distclean
	rm -rf $(adon_MAINTAINERCLEAN_FILES)

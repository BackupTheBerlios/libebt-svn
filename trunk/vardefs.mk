$(eval $(call new_adonvar,lib_SHLIBS,$(PREFIX)/lib,755,adon_doshlibs))
#this one is for sort of compatibility. It does _not_ use libtool
$(eval $(call new_adonvar,lib_LTLIBRARIES,$(PREFIX)/lib,755,adon_doshlibs))
$(eval $(call new_adonvar,lib_STLIBS,$(PREFIX)/lib,755,adon_dostlibs))
$(eval $(call new_adonvar,bin_PROGRAMS,$(PREFIX)$(BIN),755,adon_doprogs))
$(eval $(call new_adonvar,sbin_PROGRAMS,$(PREFIX)$(SBIN),755,adon_doprogs))
$(eval $(call new_adonvar,bin_SCRIPTS,$(PREFIX)$(BIN),755,adon_doscripts))
$(eval $(call new_adonvar,init_SCRIPTS,$(PREFIX)/etc/init.d,755,adon_doscripts))
$(eval $(call new_adonvar,conf_DATA,$(PREFIX)/var/lib/conf,644,adon_doscripts))
$(eval $(call new_adonvar,pkginclude_HEADERS,$(PREFIX)/usr/include/$(PKG_NAME),644,adon_doscripts))
$(eval $(call new_adonvar,check_PROGRAMS,NONE,000,adon_dochecks))
$(eval $(call new_adonvar,check_LIBRARIES,NONE,000,adon_dostlibs,nosinst))
$(eval $(call new_adonvar,check_STLIBS,NONE,000,adon_dostlibs,nosinst))
$(eval $(call new_adonvar,noinst_PROGRAMS,NONE,000,adon_doprogs,noinst))
$(eval $(call new_adonvar,noinst_HEADERS,NONE,000,adon_dodist,noinst))
$(eval $(call new_adonvar,EXTRA_DIST,NONE,000,adon_dodist,noinst))

# put all the custom variables in vardefs-custom.mk, as this one
# will be blown away by updates and such
-include vardefs-custom.mk

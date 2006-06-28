#####################################################
ifneq ($(strip $(shell $(CXX) -v 2>&1| grep '^gcc')),)
$(eval $(call cxx_flag_kirpich,-W))
$(eval $(call cxx_flag_kirpich,-Wall))
$(eval $(call cxx_flag_kirpich,-Wextra))
$(eval $(call cxx_flag_kirpich,-Wold-style-cast))
$(eval $(call cxx_flag_kirpich,-Woverloaded-virtual))
$(eval $(call cxx_flag_kirpich,-Wredundant-decls))
$(eval $(call cxx_flag_kirpich,-pedantic))
else
$(eval $(call cxx_flag_kirpich,-Wall))
$(eval $(call cxx_flag_kirpich,-wd981,279,383,1418,304,444,488))
endif
$(if $(ENABLE_WSTRING_TESTS),$(eval $(wstring_kirpich)))
$(if $(ENABLE_BOOST_THREAD_TESTS),$(eval $(boost_thread_kirpich)))
$(if $(ENABLE_ZTHREAD_TESTS),$(eval $(zthread_kirpich)))
$(if $(ENABLE_PTHREAD_TESTS),$(eval $(pthread_kirpich)))
#####################################################

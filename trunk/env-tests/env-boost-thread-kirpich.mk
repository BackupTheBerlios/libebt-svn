define boost_thread_kirpich
$(call cxx_link_test,boost-thread-kirpich.cpp,-lboost_thread,HAVE_BOOST_THREAD)
endef

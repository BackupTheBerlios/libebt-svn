define pthread_kirpich
$(call c_compile_test,pthread-kirpich.c,,HAVE_PTHREAD)
endef

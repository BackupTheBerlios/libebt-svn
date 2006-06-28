define cxx_flag_kirpich
$(eval FLAG_NAME:=$(shell echo $(1) | tr ",-" "_"))
$(call env_test_target,$(FLAG_NAME)): $(ENV_TEST_DIR)/cxx-gen-kirpich.cpp
	COMPILER=$$(CXX) ./$(ENV_TEST_DIR)/compile-test.sh "cxx-gen-kirpich.cpp" \
	    "cxx-gen-kirpich.o" "$(1)" "TOP_CXXFLAGS+=$(1)" "">$$@
endef

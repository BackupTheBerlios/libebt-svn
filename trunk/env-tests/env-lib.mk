env_test_file=$(ENV_TEST_RUN)/$(1).$(2)
env_test_target_name=$(ENV_TEST_RUN)/$(1).cache

define env_test_target
$(shell echo "env test target for $(3)">>log)
$(eval adon_ENV_TEST_TARGETS+= $(ENV_TEST_RUN)/$(1).cache)
$(call env_test_target_name,$(1))
endef

# C++ compile test
# source,flags,result var name
define cxx_compile_test
$(eval tmp_FBASE:=$(basename $(1)))
$(call env_test_target,$(3)): $(ENV_TEST_DIR)/$(1)
	COMPILER=$$(CXX) ./$(ENV_TEST_DIR)/compile-test.sh "$(1)" \
	    "$(tmp_FBASE).o" "$(2)" "$(3):=1" "$(3):=0">$$@
endef

# C compile test
# source,flags,result var name
define c_compile_test
$(eval tmp_FBASE:=$(basename $(1)))
$(call env_test_target,$(3)): $(ENV_TEST_DIR)/$(1)
	COMPILER=$$(CC) ./$(ENV_TEST_DIR)/compile-test.sh "$(1)" \
	    "$(tmp_FBASE).o" "$(2)" "$(3):=1" "$(3):=0">$$@
endef

# C++ link test
# source,flags,result var name
define cxx_link_test
$(eval tmp_FBASE:=$(basename $(1)))
$(call env_test_target,$(3)): $(ENV_TEST_DIR)/$(1)
	COMPILER=$$(CXX) ./$(ENV_TEST_DIR)/link-test.sh "$(1)" \
	    "$(tmp_FBASE)" "$(2)" "$(3):=1" "$(3):=0">$$@
endef

# C link test
# source,flags,result var name
define c_link_test
$(eval tmp_FBASE:=$(basename $(1)))
$(call env_test_target,$(3)): $(ENV_TEST_DIR)/$(1)
	$(3):="`COMPILER=$$(CC) ./$(ENV_TEST_DIR)/link-test.sh "$(1)" \
	    "$(ENV_TEST_RUN)/$(tmp_FBASE)" "$(2)" "$(3):=1" "$(3):=0">$$@
endef

ENV_TEST_DIR:=env-tests
ENV_TEST_RUN:=$(ADON_BUILD)/env-tests
$(shell mkdir -p $(ENV_TEST_RUN))
export ENV_TEST_DIR
export ENV_TEST_RUN

adon_ENV_TEST_TARGETS:=

$(shell echo -n "">$(ENV_TEST_RUN)/log)

include $(ENV_TEST_DIR)/env-lib.mk
include $(ENV_TEST_DIR)/env-*-kirpich.mk

# enable tests in env-test-custom.mk
-include $(ENV_TEST_DIR)/env-test-custom.mk
include $(adon_ENV_TEST_TARGETS)

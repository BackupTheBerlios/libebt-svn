#!/bin/bash
. ${ENV_TEST_DIR}/env-lib.sh

export IN_FILE="$1"
shift
export OUT_FILE="$1"
shift
export FLAGS="$1"
shift
export ACTION_T="$1"
shift
export ACTION_F="$1"

do_compile

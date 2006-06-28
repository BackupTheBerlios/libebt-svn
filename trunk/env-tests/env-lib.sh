ECHO="echo -n"
ECHOLINE="echo"
ENV_LOG=${ENV_TEST_RUN}/log
function die() {
    ${ECHOLINE} $@
    exit 1
}

# do_compile
#  env   : COMPILER FLAGS IN_FILE OUT_FILE ACTION_T ACTION_F
#  stdout: 1 for success, 0 for failure
function do_compile() {
    if ${COMPILER} ${FLAGS} -c "${ENV_TEST_DIR}/${IN_FILE}" -o "${ENV_TEST_RUN}/${OUT_FILE}" >> ${ENV_LOG} 2>&1; then
	${ECHO} "${ACTION_T}";
    else
	${ECHO} "${ACTION_F}";
    fi
    #test -f "${OUT_FILE}" && rm -f "${OUT_FILE}"
}

# do_link
#  env   : COMPILER FLAGS IN_FILE OUT_FILE ACTION_T ACTION_F
#  stdout: $ACTION_T for success, $ACTION_F for failure
function do_link() {
    if ${COMPILER} ${FLAGS} ${ENV_TEST_DIR}/${IN_FILE} -o ${ENV_TEST_RUN}/${OUT_FILE} >> ${ENV_LOG} 2>&1; then
	${ECHO} "${ACTION_T}";
    else
	${ECHO} "${ACTION_F}";
    fi
}

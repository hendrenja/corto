#include "cortotool_test.h"

corto_int16 cortotool_test(int argc, char *argv[]) {
    corto_string testCase = NULL;
    corto_int8 ret, sig, err = 0;

    if (argc > 1) {
        if (corto_chdir(argv[1])) {
            corto_error("corto: can't change to directory '%s'", argv[1]);
            goto error;
        }
        if (argc > 2) {
            testCase = argv[2];
        }
    }

    if (!testCase) {
        corto_pid pid = corto_procrun("rake", (char*[]){"rake", "test", NULL});
        if ((sig = corto_procwait(pid, &ret) || ret)) {
            if (sig > 0) {
                corto_error("corto: the tests failed to build (build aborted with signal %d)", sig);
            }
            err = 1;
        }
    } else {
        if (!corto_chdir("test")) {
            corto_loadPackages();
            if (corto_load("./.corto/libtest.so", 2, (char*[]){"./.corto/libtest.so", testCase, NULL})) {
                err = 1;
            }
        }
    }

    if (err) {
        goto error;
    }

    return 0;
error:
    return -1;
}

void cortotool_testHelp(void) {
    printf("Usage: corto test\n");
    printf("       corto test <project>\n");
    printf("       corto test <project> <testcase>\n");
    printf("\n");
    printf("Test your Corto project. Returns -1 if tests fails.\n");
    printf("\n");
}

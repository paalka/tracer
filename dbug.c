#include <stdio.h>
#include "debugger.h"
#include "util.h"

int main(int argc, char** argv)
{
    if (argc < 2) {
        log_error("Name of program to debug was not provided!");
        return -1;
    }

    init_debugger(argv[1]);
}

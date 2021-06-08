#include "cachelab.h"
#include "cache.h"
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    parse_option(argc, argv);
    printSummary(0, 0, 0);
    return 0;
}

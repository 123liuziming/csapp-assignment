#include "cachelab.h"
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    parse_option(argc, argv);
    analyse();
    printSummary(hits, misses, evitions);
    return 0;
}

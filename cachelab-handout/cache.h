//b
int num_of_block_bits;
//E
int associativity;
//s
int num_of_set_index_bits;
char* filename;
bool print_usage;
bool print_verbose;
char* trace_message[];

static int time = 0;

typedef struct cache_item {
    bool valid;
    int flag;
    //LRU时间
    int visit_time;
}

cache_item cache_items[][];

void parse_option(int argc, char* argv[]);
void analyse();
void load(int index, int flag);
void modify(int index, int flag);
void store(int index, int flag);
void init();
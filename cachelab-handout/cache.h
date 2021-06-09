#define MAX_TRACE_NUM 10005
#define MAX_CACHE_NUM 10005
#define MAX_E 105

//b
int num_of_block_bits;
//E
int associativity;
//s
int num_of_set_index_bits;
char* filename;
bool print_usage;
bool print_verbose;
char* trace_message[MAX_TRACE_NUM];

static trace_index = 0;
static int time = 0;

typedef struct cache_item {
    bool valid;
    int flag;
    //LRU时间
    int visit_time;
}

cache_item cache_items[MAX_CACHE_NUM][MAX_E];

void parse_option(int argc, char* argv[]);
void analyse();
void load(int index, int flag, char* trace_message);
void modify(int index, int flag, char* trace_message);
void store(int index, int flag, char* trace_message);
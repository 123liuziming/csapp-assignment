/*
 * cachelab.c - Cache Lab helper functions
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cachelab.h"
#include <time.h>
#include <string.h>
#include <getopt.h>

//b
int num_of_block_bits;
//E
int associativity;
//s
int num_of_set_index_bits;
char* filename;
int print_usage;
int print_verbose;
char* trace_messages[MAX_TRACE_NUM];

int trace_index = 0;
int times = 0;

int hits;
int evitions;
int misses;

cache_item* cache_items[MAX_CACHE_NUM][MAX_E];

trans_func_t func_list[MAX_TRANS_FUNCS];
int func_counter = 0; 

/* 
 * printSummary - Summarize the cache simulation statistics. Student cache simulators
 *                must call this function in order to be properly autograded. 
 */
void printSummary(int hits, int misses, int evictions)
{
    printf("hits:%d misses:%d evictions:%d\n", hits, misses, evictions);
    FILE* output_fp = fopen(".csim_results", "w");
    assert(output_fp);
    fprintf(output_fp, "%d %d %d\n", hits, misses, evictions);
    fclose(output_fp);
}

/* 
 * initMatrix - Initialize the given matrix 
 */
void initMatrix(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;
    srand(time(NULL));
    for (i = 0; i < N; i++){
        for (j = 0; j < M; j++){
            // A[i][j] = i+j;  /* The matrix created this way is symmetric */
            A[i][j]=rand();
            B[j][i]=rand();
        }
    }
}

void randMatrix(int M, int N, int A[N][M]) {
    int i, j;
    srand(time(NULL));
    for (i = 0; i < N; i++){
        for (j = 0; j < M; j++){
            // A[i][j] = i+j;  /* The matrix created this way is symmetric */
            A[i][j]=rand();
        }
    }
}

/* 
 * correctTrans - baseline transpose function used to evaluate correctness 
 */
void correctTrans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;
    for (i = 0; i < N; i++){
        for (j = 0; j < M; j++){
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    
}



/* 
 * registerTransFunction - Add the given trans function into your list
 *     of functions to be tested
 */
void registerTransFunction(void (*trans)(int M, int N, int[N][M], int[M][N]), 
                           char* desc)
{
    func_list[func_counter].func_ptr = trans;
    func_list[func_counter].description = desc;
    func_list[func_counter].correct = 0;
    func_list[func_counter].num_hits = 0;
    func_list[func_counter].num_misses = 0;
    func_list[func_counter].num_evictions =0;
    func_counter++;
}

void get_flag_and_index(int* flag, int* index, int address) {
    const int t = 32 - (num_of_block_bits + num_of_set_index_bits);
    *flag = (0x80000000 >> (t - 1)) & address;
    *index = (address << t) & (0x80000000 >> (num_of_set_index_bits - 1));
}

/**
 * 找对应的item，找不到返回NULL
 */
cache_item* find_cache_item(int index, int flag) {
    int i;
    for (i = 0; i < associativity; ++i) {
        if (cache_items[index][i] == NULL) {
            continue;
        }
        cache_item tmp = *cache_items[index][i];
        if (tmp.valid && tmp.flag == flag) {
            return cache_items[index][i];
        }
    }
    return NULL;
}

/**
 * 找第一个空闲条目位置
 */ 
int find_first_empty_item(int index) {
    int i;
    for (i = 0; i < associativity; ++i) {
        if (cache_items[index][i] != NULL) {
            return i;
        }
    }
    return -1;
}

/**
 * LRU驱逐，驱逐time最小的item
 */
int evict(int index) {
    ++evitions;  
    int i;
    int min_time = 0x3f3f3f3f;
    int evict_index = -1;
    for (i = 0; i < associativity; ++i) {
        if (cache_items[index][i] == NULL) {
            continue;
        }
        cache_item tmp = *cache_items[index][i];
        if (tmp.visit_time < min_time) {
            evict_index = i;
            min_time = tmp.visit_time;
        }
    }
    if (evict_index != -1) {
        cache_items[index][evict_index] = NULL;
    }
    return evict_index;
}

/**
 * 把块读进缓存
 */ 
void load_block_to_cache(int index, int flag, char* trace_message) {
    cache_item item;
    item.visit_time = ++times;
    item.flag = flag;
    item.valid = 1;
    int first_empty_index = find_first_empty_item(index);
    if (first_empty_index == -1) {
        first_empty_index = evict(index);
        strncat(trace_message, " eviction", 10);
    }
    cache_items[index][first_empty_index] = &item;
}

void parse_option(int argc, char* argv[]) {
    char c;
    while((c = getopt(argc, argv, "hvs:E:b:t:")) != -1) {
        switch (c) {
        case 'h':
            print_usage = 1;
            break;
        case 'v':
            print_verbose = 1;
            break;
        case 'E':
            associativity = atoi(optarg);
            break;
        case 'b':
            num_of_set_index_bits = atoi(optarg);
            break;
        case 't':
            filename = optarg;
            break;
        default:
            break;
        }
    }
}

void analyse() {
    FILE* fp = NULL;
    if ((fp = fopen(filename, "r")) == NULL) {
        printf("File cannnot be opened\n");
        exit(-1);
    }
    char tmp[20];
    while ((fgets(tmp, 20, fp))) {
        if (tmp[0] == 'I') {
            continue;
        }
        char* head = tmp;
        if (head[0] == ' ') {
            ++head;
        }
        char addr[10];
        int i = 0, j = 1;
        while (head[j] != ',') {
            addr[i++] = head[j++];
        }
        addr[i] = '\0';
        int address = strtol(addr, NULL, 16);
        int flag, index;
        get_flag_and_index(&flag, &index, address);
        char trace_message[50];
        strcpy(trace_message, head + 2);
        switch (head[0]) {
            case 'M':
                modify(index, flag, trace_message);
                break;
            case 'L':
                load(index, flag, trace_message);
                break;
            case 'S':
                store(index, flag, trace_message);
                break;
            default:
                break;
        }
        trace_messages[trace_index++] = trace_message;
    }
    fclose(fp);
}

/**
 * 1. 组索引
 * 2. 看有没有标记一致的条目
 * 3. 没有的话输出miss，需要驱逐的话输出eviction
 * 4. 有的话输出hit
 */ 
void load(int index, int flag, char* trace_message) {
    cache_item* item = find_cache_item(index, flag);
    if (!item) {
        strncat(trace_message, " miss", 6);
        ++misses;
        load_block_to_cache(index, flag, trace_message);
    }
    else {
        strncat(trace_message, " hit", 5);
        ++hits;
    }
}

/**
 * 1. 找到条目
 * 2. 如果有，更改valid为false
 * 3. 如果没有，读到缓存再修改
 */ 
void store(int index, int flag, char* trace_message) {
    cache_item* item = find_cache_item(index, flag);
    if (!item) {
        load_block_to_cache(index, flag, trace_message);
    }
    item->valid = 0;
    strncat(trace_message, " hit", 5);
    ++hits;
}


/**
 * 1. load
 * 2. 
 */ 
void modify(int index, int flag, char* trace_message) {
    load(index, flag, trace_message);
    store(index, flag, trace_message);
}

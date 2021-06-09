#include "cache.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void get_flag_and_index(int& flag, int& index, int address) {
    const int t = 32 - (num_of_block_bits + num_of_set_index_bits);
    *flag = (0x80000000 >> (t - 1)) & address;
    *index = (address << t) & (0x80000000 >> (s - 1))
}

/**
 * 找对应的item，找不到返回NULL
 */
cache_item* find_cache_item(int index, int flag) {
    int i;
    for (i = 0; i < associativity; ++i) {
        cache_item tmp = cache_items[index][i];
        if (tmp.valid && tmp.flag == flag) {
            return &cache_items[index][i];
        }
    }
    return NULL;
}

/**
 * 找第一个空闲条目位置
 */ 
int find_first_empty_item(int index) {
    int i;
    for (int i = 0; i < associativity; ++i) {
        if (cache_items[index][i] != NULL) {
            return i;
        }
    }
    return -1;
}

/**
 * 把块读进缓存
 */ 
void load_block_to_cache(int index, int flag, char* trace_message) {
    cache_item item;
    item.visit_time = ++time;
    item.flag = flag;
    item.valid = true;
    int first_empty_index = find_first_empty_item(index);
    if (find_first_empty_item == -1) {
        find_first_empty_item = evict(index);
        strncat(trace_message, " eviction");
    }
    cache_items[index][first_empty_index] = item;
}


/**
 * LRU驱逐，驱逐time最小的item
 */
int evict(int index) {  
    int i;
    int min_time = 0x3f3f3f3f;
    int evict_index = -1;
    for (i = 0; i < associativity; ++i) {
        cache_item tmp = cache_items[index][i];
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

void parse_option(int argc, char* argv[]) {
    char c;
    while((c = getopt(argc, argv, "hvs:E:b:t:")) != -1) {
        switch (c) {
        case 'h':
            print_usage = true;
            break;
        case 'v':
            print_verbose = true;
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
    if ((fp = fopen(filename, "w+")) == NULL) {
        printf("File cannnot be opened\n");
        exit(-1);
    }
    char tmp[20];
    while ((fgets(tmp, 20, fp)) != NULL) {
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
        addr[i] = '/0';
        int address = strtol(addr, NULL, 16);
        int flag, index;
        get_flag_and_index(&flag, &index, address);
        char trace_message[50];
        strcpy(trace_message, head + 2, strlen(head) - 2);
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
        trace_message[trace_index++] = trace_message;
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
        strncat(trace_message, " miss");
        load_block_to_cache(index, flag, trace_message);
    }
    else {
        strncat(trace_message, " hit");
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
    item.valid = false;
    strncat(trace_message, " hit");
}


/**
 * 1. load
 * 2. 
 */ 
void modify(int index, int flag, char* trace_message) {
    load(index, flag, trace_message);
    store(index, flag, trace_message);
}

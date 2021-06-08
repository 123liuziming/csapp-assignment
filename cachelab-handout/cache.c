#include "cache.h"
#include <stdio.h>
#include <stdlib.h>


void get_flag_and_index(int& flag, int& index, int address) {
    const int t = 32 - (num_of_block_bits + num_of_set_index_bits);
    *flag = (0x80000000 >> (t - 1)) & address;
    *index = (address << t) & (0x80000000 >> (s - 1))
}

/**
 * 找对应的item，找不到返回-1
 */
int find_cache_item(int index, int flag) {

}

/**
 * LRU驱逐，驱逐time最小的item
 */
void evict(int index) {  
    int i;
    
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
        int addr = strtol(addr, NULL, 16);
        int flag, index;
        get_flag_and_index(&flag, &index, addr);
        switch (head[0]) {
            case 'M':
                modify(index, flag);
                break;
            case 'L':
                load(index, flag);
                break;
            case 'S':
                store(index, flag);
                break;
            default:
                break;
        }
    }
    fclose(fp);
}

/**
 * 1. 组索引
 * 2. 看有没有标记一致的条目
 * 3. 没有的话输出miss，需要驱逐的话输出eviction
 * 4. 有的话输出hit
 */ 
void load(int index, int flag) {

}

void store(int index, int flag) {

}

void modify(int index, int flag) {

}



/**
 * 根据b,E,s来确定缓存以及trace_message有多少个条目以及标记位t
 * t = 32 - (s + b)
 * 缓存大小：[2 ^ s - 1][E]
 */ 
void init() {

}

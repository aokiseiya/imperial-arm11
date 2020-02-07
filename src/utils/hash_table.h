#ifndef ARM11_35_HASH_TABLE_H
#define ARM11_35_HASH_TABLE_H

// Don't need delete functionality for now.

#include <stdbool.h>
#include <stdint.h>

#define HASH_TABLE_SIZE 101
//static const int HASH_TABLE_SIZE = 101;

typedef struct DataItem {
    void *value;
    void *key;
} DataItem;

typedef DataItem HashTable[HASH_TABLE_SIZE];

HashTable *create_hash_table();

int hash_string(char *str);

int hash_code(uint32_t key);

void insert(HashTable *hash_table, void *key, void *data);

void insert_si(HashTable *hash_table, char *key, int value);

int search_si(HashTable *hash_table, char *key);

void free_hash_table(HashTable *hash_table);

HashTable *create_instruction_type_table();

HashTable *create_cond_table();

HashTable *create_opcode_table();

HashTable *create_shift_table();

#endif //ARM11_35_HASH_TABLE_H
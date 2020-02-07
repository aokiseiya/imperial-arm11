#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "hash_table.h"
#include "arm11.h"

// Checks if data item is null
bool is_null(DataItem data_item);

// Takes two pointers of strings and returns whether they are true or not
// PRE: Both the pointers point to strings.
bool is_equal_string(void *s1, void *s2);

int hash_string(char *str) {
  uint32_t hash = 7;
  int c;
  while ((c = *(str++))) {
    hash = ((hash << 5) - hash) + c; /* hash * 31 + c */
  }
  return hash_code(hash);
}

int hash_code(uint32_t key) {
  return key % HASH_TABLE_SIZE;
}

void insert(HashTable *hash_table, void *key, void *data) {
  // We do not need to put this in the heap as they only carry two pointers.
  DataItem item;
  item.value = data;
  item.key = key;

  // Get the hash code
  int hash_index = hash_string(key);

  // Move in array until an empty or deleted cell
  while (!is_null((*hash_table)[hash_index])) {
    // Go to next index
    ++hash_index;

    // Wrap around the table
    hash_index %= HASH_TABLE_SIZE;
  }
  (*hash_table)[hash_index] = item;
}

// Pass in an appropriate equality function
void *search(HashTable *hash_table, void *key, bool (*eq_check)(void *, void *), int hash_index) {

  // Move in array until an empty
  while (!is_null((*hash_table)[hash_index])) {

    if ((eq_check)(((*hash_table)[hash_index]).key, key)) {
      return ((*hash_table)[hash_index]).value;
    }

    // Go to next cell
    ++hash_index;

    // Wrap around the table
    hash_index %= HASH_TABLE_SIZE;
  }

  return NULL;
}

int search_si(HashTable *hash_table, char *key) {
  // Get the hash
  int hash_index = hash_string(key);

  // Create a reference and copy the string
  char *r_key = calloc(strlen(key) + 1, sizeof(char));
  if (r_key == NULL) {
    perror("Memory allocation failed.");
    exit(EXIT_FAILURE);
  }
  strcpy(r_key, key);

  int ret = *(int *) search(hash_table, r_key, is_equal_string, hash_index);
  free(r_key);
  return ret;
}

bool is_equal_string(void *input1, void *input2) {
  return !(strcmp(((char *) input1), ((char *) input2)));
}

void free_hash_table(HashTable *hash_table) {
  for (int i = 0; i < HASH_TABLE_SIZE; ++i) {
    if (!is_null((*hash_table)[i])) {
      free((*hash_table)[i].key);
      free((*hash_table)[i].value);
    }
  }
  free(hash_table);
}

void insert_si(HashTable *hash_table, char *key, int value) {
  // Create a string reference and copy the input into it
  char *r_key = calloc(strlen(key) + 1, sizeof(char));
  if (r_key == NULL) {
    fprintf(stderr, "Memory allocation failed.");
    exit(EXIT_FAILURE);
  }

  strcpy(r_key, key);
  int *r_value = malloc(sizeof(int));
  if (r_value == NULL) {
    fprintf(stderr, "Memory allocation failed.");
    exit(EXIT_FAILURE);
  }
  *r_value = value;
  insert(hash_table, r_key, r_value);
}

HashTable *create_hash_table() {
  // Calloc since we need to initialize to 0.
  HashTable *r = calloc(1, sizeof(HashTable));
  if (r == NULL) {
    fprintf(stderr, "Memory allocation failed.");
    exit(EXIT_FAILURE);
  }
  return r;
}

bool is_null(DataItem item) {
  return (item.value == NULL) && (item.key == NULL);
}

//creates hash table of mnemonics to instruction type
HashTable *create_instruction_type_table() {
  HashTable *t = create_hash_table();
  insert_si(t, "add", DATAP);
  insert_si(t, "sub", DATAP);
  insert_si(t, "rsb", DATAP);
  insert_si(t, "and", DATAP);
  insert_si(t, "eor", DATAP);
  insert_si(t, "orr", DATAP);
  insert_si(t, "mov", DATAP);
  insert_si(t, "tst", DATAP);
  insert_si(t, "teq", DATAP);
  insert_si(t, "cmp", DATAP);
  insert_si(t, "mul", MULTIPLY);
  insert_si(t, "mla", MULTIPLY);
  insert_si(t, "ldr", SINGLEDT);
  insert_si(t, "str", SINGLEDT);
  insert_si(t, "beq", BRANCH);
  insert_si(t, "bne", BRANCH);
  insert_si(t, "bge", BRANCH);
  insert_si(t, "blt", BRANCH);
  insert_si(t, "bgt", BRANCH);
  insert_si(t, "ble", BRANCH);
  insert_si(t, "b", BRANCH);
  insert_si(t, "lsl", DATAP);
  insert_si(t, "andeq", DATAP);
  return t;
}

//creates hash table of mnemonics to cond enums
HashTable *create_cond_table() {
  HashTable *t = create_hash_table();
  insert_si(t, "add", AL);
  insert_si(t, "sub", AL);
  insert_si(t, "rsb", AL);
  insert_si(t, "and", AL);
  insert_si(t, "eor", AL);
  insert_si(t, "orr", AL);
  insert_si(t, "mov", AL);
  insert_si(t, "tst", AL);
  insert_si(t, "teq", AL);
  insert_si(t, "cmp", AL);
  insert_si(t, "mul", AL);
  insert_si(t, "mla", AL);
  insert_si(t, "ldr", AL);
  insert_si(t, "str", AL);
  insert_si(t, "beq", EQ);
  insert_si(t, "bne", NE);
  insert_si(t, "bge", GE);
  insert_si(t, "blt", LT);
  insert_si(t, "bgt", GT);
  insert_si(t, "ble", LE);
  insert_si(t, "b", AL);
  insert_si(t, "lsl", AL);
  insert_si(t, "andeq", EQ);
  return t;
}

//creates hash table of mnemonics to instruction opcode
HashTable *create_opcode_table() {
  HashTable *t = create_hash_table();
  insert_si(t, "add", ADD);
  insert_si(t, "sub", SUB);
  insert_si(t, "rsb", RSB);
  insert_si(t, "and", AND);
  insert_si(t, "eor", EOR);
  insert_si(t, "orr", ORR);
  insert_si(t, "tst", TST);
  insert_si(t, "teq", TEQ);
  insert_si(t, "cmp", CMP);
  insert_si(t, "mov", MOV);
  insert_si(t, "lsl", SPECIAL_LSL);
  insert_si(t, "andeq", SPECIAL_ANDEQ);
  return t;
}

//creates hash table of mnemonics to shift type
HashTable *create_shift_table() {
  HashTable *t = create_hash_table();
  insert_si(t, "lsl", LSL);
  insert_si(t, "lsr", LSR);
  insert_si(t, "asr", ASR);
  insert_si(t, "ror", ROR);
  return t;
}
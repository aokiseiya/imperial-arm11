#ifndef ARM11_35_SPLIT_STRING_H
#define ARM11_35_SPLIT_STRING_H

typedef struct SplitString {
    // Number of words.
    int size;
    // Internal representation of string.
    char *str;
    // Pointers pointing to the beginning of each word.
    char (*words[]);
} SplitString;

SplitString *split_string(char *line);

void free_split_string(SplitString *ss);

#endif //ARM11_35_SPLIT_STRING_H

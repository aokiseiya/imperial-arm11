#include "split_string.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "arm11.h"

#define SPLIT_STRING_SIZE 511
#define SPLIT_STRING_COUNT 511

// Splits a string by ' ' and ','.
// It also combines back anything contained inside '[' and ']'.
SplitString *split_string(char *input_line) {
  // We will allocate space and save the input into the heap.
  char *line = malloc((strlen(input_line) + 1) * sizeof(char));
  if (line == NULL) {
    fprintf(stderr, "Memory allocation failed");
    exit(EXIT_FAILURE);
  }
  strcpy(line, input_line);
  char (*buffer[SPLIT_STRING_COUNT]) = {0};
  char *token;
  int i = 0;
  token = strtok(line, " ,");

  while (token != NULL) { //tokenizes the string by delimiters
    buffer[i] = token;
    token = strtok(NULL, " ,");
    i++;
  }

  bool combine = false;
  int k = 0;
  int j = 0;
  const int open = '[';
  const int close = ']';

  while ((buffer)[j] && j < SPLIT_STRING_SIZE) {
    //concatenates elements in []
    if (combine) {
      char *temp = calloc(1, strlen(buffer[j]) + 1);
      if (temp == NULL) {
        fprintf(stderr, "Memory allocation failed.");
        exit(EXIT_FAILURE);
      }

      strcpy(temp, buffer[j]);

      strcpy((buffer)[k], strcat(strcat((buffer)[k], ","), temp));
      free(temp);
      if (strchr((buffer)[j], close)) {
        combine = false;
        k++;
      }
    }
      //checks for '[' and ']' and sets combine flags
    else {
      if (strchr((buffer)[j], open)) {
        combine = true;
      }
      if (strchr((buffer)[j], close)) {
        combine = false;
      }
      if (!combine) {
        k++;
      }
    }
    j++;
  }

  // Transfer the result from the buffer to SplitString
  SplitString *r = calloc(1, sizeof(SplitString) + sizeof(char *) * (k + 1));
  if (r == NULL) {
    fprintf(stderr, "Memory allocation failed.");
    exit(EXIT_FAILURE);
  }
  r->str = line;
  r->size = k;
  for (int l = 0; l < r->size; ++l) {
    r->words[l] = buffer[l];
  }

  return r;
}

void free_split_string(SplitString *ss) {
  free(ss->str);
  free(ss);
}


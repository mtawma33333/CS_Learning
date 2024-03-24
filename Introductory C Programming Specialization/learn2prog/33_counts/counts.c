#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "counts.h"
counts_t * createCounts(void) {
  //WRITE ME
  counts_t *counts = malloc(sizeof(*counts));
  counts->counts = NULL;
  counts->Length = 0;
  counts->unknown = 0;
  return counts;
}
void addCount(counts_t * c, const char * name) {
  //WRITE ME
  if (name == NULL) {
    c->unknown++;
    return ;
  }
  one_count_t * tmp = c->counts;
  for (int i=c->Length-1; i >= 0; i--) {
    if (strcmp(name, tmp[i].name) == 0) {
      tmp[i].count++;
      return ;
    }
  }
  one_count_t * counter = malloc(sizeof(*counter));
  counter->name = malloc(strlen(name) + 1);
  strcpy(counter->name, name);
  counter->count = 1;
  tmp = realloc(tmp, (c->Length+1)*sizeof(*tmp));
  tmp[c->Length] = *counter;
  free(counter);  // OBSERVE
  c->counts = tmp;
  c->Length++;
  return ;
}
void printCounts(counts_t * c, FILE * outFile) {
  //WRITE ME
  for (int i=0, l=c->Length; i < l; i++) {
    one_count_t counter = c->counts[i];
    fprintf(outFile, "%s: %ld\n", counter.name, counter.count);
  }
  if (c->unknown > 0) {
    fprintf(outFile, "<unknown>: %ld\n", c->unknown);
  }
}

void freeCounts(counts_t * c) {
  one_count_t *counter = c->counts;
  for (int i=c->Length-1; i >= 0; i--) {
    free(counter[i].name);
  }
  free(c->counts);
  free(c);
}

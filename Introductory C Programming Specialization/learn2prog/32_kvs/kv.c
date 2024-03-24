#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "kv.h"

// 1.cpoy string res[*start:x](exclude x) into a new string, where res[x] == delim.
// 2.fast fail if read a '\0' before delim
// 3.for security in avoiding 'read out of range of buffer', 
//   res must be a valid string, namely end up with '\0'
char * dynamicStrcpyUntil(const char * res, size_t *start, char delim) {
  int i = *start;
  size_t sz = 0;
  char * str = NULL;
  for (; res[i] && res[i] != delim; i++) {
    str = realloc(str, (sz+1)*sizeof(*str));
    str[sz] = res[i];
    sz++;
  }
  // add an null terminate
  str = realloc(str, (sz+1)*sizeof(*str));
  str[sz] = '\0';
  // update start
  *start = i;
  return str;
}

kvarray_t * readKVs(const char * fname) {
  if (fname == NULL) {
    perror("fname equals to NULL\n");
    return NULL;
  }
  FILE * file = fopen(fname, "r");
  if (file == NULL) {
    fprintf(stderr, "File(%s) not exist\n", fname);
    exit(-1);
  }

  kvarray_t * kvArray = malloc(sizeof(*kvArray));
  kvArray->pairs = NULL;
  kvArray->size = 0;

  char * buf = NULL;
  size_t sz = 0;

  for (; getline(&buf, &sz, file) >= 0; ) {
    size_t i = 0;
    char *key = dynamicStrcpyUntil(buf, &i, '=');
    // fast fail
    assert(buf[i] == '=');
    i++;
    char *value = dynamicStrcpyUntil(buf, &i, '\n');
    assert(buf[i] == '\n');
    // assign key/value
    kvpair_t * pair = malloc(sizeof(*pair));
    pair->key = key;
    pair->value = value;
    int len = kvArray->size;
    kvArray->pairs = realloc(kvArray->pairs, (len + 1) * sizeof(*(kvArray->pairs)));
    kvArray->pairs[len] = pair;
    kvArray->size++;
  }
  free(buf);
  if (fclose(file) != 0) {
    fprintf(stderr, "Failed to close the input file(%s)!\n", fname);
    return NULL;
  }
  return kvArray;
}

void freeKVs(kvarray_t * pairs) {
  //WRITE ME
  for (int i = 0; i < pairs->size; ++i) {
    free(pairs->pairs[i]->key);
    free(pairs->pairs[i]->value);
    free(pairs->pairs[i]);
  }
  free(pairs->pairs);
  free(pairs);
}

void printKVs(kvarray_t * pairs) {
  //WRITE ME
  for (int i = 0; i < pairs->size; ++i) {
    printf("key = '%s' value = '%s'\n",
      pairs->pairs[i]->key,
      pairs->pairs[i]->value);
  }
}

char * lookupValue(kvarray_t * pairs, const char * key) {
  //WRITE ME
  for (int i = 0; i < pairs->size; ++i) {
    if (strcmp(pairs->pairs[i]->key, key) == 0) {
      return pairs->pairs[i]->value;
    }
  }
  return NULL;
}


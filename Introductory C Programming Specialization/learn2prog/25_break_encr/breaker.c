#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int CaesarBreaker(FILE * file) {
  int cnt[26] = {0};          // must initialize
  int ch = 0;                 // must initialize
  int E = 0;                  // must initialize
  int maxFrq = 0; 
  while ((ch = fgetc(file)) != EOF) {
    // to lower
    if ('A' <= ch && ch <= 'Z') {
        ch = tolower(ch);
    }
    // ignore not char
    if (ch < 'a' || 'z' < ch) {
        continue;
    }
    // update array
    int idx = ch-'a';
    cnt[idx]++;
    // update maxFreq
    if (cnt[idx] > maxFrq) {
        E = ch;
        maxFrq = cnt[idx];
    }
  }
  return (E - 'e' + 26) % 26;
}


int main(int argc, char ** argv) {  
  if (argc != 2) {
    fprintf(stderr, "argument number error\n");
    return EXIT_FAILURE;
  }
  FILE * file = fopen(argv[1], "r");  
  if (file == NULL) {
    fprintf(stderr, "open file(%s) error\n", argv[1]);
    return EXIT_FAILURE;
  }
  printf("%d\n",CaesarBreaker(file));
  if (fclose(file) != 0) {
    fprintf(stderr, "Failed to close the input file!\n");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

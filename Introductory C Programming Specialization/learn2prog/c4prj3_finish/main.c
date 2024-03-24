#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include "cards.h"
#include "deck.h"
#include "eval.h"
#include "future.h"
#include "input.h"


// it take 1 or 2 command line arguments as input.
// The first is the name of the input file to read.
// The second is optional and is the number of Monte Carlo trials to perform.
int main(int argc, char ** argv) {
//   - Check command line arguments/report errors
  if (argc < 2) {
    perror("argument number error\n");
    return EXIT_FAILURE;
  }
//   - If second arguments is not given, use 10,000 as a default value for Monte Carlo trials.
  int num_trials = 10000;
  if (argc ==3) {
    num_trials = atoi(argv[2]);
  }
//   - Open the input file and read the hands in it
//      (you just wrote read_input!)
  FILE *f = fopen(argv[1], "r");
  if (f == NULL) {
    perror("open file error\n");
    return EXIT_FAILURE;
  }


//   - Create a deck with the remaining cards    
//      (you just wrote build_remaining_deck)
  size_t n_hands = 0;
  future_cards_t *fc = malloc(sizeof(*fc));
  fc->decks = NULL;
  fc->n_decks = 0;
  deck_t **decks = read_input(f, &n_hands, fc);
  deck_t *remain = build_remaining_deck(decks, n_hands);
//   - Create an array to count how many times each hand wins, 
//      with one more element for if there was a tie 
//      (so if there are 2 hands, you should have 3 elements).
//      and initialize all its values to 0.
  int winCnts[n_hands+1];
  for (int i=0; i <= n_hands; i++) {
    winCnts[i] = 0;
  }
//   - Do each Monte Carlo trial (repeat num_trials times)
  for (int mc = 0; mc < num_trials; ++mc) {
//      - Shuffle the deck of remaining cards
//        (you wrote shuffle in Course 3)
    shuffle(remain);
//      - Assign unknown cards from the shuffled deck
//        (you just wrote future_cards_from_deck)
    future_cards_from_deck(remain, fc);
//   - Use compare_hands (from Course 3) to figure out which hand won. 
//      Note that with potentially more than 2 hands,
//      this is much like finding the max of an array,
//      but using compare_hands instead of >

// - use cnt[] to count the result of compare, use it to find most win hand
    unsigned cnts[n_hands];
    for (int i=0; i < n_hands; i++) {
      cnts[i] = 0;
    }
    for(int i=0; i < n_hands; i++){
      for (int j=i+1; j < n_hands; j++){
        int cmp = compare_hands(decks[i], decks[j]);
        if(cmp > 0) {
          cnts[i]++;
        } else if (cmp < 0) {
          cnts[j]++;
        } 
      }
    }
    int tie = 0;
    unsigned max = 0;
    // if all hand has same cnt, it will be tie, else update most win hand index
    for (int i=1; i < n_hands; i++) {
      if (cnts[max] < cnts[i]) {
        max = i;
        tie = 0;
      } else if (cnts[max] == cnts[i]) {
        tie = 1;
      }
    }
//      - Increment the win count for the winning hand 
//        (or for the "ties" element of the array if there was a tie).
    if (tie == 0) {
      winCnts[max]++;
    } else {
      winCnts[n_hands]++;
    }
  }
//   - After you do all your trials, you just need to print your results,
  for (size_t i=0; i < n_hands; i++) {
    printf("Hand %zu won %u / %u times (%.2f%%)\n", i, winCnts[i], num_trials, (((float)winCnts[i])/num_trials)*100);
  }
  printf("And there were %u ties\n", winCnts[n_hands]);


//   - free any memory you allocated, and close any open files.
  if (fclose(f) != 0) {
    perror("close file error\n");
    return EXIT_FAILURE;
  }
  for (int i=0; i < n_hands; i++) {
    free_deck(decks[i]);
  }
  free(decks);
  free_deck(remain);
  for(int o=fc->n_decks-1; o >= 0; o--){
    if(fc->decks[o].n_cards != 0) { // Observe: avoid operating on NULL pointer
      free(fc->decks[o].cards);
    }
  }
  free(fc->decks);
  free(fc);
  return EXIT_SUCCESS;
}


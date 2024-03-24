#include <stdlib.h>
#include <stdio.h>
#include "future.h"

void add_future_card(future_cards_t * fc, size_t index, card_t * ptr) {
	// reallocing fc->decks array to be large enough to handle the specified index
	for (; fc->n_decks <= index; ) {
        fc->decks = realloc(fc->decks, (fc->n_decks+1)*sizeof(*(fc->decks)));
        fc->decks[fc->n_decks].cards = NULL;
        fc->decks[fc->n_decks].n_cards = 0;
        fc->n_decks++;
	}
	// This adds ptr into the future cards for the given index
    fc->decks[index].cards = realloc(fc->decks[index].cards, (fc->decks[index].n_cards+1)*sizeof(*(fc->decks[index].cards)));
    fc->decks[index].cards[fc->decks[index].n_cards] = ptr;
    fc->decks[index].n_cards++;
}
void future_cards_from_deck(deck_t * deck, future_cards_t * fc) {
    // check deck space enough for placeholders
    if (deck->n_cards < fc->n_decks) {
        perror("card in deck can't fill in all placeholders\n");
        return ;
    }
    for (int i=0; i < fc->n_decks; i++) {
        card_t **future_cards = fc->decks[i].cards;
        size_t l = fc->decks[i].n_cards;
        if (fc->decks[i].n_cards == 0) {
            continue;
        }
        for (int j=0; j < l; j++) {
            future_cards[j]->value = deck->cards[i]->value;
            future_cards[j]->suit = deck->cards[i]->suit;
        }
    }
}

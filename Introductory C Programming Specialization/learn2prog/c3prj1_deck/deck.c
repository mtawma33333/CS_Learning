#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "deck.h"
void print_hand(deck_t * hand){
    for (int i = 0; i < hand->n_cards; ++i) {
        print_card(*hand->cards[i]);
        printf(" ");
    }
}

int deck_contains(deck_t * d, card_t c) {
    for (int i = 0; i < d->n_cards; ++i) {
        if (c.suit == d->cards[i]->suit && c.value == d->cards[i]->value) return 1;
    }
    return 0;
}

void shuffle(deck_t * d){
    card_t *temp;
    int index1, index2;

    for (int i = 0; i < d->n_cards; ++i) {
        index1 = random() % d->n_cards;
        index2 = random() % d->n_cards;

        temp = d->cards[index1];
        d->cards[index1] = d->cards[index2];
        d->cards[index2] = temp;
    }
}

void assert_full_deck(deck_t * d) {
    assert(d->n_cards == 52);
    for (unsigned i = 0; i < 52; ++i) {
        assert(deck_contains(d, card_from_num(i)));
    }

}
void add_card_to(deck_t * deck, card_t c) {
    card_t *card = malloc(sizeof(*card));
    card->value = c.value;
    card->suit = c.suit;
    deck->cards = realloc(deck->cards, (deck->n_cards + 1)*sizeof(*(deck->cards)));
    deck->cards[deck->n_cards] = card;
    deck->n_cards++;
}
card_t * add_empty_card(deck_t * deck) {
    card_t emptyCard = {.value=0,.suit=0};
    add_card_to(deck, emptyCard);
    return deck->cards[deck->n_cards - 1];
}

deck_t * make_deck_exclude(deck_t * excluded_cards) {
    deck_t *deck = malloc(sizeof(*deck));
    deck->cards = NULL;
    deck->n_cards = 0;
    for (int i = 0; i < 52; ++i) {
        card_t card = card_from_num(i);
        if (deck_contains(excluded_cards,card)) {
            continue;
        }
        add_card_to(deck, card);
    }
    return deck;
}

deck_t * build_remaining_deck(deck_t ** hands, size_t n_hands) {
    // Create a new deck that will hold all the cards from all the hands.
    deck_t *all_hand_cards = malloc(sizeof(*all_hand_cards));
    all_hand_cards->cards = NULL;
    all_hand_cards->n_cards = 0;
    // Loop through each hand in the hands array. For each hand, loop through each card in that hand and add it to all_hand_cards
    for (int i = 0; i < n_hands; ++i) {
        card_t **cards = hands[i]->cards;
        size_t len = hands[i]->n_cards;
        for (int j = 0; j < len; ++j) {
            add_card_to(all_hand_cards, *cards[j]);
        }        
    }
    // create a new deck that excludes these cards. This will be remaining deck.
    deck_t *remaining_deck = make_deck_exclude(all_hand_cards);
    free(all_hand_cards);
    return remaining_deck;
}

void free_deck(deck_t * deck) {
  if (deck == NULL) {  // avoid subsequent NULL pointer exception
    return ; 
  }
  for (int i=0; i < deck->n_cards; i++) {
    free(deck->cards[i]);
  }
  free(deck->cards);
  free(deck);
}


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "input.h"

deck_t * hand_from_string(const char * str, future_cards_t * fc) {
	// create a new hand(deck)
    deck_t *hand = malloc(sizeof(*hand));
    hand->cards = NULL;
    hand->n_cards = 0;
    // read the string
    for (int i=0, len=strlen(str); i < len; ) {
        if (str[i] == '\n' || str[i] == ' ') {
            i++;
            continue;
        }
        assert(i+1 < len);
        char v = str[i++], s = str[i];
        // handle to unknown
        if (v == '?') {
            assert(isdigit(s));
            // Observe the digit after '?' may >= 10 !!!
            size_t sz = 0;
            char num[len];
            for (; isdigit(str[i]); i++, sz++) {
                num[sz] = str[i];
            }
            num[sz] = '\0';
            card_t *ptr = add_empty_card(hand);
            add_future_card(fc, atoi(num), ptr);
        // base case
        } else {
            i++;
            assert(str[i] == ' ' || str[i] == '\n');
            card_t card = card_from_letters(v, s);
            add_card_to(hand, card);
        }
    }
    // handle the invalid input
    if (hand->n_cards < 5) {
        perror("there are fewer than 5 cards\n");
        exit(-1);
    }
    return hand;
}

deck_t ** read_input(FILE * f, size_t * n_hands, future_cards_t * fc) {
    int n = 0;
    deck_t **hand = NULL;
    size_t sz = 0;
    char * buf = NULL;
    // readline to get hand
    for (; getline(&buf, &sz, f) >= 0; ) {
        deck_t *deck = hand_from_string(buf, fc);
        hand = realloc(hand, (n+1)*sizeof(*hand));
        hand[n] = deck;
        n++;
    }
    *n_hands = n;
    free(buf);
    return hand;
}

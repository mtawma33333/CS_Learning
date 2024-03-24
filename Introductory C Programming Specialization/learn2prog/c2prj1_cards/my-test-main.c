#include "cards.h"
#include <stdio.h>

int main(void) {
    const char* toak = ranking_to_string(THREE_OF_A_KIND);
    printf("%s\n", toak);
    printf("correct output: THREE_OF_A_KIND\n");

    card_t t1 = {13, CLUBS};
    assert_card_valid(t1);
    char c1 = value_letter(t1);
    char c2 = suit_letter(t1);
    printf("%c %c\n", c1, c2);
    printf("correct output: K c\n");

    card_t t2 = {14, DIAMONDS};
    assert_card_valid(t2);
    print_card(t2);
    printf("\n");
    printf("correct output: Ad\n");

    card_t t3 = card_from_letters('A', 's');
    assert_card_valid(t3);
    print_card(t3);
    printf("\n");
    printf("correct output: As\n");

    printf("invalid input: {0, SPADES}\n");
    card_t t0 = {0, SPADES};
    assert_card_valid(t0);
}


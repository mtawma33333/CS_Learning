#include "eval.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int card_ptr_comp(const void * vp1, const void * vp2) {
  /*
    指针需要从 const void * 转换为 const card_t *const * ，因为在 C 中， void * 是通用指针类型，可以指向任何类型的数据。
    如果没有这种强制转换，取消引用 vp1 和 vp2 将无效，因为编译器不知道指针指向什么类型的对象，
    因此不知道要读取多少内存或如何解释它。这种强制转换允许指针算术正确工作，并且可以访问 card_t 结构的成员进行比较
  */
    const card_t *const *cp1 = vp1;
    const card_t *const *cp2 = vp2;
    int value_diff = (*cp2)->value - (*cp1)->value;
    if (value_diff != 0) return value_diff;
    return (*cp2)->suit - (*cp1)->suit;
}

suit_t flush_suit(deck_t * hand) {
    if (hand == NULL || hand->n_cards < 5) {
        // Early return if the hand is null or has fewer than 5 cards
        return NUM_SUITS; // Or your equivalent for an invalid suit
    }
    int suit_counts[NUM_SUITS] = {0}; // Initialize count for all suits to 0
    for (size_t i = 0; i < hand->n_cards; i++) {
        suit_t suit = hand->cards[i]->suit;
        suit_counts[suit]++;
        if (suit_counts[suit] >= 5) {
            return suit; // Return as soon as a flush is found
        }
    }
    return NUM_SUITS; // Return invalid suit if no flush is found
}

unsigned get_largest_element(unsigned * arr, size_t n) {
    if (arr == NULL || n == 0) {
        // Handle error if the array is NULL or empty.
        return 0; // Return an appropriate value to indicate an error.
    }
    unsigned largest = arr[0];
    for (size_t i = 1; i < n; i++) {
        if (arr[i] > largest) {
            largest = arr[i];
        }
    }
    return largest;
}

size_t get_match_index(unsigned * match_counts, size_t n,unsigned n_of_akind){
    for (int i=0; i < n; i++) {
        if (match_counts[i] == n_of_akind) {
            return i;
        }
    }
    return 0;
}
ssize_t  find_secondary_pair(deck_t * hand,
			     unsigned * match_counts,
			     size_t match_idx) {
    card_t** card = hand -> cards;
    card_t card1,card2;
    card2 = **(card + match_idx);
    card1=**(card+0);

    for (size_t i=0 ;i< (hand ->n_cards);i++){
        card1=**(card+i);
        if ((match_counts[i] > 1)&&(card1.value != card2.value)) return i;
    }

    return -1;
}

int is_n_length_straight_at(deck_t * hand, size_t index, suit_t fs, int n) {
    card_t ** cards = hand->cards;
    if (fs != NUM_SUITS && fs != cards[index]->suit) {
        return 0;
    }
    int pre = (cards[index]->value) + 1;
    for (int i = index; i < hand->n_cards; ++i)
    {
        int val = cards[i]->value;
        if (pre - 1 == val && (fs == NUM_SUITS || fs == cards[i]->suit)) {
            pre--;
        } else if (pre - val > 1) {
            if (cards[index]->value + 1 - pre >= n) {           // pay attention !!!
                return 1;
            }
            return 0;
        }
    }
    if (cards[index]->value + 1 - pre >= n) {
        return 1;
    }
    return 0;
}

int is_ace_low_straight_at(deck_t * hand, size_t index, suit_t fs) {
    // [index, hand->n_cards) interval must contains 2? 3? 4? 5?
    int isExsit[4] = {0};
    card_t ** cards = hand->cards;
    for (int i=index; i < hand->n_cards; i++) {
        int val = cards[i]->value;
        if (((fs == NUM_SUITS || fs == cards[i]->suit)) && (2 <= val && val <= 5)) {
            isExsit[val-2] = 1;
        }
    }
    return isExsit[0] == 1 && isExsit[1] == 1 && isExsit[2] == 1 && isExsit[3] == 1;
}

int is_straight_at(deck_t * hand, size_t index, suit_t fs) {
    card_t ** cards = hand->cards;
    if (index > hand->n_cards - 5) {
        return 0;
    }
    if (is_n_length_straight_at(hand, index, fs, 5) == 1) {
        return 1;
    }
    if (cards[index]->value == 14 && 
    (fs == NUM_SUITS || fs == cards[index]->suit) &&
    is_ace_low_straight_at(hand, index+1, fs) == 1) {
        return -1;
    }
    return 0;
}

hand_eval_t build_hand_from_match(deck_t * hand,
				                  unsigned n,
				                  hand_ranking_t what,
				                  size_t idx) {
    hand_eval_t ans;
    ans.ranking = what;
    for (int i = 0; i < n; i++) {
        ans.cards[i] = hand->cards[idx + i];
    }

    if (idx == 0) {
        for (int i = n; i < 5; i++) {
            ans.cards[i] = hand->cards[i];
        }
    } else if (idx >= 5 - n) {
        for (int i = 0; i < 5 - n; i++) {
            ans.cards[n + i] = hand->cards[i];
        }
    } else {
        for (int i = 0; i < idx; i++) {
            ans.cards[n + i] = hand->cards[i];
        }
        for (int i = idx + n; i < 5; i++) {
            ans.cards[i] = hand->cards[i];
        }
    }
    return ans;
}


int compare_hands(deck_t * hand1, deck_t * hand2) {
    qsort(hand1->cards, hand1->n_cards, sizeof(card_t*), card_ptr_comp);
    qsort(hand2->cards, hand2->n_cards, sizeof(card_t*), card_ptr_comp);
    hand_eval_t eval1 = evaluate_hand(hand1);
    hand_eval_t eval2 = evaluate_hand(hand2);
    if (eval1.ranking > eval2.ranking) {
        return -1;
    } else if (eval1.ranking < eval2.ranking) {
        return 1;
    } else {
        for (int i=0; i < 5; i++) {
            card_t* card1 = eval1.cards[i];
            card_t* card2 = eval2.cards[i];
            if (card1->value > card2->value) {
                return 1;
            } else if (card1->value < card2->value) {
                return -1;
            }
        }
    }
    return 0;
}



//You will write this function in Course 4.
//For now, we leave a prototype (and provide our
//implementation in eval-c4.o) so that the
//other functions we have provided can make
//use of get_match_counts.
unsigned * get_match_counts(deck_t * hand) {
    size_t n = hand->n_cards;
    unsigned *ans = malloc(n*sizeof(*ans));
    size_t cnts[15];
    for (int i=0; i < 15; i++) {
        cnts[i] = 0;
    }
    for (int i=0; i < n; i++) {
        cnts[hand->cards[i]->value]++;
    }
    for (int i=0; i < n; i++) {
        ans[i] =  cnts[hand->cards[i]->value];
    }
    return ans;
}

// We provide the below functions.  You do NOT need to modify them
// In fact, you should not modify them!


//This function copies a straight starting at index "ind" from deck "from".
//This copies "count" cards (typically 5).
//into the card array "to"
//if "fs" is NUM_SUITS, then suits are ignored.
//if "fs" is any other value, a straight flush (of that suit) is copied.
void copy_straight(card_t ** to, deck_t *from, size_t ind, suit_t fs, size_t count) {
  assert(fs == NUM_SUITS || from->cards[ind]->suit == fs);
  unsigned nextv = from->cards[ind]->value;
  size_t to_ind = 0;
  while (count > 0) {
    assert(ind < from->n_cards);
    assert(nextv >= 2);
    assert(to_ind <5);
    if (from->cards[ind]->value == nextv &&
	(fs == NUM_SUITS || from->cards[ind]->suit == fs)){
      to[to_ind] = from->cards[ind];
      to_ind++;
      count--;
      nextv--;
    }
    ind++;
  }
}


//This looks for a straight (or straight flush if "fs" is not NUM_SUITS)
// in "hand".  It calls the student's is_straight_at for each possible
// index to do the work of detecting the straight.
// If one is found, copy_straight is used to copy the cards into
// "ans".
int find_straight(deck_t * hand, suit_t fs, hand_eval_t * ans) {
  if (hand->n_cards < 5){
    return 0;
  }
  for(size_t i = 0; i <= hand->n_cards -5; i++) {
    int x = is_straight_at(hand, i, fs);
    if (x != 0){
      if (x < 0) { //ace low straight
	assert(hand->cards[i]->value == VALUE_ACE &&
	       (fs == NUM_SUITS || hand->cards[i]->suit == fs));
	ans->cards[4] = hand->cards[i];
	size_t cpind = i+1;
	while(hand->cards[cpind]->value != 5 ||
	      !(fs==NUM_SUITS || hand->cards[cpind]->suit ==fs)){
	  cpind++;
	  assert(cpind < hand->n_cards);
	}
	copy_straight(ans->cards, hand, cpind, fs,4) ;
      }
      else {
	copy_straight(ans->cards, hand, i, fs,5);
      }
      return 1;
    }
  }
  return 0;
}


//This function puts all the hand evaluation logic together.
//This function is longer than we generally like to make functions,
//and is thus not so great for readability :(
hand_eval_t evaluate_hand(deck_t * hand) {
  suit_t fs = flush_suit(hand);
  hand_eval_t ans;
  if (fs != NUM_SUITS) {
    if(find_straight(hand, fs, &ans)) {
      ans.ranking = STRAIGHT_FLUSH;
      return ans;
    }
  }
  unsigned * match_counts = get_match_counts(hand);
  unsigned n_of_a_kind = get_largest_element(match_counts, hand->n_cards);
  assert(n_of_a_kind <= 4);
  size_t match_idx = get_match_index(match_counts, hand->n_cards, n_of_a_kind);
  ssize_t other_pair_idx = find_secondary_pair(hand, match_counts, match_idx);
  free(match_counts);
  if (n_of_a_kind == 4) { //4 of a kind
    return build_hand_from_match(hand, 4, FOUR_OF_A_KIND, match_idx);
  }
  else if (n_of_a_kind == 3 && other_pair_idx >= 0) {     //full house
    ans = build_hand_from_match(hand, 3, FULL_HOUSE, match_idx);
    ans.cards[3] = hand->cards[other_pair_idx];
    ans.cards[4] = hand->cards[other_pair_idx+1];
    return ans;
  }
  else if(fs != NUM_SUITS) { //flush
    ans.ranking = FLUSH;
    size_t copy_idx = 0;
    for(size_t i = 0; i < hand->n_cards;i++) {
      if (hand->cards[i]->suit == fs){
	ans.cards[copy_idx] = hand->cards[i];
	copy_idx++;
	if (copy_idx >=5){
	  break;
	}
      }
    }
    return ans;
  }
  else if(find_straight(hand,NUM_SUITS, &ans)) {     //straight
    ans.ranking = STRAIGHT;
    return ans;
  }
  else if (n_of_a_kind == 3) { //3 of a kind
    return build_hand_from_match(hand, 3, THREE_OF_A_KIND, match_idx);
  }
  else if (other_pair_idx >=0) {     //two pair
    assert(n_of_a_kind ==2);
    ans = build_hand_from_match(hand, 2, TWO_PAIR, match_idx);
    ans.cards[2] = hand->cards[other_pair_idx];
    ans.cards[3] = hand->cards[other_pair_idx + 1];
    if (match_idx > 0) {
      ans.cards[4] = hand->cards[0];
    }
    else if (other_pair_idx > 2) {  //if match_idx is 0, first pair is in 01
      //if other_pair_idx > 2, then, e.g. A A K Q Q
      ans.cards[4] = hand->cards[2];
    }
    else {       //e.g., A A K K Q
      ans.cards[4] = hand->cards[4];
    }
    return ans;
  }
  else if (n_of_a_kind == 2) {
    return build_hand_from_match(hand, 2, PAIR, match_idx);
  }
  return build_hand_from_match(hand, 0, NOTHING, 0);
}



#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../include/wordle.h"
#include "../include/dictionary.h"

/************************************************************************************************
 * --> Wordle Word
 */
const char* WordleWord() {
    const uint32_t rando = arc4random_uniform(DICTIONARY_INDEX_UPPER_BOUND);
    return dictionary[rando];
}

/************************************************************************************************
* --> In Dictionary
*/
bool InDictionary(const char* find_me) {
    // Binary Search
    int lower_bound  = 0;
    int upper_bound = DICTIONARY_INDEX_UPPER_BOUND;
    int mid = DICTIONARY_SIZE / 2;

    while (mid >= 0) {
        const int cmp = strcmp(find_me, dictionary[mid]);

        if (cmp == 0) {
            return true;
        }

        if (cmp < 0) {
            upper_bound = mid;
            mid -= (upper_bound - lower_bound) / 2;
        } else {
            lower_bound = mid;
            mid += (upper_bound - lower_bound) / 2;
        }
    }
    return false;
}
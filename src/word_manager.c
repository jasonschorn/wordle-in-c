#include "../include/dictionary.h"
#include "../include/wordle.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**************************************************************************************************
 *
 * --> Word Manager
 *
 *************************************************************************************************
 *
 * --> Generate New Word Manager
 */
struct WordManager* NewWordManager() {
    struct WordManager* WM = calloc( 1, sizeof( struct WordManager ) );
    if ( WM == NULL ) {
        perror( "Unable to allocate memory --> NewWordManager" );
        exit( EXIT_FAILURE );
    }
    WM->WordleWord = newWordleWord();
    return WM;
}


//-------------------------------------------------------------------------------------------------
// --> Process Key Press
//
void ProcessKeyPress(
        struct WordManager* WM,
        struct DisplayManager* DM,
        const int key
) {
    // Function GetKeyPressed() returns 0 with no key press
    if ( key == 0 ) {
        return;
    }

    if (START) {
        // Handle Return Key pressed
        //
        if ( WM->UserEnteredLetters.cur_col == MAX_NUM_COLS && key == RETURN ) {
            if ( WM->UserEnteredLetters.cur_row < ROWS && ProcessUserEnteredLetters( WM, DM ) ) {
                WM->UserEnteredLetters.cur_col = 0;
                WM->UserEnteredLetters.cur_row++;
            }

            // Handle Delete Key pressed
            //
        } else if ( WM->UserEnteredLetters.cur_col > 0 && key == DELETE ) {
            WM->UserEnteredLetters.cur_col--;
            WM->UserEnteredLetters.total_num_letters--;
            free( WM->UserEnteredLetters.letters[WM->UserEnteredLetters.total_num_letters] );
            WM->UserEnteredLetters.letters[WM->UserEnteredLetters.total_num_letters] = NULL;

            // Handle any Alpha Key pressed
            //
        } else if ( WM->UserEnteredLetters.cur_col < MAX_NUM_COLS ) {
            /**********************************************************************
             *  Convert users keyboard entry to an uppercase letter for display
             **********************************************************************/
            int to_upper = 0;
            if ( LOWER_A <= key && key <= LOWER_Z ) {
                to_upper = key - 32;
            } else if ( UPPER_A <= key && key <= UPPER_Z ) {
                to_upper = key;
            } else {
                return;
            }

            BuildUserEnteredLetters( WM, DM, to_upper );
            WM->UserEnteredLetters.cur_col++;
            WM->UserEnteredLetters.total_num_letters++;
        }
    }
}


//-------------------------------------------------------------------------------------------------
// --> Convert Letters to Words
//
// TODO --> This may be unnecessary
void ConvertLettersToWords( struct WordManager* WM ) {
    const int row = WM->UserEnteredLetters.cur_row;
    const int row_offset = row * COLS;

    snprintf( WM->UserEnteredWords.words[WM->UserEnteredWords.cur_num_words], 6,
              "%c%c%c%c%c",
              WM->UserEnteredLetters.letters[row_offset + 0]->letter[0],
              WM->UserEnteredLetters.letters[row_offset + 1]->letter[0],
              WM->UserEnteredLetters.letters[row_offset + 2]->letter[0],
              WM->UserEnteredLetters.letters[row_offset + 3]->letter[0],
              WM->UserEnteredLetters.letters[row_offset + 4]->letter[0] );

    WM->UserEnteredWords.cur_num_words++;
}


//-------------------------------------------------------------------------------------------------
// --> Wordle Word
//
const char* newWordleWord() {
    const uint32_t rando = arc4random_uniform( DICTIONARY_INDEX_UPPER_BOUND );
    return dictionary[rando];
}


//-------------------------------------------------------------------------------------------------
// --> In Dictionary
//
bool InDictionary( const char* find_me ) {
    // Binary Search
    int lower_bound = 0;
    int upper_bound = DICTIONARY_INDEX_UPPER_BOUND;;

    while ( lower_bound <= upper_bound ) {
        const int mid = (lower_bound + upper_bound) / 2;
        const int cmp = strcmp( find_me, dictionary[mid] );

        if ( cmp == 0 ) {
            return true;
        }

        if ( cmp < 0 ) {
            upper_bound = mid - 1;
        } else {
            lower_bound = mid + 1;
        }
    }
    return false;
}


/******************************************************************************
*
* -- Utility functions for Process User Entered Words
*
*******************************************************************************
*
* --> Is Letter in Word
*/
int is_letter_in_word(
    const char letter,
    const char* word
) {
    /*
     * Check if a given letter is in the Wordle Word
     */
    for ( int i = 0; i < 5; ++i ) {
        if ( letter == word[i] )
            return 1;
    }
    return 0;
}


//-------------------------------------------------------------------------------------------------
// --> In Found
//
int in_found(
    const int index,
    const int found[5]
) {
    /*
     * The found array holds the index at which a given user entered letter
     * appears within the Wordle Word.  This function checks if the given
     * index is in found and is used for determining is a given word contains
     * multiples of the same letter (i.e. feet)
     */
    for ( int i = 0; i < 5; ++i ) {
        if ( index == found[i] )
            return 1;
    }
    return 0;
}


//-------------------------------------------------------------------------------------------------
// --> Set Tile Background Color
//
void set_tile_bg_color(
    const int cur_array_offset,
    const int flags[5],
    struct DisplayManager* DM
) {
    // The cur_array_offset is the total_num_letters from Word Manager UEL
    // and passed in from ProcessUserEnteredLetters
    const int start = cur_array_offset - 5;

    // For each user entered Wordle word, the individual letters are compared with those of the
    // chosen wordle word and the background color for the tiles in each row is set based on
    // the following criteria:
    //      - For every UEL contained in the Wordle Word
    //          - If the positions match, set color to yellow
    //          - Otherwise set that letters background color to green
    //      - For every UEL not in the Wordle Word, do not change that tiles background color
    //
    for ( int i = start, j = 0; i < cur_array_offset; i++, j++ ) {
        switch ( flags[j] ) {
            case CorrectPosition:
                DM->Tiles.bg_color[i] = CORRECT_POS_KEY_COLOR; // Yellow
                break;
            case IncorrectPosition:
                DM->Tiles.bg_color[i] = INCORRECT_POS_KEY_COLOR; // Green
                break;
            case NotInWord:
                DM->Tiles.bg_color[i] = INVALID_CHOSEN_KEY_COLOR; // Gray
                break;
            default:
                break;
        }
    }
}


//-------------------------------------------------------------------------------------------------
// --> Set Keypad Background Colors
//
void set_keypad_bg_color(
    const char word[6],
    const int flags[5],
    struct DisplayManager* DM
) {
    for ( int i = 0; i < 5; i++ ) {
        for ( int j = 0; j < 26; j++ ) {
            if ( word[i] == DM->KeyPad.letters[j] ) {
                switch ( flags[i] ) {
                    case CorrectPosition:
                        DM->KeyPad.bg_color[j] = CORRECT_POS_KEY_COLOR;
                        break;
                    case IncorrectPosition:
                        DM->KeyPad.bg_color[j] = INCORRECT_POS_KEY_COLOR;
                        break;
                    case NotInWord:
                        DM->KeyPad.bg_color[j] = INVALID_CHOSEN_KEY_COLOR;
                        break;
                    default:
                        break;
                }
            }
        }
    }
}


/**************************************************************************************************
 *
 * --> Process the User Entered Letters
 *
**************************************************************************************************/
bool ProcessUserEnteredLetters(
    struct WordManager* WM,
    struct DisplayManager* DM
) {
    /*
     *   Function Summary
     *
     *   1- Copy letters to newWord
     *   2- Check if the word is in the dictionary
     *       NO- Free memory for letters and clear the respective row of letters
     *
     *       YES-
     *           1- Check for all letters that match positionally
     *               - Change flag to Correct Position
     *               - Update found array with the index of the letter in the
     * correct position 2- For every letter in the new word
     *               - Check if it is in the wordle word and the flag isn't for
     * the correct position
     *               - Find all occurrences of the given letter in the wordle word
     *               - Check if any occurrence is NOT in the found array
     *               - Set flag at the given index to Incorrect Position
     *               - Update found array for the given index
     *
     *   This form sets temp[0-5] = '\0' therefore there is no need to explicitly
     *   set temp[5] = '\0'
     *
     *   UEL --> User Entered Letters
     *   UE  --> User Entered
     *   WW  --> Wordle Word
     *
     */

    const int row = WM->UserEnteredLetters.cur_row;
    if ( row < 0 || row >= ROWS ) {
        return false;
    }

    char newWord[6] = {0};
    const int row_offset = row * COLS;

    // Create a string from each UEL
    for ( int i = 0; i < 5; ++i ) {
        newWord[i] = WM->UserEnteredLetters.letters[row_offset + i]->letter[0];
    }

    // Determine if the string is in the Wordle Dictionary
    if ( InDictionary( newWord ) ) {
        int found_index = 0;
        // int flags[5] = {NotInWord, NotInWord, NotInWord, NotInWord, NotInWord}; // Assume no letters match
        int found[5] = {-1, -1, -1, -1, -1,};                                   // Assume no letters will be found

        // Find all UEL in WW that occur in the same position
        for ( int i = 0; i < 5; i++ ) {
            if ( newWord[i] == WM->WordleWord[i] ) {
                DM->flags[i] = CorrectPosition;
                found[found_index++] = i;               // Save index of occurrences. Used when checking for multiples
            }
        }

        // Check for multiples of the same letter (i.e. WW=where and UEL=feet)
        for ( int i = 0; i < 5; i++ ) {
            // Multiple found
            if ( is_letter_in_word( newWord[i], WM->WordleWord ) && DM->flags[i] != CorrectPosition ) {
                int index = 0;
                int occurrences[5] = {-1, -1, -1, -1, -1};

                // Find all multiples and save the index where they occur
                for ( int j = 0; j < 5; j++ ) {
                    if ( newWord[i] == WM->WordleWord[j] ) {
                        occurrences[index++] = j;
                    }
                }

                // Compare all multiples to previously found multiples and, for each
                // new occurrence, add the appropriate flag as well as the given index
                // to the found array
                for ( int k = 0; k < index; k++ ) {
                    if ( !in_found( occurrences[k], found ) ) {
                        DM->flags[i] = IncorrectPosition;
                        found[found_index++] = i;
                        break;
                    }
                }
            }
        }

        //
        // Add flags to Display Manager --> BgColorFlags arrays using WM->total_num_letters
        set_tile_bg_color( WM->UserEnteredLetters.total_num_letters, DM->flags, DM );
        set_keypad_bg_color( newWord, DM->flags, DM );

        if ( strcmp( newWord, WM->WordleWord ) == 0 ) {
            START = false;
        }

        return true;
    } // End if( InDictionary )

    /*
     *  Clear the current row of user entered letters and free memory for each
     *  allocated letter (Text*.letter)
     */
    for ( int i = 1; i < 6; i++ ) {
        free( WM->UserEnteredLetters.letters[WM->UserEnteredLetters.total_num_letters - i] );
        WM->UserEnteredLetters.letters[WM->UserEnteredLetters.total_num_letters - i] = NULL;
    }

    WM->UserEnteredLetters.total_num_letters -= 5;
    WM->UserEnteredLetters.cur_col = 0;
    return false;
}


//-------------------------------------------------------------------------------------------------
// --> Build User Entered Letters
//
void BuildUserEnteredLetters(
    struct WordManager* WM,
    const struct DisplayManager* DM,
    const int key
) {
    const int tile_index = WM->UserEnteredLetters.cur_row * COLS + WM->UserEnteredLetters.cur_col;
    WM->UserEnteredLetters.letters[tile_index] = malloc( sizeof( struct KeyPadLetter ) );

    if ( WM->UserEnteredLetters.letters[tile_index] == NULL ) {
        exit( EXIT_FAILURE );
    }

    WM->UserEnteredLetters.letters[tile_index]->font_size = TILE_FONT_SIZE;
    WM->UserEnteredLetters.letters[tile_index]->color = TILE_TEXT_COLOR;
    WM->UserEnteredLetters.letters[tile_index]->position = (Vector2){
        DM->Tiles.tile[tile_index].x + (DM->Tiles.tile[tile_index].width / 2) - 10,
        DM->Tiles.tile[tile_index].y + (DM->Tiles.tile[tile_index].height / 2) - ((float) TILE_FONT_SIZE / 2),
    };
    WM->UserEnteredLetters.letters[tile_index]->letter[0] = (char) key;
    WM->UserEnteredLetters.letters[tile_index]->letter[1] = '\0';
}


//-------------------------------------------------------------------------------------------------
// --> Render User Entered Letters
//
void RenderUserEnteredLetters( const struct WordManager* WM ) {
    if ( WM->UserEnteredLetters.total_num_letters > 0 ) {
        for ( int i = 0; i < WM->UserEnteredLetters.total_num_letters; ++i ) {
            DrawTextEx( GameFont, WM->UserEnteredLetters.letters[i]->letter,
                        WM->UserEnteredLetters.letters[i]->position,
                        (float) WM->UserEnteredLetters.letters[i]->font_size, 0,
                        WM->UserEnteredLetters.letters[i]->color );
        }
    }
}


//-------------------------------------------------------------------------------------------------
// --> Free User Entered Letters
//
void FreeUserEnteredLetters( struct WordManager* wm ) {
    if ( wm->UserEnteredLetters.total_num_letters > 0 ) {
        for ( int l = 0; l < wm->UserEnteredLetters.total_num_letters; ++l ) {
            free( wm->UserEnteredLetters.letters[l] );
            wm->UserEnteredLetters.letters[l] = NULL;
        }
    }

    wm->UserEnteredLetters.cur_col = 0;
    wm->UserEnteredLetters.cur_row = 0;
    wm->UserEnteredLetters.total_num_letters = 0;
}
#include "stdio.h"
#include "stdlib.h"
#include "../include/wordle.h"

enum Index {
    Start = 0,
    New,
    Quit,
    Hint,
};

/**************************************************************************************************
*
* --> Collision Manager
*
***************************************************************************************************
*
* --> New Manager
*/
struct KeyCollisionManager* NewKeyCollisionManager( struct DisplayManager* DM ) {
    struct KeyCollisionManager* KCM = malloc( sizeof( struct KeyCollisionManager ) );
    if ( KCM ) {
        // Note each element in qwerty_keys is a const int defined in wordle.c
        //
        const int qwerty_keys[28] = {
            Q, W, E, R, T, Y, U, I, O, P, // Upper row of keys
            A, S, D, F, G, H, J, K, L,    // Middle row of keys
            Z, X, C, V, B, N, M,          // Lower rows of keys
            RETURN,                       // Left side of lower key set
            DELETE,                       // Right side of lower key set
        };

        // Map keyboard letters as well as Return and Backspace to their respective rectangles
        //
        for ( int i = 0; i < 28; i++ ) {
            KCM->keys[i] = qwerty_keys[i];

            // KeyPad.upperKey [middleKey, lowerKey] are type Rectangle
            if ( i < 10 ) {
                KCM->keypad_rects[i] = DM->KeyPad.upper_key_rect_rect[i];
            }

            if ( i < 9 ) {
                KCM->keypad_rects[i + 10] = DM->KeyPad.middle_key_rect[i];
            }

            if ( i < 7 ) {
                KCM->keypad_rects[i + 19] = DM->KeyPad.lower_key_rect[i];
            }
        }

        KCM->keypad_rects[26] = DM->KeyPad.return_key_rect;
        KCM->keypad_rects[27] = DM->KeyPad.delete_key_rect;

        return KCM;
    }

    return NULL; // Allocation failed
}

/**********************************************************************************************
* --> Process Mouse Click
*/
void ProcessMouseClick( struct WordManager* WM,
                        struct DisplayManager* DM,
                        struct NavButtonManager* NBM,
                        const struct KeyCollisionManager* KCM,
                        const Vector2 mouse_pos
) {
    /**********************************************************************************************
    * --> Keypad Collision Detection
    */
    if ( START ) {
        for ( int i = 0; i < 28; ++i ) {
            const int cur_key = KCM->keys[i];

            /**************************************************************************************
             *  Only 3 allowable options for collisions:
             *      1- Alpha key (a, ..., z)
             *      2- Return key
             *      3- Delete key (built on a Mac)
             *************************************************************************************/
            if ( CheckCollisionPointRec( mouse_pos, KCM->keypad_rects[i] ) ) {
                /**********************************************************************************
                 *  Build User Entered Letter
                 */
                if ( A <= cur_key && cur_key <= Z ) {
                    if ( WM->UserEnteredLetters.cur_col < MAX_NUM_COLS ) {
                        BuildUserEnteredLetters( WM, DM, cur_key );
                        WM->UserEnteredLetters.cur_col++;
                        WM->UserEnteredLetters.total_num_letters++;
                    }
                /**********************************************************************************
                 * --> Process User Entered Letters
                 */
                } else if ( WM->UserEnteredLetters.cur_col == MAX_NUM_COLS && cur_key == RETURN ) {
                    if ( WM->UserEnteredLetters.cur_row < ROWS && ProcessUserEnteredLetters( WM, DM ) ) {
                        WM->UserEnteredLetters.cur_col = 0;
                        WM->UserEnteredLetters.cur_row++;
                    }

                /**********************************************************************************
                 * --> Delete a User Entered Letter
                 */
                } else if ( WM->UserEnteredLetters.cur_col > 0 && cur_key == DELETE ) {
                    WM->UserEnteredLetters.cur_col--;
                    WM->UserEnteredLetters.total_num_letters--;
                    free( WM->UserEnteredLetters.letters[WM->UserEnteredLetters.total_num_letters] );
                    WM->UserEnteredLetters.letters[WM->UserEnteredLetters.total_num_letters] = NULL;
                }
            }
        }
    }

    /**********************************************************************************************
    * --> Start Game
    */
    if ( CheckCollisionPointRec( mouse_pos, NBM->nav_buttons[Start].body ) ) {
        if ( !START ) {
            START = true;
        }
    }

    /**********************************************************************************************
    * --> New Game
    */
    if ( CheckCollisionPointRec( mouse_pos, NBM->nav_buttons[New].body ) ) {
        ResetBgColors( DM );
        FreeUserEnteredLetters(WM);
        WM->WordleWord = newWordleWord();
    }

    /**********************************************************************************************
    * --> Hint
    */
    if (START && CheckCollisionPointRec( mouse_pos, NBM->nav_buttons[Hint].body ) ) {
        if (DM->cur_num_hints < 2) {
            // Iterate over the collection of positional flags
            for (int i=0; i<5; ++i) {

                // Find the first index that does NOT correspond to a correctly
                // chosen letter
                if (DM->flags[i] != CorrectPosition) {

                    // Find the corresponding KeyPad letter and set its
                    // background color to green
                    for (int j=0; j<26; ++j) {
                        if (DM->KeyPad.letters[j] == WM->WordleWord[i]) {
                            DM->KeyPad.bg_color[j] = CORRECT_POS_KEY_COLOR;
                        }
                    }
                    DM->cur_num_hints++;
                    break;
                }
            }
            printf("Cur num hints %d\n", DM->cur_num_hints);
        }


    }
    /**********************************************************************************************
    * --> Quit Game
    */
    if ( CheckCollisionPointRec( mouse_pos, NBM->nav_buttons[Quit].body ) ) {
        QUIT = true;
    }

}

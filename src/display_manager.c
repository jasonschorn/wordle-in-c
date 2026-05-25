#include <stdlib.h>
#include <stdio.h>
#include "../include/wordle.h"

/**************************************************************************************************
*
* --> Display Manager
*
**************************************************************************************************
*
* --> Build a New Display Manager
*/
struct DisplayManager* NewDisplayManager() {
    struct DisplayManager* DM = malloc(sizeof(struct DisplayManager));
    if (DM == NULL) {
        perror("Allocation failure --> NewDisplayManager()");
        exit(EXIT_FAILURE);
    }

    for (int i=0; i<30; ++i) {
        DM->Tiles.bg_color[i] = TILE_BG_COLOR;

        if (i<26) {
            DM->KeyPad.bg_color[i] = KEY_AVAILABLE_COLOR;
        }
    }

    BuildWordleDisplay(DM);
    BuildWordleKeyPad(DM);
    BuildKeypadText(DM);

    for (int i=0; i<26; ++i) {
        if (i < 5) {
            DM->flags[i] = NotInWord;
        }
        DM->KeyPad.letters[i] = QWERTY[i];
    }

    return DM;
}


/**************************************************************************************************
* --> Reset Tile Background Colors
*/
void ResetBgColors(struct DisplayManager* DM) {
    /*
     *  Called when New button is clicked
     */
    for (int i=0; i<30; ++i) {
        DM->Tiles.bg_color[i] = TILE_BG_COLOR;
        if (i<26) {
            DM->KeyPad.bg_color[i] = KEY_AVAILABLE_COLOR;
        }
    }
}


/**************************************************************************************************
*
* --> Builders
*
***************************************************************************************************
*
* --> Build Wordle Keypad
*/
void BuildWordleDisplay( struct DisplayManager* DM ) {
    int count = 0;

    for ( int row = 0; row < 6; ++row ) {
        for ( int col = 0; col < 5; ++col ) {
            DM->Tiles.tile[count] = (Rectangle){
                TILE_START_X + ( TILE_GAP * (float)col ),
                TILE_START_Y + ( TILE_GAP * (float)row ),
                TILE_W,
                TILE_H
            };

            DM->Tiles.border[count] = (Rectangle){
                TILE_START_X + ( TILE_GAP * (float)col ) - TILE_BORDER_OFFSET,
                TILE_START_Y + ( TILE_GAP * (float)row ) - TILE_BORDER_OFFSET,
                TILE_W + 2 * TILE_BORDER_OFFSET,
                TILE_H + 2 * TILE_BORDER_OFFSET
            };
            count++;
        }
    }
}


/**************************************************************************************************
* --> Build Wordle Keypad
*/
void BuildWordleKeyPad( struct DisplayManager* DM ) {
    /**************************************************************************
    * Build Return Key Rectangle
    */
    DM->KeyPad.return_key_rect = (Rectangle) {
        RETURN_KEY_START_X,
        RETURN_DEL_KEY_START_Y,
        RETURN_DEL_KEY_W,
        RETURN_DEL_KEY_H
    };

    /**************************************************************************
    * Build Delete Key Rectangle
    */
    DM->KeyPad.delete_key_rect = (Rectangle) {
        DELETE_KEY_START_X,
        RETURN_DEL_KEY_START_Y,
        RETURN_DEL_KEY_W,
        RETURN_DEL_KEY_H
    };

    /**************************************************************************
    *
    */
    for ( int i = 0; i < 10; ++i ) {
        /**********************************************************************
        * Middle Keypad Key Rectangles
        */
        if ( i < 9 ) {
            DM->KeyPad.middle_key_rect[i] = (Rectangle){
                MID_KEYPAD_START_X + ( KEYPAD_GAP * (float)i ),
                MID_KEYPAD_START_Y,
                KEYPAD_W,
                KEYPAD_H
            };
        }

        /**********************************************************************
        * Lower Keypad Key Rectangles
        */
        if ( i < 7 ) {
            DM->KeyPad.lower_key_rect[i] = (Rectangle){
                LOWER_KEYPAD_START_X + ( KEYPAD_GAP * (float)i ),
                LOWER_KEYPAD_START_Y,
                KEYPAD_W,
                KEYPAD_H
            };
        }
        /**********************************************************************
        * Upper Keypad Key Rectangles
        */
        DM->KeyPad.upper_key_rect_rect[i] = (Rectangle){
            UPPER_KEYPAD_START_X + ( KEYPAD_GAP * (float)i ),
            UPPER_KEYPAD_START_Y,
            KEYPAD_W,
            KEYPAD_H
        };
    }
}


/**************************************************************************************************
* --> Build Wordle Keypad Text
*/
void BuildKeypadText(struct DisplayManager* DM) {
    //
    // --> Return Key
    //
    DM->KeyPadText.return_key = (struct ReturnDeleteKeys) {
        .font_size = KEYPAD_FONT_SIZE,
        .color = KEYPAD_TEXT_COLOR,
        .text = "Return",
        .position = (Vector2) {
            DM->KeyPad.return_key_rect.x + (DM->KeyPad.return_key_rect.width/2) - 45,
            DM->KeyPad.return_key_rect.y + (DM->KeyPad.return_key_rect.height/2) - (float)KEYPAD_FONT_SIZE/2
        }
    };
    //
    // -->Delete Key
    //
    DM->KeyPadText.delete_key = (struct ReturnDeleteKeys) {
        .font_size = KEYPAD_FONT_SIZE,
        .color = KEYPAD_TEXT_COLOR,
        .text = "Delete",
        .position = (Vector2) {
            DM->KeyPad.delete_key_rect.x + (DM->KeyPad.delete_key_rect.width/2) - 45,
            DM->KeyPad.delete_key_rect.y + (DM->KeyPad.delete_key_rect.height/2) - (float)KEYPAD_FONT_SIZE/2
        }
    };
    //
    // --> Upper keys
    //
    for (int i=0; i<10; ++i) {
        DM->KeyPadText.keypad_text[i] = (struct KeyPadLetter) {
            .font_size  = KEYPAD_FONT_SIZE,
            .color      = KEYPAD_TEXT_COLOR,
            .letter     = { UPPER_KEYS[i][0], '\0' },
            .position   = (Vector2) {
                DM->KeyPad.upper_key_rect_rect[i].x + (DM->KeyPad.upper_key_rect_rect[i].width/2) - 10,
                DM->KeyPad.upper_key_rect_rect[i].y + (DM->KeyPad.upper_key_rect_rect[i].height/2) - ((float)KEYPAD_FONT_SIZE/2)
            },
        };
    }
    //
    // --> Middle Keys
    //
    for (int i=0; i<9; ++i) {
        DM->KeyPadText.keypad_text[i + 10] = (struct KeyPadLetter) {
            .font_size  = KEYPAD_FONT_SIZE,
            .color      = KEYPAD_TEXT_COLOR,
            .letter     = { MID_KEYS[i][0], '\0' },
            .position   = (Vector2) {
                DM->KeyPad.middle_key_rect[i].x + (DM->KeyPad.middle_key_rect[i].width/2) - 10,
                DM->KeyPad.middle_key_rect[i].y + (DM->KeyPad.middle_key_rect[i].height/2) - ((float)KEYPAD_FONT_SIZE/2)
            },

        };
    }
    //
    // --> Lower Keys
    //
    for (int i=0; i<7; ++i) {
        DM->KeyPadText.keypad_text[i + 19] = (struct KeyPadLetter) {
            .font_size  = KEYPAD_FONT_SIZE,
            .color      = KEYPAD_TEXT_COLOR,
            .letter     = { LOWER_KEYS[i][0], '\0' },
            .position   = (Vector2) {
                DM->KeyPad.lower_key_rect[i].x + (DM->KeyPad.lower_key_rect[i].width/2) - 10,
                DM->KeyPad.lower_key_rect[i].y + (DM->KeyPad.lower_key_rect[i].height/2) - ((float)KEYPAD_FONT_SIZE/2)
            },

        };
    }
}


/**************************************************************************************************
*
* --> Renders
*
***************************************************************************************************
*
* --> Render Wordle KeyPad
*/
void RenderWordleKeyPad( const struct DisplayManager* DM ) {

    /**********************************************************************************************
    * --> Return Key
    */
    if (CheckCollisionPointRec( GetMousePosition(), DM->KeyPad.return_key_rect)) {
        DrawRectangleRounded( DM->KeyPad.return_key_rect, ROUNDNESS, SEGMENTS, TILE_MOUSE_OVER_COLOR);
    } else {
        DrawRectangleRounded( DM->KeyPad.return_key_rect, ROUNDNESS, SEGMENTS, KEY_AVAILABLE_COLOR);
    }

    /**********************************************************************************************
    * --> Delete Key
    */
    if (CheckCollisionPointRec( GetMousePosition(), DM->KeyPad.delete_key_rect)) {
        DrawRectangleRounded( DM->KeyPad.delete_key_rect, ROUNDNESS, SEGMENTS, TILE_MOUSE_OVER_COLOR);
    } else {
        DrawRectangleRounded( DM->KeyPad.delete_key_rect, ROUNDNESS, SEGMENTS, KEY_AVAILABLE_COLOR);
    }


    /**********************************************************************************************
    * --> Upper Keyboard Keys
    */
    for(int i=0; i<10; ++i) {
        if ( CheckCollisionPointRec( GetMousePosition(), DM->KeyPad.upper_key_rect_rect[i] ) ) {
            DrawRectangleRounded( DM->KeyPad.upper_key_rect_rect[i], ROUNDNESS, SEGMENTS, TILE_MOUSE_OVER_COLOR );

        } else {
            DrawRectangleRounded( DM->KeyPad.upper_key_rect_rect[i], ROUNDNESS, SEGMENTS, DM->KeyPad.bg_color[i] );
        }
    }

    /**********************************************************************************************
    * --> Middle Keyboard Keys
    */
    for (int i=0; i<9; ++i) {
        if ( CheckCollisionPointRec( GetMousePosition(), DM->KeyPad.middle_key_rect[i] ) ) {
            DrawRectangleRounded( DM->KeyPad.middle_key_rect[i], ROUNDNESS, SEGMENTS, TILE_MOUSE_OVER_COLOR );

        } else {
            DrawRectangleRounded( DM->KeyPad.middle_key_rect[i], ROUNDNESS, SEGMENTS, DM->KeyPad.bg_color[i+10] );
        }
    }

    /**********************************************************************************************
    * --> Lower Keyboard Keys
    */
    for (int i=0; i<7; ++i) {
        if ( CheckCollisionPointRec( GetMousePosition(), DM->KeyPad.lower_key_rect[i] ) ) {
            DrawRectangleRounded( DM->KeyPad.lower_key_rect[i], ROUNDNESS, SEGMENTS, TILE_MOUSE_OVER_COLOR );
        } else {
            DrawRectangleRounded( DM->KeyPad.lower_key_rect[i], ROUNDNESS, SEGMENTS, DM->KeyPad.bg_color[i+19] );
        }
    }
}


/**************************************************************************************************
* --> Render Wordle Display 6x5 grid
*/
void RenderWordleDisplay( const struct DisplayManager* DM ) {
    for ( int i = 0; i < 30; ++i ) {
        DrawRectangleRounded( DM->Tiles.tile[i], ROUNDNESS, SEGMENTS, DM->Tiles.bg_color[i]);
        DrawRectangleRoundedLinesEx(DM->Tiles.border[i], ROUNDNESS, SEGMENTS, (float)THICKNESS, TILE_BORDER_COLOR );
    }
}


/**************************************************************************************************
* --> Render Keypad Text
*/
void RenderKeyPadText(const struct DisplayManager* DM) {
    //
    // --> Qwerty Keys
    //
    for (int i=0; i<26; ++i) {
        DrawTextEx(
            GameFont, DM->KeyPadText.keypad_text[i].letter,
            DM->KeyPadText.keypad_text[i].position, (float)KEYPAD_FONT_SIZE,
            0,
            KEYPAD_TEXT_COLOR
        );
    }
    //
    // --> Return Key
    //
    DrawTextEx(
            GameFont,
            DM->KeyPadText.return_key.text,
            DM->KeyPadText.return_key.position,
            (float)KEYPAD_FONT_SIZE,
            0,
            KEYPAD_TEXT_COLOR
    );
    //
    // --> Delete Key
    //
    DrawTextEx(
            GameFont,
            DM->KeyPadText.delete_key.text,
            DM->KeyPadText.delete_key.position,
            (float)KEYPAD_FONT_SIZE,
            0,
            KEYPAD_TEXT_COLOR
    );
}
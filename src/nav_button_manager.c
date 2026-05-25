#include <stdlib.h>
#include "../include/wordle.h"

/**************************************************************************************************
*
* --> Navigation Button Manager
*
***************************************************************************************************
*
* --> New Manager
*/
struct NavButtonManager* NewNavButtonManager() {
    struct NavButtonManager* nav = calloc(1, sizeof(struct NavButtonManager));

    if (nav != NULL) {
        return nav;
    }
    return NULL;
}

/**************************************************************************************************
* --> Generate Buttons
*/
void GenerateNavButtons(struct NavButtonManager* NBM) {
    /*
     * There is really no point in writing this function since I already know
     * the number of buttons but, I tried to "generalize" the creation of buttons.
     *
     */
    int num_buttons = NUM_BUTTONS;
    float offset;
    float start_x;

    if (num_buttons % 2 == 0) {
        // Even number of buttons
        offset = (float)num_buttons/2;
        start_x = ((float)SCREEN_W / 2) - offset*(NAV_W + NAV_BTN_GAP) + NAV_BTN_GAP/2;

    } else {
        // Odd number of buttons
        offset = (float)num_buttons/2;
        start_x = (float)SCREEN_W / 2 - offset*(NAV_W + NAV_BTN_GAP)  + NAV_BTN_GAP/2;
    }

    // Create buttons and store them in NavButtonManager->nav_buttons
    for (int i=0; i< num_buttons; ++i) {
        // Create the button body
        NBM->nav_buttons[i].body = (Rectangle) {
            start_x + (float)i*(NAV_W + NAV_BTN_GAP),
            START_Y,
            NAV_W,
            NAV_H,
        };

        // Create the button border
        NBM->nav_buttons[i].border = (Rectangle) {
            start_x + (float)i*(NAV_W + NAV_BTN_GAP),
            START_Y,
            NAV_W,
            NAV_H,
        };

        // Labels must be set prior to MeasureTextEx
        NBM->nav_buttons[i].label = BUTTON_LABELS[i];

        Vector2 textSize = MeasureTextEx(
            GameFont,
            NBM->nav_buttons[i].label,
            NAV_BTN_FONT_SIZE,
            0
        );
        NBM->nav_buttons[i].text_pos = (Vector2) {
            NBM->nav_buttons[i].body.x + (NBM->nav_buttons[i].body.width - textSize.x) / 2.0f,
            NBM->nav_buttons[i].body.y + (NBM->nav_buttons[i].body.height - textSize.y) / 2.0f
        };

        NBM->nav_buttons[i].btn_bg_color = TILE_BG_COLOR;
        NBM->nav_buttons[i].border_color = TILE_BORDER_COLOR;
    }
}

/**************************************************************************************************
* --> Render Buttons
*/
void RenderNavButtons(struct NavButtonManager* NBM) {
    for (int i=0; i<NUM_BUTTONS;++i) {
        if (CheckCollisionPointRec( GetMousePosition(), NBM->nav_buttons[i].body ) ) {
            DrawRectangleRounded(
                NBM->nav_buttons[i].body,
                ROUNDNESS,
                SEGMENTS,
                TILE_MOUSE_OVER_COLOR //NBM->nav_buttons[i].btn_bg_color
            );

            DrawTextEx(
                GameFont,
                NBM->nav_buttons[i].label,
                NBM->nav_buttons[i].text_pos,
                NAV_BTN_FONT_SIZE,
                0,
                (Color) {15, 15, 15, 255}
            );

        } else {
            DrawRectangleRounded(
                NBM->nav_buttons[i].body,
                ROUNDNESS,
                SEGMENTS,
                NBM->nav_buttons[i].btn_bg_color
                );

            DrawTextEx(
                GameFont,
                NBM->nav_buttons[i].label,
                NBM->nav_buttons[i].text_pos,
                NAV_BTN_FONT_SIZE,
                0,
                NAV_BTN_FONT_COLOR
            );
        }

        DrawRectangleRoundedLinesEx(
            NBM->nav_buttons[i].border,
            ROUNDNESS,
            SEGMENTS,
            (float)THICKNESS,
            TILE_BORDER_COLOR
            );
    }
}

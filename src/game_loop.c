#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "../include/wordle.h"


void game_loop() {
    /**********************************************************************************************
    * -->
    */
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(SCREEN_W, SCREEN_H, "Worlde");
    SetTargetFPS(60);
    LoadGameFont();

    /**********************************************************************************************
    * --> Initialize Managers
    */
    struct DisplayManager* DM       = NewDisplayManager();
    struct WordManager* WM          = NewWordManager();
    struct KeyCollisionManager* KCM = NewKeyCollisionManager(DM);
    struct NavButtonManager* NBM    = NewNavButtonManager();
    GenerateNavButtons( NBM );


    while (!WindowShouldClose() && !QUIT) {
        /******************************************************************************************
        * --> Capture Key Presses
        */
        const int key = GetKeyPressed();
        ProcessKeyPress(WM, DM, key);

        /******************************************************************************************
        * --> Capture Mouse Clicks
        */
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            ProcessMouseClick(WM, DM, NBM, KCM, GetMousePosition());
        }

        /******************************************************************************************
        * --> Draw objects to screen
        */
        BeginDrawing();
        ClearBackground(SCREEN_BG_COLOR);
        DrawText(
            LOGO,
            (SCREEN_W / 2) - MeasureText( LOGO, LOGO_FONT_SIZE ) / 2,
            LOGO_Y,
            LOGO_FONT_SIZE,
            LOGO_COLOR
        );


        /******************************************************************************************
        * --> Renderers
        */
        RenderWordleDisplay(DM);
        RenderWordleKeyPad(DM);
        RenderKeyPadText(DM);
        RenderNavButtons(NBM);
        RenderUserEnteredLetters(WM);

        // For ensuring objects are centered
        // DrawLine( SCREEN_W/2, 0, SCREEN_W/2, 800, WHITE  );
        EndDrawing();
    }

    /**********************************************************************************************
    * --> Free dynamically allocated resources
    */
    FreeUserEnteredLetters(WM);
    free(WM);
    free(DM);
    free(NBM);
    WM = NULL;
    DM = NULL;
    NBM = NULL;

    /**********************************************************************************************
    * --> Cleanup
    */
    UnloadGameFont();
    CloseWindow();
}

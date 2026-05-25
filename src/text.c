#include <stddef.h>
#include "../include/wordle.h"

/**************************************************************************************************
* --> Game Font Preset
*/
Font GameFont = {0};    // Required because GameFont is defined as extern in wordle.h


/**************************************************************************************************
* --> Load Game Font
*/
void LoadGameFont() {
    GameFont = LoadFontEx("../assets/MonaspiceRnNerdFontMono-Regular.otf", 64, NULL, 0);
    SetTextureFilter(GameFont.texture, TEXTURE_FILTER_BILINEAR);    // or TEXTURE_FILTER_TRILINEAR
}


/**************************************************************************************************
* --> Unload the Game Font
*/
void UnloadGameFont() {
    UnloadFont(GameFont);
}




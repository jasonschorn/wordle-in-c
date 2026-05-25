#ifndef CWORDLE_WORDLE_H
#define CWORDLE_WORDLE_H
#include "raylib.h"

/**************************************************************************************************
***************************************************************************************************
*
* --> Game Constants
*
**************************************************************************************************
**************************************************************************************************/
// clang-format off
//
extern bool START;
extern bool NEW;
extern bool QUIT;

extern const char* LOGO;
extern const int LOGO_FONT_SIZE;
extern const int LOGO_Y;
extern const Color LOGO_COLOR;

extern const int DICTIONARY_SIZE;
extern const int DICTIONARY_INDEX_UPPER_BOUND;
/**********************************************************
* --> Keypad Entry Constraints
**********************************************************/
extern const int  LOWER_A;
extern const int  LOWER_Z;
extern const int  UPPER_A;
extern const int  UPPER_Z;
extern const int  MAX_NUM_COLS;

/**********************************************************
* --> Mouse Click
**********************************************************/
extern const int  A;
extern const int  B;
extern const int  C;
extern const int  D;
extern const int  E;
extern const int  F;
extern const int  G;
extern const int  H;
extern const int  I;
extern const int  J;
extern const int  K;
extern const int  L;
extern const int  M;
extern const int  N;
extern const int  O;
extern const int  P;
extern const int  Q;
extern const int  R;
extern const int  S;
extern const int  T;
extern const int  U;
extern const int  V;
extern const int  W;
extern const int  X;
extern const int  Y;
extern const int  Z;
extern const int  RETURN;
extern const int  DELETE;

/**********************************************************
* --> Screen Dimensions
**********************************************************/
extern const int   SCREEN_W;
extern const int   SCREEN_H;

/**********************************************************
* --> Tile Colors
**********************************************************/
extern const Color TILE_BG_COLOR;
extern const Color TILE_BORDER_COLOR;
extern const Color TILE_TEXT_COLOR;
extern const Color TILE_MOUSE_OVER_COLOR;

/**********************************************************
* --> Keypad Colors
**********************************************************/
extern const Color KEYPAD_TEXT_COLOR;
extern const Color SCREEN_BG_COLOR;
extern const Color KEY_AVAILABLE_COLOR;

/**********************************************************
* --> Key Validation Colors
**********************************************************/
extern const Color CORRECT_POS_KEY_COLOR;
extern const Color INCORRECT_POS_KEY_COLOR;
extern const Color INVALID_CHOSEN_KEY_COLOR;

/**********************************************************
* --> Tile Constants
**********************************************************/
extern const int   ROWS;
extern const int   COLS;
extern const float TILE_W;
extern const float TILE_H;
extern const float TILE_GAP;
extern const float TILE_START_X;
extern const float TILE_START_Y;
extern const float TILE_BORDER_OFFSET;
extern const int   TILE_FONT_SIZE;

/**********************************************************
* --> Keypad Constants
**********************************************************/
extern const int   KEYPAD_FONT_SIZE;
extern const float KEYPAD_W;
extern const float KEYPAD_H;
extern const float KEYPAD_GAP;
extern const float UPPER_KEYPAD_START_X;
extern const float MID_KEYPAD_START_X;
extern const float LOWER_KEYPAD_START_X;
extern const float UPPER_KEYPAD_START_Y;
extern const float MID_KEYPAD_START_Y;
extern const float LOWER_KEYPAD_START_Y;
extern const float RETURN_DEL_KEY_W;
extern const float RETURN_DEL_KEY_H;
extern const float RETURN_KEY_START_X;
extern const float DELETE_KEY_START_X;
extern const float RETURN_DEL_KEY_START_Y;

/**********************************************************
* --> Navigation Button Constants
**********************************************************/
#define NUM_BUTTONS 4
extern const int    ALLOWABLE_HINTS;
extern char*        BUTTON_LABELS[];

extern const float  NAV_W;
extern const float  NAV_H;
extern const int    SEGMENTS;
extern const float  ROUNDNESS;
extern const int    THICKNESS;
extern const float  NAV_BTN_GAP;
extern const float  NAV_BTN_BORDER_OFFSET;
extern const float  NAV_BTN_FONT_SIZE;
extern const Color  NAV_BTN_FONT_COLOR;
extern const float  START_Y;

/**********************************************************
* --> Qwerty Keyboard Constants
**********************************************************/
extern char* UPPER_KEYS[];
extern char* MID_KEYS[];
extern char* LOWER_KEYS[];
extern char  QWERTY[];

/**************************************************************************************************
* --> FLags
*/
enum Flags {
    NotInWord           = 0,     // Letter's not in the given Wordle Word
    IncorrectPosition   = 1,     // Letter's in word but not in the correct position
    CorrectPosition     = 2,     // Letter's in word and in the correct position
};

/**************************************************************************************************
 **************************************************************************************************
 *
 * --> Struct Definitions
 *
 *************************************************************************************************
 *************************************************************************************************/

extern Font GameFont;       // Required due to using Monaspice RN Nerd Font as game font


struct KeyPadLetter {       // Used to build QWERTY keypad letters and User Entered Letters
    Vector2  position;
    Color    color;
    int      font_size;
    char     letter[2];
};


struct ReturnDeleteKeys {   // Used to build Return and Delete keys
    Vector2     position;
    Color       color;
    int         font_size;
    const char* text;
};

/**************************************************************************************************
*
* --> Display Manager
*
**************************************************************************************************/
struct DisplayManager {
    int cur_num_hints;
    int flags[5];
    /****************************************************************
    * Flags for setting background colors
    *    1- Letter in word and correct pos
    *    2- Letter in word but incorrect pos
    *    3- Letter not in word
    *    4- Keypad keys that have already been used
    */
    /****************************************************************
    * --> Tiles
    */
    struct {
        Rectangle   tile[30];
        Rectangle   border[30];
        Color       bg_color[30];
    } Tiles;

    /****************************************************************
    * --> Key Pad
    */
    struct {
        Rectangle   upper_key_rect_rect[10];
        Rectangle   middle_key_rect[9];
        Rectangle   lower_key_rect[7];
        Rectangle   return_key_rect;
        Rectangle   delete_key_rect;
        Color       bg_color[26];
        char        letters[26];
    } KeyPad;

    /****************************************************************
    * --> Keypad Text
    */
    struct {
        struct KeyPadLetter      keypad_text[26];
        struct ReturnDeleteKeys  delete_key;
        struct ReturnDeleteKeys  return_key;
    } KeyPadText;
};

/**************************************************************************************************
*
*  Word Manager
*
**************************************************************************************************/
struct WordManager {
    const char*  WordleWord;

    /****************************************************************
    * --> User Entered Letters
    */
    struct {
        struct KeyPadLetter*  letters[30];
        int                   cur_row;            // Track current row and ensure <= 6
        int                   cur_col;            // Track current col and ensure <= 5 letters
        int                   total_num_letters;  // Ensure no more than 30 letters
    } UserEnteredLetters;

    /****************************************************************
     * --> User Entered Words
     */
    struct {
        char  words[6][6];
        int   cur_num_words;
    } UserEnteredWords;
};

/**************************************************************************************************
*
*  Key Collision Manager
*
**************************************************************************************************/
struct KeyCollisionManager {
    Rectangle  keypad_rects[28];
    int        keys[28];
};

/**************************************************************************************************
*
*  Navigation Button Manager
*
**************************************************************************************************/
struct NavButtonManager {
    /************************************************************
    * --> Button
    */
    struct NavButton {
        Rectangle   body;
        Rectangle   border;
        Vector2     text_pos;
        Color       btn_bg_color;
        Color       border_color;
        char*       label;
    } NavButton;

    /************************************************************
    * --> Collection of Game Buttons
    */
    struct NavButton nav_buttons[NUM_BUTTONS];
};

/**************************************************************************************************
*
*  Struct Function Declarations
*
*      Function definitions are contained in:
*          collision_manager.c
*          display_manager.c
*          game_loop.c
*          nav_button_manager.c
*          text.c
*          word_manager.c
*          worlde.c
*
**************************************************************************************************
*
* --> Game Loop
*/
void    game_loop();

/**************************************************************************************************
* --> Builders
*/
void    BuildWordleDisplay(struct DisplayManager*);
void    BuildWordleKeyPad(struct DisplayManager*);
void    BuildKeypadText(struct DisplayManager*);

/*************************************************************************************************
* --> Renderers
*/
void    RenderWordleDisplay(const struct DisplayManager*);
void    RenderWordleKeyPad(const struct DisplayManager*);
void    RenderKeyPadText(const struct DisplayManager*);
void    RenderUserEnteredLetters(const struct WordManager*);

/*************************************************************************************************
* --> Display Manager
*/
struct  DisplayManager* NewDisplayManager(void);
void    ResetBgColors(struct DisplayManager*);

/*************************************************************************************************
* --> Key Collision Manager
*/
struct  KeyCollisionManager* NewKeyCollisionManager(struct DisplayManager*);
void    ProcessMouseClick(
                struct WordManager*,
                struct DisplayManager*,
                struct NavButtonManager*,
                const struct KeyCollisionManager*,
                Vector2 );

/*************************************************************************************************
* --> Word Manager
*/
struct      WordManager* NewWordManager(void);
void        ProcessKeyPress(struct WordManager*, struct DisplayManager*, int);
bool        ProcessUserEnteredLetters(struct WordManager*, struct DisplayManager*);
void        BuildUserEnteredLetters(struct WordManager*, const struct DisplayManager*, int);
void        ClearUserEnteredLetters(struct WordManager* WM);
void        FreeUserEnteredLetters(struct WordManager*);
const char* newWordleWord(void);

/*************************************************************************************************
* --> Navigation Button Manager
*/
struct  NavButtonManager* NewNavButtonManager(void);
void    GenerateNavButtons(struct NavButtonManager*);
void    RenderNavButtons(struct NavButtonManager*);

/*************************************************************************************************
* --> Game Font
*/
void    LoadGameFont(void);
void    UnloadGameFont(void);

/*************************************************************************************************
* --> Wordle Word
*/
const char* WordleWord(void);
bool        InDictionary(const char*);

/*************************************************************************************************
* --> Utility Functions
*/
void    GameOver(void);


#endif //CWORDLE_WORDLE_H

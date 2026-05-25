#include "../include/wordle.h"
/**************************************************************************************************
*
*  Game Constants
*
***************************************************************************************************
*
*  State
**************************************************************************************************/
bool START  = false;
bool NEW    = false;
bool QUIT   = false;

/**************************************************************************************************
*  Logo
**************************************************************************************************/
const char* LOGO            = "Wordle";
const int LOGO_FONT_SIZE    = 100;
const int LOGO_Y            = 15;
const Color LOGO_COLOR      = {233, 233, 233, 255};

/**************************************************************************************************
*  Dictionary
**************************************************************************************************/
const int DICTIONARY_SIZE               = 5757;
const int DICTIONARY_INDEX_UPPER_BOUND  = 5756;

/**************************************************************************************************
*  Keyboard Entry
**************************************************************************************************/
//
// --> Used in ProcessMouseClick
//
const int A             = 65;
const int B             = 66;
const int C             = 67;
const int D             = 68;
const int E             = 69;
const int F             = 70;
const int G             = 71;
const int H             = 72;
const int I             = 73;
const int J             = 74;
const int K             = 75;
const int L             = 76;
const int M             = 77;
const int N             = 78;
const int O             = 79;
const int P             = 80;
const int Q             = 81;
const int R             = 82;
const int S             = 83;
const int T             = 84;
const int U             = 85;
const int V             = 86;
const int W             = 87;
const int X             = 88;
const int Y             = 89;
const int Z             = 90;
const int RETURN        = 257;
const int DELETE        = 259;
const int BACKSPACE     = 261;
//
// --> Constraints for checking keyboard entries
//
const int LOWER_A       = 97;
const int LOWER_Z       = 122;
const int UPPER_A       = 65;
const int UPPER_Z       = 90;
const int MAX_NUM_COLS  = 5;

/**************************************************************************************************
*  Screen
**************************************************************************************************/
const int SCREEN_W                      = 800;
const int SCREEN_H                      = 900;
const Color SCREEN_BG_COLOR             = {18, 18, 19, 255};
/**************************************************************************************************
*  Colors
**************************************************************************************************/
//
// Tiles
//
const Color TILE_BG_COLOR               = {28, 28, 29, 250};
const Color TILE_BORDER_COLOR           = {250, 250, 250, 200};
const Color TILE_TEXT_COLOR             = {200, 200, 200, 255};
const Color TILE_MOUSE_OVER_COLOR       = {175, 175, 175, 255};
//
// Keyboard
//
const Color KEYPAD_TEXT_COLOR           = {30, 30, 30, 255};
const Color KEY_AVAILABLE_COLOR         = {129, 131, 132, 255};
//
// Key Validation
//
const Color CORRECT_POS_KEY_COLOR       = {83, 141, 78, 255};
const Color INCORRECT_POS_KEY_COLOR     = {181,159, 59, 255};
const Color INVALID_CHOSEN_KEY_COLOR    = {58, 58,  60, 255};

/**************************************************************************************************
*  Tiles
**************************************************************************************************/
const int   ROWS                        = 6;
const int   COLS                        = 5;
const float TILE_W                      = 60.f;
const float TILE_H                      = 60.f;
const float TILE_GAP                    = 75.f;
const float TILE_START_X                = (((float)SCREEN_W - TILE_W) / 2) - 2 * TILE_GAP;
const float TILE_START_Y                = 120.f;
const float TILE_BORDER_OFFSET          = 2.f;
const int   TILE_FONT_SIZE              = 40;

/**************************************************************************************************
*  Keypad
**************************************************************************************************/
//
// -- Qwerty Keys
//
const int   KEYPAD_FONT_SIZE            = 30;
const float KEYPAD_W                    = 50.f;
const float KEYPAD_H                    = 60.f;
const float KEYPAD_GAP                  = 55.f;
const float UPPER_KEYPAD_START_X        = ((float)SCREEN_W / 2) - (5 * KEYPAD_W + 22);
const float MID_KEYPAD_START_X          = (((float)SCREEN_W - KEYPAD_W) / 2) - (4 * KEYPAD_W + 20);
const float LOWER_KEYPAD_START_X        = (((float)SCREEN_W - KEYPAD_W) / 2) - (3 * KEYPAD_W + 15);
const float UPPER_KEYPAD_START_Y        = TILE_START_Y + 6 * TILE_GAP;                  // Dependent on Tile Start Y for ease of reformatting layout
const float MID_KEYPAD_START_Y          = UPPER_KEYPAD_START_Y + KEYPAD_H + 10;         // Dependent on Upper
const float LOWER_KEYPAD_START_Y        = UPPER_KEYPAD_START_Y + 2 * KEYPAD_H + 20;     // Dependent on Middle
//
// -- Return and Delete keys
//
const float RETURN_DEL_KEY_W            = 120.0f;
const float RETURN_DEL_KEY_H            = 60.0f;
const float RETURN_KEY_START_X          = LOWER_KEYPAD_START_X - RETURN_DEL_KEY_W - 5;
const float DELETE_KEY_START_X          = (float)SCREEN_W/ 2 + (3.5f * KEYPAD_W + 20);
const float RETURN_DEL_KEY_START_Y      = LOWER_KEYPAD_START_Y;

/**************************************************************************************************
*  Navigation Buttons
**************************************************************************************************/
const float NAV_W                       = 90.f;
const float NAV_H                       = 40.f;
const float ROUNDNESS                   = 0.2f;
const int   SEGMENTS                    = 30;
const int   THICKNESS                   = 2;
const float NAV_BTN_GAP                 = 20;
const float NAV_BTN_BORDER_OFFSET       = 2.0f;
const float NAV_BTN_FONT_SIZE           = 30.0f;
const Color NAV_BTN_FONT_COLOR          = RAYWHITE;
const float START_Y                     = (float)(SCREEN_H - 100);  //TODO --> value is just a place holder
const int   ALLOWABLE_HINTS             = 2;
char*       BUTTON_LABELS[]             = {"Start", "New", "Quit", "Hint"};

/**************************************************************************************************
*  QWERTY Keyboard Setup
**************************************************************************************************/
char* UPPER_KEYS[]  = {"Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P"};
char* MID_KEYS[]    = {"A", "S", "D", "F", "G", "H", "J", "K", "L"};
char* LOWER_KEYS[]  = {"Z", "X", "C", "V", "B", "N", "M"};
char  QWERTY[]      = {'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'Z', 'X', 'C', 'V', 'B', 'N', 'M'};

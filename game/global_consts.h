#ifndef GLOBAL_CONSTANTS_H_
#define GLOBAL_CONSTANTS_H_

// Screen constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_FPS = 60; 

// Game constants
const int GRID_X = 8;
const int GRID_Y = 8;
const int WIN_LINE_SIZE = 3;
const int MOVE_TIME_SEC = 1;

// Gem masks/types
// Let gem masks to be OR'ed or AND'ed in case we have multiple comparisons
enum GemMask
{
    GM_NONE   = 0x00000000,
    GM_BLUE   = 0x00000001, //1
    GM_GREEN  = 0x00000002, //2
    GM_PURPLE = 0x00000004, //3
    GM_RED    = 0x00000008, //4
    GM_YELLOW = 0x00000010  //5
};
const int NUMBER_OF_GEM_MASKS = 5; //TODO OLGA it's not needed prob

const char IMG_PATH [] = {"./"};
#endif // GLOBAL_CONSTANTS_H_

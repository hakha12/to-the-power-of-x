#include <raylib.h>
#include <raymath.h>

// ** The "Defines" ** //
#define MAX(a, b) ((a)>(b)? (a) : (b))
#define MIN(a, b) ((a)<(b)? (a) : (b))

#define CUSTOM_BLACK (Color){15, 15, 15, 255}

// ** Struct and Enum Declaration ** //
enum Scene {
    TITLE = 0,
    TUTORIAL,
    LEVEL,
    FINISH
};

// ** Global Variable Declaration ** //
extern int gameScreenWidth;
extern int gameScreenHeight;
extern Vector2 virtualMouse;
extern int currentScene;
extern int score;
extern int collectionCount;
extern int rootCount;

// ** Function Declaration ** //
void InitTitle();
void UpdateTitle();
void DrawTitle();

void InitTutorial();
void UpdateTutorial();
void DrawTutorial();

void InitLevel();
void UpdateLevel();
void DrawLevel();

void InitFinish();
void UpdateFinish();
void DrawFinish();
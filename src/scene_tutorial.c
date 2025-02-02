#include "game.h"

#define TUTORIAL_FONT_SIZE 40
#define TUTORIAL_FONT_COLOR RAYWHITE
#define LEFT_MARGIN gameScreenWidth/9
#define TOP_MARGIN gameScreenHeight/9

static int counter = {0};
static bool isActive = {0};

void InitTutorial(){
    counter = 0;
    isActive = false;
}

void UpdateTutorial(){
    counter++;

    if (counter < 210) return;

    if (counter % 60 == 0) isActive = !isActive;

    if (IsKeyPressed(KEY_ENTER)){
        currentScene = LEVEL;
        InitLevel();
    }    
}

void DrawTutorial(){
    ClearBackground(CUSTOM_BLACK);

    DrawText("Every jump increase it's POWER exponentially", LEFT_MARGIN, TOP_MARGIN, TUTORIAL_FONT_SIZE, TUTORIAL_FONT_COLOR);

    if (counter < 60) return;
    DrawText("Use said POWER to climb out of the tower", LEFT_MARGIN, TOP_MARGIN + 3*TUTORIAL_FONT_SIZE, TUTORIAL_FONT_SIZE, TUTORIAL_FONT_COLOR);

    if (counter < 120) return;
    DrawText("Collect SQUARES for reward, although too many will UNDERFLOWED", LEFT_MARGIN, TOP_MARGIN + 6*TUTORIAL_FONT_SIZE, TUTORIAL_FONT_SIZE, TUTORIAL_FONT_COLOR);

    if (counter < 180) return;
    DrawText("RED ROOT halves POWER, distancing it from UNDERFLOW", LEFT_MARGIN, TOP_MARGIN + 9*TUTORIAL_FONT_SIZE, TUTORIAL_FONT_SIZE, TUTORIAL_FONT_COLOR);

    if (isActive == true) DrawText("Press ENTER to PLAY", gameScreenWidth/9, gameScreenHeight/4 * 3, 25, TUTORIAL_FONT_COLOR);
}
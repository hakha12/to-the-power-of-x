#include "game.h"

#define TOP_BG_COLOR CUSTOM_BLACK
#define BOTTOM_BG_COLOR RAYWHITE

#define TITLE_FONT_SIZE 50
#define SUBTITLE_FONT_SIZE TITLE_FONT_SIZE/2

#define LEFT_MARGIN gameScreenWidth/9

static Rectangle topBackground = {0};
static Rectangle bottomBackground = {0};

static void DrawSuccess();
static void DrawFail();

void InitFinish(){
    topBackground = (Rectangle){0, 0, gameScreenWidth, gameScreenHeight/2};
    bottomBackground = (Rectangle){0, gameScreenHeight/2, gameScreenWidth, gameScreenHeight/2};
}

void UpdateFinish(){
    if (IsKeyPressed(KEY_ENTER)){
        currentScene = LEVEL;
        InitLevel();
    }
    if (IsKeyPressed(KEY_BACKSPACE)){
        currentScene = TITLE;
    }
}

void DrawFinish(){
    DrawRectangleRec(topBackground, TOP_BG_COLOR);
    DrawRectangleRec(bottomBackground, BOTTOM_BG_COLOR);    

    if (score >= 0) DrawSuccess();
    else DrawFail();
}

static void DrawSuccess(){
    DrawText("Congratulations, you have climb The Power Tower!", LEFT_MARGIN, gameScreenHeight/2 - 2*TITLE_FONT_SIZE, TITLE_FONT_SIZE, BOTTOM_BG_COLOR);
    DrawText(TextFormat("You have managed to scored %09i", score), LEFT_MARGIN, gameScreenHeight/2 + TITLE_FONT_SIZE, TITLE_FONT_SIZE, TOP_BG_COLOR);
    DrawText("Press ENTER to try again", LEFT_MARGIN, gameScreenHeight/4 * 3, SUBTITLE_FONT_SIZE, TOP_BG_COLOR);
    DrawText("Press BACKSPACE to go back to title screen", LEFT_MARGIN, gameScreenHeight/4 * 3 + 3*SUBTITLE_FONT_SIZE, SUBTITLE_FONT_SIZE, TOP_BG_COLOR);
}

static void DrawFail(){
    DrawRectangle(0, 0, gameScreenWidth, gameScreenHeight, CUSTOM_BLACK);

    DrawText("YOU COLLECTED TOO MUCH!!", LEFT_MARGIN, gameScreenHeight/2 - TITLE_FONT_SIZE/2, TITLE_FONT_SIZE, BOTTOM_BG_COLOR);
    DrawText("Press ENTER to try again", LEFT_MARGIN, gameScreenHeight/4 * 3, SUBTITLE_FONT_SIZE, BOTTOM_BG_COLOR);
}
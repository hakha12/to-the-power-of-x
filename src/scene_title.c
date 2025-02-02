#include "game.h"

#define MAX_REC 8
#define MIN_REC_SIZE 5

#define PLAYER_SIZE 50
#define PLAYER_COLOR RED

#define TITLE_FONT_SIZE 60

static Rectangle background[MAX_REC] = {0};
static int counter = {0};
static bool isActive = {0};

void InitTitle(){
    counter = 0;
    isActive = true;

    int size = MIN_REC_SIZE;
    int total = 0;
    for (int i = 0; i < MAX_REC; i++){
        background[i].x = 0 + 2*total;
        background[i].y = gameScreenHeight - size - total;
        background[i].width = 2*size;
        background[i].height = size;

        total += size;
        size *= 2;
    }
}

void UpdateTitle(){
    if (counter % 60 == 0) isActive = !isActive;

    if (IsKeyPressed(KEY_ENTER)){
        currentScene = TUTORIAL;
        InitTutorial();
    }

    counter++;
}

void DrawTitle(){
    for (int i = 0; i < MAX_REC; i++){
        DrawRectangleRec(background[i], CUSTOM_BLACK);
        DrawText(TextFormat("%01i", i), background[i].x + background[i].width - background[i].height/3, background[i].y + background[i].height/18, background[i].height/2, GRAY);
    }

    DrawText("TO THE POWER OF", background[6].x + 10, background[6].y - 1.5*TITLE_FONT_SIZE, TITLE_FONT_SIZE, CUSTOM_BLACK);
    DrawText("X", background[7].x + TITLE_FONT_SIZE, background[6].y - 1.5*TITLE_FONT_SIZE - 3*TITLE_FONT_SIZE/2, 3*TITLE_FONT_SIZE, RAYWHITE);
    if (isActive == true) DrawText("Press ENTER to START", background[7].x + TITLE_FONT_SIZE, background[6].y + background[6].height/18, TITLE_FONT_SIZE/2, CUSTOM_BLACK);

    DrawRectangle(gameScreenWidth - 2*PLAYER_SIZE, PLAYER_SIZE, PLAYER_SIZE, PLAYER_SIZE, PLAYER_COLOR);

    DrawRectangleLinesEx((Rectangle){background[5].x + background[5].width/2, -50, gameScreenWidth, background[5].y + background[5].width/2}, 10.0f, DARKGRAY);
}
#include "game.h"

#define PLAYER_SIZE 50
#define PLAYER_COLOR RED
#define BASE_PLAYER_SPEED 100

#define MAX_ITEM_AUX_REC 4

#define MAX_COLLECTIONS 100
#define COLLECTIONS_REC_SIZE 20
#define COLLECTIONS_AUX_REC_SIZE COLLECTIONS_REC_SIZE/2
#define BASE_SCORE_MULTIPLIER 1

#define MAX_ROOT 10
#define MAX_ROOT_AUX_REC 4
#define ROOT_REC_SIZE 10
#define ROOT_COLOR RED

#define GRAVITY 10
#define LEVEL_HEIGHT 26000
#define LEVEL_COLOR CUSTOM_BLACK
#define MAX_BACKGROUND_REC 500
#define MIN_BACKGROUND_REC_SIZE 100
#define MAX_BACKGROUND_REC_SIZE 400

#define COLLECTION_SCORE_UI 99
#define ROOT_SCORE_UI 9
#define SCORE_FONT_SIZE 25
#define SCORE_FONT_COLOR RAYWHITE
#define COLLECTION_FONT_SIZE 50

struct Player {
    Rectangle rec;
    Vector2 speed;
    bool isActive;
};

struct Level {
    Rectangle leftWall;
    Rectangle rightWall;
    Rectangle ground;
};

struct BackGround {
    Rectangle rec;
    Color color;
};

struct Item {
    Rectangle main;
    Rectangle aux[MAX_ITEM_AUX_REC];
    Color color;
    bool isActive;
};

int score = {0};
int collectionCount = {0};
int rootCount = {0};

static struct Player player = {0};
static struct Level level = {0};
static struct BackGround background[MAX_BACKGROUND_REC] = {0};
static struct Item collection[MAX_COLLECTIONS] = {0};
static struct Item root[MAX_ROOT] = {0};
static Camera2D camera = {0};
static int currentSpeed = {0};
static bool isPaused = false;
static int scoreMultiplier = {0};

static void InitPlayer();
static void UpdatePlayer();
static void CheckPlayerInput();
static void CheckPlayerCollision();
static void DrawPlayer();

static void CheckFinishCondition();

static void InitCamera();
static void UpdateCameraPosition();

static void InitCollections();
static void UpdateCollections();
static void DrawCollections();

static void InitRoot();
static void UpdateRoot();
static void DrawRoot();

static void InitBackground();
static void DrawBackground();

static void DrawUI();

void InitLevel(){
    level.leftWall = (Rectangle){0, gameScreenHeight - LEVEL_HEIGHT, gameScreenWidth/3, LEVEL_HEIGHT};
    level.rightWall = (Rectangle){gameScreenWidth/3 * 2, gameScreenHeight - LEVEL_HEIGHT, gameScreenWidth/3, LEVEL_HEIGHT};
    level.ground = (Rectangle){0, gameScreenHeight/9 * 8, gameScreenWidth, gameScreenHeight};

    score = 0;
    collectionCount = 0;
    rootCount = 0;
    currentSpeed = BASE_PLAYER_SPEED;
    scoreMultiplier = BASE_SCORE_MULTIPLIER;

    InitBackground();
    InitPlayer();
    InitCamera();
    InitCollections();
    InitRoot();
}

void UpdateLevel(){
    if (IsKeyPressed(KEY_P)) isPaused = !isPaused;
    if (isPaused == true) return;

    CheckFinishCondition();
    UpdateCollections();
    UpdateRoot();
    UpdatePlayer();
    UpdateCameraPosition();
}

void DrawLevel(){
    BeginMode2D(camera);

    DrawBackground();
    DrawCollections();
    DrawRoot();
    DrawPlayer();

    DrawRectangleRec(level.ground, LEVEL_COLOR);
    DrawRectangleRec(level.leftWall, LEVEL_COLOR);
    DrawRectangleRec(level.rightWall, LEVEL_COLOR);

    EndMode2D();

    DrawUI();    
}

static void InitPlayer(){
    player.rec = (Rectangle){gameScreenWidth/2 - PLAYER_SIZE/2, gameScreenHeight/2 - PLAYER_SIZE/2, PLAYER_SIZE, PLAYER_SIZE};
    player.speed = (Vector2){0, 0};
    player.isActive = true;
}

static void UpdatePlayer(){
    if (player.isActive == false) return;

    player.speed = (Vector2){0, GRAVITY};
    
    CheckPlayerInput();

    Vector2 pos = Vector2Add((Vector2){player.rec.x, player.rec.y}, player.speed);
    player.rec.x = pos.x;
    player.rec.y = pos.y;

    CheckPlayerCollision();
}

static void CheckPlayerInput(){
    if (IsKeyPressed(KEY_UP)) {
        player.speed = (Vector2){0, -currentSpeed};
        currentSpeed *= 2;
    }
    if (IsKeyDown(KEY_LEFT)) {
        player.speed.x = -10;
    }
    if (IsKeyDown(KEY_RIGHT)) {
        player.speed.x = 10;
    }
}

static void CheckPlayerCollision(){
    if (CheckCollisionRecs(player.rec, level.ground)){
        player.rec.y = level.ground.y - PLAYER_SIZE;
    }
    if (CheckCollisionRecs(player.rec, level.rightWall)){
        player.rec.x = level.rightWall.x - PLAYER_SIZE;
    }
    if (CheckCollisionRecs(player.rec, level.leftWall)){
        player.rec.x = level.leftWall.x + level.leftWall.width;
    }
}

static void DrawPlayer(){
    if (player.isActive == false) return;

    DrawRectangleRec(player.rec, PLAYER_COLOR);
}

static void CheckFinishCondition(){
    if (player.rec.y < level.leftWall.y || score < 0) {
        currentScene = FINISH;
    }
}

static void InitCamera(){
    camera.target = (Vector2){gameScreenWidth/2.0f, player.rec.y + PLAYER_SIZE/2};
    camera.offset = (Vector2){gameScreenWidth/2.0f, gameScreenHeight/2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

static void UpdateCameraPosition(){
    camera.target = (Vector2){gameScreenWidth/2.0f, player.rec.y + PLAYER_SIZE/2};
}

static void InitCollections(){
    for (int i = 0; i < MAX_COLLECTIONS; i++){
        int posX = GetRandomValue(gameScreenWidth/3 + 2*COLLECTIONS_REC_SIZE, gameScreenWidth/3 * 2 - 2*COLLECTIONS_REC_SIZE);
        int posY = level.ground.y - (i+1)*(LEVEL_HEIGHT/MAX_COLLECTIONS);
        Color color = LEVEL_COLOR;

        if (i == COLLECTION_SCORE_UI) {
            posX = 3*COLLECTIONS_REC_SIZE;
            posY = 8*COLLECTIONS_REC_SIZE;
            color = SCORE_FONT_COLOR;
        };

        collection[i].main.x = posX - COLLECTIONS_REC_SIZE/2;
        collection[i].main.y = posY - COLLECTIONS_REC_SIZE/2;
        collection[i].main.width = COLLECTIONS_REC_SIZE;
        collection[i].main.height = COLLECTIONS_REC_SIZE;

        collection[i].aux[0].x = posX - COLLECTIONS_AUX_REC_SIZE/2;
        collection[i].aux[0].y = posY - COLLECTIONS_REC_SIZE - COLLECTIONS_AUX_REC_SIZE/2;
        collection[i].aux[1].x = posX - COLLECTIONS_REC_SIZE - COLLECTIONS_AUX_REC_SIZE/2;
        collection[i].aux[1].y = posY - COLLECTIONS_AUX_REC_SIZE/2;
        collection[i].aux[2].x = posX - COLLECTIONS_AUX_REC_SIZE/2;
        collection[i].aux[2].y = posY + COLLECTIONS_REC_SIZE - COLLECTIONS_AUX_REC_SIZE/2;
        collection[i].aux[3].x = posX + COLLECTIONS_REC_SIZE - COLLECTIONS_AUX_REC_SIZE/2;
        collection[i].aux[3].y = posY - COLLECTIONS_AUX_REC_SIZE/2;
        for (int j = 0; j < MAX_ITEM_AUX_REC; j++){
            collection[i].aux[j].width = COLLECTIONS_AUX_REC_SIZE;
            collection[i].aux[j].height = COLLECTIONS_AUX_REC_SIZE;
        }        

        collection[i].isActive = true;
        collection[i].color = color;
    }
}

static void UpdateCollections(){
    for (int i = 0; i < MAX_COLLECTIONS-1; i++){
        if (collection[i].isActive == false) continue;

        if (CheckCollisionRecs(player.rec, collection[i].main)){
            collection[i].isActive = false;
            collectionCount++;
            
            scoreMultiplier *= 2;
            score += scoreMultiplier;
        }
    }    
}

static void DrawCollections(){
    for (int i = 0; i < MAX_COLLECTIONS-1; i++){
        if (collection[i].isActive == false) continue;

        DrawRectangleRec(collection[i].main, collection[i].color);
        for (int j = 0; j < MAX_ITEM_AUX_REC; j++){
            DrawRectangleRec(collection[i].aux[j], collection[i].color);
        }
    }    
}

static void InitRoot(){
    for (int i = 0; i < MAX_ROOT; i++){
        int posX = GetRandomValue(gameScreenWidth/3 + 2*COLLECTIONS_REC_SIZE, gameScreenWidth/3 * 2 - 2*COLLECTIONS_REC_SIZE);
        int posY = level.ground.y - (i+1)*(LEVEL_HEIGHT/MAX_ROOT);
        Color color = ROOT_COLOR;

        if (i == ROOT_SCORE_UI) {
            posX = 3*COLLECTIONS_REC_SIZE;
            posY = 10*COLLECTIONS_REC_SIZE + SCORE_FONT_SIZE;
            color = SCORE_FONT_COLOR;
        };

        root[i].main.x = posX - ROOT_REC_SIZE/2;
        root[i].main.y = posY - ROOT_REC_SIZE/2;
        root[i].main.width = ROOT_REC_SIZE;
        root[i].main.height = ROOT_REC_SIZE;

        for (int j = 0; j < MAX_ROOT_AUX_REC; j++){
            root[i].aux[j].x = root[i].main.x;
            root[i].aux[j].y = root[i].main.y + 2*ROOT_REC_SIZE*(j+1);
            root[i].aux[j].width = ROOT_REC_SIZE;
            root[i].aux[j].height = ROOT_REC_SIZE;
        }

        root[i].isActive = true;
        root[i].color = color;
    }
}

static void UpdateRoot(){
    for (int i = 0; i < MAX_ROOT-1; i++){
        if (root[i].isActive == false) continue;

        for (int j = 0; j < MAX_ROOT_AUX_REC; j++){
            if (CheckCollisionRecs(player.rec, root[i].aux[j])){
                root[i].isActive = false;
                player.rec.y = level.ground.y - 2*PLAYER_SIZE;

                rootCount++;
                currentSpeed /= 2;
                scoreMultiplier = BASE_SCORE_MULTIPLIER;
            }
        }
    }
}

static void DrawRoot(){
    for (int i = 0; i < MAX_ROOT-1; i++){
        if (root[i].isActive == false) continue;

        DrawRectangleRec(root[i].main, root[i].color);
        for (int j = 0; j < MAX_ROOT_AUX_REC; j++){
            DrawRectangleRec(root[i].aux[j], root[i].color);
        }
    }     
}

static void InitBackground(){
    for (int i = 0; i < MAX_BACKGROUND_REC; i++){
        background[i].rec.x = GetRandomValue(gameScreenWidth/4, gameScreenWidth/3 * 2);
        background[i].rec.y = GetRandomValue(gameScreenHeight - LEVEL_HEIGHT, gameScreenHeight*2);

        int size = GetRandomValue(MIN_BACKGROUND_REC_SIZE, MAX_BACKGROUND_REC_SIZE);
        background[i].rec.width = size;
        background[i].rec.height = size;

        int colorValue = GetRandomValue(120, 220);
        background[i].color = (Color){colorValue, colorValue, colorValue, 255};
    }
}

static void DrawBackground(){
    for (int i = 0; i < MAX_BACKGROUND_REC; i++){
        DrawRectangleRec(background[i].rec, background[i].color);
    }
}

static void DrawUI(){
    DrawRectangleRec(collection[COLLECTION_SCORE_UI].main, collection[COLLECTION_SCORE_UI].color);
    for (int j = 0; j < MAX_ITEM_AUX_REC; j++){
        DrawRectangleRec(collection[COLLECTION_SCORE_UI].aux[j], collection[COLLECTION_SCORE_UI].color);
    }
    DrawRectangleRec(root[ROOT_SCORE_UI].main, root[ROOT_SCORE_UI].color);
    for (int j = 0; j < MAX_ROOT_AUX_REC; j++){
        DrawRectangleRec(root[ROOT_SCORE_UI].aux[j], root[ROOT_SCORE_UI].color); 
    }        

    if (score >= 0) DrawText(TextFormat("SCORE : %09i", score), SCORE_FONT_SIZE, SCORE_FONT_SIZE, SCORE_FONT_SIZE, SCORE_FONT_COLOR);
    else DrawText("SCORE : UNDERFLOW", SCORE_FONT_SIZE, SCORE_FONT_SIZE, SCORE_FONT_SIZE, SCORE_FONT_COLOR);

    if (scoreMultiplier >= 1) DrawText(TextFormat("MULTIPLIER : %01i", scoreMultiplier), SCORE_FONT_SIZE, 3*SCORE_FONT_SIZE, SCORE_FONT_SIZE, SCORE_FONT_COLOR);
    else DrawText("MULTIPLIER : UNDERFLOW", SCORE_FONT_SIZE, 3*SCORE_FONT_SIZE, SCORE_FONT_SIZE, SCORE_FONT_COLOR);

    DrawText(TextFormat("%01i", collectionCount), collection[COLLECTION_SCORE_UI].aux[3].x + 2*COLLECTIONS_REC_SIZE, collection[COLLECTION_SCORE_UI].main.y + COLLECTIONS_REC_SIZE/2 - COLLECTION_FONT_SIZE/2, COLLECTION_FONT_SIZE, SCORE_FONT_COLOR);
    DrawText(TextFormat("%01i", rootCount), collection[COLLECTION_SCORE_UI].aux[3].x + 2*COLLECTIONS_REC_SIZE, root[ROOT_SCORE_UI].main.y, COLLECTION_FONT_SIZE, SCORE_FONT_COLOR);

    DrawText("Use ARROW KEYS to navigate", SCORE_FONT_SIZE, gameScreenHeight - 2*COLLECTION_FONT_SIZE, 38, SCORE_FONT_COLOR);
}
#include "game.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define GAME_TITLE "To the Power of X"
#define TARGET_FPS 60
#define MINIMUM_WINDOW_WIDTH WINDOW_WIDTH/5
#define MINIMUM_WINDOW_HEIGHT WINDOW_HEIGHT/5

Vector2 virtualMouse = {0};
int gameScreenWidth = 1920;
int gameScreenHeight = 1080;
int currentScene = TITLE;

static RenderTexture2D target = {0};

static void UpdateDrawFrame();
static float UpdateScaling();

static void InitScene();
static void UpdateScene();
static void DrawScene();
static void RenderSceneToWindow(float windowScale);

int main(){
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, GAME_TITLE);
    SetWindowMinSize(MINIMUM_WINDOW_WIDTH, MINIMUM_WINDOW_HEIGHT);

    target = LoadRenderTexture(gameScreenWidth, gameScreenHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

    InitScene();

    #if defined(PLATFORM_WEB)
        emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
    #else
        SetTargetFPS(TARGET_FPS);

        while (!WindowShouldClose()){
            UpdateDrawFrame();
        }
    #endif

    UnloadRenderTexture(target);
    CloseWindow();
}

static void UpdateDrawFrame(){
    float scale = UpdateScaling();
    UpdateScene();
    DrawScene();
    RenderSceneToWindow(scale);
}

static void InitScene(){
    InitTitle();
    InitTutorial();
    InitLevel();
    InitFinish();
}

static void UpdateScene(){
    switch (currentScene){
        case TITLE : UpdateTitle(); break;
        case TUTORIAL : UpdateTutorial(); break;
        case LEVEL : UpdateLevel(); break;
        case FINISH : UpdateFinish(); break;
    }
}

static void DrawScene(){
    BeginTextureMode(target);
    ClearBackground(RAYWHITE);
    
    switch (currentScene){
        case TITLE : DrawTitle(); break;
        case TUTORIAL : DrawTutorial(); break;
        case LEVEL : DrawLevel(); break;
        case FINISH : DrawFinish(); break;
    }

    EndTextureMode();
}

static float UpdateScaling(){
    float scale = MIN((float)GetScreenWidth()/gameScreenWidth, (float)GetScreenHeight()/gameScreenHeight);

    Vector2 mouse = GetMousePosition();
    
    virtualMouse.x = (mouse.x - (GetScreenWidth() - (gameScreenWidth*scale))*0.5f)/scale;
    virtualMouse.y = (mouse.y - (GetScreenHeight() - (gameScreenHeight*scale))*0.5f)/scale;
    virtualMouse = Vector2Clamp(virtualMouse, (Vector2){ 0, 0 }, (Vector2){ (float)gameScreenWidth, (float)gameScreenHeight });   

    return scale; 
}

static void RenderSceneToWindow(float windowScale){
    BeginDrawing();

    Rectangle src = { 0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height };
    Rectangle dst = { (GetScreenWidth() - ((float)gameScreenWidth*windowScale))*0.5f, (GetScreenHeight() - ((float)gameScreenHeight*windowScale))*0.5f, (float)gameScreenWidth*windowScale, (float)gameScreenHeight*windowScale};
    Vector2 origin = {0, 0};
    ClearBackground(CUSTOM_BLACK);
    DrawTexturePro(target.texture, src, dst, origin, 0.0f, RAYWHITE);

    EndDrawing();    
}
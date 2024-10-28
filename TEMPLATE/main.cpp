//tokyocplusplus
#include <raylib.h>

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
static inline float getNewIncrement()
{
    float r = 0;
    while (r < 0.05) {
        r = GetRandomValue(0, 100) / 100.0;
    }
    return r;
}

float crouchHeight = 1.0f; // Normal height
float standHeight = 2.0f;  // Height when standing

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1280;
    const int screenHeight = 720;
    const int ingameWidth = 640;
    const int ingameHeight = 360;

    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "Low Res Effect with CRT");

    RenderTexture2D lowres = LoadRenderTexture(ingameWidth, ingameHeight);

    Rectangle sourceRec = { 0.0f, 0.0f, (float)lowres.texture.width, -(float)lowres.texture.height };
    Rectangle destRec = { 0, 0, (float)screenWidth, (float)screenHeight };

    // Define the camera to look into our 3D world
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 10.0f, 2.0f, 10.0f }; // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };

    ToggleFullscreen();
    DisableCursor();                    // Limit cursor to relative movement inside the window

    // Load shaders for CRT effect
    Shader shader = LoadShader("data/scan.vs", "data/scan.fs");

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera, CAMERA_FIRST_PERSON);

        /*-------------------- INPUT START --------------------*/
        if (IsKeyDown(KEY_E)) {
            camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
        }
        if (IsKeyDown(KEY_Q)) {
            camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
        }
        if (IsKeyDown(KEY_LEFT_CONTROL)) {

        }
        if (IsKeyDown(KEY_C)) {
            camera.position.y = crouchHeight; // Lower the camera height for crouching
        } else {
            camera.position.y = standHeight; // Reset to standing height
        }
        /*-------------------- INPUT END --------------------*/

        // Draw
        //----------------------------------------------------------------------------------
        BeginTextureMode(lowres);
            ClearBackground(RAYWHITE);
            BeginMode3D(camera);
                DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, PINK);
                DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, BLUE);
            EndMode3D();

            // Draw speckles
            BeginBlendMode(BLEND_ALPHA);
            for (int i = 0; i < 10; i++) {
                DrawRectangle(GetRandomValue(0, ingameWidth - 1),
                               GetRandomValue(0, ingameHeight - 1),
                               1, 1, BLACK);
            }
            EndBlendMode();
        EndTextureMode();

        // Draw with CRT effect
        BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginShaderMode(shader);
                DrawTexturePro(lowres.texture, sourceRec, destRec, (Vector2){ 0, 0 }, 0.0f, WHITE);
            EndShaderMode();
            DrawFPS(10, 10);
        EndDrawing();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadShader(shader);               // Unload shader
    UnloadRenderTexture(lowres);        // Unload render texture
    CloseWindow();                      // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

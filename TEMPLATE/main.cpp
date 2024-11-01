//tokyocplusplus
// ADD MULTICORE SUPPORT
#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>
#include <cstdint>
#include <vector>
#include <iostream>
#include <thread>
#include "other.h"

typedef uint16_t uint;

/*
// Use a vector for dynamic allocation of the grid
std::vector<std::vector<std::vector<Voxel>>> grid(GRID_SIZE, 
    std::vector<std::vector<Voxel>>(GRID_SIZE, std::vector<Voxel>(GRID_SIZE)));

// Function to initialize the voxel grid
void InitVoxelGrid() {
    for (int x = 0; x < GRID_SIZE; x++) {
        for (int y = 0; y < GRID_SIZE; y++) {
            for (int z = 0; z < GRID_SIZE; z++) {
                grid[x][y][z].position = (Vector3){ x * 1.0f, y * 1.0f, z * 1.0f };
                grid[x][y][z].color = (Color){ 
                    static_cast<unsigned char>(GetRandomValue(100, 255)), 
                    static_cast<unsigned char>(GetRandomValue(100, 255)), 
                    static_cast<unsigned char>(GetRandomValue(100, 255)), 
                    static_cast<unsigned char>(255) 
                };
            }
        }
    }
}

// Function to prepare voxel data in a specified section
void PrepareVoxelsSection(int startX, int endX) {
    for (int x = startX; x < endX; x++) {
        for (int y = 0; y < GRID_SIZE; y++) {
            for (int z = 0; z < GRID_SIZE; z++) {
                grid[x][y][z].color = (Color){
                    static_cast<unsigned char>(GetRandomValue(100, 255)),
                    static_cast<unsigned char>(GetRandomValue(100, 255)),
                    static_cast<unsigned char>(GetRandomValue(100, 255)),
                    static_cast<unsigned char>(255)
                };
            }
        }
    }
}

// Function to draw all voxels in batches
void DrawVoxels() {
    // Batch drawing using DrawCubeV
    for (int x = 0; x < GRID_SIZE; x++) {
        for (int y = 0; y < GRID_SIZE; y++) {
            for (int z = 0; z < GRID_SIZE; z++) {
                DrawCubeV(grid[x][y][z].position, (Vector3){ 1.0f, 1.0f, 1.0f }, grid[x][y][z].color);
            }
        }
    }
}
*/

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
float crouchHeight = 1.0f; // Normal height
float standHeight = 2.0f;  // Height when standing

int main() {
    // Initialization
    const uint screenWidth = 1280;
    const uint screenHeight = 720;
    const uint ingameWidth = 256; 
    const uint ingameHeight = 144; 

    SetConfigFlags(FLAG_VSYNC_HINT);
    SetTargetFPS(60);
    InitWindow(screenWidth, screenHeight, "tcpp-VOXEL");

    RenderTexture2D lowres = LoadRenderTexture(ingameWidth, ingameHeight);

    Rectangle sourceRec = { 0.0f, 0.0f, static_cast<float>(lowres.texture.width), -static_cast<float>(lowres.texture.height) };
    Rectangle destRec = { 0, 0, static_cast<float>(screenWidth), static_cast<float>(screenHeight) };

    // Define the camera
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 10.0f, 2.0f, 10.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          
    camera.fovy = 90.0f;                                
    camera.projection = CAMERA_PERSPECTIVE;             

    ToggleFullscreen();
    DisableCursor();

    // Load shaders for CRT effect
    Shader shader = LoadShader("data/scan.vs", "data/scan.fs");
    Mesh cubeMesh = GenMeshCube(2.0f,2.0f,2.0f);
    Material cubeMaterial = LoadMaterialDefault();
    cubeMaterial.maps[MATERIAL_MAP_DIFFUSE].color = PINK;
    Matrix matrix = MatrixTranslate(0.0f,0.0f,0.0f);

    rlEnableBackfaceCulling();

    /*
    // Initialize the voxel grid
    InitVoxelGrid();

    // Prepare voxel data in parallel
    const int NUM_THREADS = 8;
    std::vector<std::thread> threads;
    int sectionSize = GRID_SIZE / NUM_THREADS;

    for (int i = 0; i < NUM_THREADS; i++) {
        int startX = i * sectionSize;
        int endX = (i == NUM_THREADS - 1) ? GRID_SIZE : startX + sectionSize;
        threads.emplace_back(PrepareVoxelsSection, startX, endX);
    }

    for (auto& thread : threads) {
        thread.join(); // Wait for all threads to finish
    }
    */

    // Main game loop
    while (!WindowShouldClose()) {
        // Update
        UpdateCamera(&camera, CAMERA_FIRST_PERSON);
        camera.position.y = IsKeyDown(KEY_LEFT_CONTROL) ? crouchHeight : standHeight;

        // Draw
        BeginTextureMode(lowres);
            ClearBackground(RAYWHITE);
            BeginMode3D(camera);
                DrawMesh(cubeMesh, cubeMaterial, matrix);
            EndMode3D();
        EndTextureMode();

        // Draw with CRT effect
        BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginShaderMode(shader);
                DrawTexturePro(lowres.texture, sourceRec, destRec, (Vector2){ 0, 0 }, 0.0f, RAYWHITE);
            EndShaderMode();
            DrawFPS(10, 10);
        EndDrawing();
    }

    // De-Initialization
    UnloadShader(shader);               // Unload shader
    UnloadRenderTexture(lowres);        // Unload render texture
    CloseWindow();                      // Close window and OpenGL context

    return 0;
}

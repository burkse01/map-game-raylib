#include "raylib.h"
#include "raymath.h"

#define SQRT3 1.73205080757

typedef struct {
    int width, height;
    float radius;
    Vector2 corner;
    //Vector2 points[100][100][2];
} HexGrid;

HexGrid create_hexgrid(int w, int h, Vector2 c, float r) {
    HexGrid grid;
    grid.width = w;
    grid.height = h;
    grid.corner = c;
    grid.radius = r;
    /*for (int i = 0; i < w * 2; i++) {
        for (int j = 0; j < h * 2; j++) {
            for (int k = 0; k < 2; k++) {
                grid.points[i][j][k] = (Vector2){ c.x + r * SQRT3 - (k % 2 * r * SQRT3 / 2), c.y + r };
            }
        }
    }*/
    return grid;
}

int arc_to_i(int a, int r, int c, int width) {
    a + (r * 2) + (c * 2 * width);
}

Vector3 arc_to_cart(int a, int r, int c, int d) {
    return (Vector3) {
        d * SQRT3 * (c + (a / 2.0f)), 
        0.0f, 
        d * 3 * (r + (a / 2.0f))
    };
}

Vector3 cart_to_arc(int x, int y,  int d) {
    return (Vector3) {
        0, 
        (int)(y / (d * 3.0f)),
        (int)(x / (d * SQRT3))
    };
}

//int draw_hexgrid_triangular(HexGrid grid, Vector3 center, float radius) {
//    for (int i = 0; i < grid.height; i++) {
//        for (int j = 0; j < grid.width; j++) {
//            int center_x = j * 3 + 1;
//            int center_y = i;
//        }
//    }
//}

int draw_hexgrid(HexGrid grid, Model hexmesh, Vector3 center, int radius) {
    int count = 0;
    for (int c = center.z; c < center.z + radius; c++) {
        for (int r = center.y; r < center.y + radius; r++) {
            for (int a = center.x; a < 2; a++) {
                DrawModelWires(hexmesh, arc_to_cart(a, r, c, grid.radius), 1.0f, RED);
                count++;
            }
        }
    }
}

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera mode");
    Model hexmesh = LoadModelFromMesh(GenMeshPoly(6, 1.0f));
    Image checked = GenImageChecked(2, 2, 1, 1, RED, GREEN);
    Texture2D texture = LoadTextureFromImage(checked);
    UnloadImage(checked);
    hexmesh.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 10.0f, 10.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };
    SetTargetFPS(60);
    HexGrid hex_grid = create_hexgrid(10, 10, (Vector2){ 0, 0 }, 1);
    float zoom_rate = 5;
    float change_rate = 0.1;
    float zoom = 0;
    Vector3 camera_momentum = (Vector3){ 0.0f, 0.0f, 0.0f };
    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_W)) {
            camera_momentum.z -= change_rate;
        }
        if (IsKeyDown(KEY_A)) {
            camera_momentum.x -= change_rate;
        }
        if (IsKeyDown(KEY_S)) {
            camera_momentum.z += change_rate;
        }
        if (IsKeyDown(KEY_D)) {
            camera_momentum.x += change_rate;
        }
        //zoom = GetMouseWheelMove();
        camera.target = Vector3Add(camera.target, camera_momentum);
        camera.position = Vector3Add(camera.position, camera_momentum);
        camera.position.y += GetMouseWheelMove() * zoom_rate;
        camera_momentum = Vector3Scale(camera_momentum, 0.50);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode3D(camera);
        draw_hexgrid(hex_grid, hexmesh, cart_to_arc(0, 0, 1), 4);
        EndMode3D();
        DrawText("Welcome to the third dimension!", 10, 40, 20, DARKGRAY);
        DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

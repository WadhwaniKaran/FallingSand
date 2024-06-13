#include <stdlib.h>
#include <stdio.h>
#include "raylib.h"

// #define RAYGUI_IMPLEMENTATION
// #include "raygui.h"

const int screen_width = 1024;
const int screen_height = 512;

const int pixel_scale = 4;
const int grid_width = screen_width / pixel_scale;
const int grid_height = screen_height / pixel_scale;


int* create_grid() {
    int *grid = (int*)malloc(sizeof(int) * grid_height * grid_width);
    for(int j = 0; j < grid_height; j++) {
        for(int i = 0; i < grid_width; i++) {
            int coords = j * grid_width + i;
            grid[coords] = 0;
        }
    }
    return grid;
}

void clear_grid(int *grid) {
    for(int j = 0; j < grid_height; j++) {
        for(int i = 0; i < grid_width; i++) {
            int coords = j * grid_width + i;
            grid[coords] = 0;
        }
    }
}

int* copy_grid(int *grid) {
    int *copy = (int*)malloc(sizeof(int) * grid_height * grid_width);
    for(int j = 0; j < grid_height; j++) {
        for(int i = 0; i < grid_width; i++) {
            int coords = j * grid_width + i;
            copy[coords] = grid[coords];
        }
    }
    return copy;
}

void bound_x(int *x) {
    if(*x < 0) { *x = 0; }
    else if (*x >= grid_width) { *x = grid_width - 1; }
}

void bound_y(int *y) {
    if(*y < 0) { *y = 0; }
    else if (*y >= grid_height) { *y = grid_height - 1; }
}

Color glyphs[2] = {BLACK, RED};
void draw_grid(int *grid) {
    for(int j = 0; j < grid_height; j++) {
        for(int i = 0; i < grid_width; i++) {
            int coords = j * grid_width + i;
            DrawRectangle(i * pixel_scale, j * pixel_scale, pixel_scale, pixel_scale, glyphs[grid[coords]]);
        }
    }
    //printf("%d\n", grid[0]);
}

void input_grid(int *grid, Vector2 mouse_pos) {
    int mx = mouse_pos.x / pixel_scale;
    int my = mouse_pos.y / pixel_scale;

    for(int j = -3; j < 4; j++) {
        for(int i = -3; i < 4; i++) {
            int coords_x = mx + i;
            int coords_y = my + j;
            bound_x(&coords_x);
            bound_y(&coords_y);
            int coords = coords_y * grid_width + coords_x;
            grid[coords] = 1;

        }
    }
}

int get_next_x(int *grid, int x, int y) { // -1 if no empty space
    if(y < 0 || y >= grid_height - 1) { return -1; }
    if(grid[(y+1)*grid_width+x] == 0) { return x; }
    if((x+1) < grid_width && grid[(y+1)*grid_width+x+1] == 0) { return x+1; }
    if((x-1) > -1 && grid[(y+1)*grid_width+x-1] == 0) { return x-1; }
    return -1;
}

void update_grid(int *grid, int *buffer) {
    clear_grid(buffer);
    for(int j = 0; j < grid_height; j++) {
        for(int i = 0; i < grid_width; i++) {
            if(grid[j * grid_width + i] == 0) { continue; }
            int next_x = get_next_x(grid, i, j);
            if(next_x == -1) { 
                buffer[j * grid_width + i] = 1;
            } else {
                buffer[(j + 1) * grid_width + next_x] = 1;
            }
        }
    }
}

void swap_buffer(int **b1, int **b2) {
    int *t = *b1;
    *b1 = *b2;
    *b2 = t;
}

int main() {
    
    InitWindow(screen_width, screen_height, "Blah");
    SetTargetFPS(60);

    int *grid = create_grid();
    int *buffer = create_grid();
    Vector2 mouse_pos = {0.0f, 0.0f};

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        draw_grid(grid);
        EndDrawing();

        mouse_pos = GetMousePosition();
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            input_grid(grid, mouse_pos);
        }
        
        update_grid(grid, buffer);
        swap_buffer(&grid, &buffer);

    }
    CloseWindow();

    return 0;
}

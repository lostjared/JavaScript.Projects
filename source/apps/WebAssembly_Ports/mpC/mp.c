#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<emscripten/emscripten.h>

bool game_over = false;
int game_speed = 1000;
int score = 0;
SDL_Texture* create_text_texture(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color);
void print_text(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color, int x, int y);
void reset_piece();
void shift_blocks();
SDL_Color int_to_color(int i);
void puzzle_grid_draw();
void draw_piece();
bool test_piece(int x, int y);
bool test_block(int x, int y, int color);
void proc_blocks();
void chk_blocks();
void move_left();
void move_right();
void move_down();
void set_piece();
void game_init(SDL_Renderer* renderer);
void game_draw(TTF_Font* font);
void game_event(SDL_Event* event);
void game_proc();
void new_game();
void cleanup();

typedef struct {
    int blocks[3];
    int x;
    int y;
    int mode;
} PuzzlePiece;

typedef struct {
    PuzzlePiece piece;
    SDL_Renderer* renderer;
    int w;
    int h;
    int off_x;
    int off_y;
    int** puzzle_grid;
} PuzzleGrid;

typedef struct {
    SDL_Renderer* renderer;
    PuzzleGrid grid;
} Game;

Game game;

SDL_Texture* create_text_texture(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    if (!surface) {
        printf("Failed to create text surface: %s\n", TTF_GetError());
        return NULL;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        printf("Failed to create texture: %s\n", SDL_GetError());
        return NULL;
    }
    return texture;
}

void print_text(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color, int x, int y) {
    SDL_Texture* texture = create_text_texture(renderer, font, text, color);
    if (!texture) {
        return;
    }
    int text_width, text_height;
    SDL_QueryTexture(texture, NULL, NULL, &text_width, &text_height);
    SDL_Rect dstrect = { x, y, text_width, text_height };
    SDL_RenderCopy(renderer, texture, NULL, &dstrect);
    SDL_DestroyTexture(texture);
}

void reset_piece() {
    do {
        game.grid.piece.blocks[0] = rand() % 6 + 1;
        game.grid.piece.blocks[1] = rand() % 6 + 1;
        game.grid.piece.blocks[2] = rand() % 6 + 1;
    } while (game.grid.piece.blocks[0] == game.grid.piece.blocks[1] && game.grid.piece.blocks[0] == game.grid.piece.blocks[2]);
    game.grid.piece.x = 3;
    game.grid.piece.y = 0;
    game.grid.piece.mode = 0;
}

void shift_blocks() {
    int temp[3];
    temp[0] = game.grid.piece.blocks[2];
    temp[1] = game.grid.piece.blocks[0];
    temp[2] = game.grid.piece.blocks[1];
    for (int i = 0; i < 3; i++) {
        game.grid.piece.blocks[i] = temp[i];
    }
}

SDL_Color int_to_color(int i) {
    SDL_Color color;
    if (i < 0) {
        color.r = rand() % 256;
        color.g = rand() % 256;
        color.b = rand() % 256;
    } else if (i == 0) {
        color.r = 0;
        color.g = 0;
        color.b = 0;
    } else if (i == 1) {
        color.r = 0;
        color.g = 255;
        color.b = 0;
    } else if (i == 2) {
        color.r = 0;
        color.g = 0;
        color.b = 255;
    } else if (i == 3) {
        color.r = 255;
        color.g = 0;
        color.b = 255;
    } else if (i == 4) {
        color.r = 255;
        color.g = 255;
        color.b = 0;
    } else if (i == 5) {
        color.r = 255;
        color.g = 0;
        color.b = 0;
    } else {
        color.r = 0;
        color.g = 255;
        color.b = 255;
    }
    color.a = 255;
    return color;
}

void puzzle_grid_init(SDL_Renderer* renderer, int cols, int rows, int offset_x, int offset_y) {
    game.grid.renderer = renderer;
    game.grid.w = cols;
    game.grid.h = rows;
    game.grid.off_x = offset_x;
    game.grid.off_y = offset_y;
    reset_piece();

    game.grid.puzzle_grid = malloc(cols * sizeof(int*));
    for (int i = 0; i < cols; i++) {
        game.grid.puzzle_grid[i] = calloc(rows, sizeof(int));
    }
}

void puzzle_grid_draw() {
    SDL_Rect rect = { game.grid.off_x, game.grid.off_y, game.grid.w * (32 * 3), game.grid.h * (16 * 3) };
    SDL_SetRenderDrawColor(game.grid.renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(game.grid.renderer, &rect);

    for (int x = 0; x < game.grid.w; x++) {
        for (int y = 0; y < game.grid.h; y++) {
            SDL_Rect cell_rect = {
                game.grid.off_x + (x * (32 * 3)) + 1,
                game.grid.off_y + (y * (16 * 3)) + 1,
                32 * 3 - 2,
                14 * 3 - 2
            };
            SDL_Color color = int_to_color(game.grid.puzzle_grid[x][y]);
            SDL_SetRenderDrawColor(game.grid.renderer, color.r, color.g, color.b, 255);
            SDL_RenderFillRect(game.grid.renderer, &cell_rect);
        }
    }
    draw_piece();
}

void draw_piece() {
    chk_blocks();
    proc_blocks();

    SDL_Rect rect1 = {
        game.grid.off_x + (game.grid.piece.x * (32 * 3)),
        game.grid.off_y + (game.grid.piece.y * (16 * 3)),
        32 * 3,
        16 * 3
    };
    SDL_Rect rect2 = {
        game.grid.off_x + (game.grid.piece.x * (32 * 3)),
        game.grid.off_y + ((game.grid.piece.y + 1) * (16 * 3)),
        32 * 3,
        16 * 3
    };
    SDL_Rect rect3 = {
        game.grid.off_x + (game.grid.piece.x * (32 * 3)),
        game.grid.off_y + ((game.grid.piece.y + 2) * (16 * 3)),
        32 * 3,
        16 * 3
    };

    SDL_Color color1 = int_to_color(game.grid.piece.blocks[0]);
    SDL_Color color2 = int_to_color(game.grid.piece.blocks[1]);
    SDL_Color color3 = int_to_color(game.grid.piece.blocks[2]);

    SDL_SetRenderDrawColor(game.grid.renderer, color1.r, color1.g, color1.b, 255);
    SDL_RenderFillRect(game.grid.renderer, &rect1);
    SDL_SetRenderDrawColor(game.grid.renderer, color2.r, color2.g, color2.b, 255);
    SDL_RenderFillRect(game.grid.renderer, &rect2);
    SDL_SetRenderDrawColor(game.grid.renderer, color3.r, color3.g, color3.b, 255);
    SDL_RenderFillRect(game.grid.renderer, &rect3);
}

bool test_piece(int x, int y) {
    if (x < game.grid.w && y < game.grid.h - 2 &&
        game.grid.puzzle_grid[x][y] == 0 &&
        game.grid.puzzle_grid[x][y + 1] == 0 &&
        game.grid.puzzle_grid[x][y + 2] == 0) {
        return true;
    }
    return false;
}

bool test_block(int x, int y, int color) {
    if (x >= 0 && y >= 0 && x < game.grid.w && y < game.grid.h &&
        game.grid.puzzle_grid[x][y] >= 1 &&
        game.grid.puzzle_grid[x][y] == color) {
        return true;
    }
    return false;
}

void proc_blocks() {
    for (int x = 0; x < game.grid.w; x++) {
        for (int y = game.grid.h - 2; y >= 0; y--) {
            int color = game.grid.puzzle_grid[x][y];
            int color2 = game.grid.puzzle_grid[x][y + 1];
            if (color >= 1 && color2 == 0) {
                game.grid.puzzle_grid[x][y + 1] = game.grid.puzzle_grid[x][y];
                game.grid.puzzle_grid[x][y] = 0;
            }
        }
    }
}

void chk_blocks() {
    for (int x = 0; x < game.grid.w; x++) {
        for (int y = 0; y < game.grid.h; y++) {
            if (game.grid.puzzle_grid[x][y] < 0) {
                game.grid.puzzle_grid[x][y] -= 1;
                if (game.grid.puzzle_grid[x][y] < -200) {
                    game.grid.puzzle_grid[x][y] = 0;
                }
            }
        }
    }

    for (int x = 0; x < game.grid.w; x++) {
        for (int y = 0; y < game.grid.h; y++) {
            int color = game.grid.puzzle_grid[x][y];
            if (color >= 1) {
                if (test_block(x, y + 1, color) && test_block(x, y + 2, color)) {
                    game.grid.puzzle_grid[x][y] = -1;
                    game.grid.puzzle_grid[x][y + 1] = -1;
                    game.grid.puzzle_grid[x][y + 2] = -1;
                    score += 1;
                    game_speed -= 25;
                    if (test_block(x, y + 3, color)) {
                        game.grid.puzzle_grid[x][y + 3] = -1;
                        score += 1;
                        if (test_block(x, y + 4, color)) {
                            game.grid.puzzle_grid[x][y + 4] = -1;
                            score += 1;
                        }
                    }
                }
                if (test_block(x + 1, y, color) && test_block(x + 2, y, color)) {
                    game.grid.puzzle_grid[x][y] = -1;
                    game.grid.puzzle_grid[x + 1][y] = -1;
                    game.grid.puzzle_grid[x + 2][y] = -1;
                    score += 1;
                    game_speed -= 25;
                    if (test_block(x + 3, y, color)) {
                        game.grid.puzzle_grid[x + 3][y] = -1;
                        score += 1;
                        if (test_block(x + 4, y, color)) {
                            game.grid.puzzle_grid[x + 4][y] = -1;
                            score += 1;
                        }
                    }
                }
                if (test_block(x + 1, y + 1, color) && test_block(x + 2, y + 2, color)) {
                    game.grid.puzzle_grid[x][y] = -1;
                    game.grid.puzzle_grid[x + 1][y + 1] = -1;
                    game.grid.puzzle_grid[x + 2][y + 2] = -1;
                    score += 1;
                    game_speed -= 25;
                    if (test_block(x + 3, y + 3, color)) {
                        game.grid.puzzle_grid[x + 3][y + 3] = -1;
                        score += 1;
                        if (test_block(x + 4, y + 4, color)) {
                            game.grid.puzzle_grid[x + 4][y + 4] = -1;
                            score += 1;
                        }
                    }
                }
                if (test_block(x + 1, y - 1, color) && test_block(x + 2, y - 2, color)) {
                    game.grid.puzzle_grid[x][y] = -1;
                    game.grid.puzzle_grid[x + 1][y - 1] = -1;
                    game.grid.puzzle_grid[x + 2][y - 2] = -1;
                    score += 1;
                    game_speed -= 25;
                    if (test_block(x + 3, y - 3, color)) {
                        game.grid.puzzle_grid[x + 3][y - 3] = -1;
                        score += 1;
                        if (test_block(x + 4, y - 4, color)) {
                            game.grid.puzzle_grid[x + 4][y - 4] = -1;
                            score += 1;
                        }
                    }
                }
            }
        }
    }
}

void move_left() {
    if (game.grid.piece.x > 0 && test_piece(game.grid.piece.x - 1, game.grid.piece.y)) {
        game.grid.piece.x -= 1;
    }
}

void move_right() {
    if (game.grid.piece.x < game.grid.w - 1 && test_piece(game.grid.piece.x + 1, game.grid.piece.y)) {
        game.grid.piece.x += 1;
    }
}

void move_down() {
    if (game.grid.piece.y < game.grid.h - 3 && test_piece(game.grid.piece.x, game.grid.piece.y + 1)) {
        game.grid.piece.y += 1;
    } else {
        set_piece();
    }
}

void set_piece() {
    if (game.grid.piece.y == 0) {
        game_over = true;
        return;
    } else {
        game.grid.puzzle_grid[game.grid.piece.x][game.grid.piece.y] = game.grid.piece.blocks[0];
        game.grid.puzzle_grid[game.grid.piece.x][game.grid.piece.y + 1] = game.grid.piece.blocks[1];
        game.grid.puzzle_grid[game.grid.piece.x][game.grid.piece.y + 2] = game.grid.piece.blocks[2];
        reset_piece();
    }
}

void game_init(SDL_Renderer* renderer) {
    game.renderer = renderer;
    puzzle_grid_init(renderer, 8, 17, 325, 25);
    game_speed = 1000;
    game_over = false;
    score = 0;
}

void game_draw(TTF_Font* font) {
    SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);
    SDL_RenderClear(game.renderer);

    if (game_over) {
        print_text(game.renderer, font, "Game Over", (SDL_Color){255, 255, 255, 255}, 50, 50);
        print_text(game.renderer, font, "Press Enter to Start a New Game", (SDL_Color){255, 0, 0, 255}, 75, 150);
    } else {
        puzzle_grid_draw();
        char score_text[50];
        sprintf(score_text, "Score: %d", score);
        print_text(game.renderer, font, score_text, (SDL_Color){255, 255, 255, 255}, 50, 50);

        char speed_text[50];
        sprintf(speed_text, "Speed: %d", game_speed);
        print_text(game.renderer, font, speed_text, (SDL_Color){255, 0, 0, 255}, 50, 90);
    }

    SDL_RenderPresent(game.renderer);
}

void game_event(SDL_Event* event) {
    if (event->type == SDL_KEYDOWN) {
        if (event->key.keysym.sym == SDLK_LEFT) {
            move_left();
        } else if (event->key.keysym.sym == SDLK_RIGHT) {
            move_right();
        } else if (event->key.keysym.sym == SDLK_UP) {
            shift_blocks();
        } else if (event->key.keysym.sym == SDLK_DOWN) {
            move_down();
        } else if (game_over && event->key.keysym.sym == SDLK_RETURN) {
            new_game();
        }
    }
}

void game_proc() {
    move_down();
}

void new_game() {
    for (int i = 0; i < game.grid.w; i++) {
        free(game.grid.puzzle_grid[i]);
    }
    free(game.grid.puzzle_grid);
    puzzle_grid_init(game.renderer, 8, 17, 325, 25);
    game_speed = 1000;
    game_over = false;
    score = 0;
}

void cleanup() {
    for (int i = 0; i < game.grid.w; i++) {
        free(game.grid.puzzle_grid[i]);
    }
    free(game.grid.puzzle_grid);
}


SDL_Event event;
Uint32 ticks = 0;
Uint32 time_tx = 0;
TTF_Font* font;

void events() {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
            } else {
                game_event(&event);
            }
        }
        game_draw(font);
        Uint32 nticks = SDL_GetTicks();
        time_tx += nticks - ticks;
        ticks = nticks;
        if (time_tx > (Uint32)game_speed) {
            game_proc();
            time_tx = 0;
        }
}

int main(int argc, char* argv[]) {
    srand((unsigned int)time(NULL));
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() == -1) {
        printf("TTF_Init Error: %s\n", TTF_GetError());
        return 1;
    }

    font = TTF_OpenFont("/data/font.ttf", 32);
    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Puzzle Game [C Edition]", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1440, 1080, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        TTF_CloseFont(font);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_CloseFont(font);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    game_init(renderer);
    ticks = SDL_GetTicks();
    emscripten_set_main_loop(events, 0, 1);
    cleanup();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    return 0;
}

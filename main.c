#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include "struct.h"
#include <stdbool.h>

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define CHARACTER_WIDTH 224
#define CHARACTER_HEIGHT 245
#define GRAVITY 0.5
#define JUMP_VELOCITY -16
#define RUN_SPEED 5.0

#define FRAMES_PER_SECOND 15
#define NUM_FRAMES 3
#define FRAME_DELAY (1000 / FRAMES_PER_SECOND)

#define FRAMES_PER_SECOND_2 25
#define FRAME_DELAY_2 (1000 / FRAMES_PER_SECOND_2)

int main(int argc, char* argv[]) {
    SDL_Surface* screen = NULL;
    personnage per, per2;
    SDL_Event event;

    // Initialize personnage 1
    per.character_position.x = 300;
    per.character_position.y = 600;
    per.character_y_velocity = 0;
    per.char_state = 0;
    per.shift_pressed = 0;
    per.acceleration = 1.0f;
    per.start_time = 0;
    per.elapsed_time = 0;
    per.is_jumping = 0;
    per.is_running = 0;
    per.is_running_right = 0;
    per.is_running_left = 0;
    per.is_facing_right = 1;
    per.is_facing_left = 0;
    per.is_hitting = 0; // New
    per.current_walk_right_frame = 0;
    per.current_walk_left_frame = 0;
    per.current_jump_right_frame = 0;
    per.current_jump_left_frame = 0;
    per.current_hit_right_frame = 0; // New
    per.current_hit_left_frame = 0;  // New
    per.heart1_pos.x = 25;
    per.heart1_pos.y = 25;
    per.heart2_pos.x = 85;
    per.heart2_pos.y = 25;
    per.heart3_pos.x = 145;
    per.heart3_pos.y = 25;
    per.hp = 3;
    per.multiplayer = 0;
    per.score = 0;

    // Initialize personnage 2
    per2.character_position.x = 400;
    per2.character_position.y = 600;
    per2.character_y_velocity = 0;
    per2.char_state = 0;
    per2.shift_pressed = 0;
    per2.acceleration = 1.0f;
    per2.start_time = 0;
    per2.elapsed_time = 0;
    per2.is_jumping = 0;
    per2.is_running = 0;
    per2.is_running_right = 0;
    per2.is_running_left = 0;
    per2.is_facing_right = 1;
    per2.is_facing_left = 0;
    per2.is_hitting = 0; // New
    per2.current_walk_right_frame = 0;
    per2.current_walk_left_frame = 0;
    per2.current_jump_right_frame = 0;
    per2.current_jump_left_frame = 0;
    per2.current_hit_right_frame = 0; // New
    per2.current_hit_left_frame = 0;  // New
    per2.heart1_pos.x = 225;
    per2.heart1_pos.y = 25;
    per2.heart2_pos.x = 285;
    per2.heart2_pos.y = 25;
    per2.heart3_pos.x = 345;
    per2.heart3_pos.y = 25;
    per2.hp = 3;
    per2.multiplayer = 0;
    per2.score = 0;

    // Initialize images and score
    initialisation_backgroundimage(&per);
    initialisation_personnage(&per);
    initialisation_et_affichage_score(&per, screen);
    initialisation_personnage(&per2);
    initialisation_et_affichage_score(&per2, screen);
    per2.pos_score.x = 500;

    // Initialize SDL
    SDL_Init(SDL_INIT_EVERYTHING);
    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_SWSURFACE);
    SDL_WM_SetCaption("Character Movement", NULL);

    int delay_reduction = 0;

    while (1) {
        // Handle events/rendering for player 1
        if (per.hp > 0) {
            affichage_backgroundimage(per, screen);
            move_personnage(&per, &per2);
            afficher_personnage(&per, screen, delay_reduction);
            affichage_score(&per, screen);
            character_life(&per, screen);
        } else {
            affichage_deadpage(screen, per);
        }

        // Handle player 2 if active
        if (per2.multiplayer && per2.hp > 0) {
            move_personnage(&per2, &per);
            afficher_personnage(&per2, screen, delay_reduction);
            affichage_score(&per2, screen);
            character_life(&per2, screen);
        } else if (per2.hp == 0) {
            affichage_deadpage(screen, per2);
        }

        SDL_Flip(screen);

        if (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    SDL_FreeSurface(per.char_chill_R);
                    SDL_FreeSurface(per.char_chill_L);
                    SDL_FreeSurface(per2.char_chill_R);
                    SDL_FreeSurface(per2.char_chill_L);
                    SDL_FreeSurface(per.background);
                    SDL_FreeSurface(per.heart1);
                    SDL_FreeSurface(per.heart2);
                    SDL_FreeSurface(per.heart3);
                    SDL_FreeSurface(per.youdead);
                    SDL_FreeSurface(per2.heart1);
                    SDL_FreeSurface(per2.heart2);
                    SDL_FreeSurface(per2.heart3);
                    SDL_FreeSurface(per2.youdead);
                    TTF_CloseFont(per.police);
                    TTF_CloseFont(per2.police);
                    TTF_Quit();
                    SDL_Quit();
                    return 0;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        // Player 1 controls
                        case SDLK_UP:
                            if (!per.is_jumping) {
                                per.character_y_velocity = JUMP_VELOCITY;
                                per.is_jumping = 1;
                            }
                            break;
                        case SDLK_RIGHT:
                            if (!per.is_running_left) {
                                per.is_facing_right = 0;
                                per.is_facing_left = 0;
                                per.is_running = 1;
                                per.is_running_right = 1;
                                per.multiplayer = 1;
                            }
                            break;
                        case SDLK_LEFT:
                            if (!per.is_running_right) {
                                per.is_facing_right = 0;
                                per.is_facing_left = 0;
                                per.is_running = 1;
                                per.is_running_left = 1;
                                per.multiplayer = 1;
                            }
                            break;
                        case SDLK_LSHIFT:
                            per.shift_pressed = 1;
                            per.start_time = SDL_GetTicks();
                            per.acceleration = 1.0f;
                            break;
                        case SDLK_SPACE:
                            if (per.hp > 0 && !per.is_hitting) {
                                per.is_hitting = 1;
                                per.current_hit_right_frame = 0;
                                per.current_hit_left_frame = 0;
                            }
                            break;
                        // Player 2 controls
                        case SDLK_f:
                            per2.multiplayer = !per2.multiplayer;
                            break;
                        case SDLK_z:
                            if (per2.multiplayer && !per2.is_jumping) {
                                per2.character_y_velocity = JUMP_VELOCITY;
                                per2.is_jumping = 1;
                            }
                            break;
                        case SDLK_q:
                            if (per2.multiplayer && !per2.is_running_right) {
                                per2.is_facing_right = 0;
                                per2.is_facing_left = 0;
                                per2.is_running = 1;
                                per2.is_running_left = 1;
                            }
                            break;
                        case SDLK_d:
                            if (per2.multiplayer && !per2.is_running_left) {
                                per2.is_facing_right = 0;
                                per2.is_facing_left = 0;
                                per2.is_running = 1;
                                per2.is_running_right = 1;
                            }
                            break;
                        case SDLK_a:
                            if (per2.multiplayer && per2.hp > 0 && !per2.is_hitting) {
                                per2.is_hitting = 1;
                                per2.current_hit_right_frame = 0;
                                per2.current_hit_left_frame = 0;
                            }
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (event.key.keysym.sym) {
                        // Player 1 controls
                        case SDLK_RIGHT:
                            per.is_facing_right = 1;
                            per.is_running = 0;
                            per.is_running_right = 0;
                            per.multiplayer = 0;
                            break;
                        case SDLK_LEFT:
                            per.is_facing_left = 1;
                            per.is_running = 0;
                            per.is_running_left = 0;
                            per.multiplayer = 0;
                            break;
                        case SDLK_LSHIFT:
                            per.shift_pressed = 0;
                            per.acceleration = 1.0f;
                            break;
                        // Player 2 controls
                        case SDLK_q:
                            if (per2.multiplayer) {
                                per2.is_facing_left = 1;
                                per2.is_running = 0;
                                per2.is_running_left = 0;
                            }
                            break;
                        case SDLK_d:
                            if (per2.multiplayer) {
                                per2.is_facing_right = 1;
                                per2.is_running = 0;
                                per2.is_running_right = 0;
                            }
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }

        if (per.multiplayer || per2.multiplayer) {
            delay_reduction = 1;
        } else {
            delay_reduction = 0;
        }
    }

    return 0;
}

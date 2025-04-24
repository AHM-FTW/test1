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
#define MIN_DISTANCE 50

#define FRAMES_PER_SECOND 15
#define NUM_FRAMES 3
#define FRAME_DELAY (1000 / FRAMES_PER_SECOND)

#define FRAMES_PER_SECOND_2 25
#define FRAME_DELAY_2 (1000 / FRAMES_PER_SECOND_2)


void animation_character_walk_right(personnage *perso, SDL_Surface* screen);
void animation_character_walk_left(personnage *perso, SDL_Surface* screen);
void animation_character_jump_right(personnage *perso, SDL_Surface* screen);
void animation_character_jump_left(personnage *perso, SDL_Surface* screen);
void animation_character_hit_right(personnage *perso, SDL_Surface* screen);
void animation_character_hit_left(personnage *perso, SDL_Surface* screen);

void initialisation_backgroundimage(personnage *perso) {
    perso->background = IMG_Load("back ground.jpeg");
    if (perso->background == NULL) {
        printf("unable to load background image %s \n", SDL_GetError());
        return;
    }
}

void affichage_backgroundimage(personnage perso, SDL_Surface *screen) {
    SDL_BlitSurface(perso.background, NULL, screen, NULL);
}

void move_personnage(personnage *perso, personnage *other_perso) {
    if (perso->acceleration >= 1.0f) {
        perso->elapsed_time = SDL_GetTicks() - perso->start_time;
        if (perso->elapsed_time < 0) {
            float new_K = (perso->elapsed_time / 750) * 0.5f + 1.0f;
            if (new_K <= 3.0f && new_K > perso->acceleration) {
                perso->acceleration = new_K;
            }
        } else {
            perso->acceleration = 3.0f;
        }
    }

    float new_x = perso->character_position.x;
    if (perso->is_running_right) {
        new_x += perso->shift_pressed ? RUN_SPEED * perso->acceleration : RUN_SPEED;
    }
    if (perso->is_running_left) {
        new_x -= perso->shift_pressed ? RUN_SPEED * perso->acceleration : RUN_SPEED;
    }

    if (other_perso->multiplayer) {
        float distance = new_x - other_perso->character_position.x;
        if (distance > 0 && distance < MIN_DISTANCE) {
            new_x = other_perso->character_position.x + MIN_DISTANCE;
        } else if (distance < 0 && distance > -MIN_DISTANCE) {
            new_x = other_perso->character_position.x - MIN_DISTANCE;
        }
    }

    perso->character_position.x = new_x;

    if (perso->is_jumping) {
        perso->character_y_velocity += GRAVITY;
        perso->character_position.y += perso->character_y_velocity;
        if (perso->character_position.y >= 600) {
            perso->character_position.y = 600;
            perso->character_y_velocity = 0;
            perso->is_jumping = 0;
        }
    }
}

void initialisation_personnage(personnage *perso) {
    perso->char_chill_R = IMG_Load("images_character/chillright.png");
    if (perso->char_chill_R == NULL) {
        printf("unable to load char_chill_R image %s \n", SDL_GetError());
        return;
    }

    perso->char_chill_L = IMG_Load("images_character/chillleft.png");
    if (perso->char_chill_L == NULL) {
        printf("unable to load char_chill_L image %s \n", SDL_GetError());
        return;
    }

    perso->heart1 = IMG_Load("heart2.png");
    if (perso->heart1 == NULL) {
        printf("unable to load heart1 image %s \n", SDL_GetError());
        return;
    }

    perso->heart2 = IMG_Load("heart2.png");
    if (perso->heart2 == NULL) {
        printf("unable to load heart2 image %s \n", SDL_GetError());
        return;
    }

    perso->heart3 = IMG_Load("heart2.png");
    if (perso->heart3 == NULL) {
        printf("unable to load heart3 image %s \n", SDL_GetError());
        return;
    }

    perso->youdead = IMG_Load("you died.png");
    if (perso->youdead == NULL) {
        printf("unable to load you died image %s \n", SDL_GetError());
        return;
    }
}

void animation_character_walk_right(personnage *perso, SDL_Surface* screen) {
    char bmpframe[100];
    sprintf(bmpframe, "images_character/character walk facing right/frame%d.png", perso->current_walk_right_frame);
    SDL_Surface* frame = IMG_Load(bmpframe);
    if (!frame) {
        printf("Failed to load frame %d: %s\n", perso->current_walk_right_frame, SDL_GetError());
        return;
    }

    SDL_BlitSurface(frame, NULL, screen, &perso->character_position);
    SDL_FreeSurface(frame);
}

void animation_character_walk_left(personnage *perso, SDL_Surface* screen) {
    char bmpframe[100];
    sprintf(bmpframe, "images_character/character walk facing left/frame%d.png", perso->current_walk_left_frame);
    SDL_Surface* frame = IMG_Load(bmpframe);
    if (!frame) {
        printf("Failed to load frame %d: %s\n", perso->current_walk_left_frame, SDL_GetError());
        return;
    }

    SDL_BlitSurface(frame, NULL, screen, &perso->character_position);
    SDL_FreeSurface(frame);
}

void animation_character_jump_right(personnage *perso, SDL_Surface* screen) {
    char bmpframe[100];
    sprintf(bmpframe, "images_character/character jump run facing right/frame%d.png", perso->current_jump_right_frame);
    SDL_Surface* frame = IMG_Load(bmpframe);
    if (!frame) {
        printf("Failed to load frame %d: %s\n", perso->current_jump_right_frame, SDL_GetError());
        return;
    }

    SDL_BlitSurface(frame, NULL, screen, &perso->character_position);
    SDL_FreeSurface(frame);
}

void animation_character_jump_left(personnage *perso, SDL_Surface* screen) {
    char bmpframe[100];
    sprintf(bmpframe, "images_character/character jump run facing left/frame%d.png", perso->current_jump_left_frame);
    SDL_Surface* frame = IMG_Load(bmpframe);
    if (!frame) {
        printf("Failed to load frame %d: %s\n", perso->current_jump_left_frame, SDL_GetError());
        return;
    }

    SDL_BlitSurface(frame, NULL, screen, &perso->character_position);
    SDL_FreeSurface(frame);
}

void animation_character_hit_right(personnage *perso, SDL_Surface* screen) {
    char bmpframe[100];
    sprintf(bmpframe, "images_character/character hit on right/frame%d.png", perso->current_hit_right_frame);
    SDL_Surface* frame = IMG_Load(bmpframe);
    if (!frame) {
        printf("Failed to load frame %d: %s\n", perso->current_hit_right_frame, SDL_GetError());
        return;
    }

    int h_chill = perso->char_chill_R->h;
    int w_chill = perso->char_chill_R->w;
    int h_hit = frame->h;
    int w_hit = frame->w;

    SDL_Rect blit_pos = perso->character_position;
    blit_pos.y += (h_chill - h_hit);  
    blit_pos.x += (w_chill - w_hit) / 2;  

    SDL_BlitSurface(frame, NULL, screen, &blit_pos);
    SDL_FreeSurface(frame);
}

void animation_character_hit_left(personnage *perso, SDL_Surface* screen) {
    char bmpframe[100];
    sprintf(bmpframe, "images_character/character hit on left/frame%d.png", perso->current_hit_left_frame);
    SDL_Surface* frame = IMG_Load(bmpframe);
    if (!frame) {
        printf("Failed to load frame %d: %s\n", perso->current_hit_left_frame, SDL_GetError());
        return;
    }

    int h_chill = perso->char_chill_L->h;
    int w_chill = perso->char_chill_L->w;
    int h_hit = frame->h;
    int w_hit = frame->w;

    SDL_Rect blit_pos = perso->character_position;
    blit_pos.y += (h_chill - h_hit);  
    blit_pos.x += (w_chill - w_hit) / 2;  

    SDL_BlitSurface(frame, NULL, screen, &blit_pos);
    SDL_FreeSurface(frame);
}

void wait_frame(void) {
    static Uint32 next_frame_time = 0;
    Uint32 current_time = SDL_GetTicks();
    if (next_frame_time > current_time) {
        SDL_Delay(next_frame_time - current_time);
    }
    next_frame_time = current_time + FRAME_DELAY;
}

void wait_frame2(void) {
    static Uint32 next_frame_time = 0;
    Uint32 current_time = SDL_GetTicks();
    if (next_frame_time > current_time) {
        SDL_Delay(next_frame_time - current_time);
    }
    next_frame_time = current_time + FRAME_DELAY_2;
}

void afficher_personnage(personnage *perso, SDL_Surface *screen, int delay_reduction) {
    if (perso->is_hitting) {
        if (perso->is_facing_right) {
            animation_character_hit_right(perso, screen);
            wait_frame();
            perso->current_hit_right_frame = (perso->current_hit_right_frame + 1) % NUM_FRAMES;
            if (perso->current_hit_right_frame == 0) {
                perso->is_hitting = 0; 
            }
        } else if (perso->is_facing_left) {
            animation_character_hit_left(perso, screen);
            wait_frame();
            perso->current_hit_left_frame = (perso->current_hit_left_frame + 1) % NUM_FRAMES;
            if (perso->current_hit_left_frame == 0) {
                perso->is_hitting = 0; 
            }
        }
    } else if (perso->is_facing_right == 1) {
        SDL_BlitSurface(perso->char_chill_R, NULL, screen, &perso->character_position);
    } else if (perso->is_facing_left == 1) {
        SDL_BlitSurface(perso->char_chill_L, NULL, screen, &perso->character_position);
    } else if (perso->is_jumping && perso->is_running_right == 1) {
        animation_character_jump_right(perso, screen);
    } else if (perso->is_jumping && perso->is_running_left == 1) {
        animation_character_jump_left(perso, screen);
    } else if (perso->is_running) {
        if (perso->is_running_right == 1 && perso->shift_pressed == 0) {
            animation_character_walk_right(perso, screen);
            if (delay_reduction == 0)
                wait_frame();
            if (delay_reduction == 1)
                wait_frame2();
            perso->current_walk_right_frame = (perso->current_walk_right_frame + 1) % NUM_FRAMES;
        } else if (perso->is_running_left == 1 && perso->shift_pressed == 0) {
            animation_character_walk_left(perso, screen);
            if (delay_reduction == 0)
                wait_frame();
            if (delay_reduction == 1)
                wait_frame2();
            perso->current_walk_left_frame = (perso->current_walk_left_frame + 1) % NUM_FRAMES;
        } else if (perso->is_running_right == 1 && perso->shift_pressed == 1) {
            animation_character_walk_right(perso, screen);
            if (delay_reduction == 0)
                wait_frame();
            if (delay_reduction == 1)
                wait_frame2();
            perso->current_walk_right_frame = (perso->current_walk_right_frame + 1) % NUM_FRAMES;
        } else if (perso->is_running_left == 1 && perso->shift_pressed == 1) {
            animation_character_walk_left(perso, screen);
            if (delay_reduction == 0)
                wait_frame();
            if (delay_reduction == 1)
                wait_frame2();
            perso->current_walk_left_frame = (perso->current_walk_left_frame + 1) % NUM_FRAMES;
        }
    }
}

void initialisation_et_affichage_score(personnage *perso, SDL_Surface *screen) {
    TTF_Init();
    perso->police = TTF_OpenFont("police.ttf", 45);
    if (perso->police == NULL) {
        printf("unable to load font %s \n", SDL_GetError());
        return;
    }
    perso->color_score.r = 255;
    perso->color_score.g = 255;
    perso->color_score.b = 255;
    perso->pos_score.x = 300;
    perso->pos_score.y = 50;
}

void affichage_score(personnage *perso, SDL_Surface *screen) {
    static float previous_x = 0;
    if (perso->is_running_right && perso->character_position.x > previous_x) {
        perso->score += (perso->character_position.x - previous_x) * 10;
    }
    previous_x = perso->character_position.x;

    char score_str[20];
    sprintf(score_str, "%d", perso->score);
    perso->score_txt = TTF_RenderText_Blended(perso->police, score_str, perso->color_score);
    if (perso->score_txt == NULL) {
        printf("unable to render score text %s \n", SDL_GetError());
        return;
    }
    SDL_BlitSurface(perso->score_txt, NULL, screen, &perso->pos_score);
    SDL_FreeSurface(perso->score_txt);
}

void character_life(personnage *perso, SDL_Surface *screen) {
    SDL_BlitSurface(perso->heart1, NULL, screen, &perso->heart1_pos);
    SDL_BlitSurface(perso->heart2, NULL, screen, &perso->heart2_pos);
    SDL_BlitSurface(perso->heart3, NULL, screen, &perso->heart3_pos);
    if (perso->character_position.y >= 536 && perso->character_position.x >= 1500) {
        perso->character_position.x = perso->character_position.x - 100;
        perso->character_position.y = 600;
        perso->hp = perso->hp - 1;
    }

    if (perso->hp == 2) {
        perso->heart3_pos.y = 2500;
    }
    if (perso->hp == 1) {
        perso->heart2_pos.y = 2500;
    }
    if (perso->hp == 0) {
        perso->heart1_pos.y = 2500;
        SDL_Delay(220);
    }
}

void affichage_deadpage(SDL_Surface *screen, personnage perso) {
    SDL_BlitSurface(perso.youdead, NULL, screen, NULL);
}

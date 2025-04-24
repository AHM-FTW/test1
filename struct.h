#ifndef STRUCT_H
#define STRUCT_H
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#define SCREEN_H 1080
#define SCREEN_W 1920

typedef struct
{
    SDL_Surface *char_chill_R;
    SDL_Surface *char_chill_L;
    SDL_Surface *background;
    SDL_Rect character_position;
    double character_y_velocity;
    int char_state;
    int shift_pressed;
    float acceleration;
    Uint32 start_time;
    Uint32 elapsed_time;
    int is_facing_right;
    int is_facing_left;
    int is_jumping;
    int is_running;
    int is_running_right;
    int is_running_left;
    int is_hitting; 
    int current_walk_right_frame;
    int current_walk_left_frame;
    int current_jump_right_frame;
    int current_jump_left_frame;
    int current_hit_right_frame; 
    int current_hit_left_frame;  
    int score;
    SDL_Surface *score_txt;
    SDL_Rect pos_score;
    SDL_Color color_score;
    TTF_Font *police;
    SDL_Surface *heart1;
    SDL_Rect heart1_pos;
    SDL_Surface *heart2;
    SDL_Rect heart2_pos;
    SDL_Surface *heart3;
    SDL_Rect heart3_pos;
    int hp;
    SDL_Surface *youdead;
    int multiplayer;
} personnage;

void initialisation_backgroundimage(personnage *perso);
void affichage_backgroundimage(personnage perso, SDL_Surface *screen);
void move_personnage(personnage *perso, personnage *other_perso);
void initialisation_personnage(personnage *perso);
void afficher_personnage(personnage *perso, SDL_Surface *screen, int delay_reduction);
void wait_frame(void);
void wait_frame2(void);
void initialisation_et_affichage_score(personnage *perso, SDL_Surface *screen);
void affichage_score(personnage *perso, SDL_Surface *screen);
void character_life(personnage *perso, SDL_Surface *screen);
void affichage_deadpage(SDL_Surface *screen, personnage perso);
void animation_character_hit_right(personnage *perso, SDL_Surface* screen);
void animation_character_hit_left(personnage *perso, SDL_Surface* screen);

#endif

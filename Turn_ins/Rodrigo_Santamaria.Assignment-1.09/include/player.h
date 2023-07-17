#ifndef PLAYER_H
#define PLAYER_H

#include "data_classes.h"
#include <time.h>
#include <string>

#define PC 0
#define HIKER 1
#define RIVAL 2
#define PACER 4
#define WANDERER 8
#define SENTRIE 16
#define EXPLORER 32
#define SWIMMER 64

class IVs{
    public:
        int hp, attack, defense, sp_attack, sp_defense, speed, accuracy, evasion;
        IVs() : hp(rand() % 16), attack(rand() % 16), defense(rand() % 16), sp_attack(rand() % 16), sp_defense(rand() % 16), speed(rand() % 16), accuracy(0), evasion(0) {}
};

class Stats{
    public:
        int b_hp, b_attack, b_defense, b_sp_attack, b_sp_defense, b_speed, b_accuracy, b_evasion;
        int hp, attack, defense, sp_attack, sp_defense, speed, accuracy, evasion;
    Stats(){}
    Stats(Data * db, IVs * ivs_t, int id, int level);
    bool set_Stats(IVs * ivs_t, int level);
};

class Pokemon{
    public:
        IVs ivs;
        Stats * stats;
        int id, hp, level, moves_t[4], n_moves;
        bool shiny, genre;
        std::string name; 

        void set_moves(Data * db);

        Pokemon(Data * db, int i, int lvl);
        ~Pokemon() {}
        
};

class character{
    public:
        Pokemon * pokemons[6];
        int item[3];
        int n_pkmn;
        char r, c, txt, last_move;
        bool add_pokemon(Data * db, int i, int lvl);
        character() : n_pkmn(0) {
            for(int j = 0; j < 6; j++){
                item[j % 3] = 3 + rand() % 3; 
                pokemons[j] = NULL;
            }
        }
        int has_pkm_alive();
};

class npc: public character{
    public:
        char type, state;
        int init(char type, int r, int c, char txt, char last_move);
};

class Pc: public character{
    public:
        char cell_type;
        int init(int r,int c, char txt, char last_move);
};

#endif 

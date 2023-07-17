#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <climits>
#include <vector>
#include "player.h"

int character::has_pkm_alive(){
    int i;
    for(i = 0; i < 6; i++){
        if(NULL != pokemons[i] && pokemons[i] -> hp > 0){
            return i;
        }
    }
    
    return -1;
}

int npc::init(char type, int r, int c,char txt, char last_move){
   // if(!(t = malloc(sizeof(t)))){
   //     return 1;
   // }
    this -> type = type;
    this -> r = r;
    this -> c = c;
    this -> txt = txt;
    this -> last_move = last_move;
    return 0;
}

int Pc::init(int r, int c, char txt, char last_move){
    this -> r = r;
    this -> c = c;
    this -> txt = txt;
    this -> last_move = last_move;
    this -> cell_type = '#';

    return 0;
}

void Pokemon::set_moves(Data * db){
    int spcs_id = db -> arr_pkm[id] -> species_id, i, j, 
        miin = INT_MAX, n_moves = 528239, n_valids, i_t; 
    std::vector<int> valid_moves;

    for(i = 1; i < n_moves; i++){
        if(db -> arr_pkm_mvs[i] -> pokemon_id == spcs_id) break;
    }
    
    j = i;

    for(; j < n_moves && db -> arr_pkm_mvs[j] -> pokemon_id == spcs_id; j++){
        if(db -> arr_pkm_mvs[j] -> pokemon_move_method_id == 1){
            valid_moves.push_back(j);
            if(db -> arr_pkm_mvs[j] -> level < miin){
                miin = db -> arr_pkm_mvs[j] -> level;
            }
        }
    }

    n_valids = (int)valid_moves.size();

    //std::cout << n_valids << std::endl;

    if(miin > level){
        level = miin;
    }

    j = 0;
    i_t = -1;

    for(i = 0; i < n_valids && j < 2; i++){
        if(db -> arr_pkm_mvs[valid_moves[i]] -> level <= level && i_t != db -> arr_pkm_mvs[valid_moves[i]] -> move_id){
            moves_t[j] = db -> arr_pkm_mvs[valid_moves[i]] -> move_id;
            i_t = moves_t[j];
            j++;
        }
    }
}
        
Pokemon::Pokemon(Data * db, int i, int lvl){
    id = i;
    level = lvl;
    name = db -> arr_pkm[i] -> identifier;

    moves_t[0] = moves_t[1] = moves_t[2] = moves_t[3] = INT_MAX;

    set_moves(db);

    n_moves = 1 + (moves_t[1] != INT_MAX);
    stats = new Stats(db, &ivs, i, lvl);
    shiny = rand() % 8192 == 0;
    genre = rand() % 2;
    hp = stats -> hp;
}

bool character::add_pokemon(Data * db, int i, int lvl){
    if(n_pkmn == 6) return false;

    pokemons[n_pkmn++] = new Pokemon(db, i, lvl);

    return true;
}

Stats::Stats(Data * db, IVs * ivs_t, int id, int level){
    id = id * 6 + 1;
    b_hp = db -> arr_pkm_stts[id] -> base_stat;
    b_attack = db -> arr_pkm_stts[id + 1] -> base_stat;
    b_defense = db -> arr_pkm_stts[id + 2] -> base_stat;
    b_sp_attack = db -> arr_pkm_stts[id + 3] -> base_stat;
    b_sp_defense = db -> arr_pkm_stts[id + 4] -> base_stat;
    b_speed = db -> arr_pkm_stts[id + 5] -> base_stat;

    set_Stats(ivs_t, level);
}

bool Stats::set_Stats(IVs * ivs_t, int level){
    if(level == 100) return false;

    hp = (((b_hp + ivs_t -> hp) << 1) * level)/100 + level + 10;
    attack = (((b_attack + ivs_t -> attack) << 1) * level)/100 + 5;
    defense = (((b_defense + ivs_t -> defense) << 1) * level)/100 + 5;
    sp_attack = (((b_sp_attack + ivs_t -> sp_attack) << 1) * level)/100 + 5;
    sp_defense = (((b_sp_defense + ivs_t -> sp_defense) << 1) * level)/100 + 5;
    speed = (((b_speed + ivs_t -> speed) << 1) * level)/100 + 5;
    accuracy = 0;
    evasion = 0;
    
    return true;
}

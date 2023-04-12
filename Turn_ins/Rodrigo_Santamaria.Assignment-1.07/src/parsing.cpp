#include "parsing.h"
#include <iostream>
#include <climits>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "data_classes.h"

Pokemon         ** arr_pkm;
Moves           ** arr_mvs;
Pokemon_Moves   ** arr_pkm_mvs;
Pokemon_species ** arr_pkm_spcs;
Experience      ** arr_xprnc;
Type_Names      ** arr_tp_nms;
Pokemon_Stats   ** arr_pkm_stts;
Stats           ** arr_stts;
Pokemon_Types   ** arr_pkm_tps;

int read_file(std::string file){
    int cont, header_size, i, j, idx, line_n, line_c, class_t;
    std::string line, identifier_t, s_tmp;
    std::string home_path = getenv("HOME"), share_path = "/share/cs327", csv_file = "/pokedex/pokedex/data/csv/", working_path, current_path = getenv("PATH");
    home_path += "/.poke327";
    working_path = share_path + csv_file + file + ".csv";

    std::ifstream file_strm;

    //printf("Trying to open: %s\n", working_path.c_str());
    file_strm.open(working_path, std::ifstream::in);

    if(file_strm.fail()){
        //printf("There was an error opening: %s\n", working_path.c_str());

        working_path = home_path + csv_file + file + ".csv";
        
        //printf("Trying to open: %s\n", working_path.c_str());
        file_strm.open(working_path, std::ifstream::in);

        if(file_strm.fail()){
            //printf("There was an error opening: %s\n", working_path.c_str());
            
            working_path = current_path + csv_file + file + ".csv";
            
            //printf("Trying to open: %s\n", working_path.c_str());
            file_strm.open(working_path, std::ifstream::in);
            
            if(file_strm.fail()){
                printf("There was a problem trying to find %s.csv\nPlease try a different file\n", file.c_str()); 
                return 0;
            }
        }
    }

    //printf("Reading %s.csv\n", file.c_str());

    std::string file_format;

    if(!file.compare("pokemon"))
        class_t = 0, line_n = 1092, arr_pkm = (Pokemon **)malloc(sizeof(Pokemon * *) * line_n), file_format = "isiiiiii";
    else if(!file.compare("moves"))
        class_t = 1, line_n = 844, arr_mvs = (Moves **)malloc(sizeof(Moves * *) * line_n), file_format = "isiiiiiiiiiiiii";
    else if(!file.compare("pokemon_moves"))
        class_t = 2, line_n = 528238, arr_pkm_mvs = (Pokemon_Moves **)malloc(sizeof(Pokemon_Moves * *) * line_n), file_format = "iiiiii";
    else if(!file.compare("pokemon_species"))
        class_t = 3, line_n = 898, arr_pkm_spcs = (Pokemon_species **)malloc(sizeof(Pokemon_species * *) * line_n), file_format = "isiiiiiiiiiiiiiiiiii";
    else if(!file.compare("experience"))
        class_t = 4, line_n = 600, arr_xprnc = (Experience **)malloc(sizeof(Experience * *) * line_n), file_format = "iii";
    else if(!file.compare("pokemon_types"))
        class_t = 5, line_n = 1675, arr_pkm_tps = (Pokemon_Types **)malloc(sizeof(Pokemon_Types * *) * line_n), file_format = "iii";
    else if(!file.compare("type_names"))
        class_t = 6, line_n = 193, arr_tp_nms = (Type_Names **)malloc(sizeof(Type_Names * *) * line_n), file_format = "iis";
    else if(!file.compare("pokemon_stats"))
        class_t = 7, line_n = 6552, arr_pkm_stts = (Pokemon_Stats **)malloc(sizeof(Pokemon_Stats * *) * line_n), file_format = "iiii";
    else if(!file.compare("stats"))
        class_t = 8, line_n = 8, arr_stts = (Stats **)malloc(sizeof(Stats * *) * line_n), file_format = "iisii";
    else{
        printf("There is no format specified for %s.csv\n", file.c_str());
        return 0;
    }

    header_size = (int)file_format.size();

    std::string header[header_size];

    int integers[header_size];
    
    std::getline(file_strm, line, '\n');

    idx = 0;

    printf("%s\n", line.c_str());


    for(i = 0; i < (int)line.size(); i++){
        j = (int)line.find(',', i);
        
        if(j == -1){
            j = (int)line.size() - 1;
        }
        
        header[idx++] = line.substr(i, j - i);
        i = j;
    }
    
    line_c = 0;

    while(std::getline(file_strm, line)){
        idx = 0;
        cont = 0; 
        for(i = 0; i < (int)line.size(); i++){
            j = (int)line.find(',', i);
            
            if(j == -1){
                j = (int)line.size();
            }
            
            s_tmp = line.substr(i, j - i);
            
            if(file_format[cont++] == 'i') 
                integers[idx++] = (j == i) ? INT_MAX : atoi(s_tmp.c_str());
            else
                identifier_t = s_tmp; 
            i = j;
            if(i == (int)line.size() - 1){
                integers[idx++] = INT_MAX;
            }
        }
        switch(class_t){
            case 0:
                arr_pkm[line_c] = new Pokemon(integers[0], identifier_t, integers[1], integers[2], integers[3], integers[4], integers[5], integers[6]); 
                //printf("%d,%s,%d,%d,%d,%d,%d,%d\n", /*arr_pkm[line_c] -> id*/(int)NULL, arr_pkm[line_c] -> identifier.c_str(), arr_pkm[line_c] -> species_id, arr_pkm[line_c] -> height, arr_pkm[line_c] -> weight, arr_pkm[line_c] -> base_experience, arr_pkm[line_c] -> order, arr_pkm[line_c] -> is_default); 
                break;
            case 1:
                arr_mvs[line_c] = new Moves(integers[0], identifier_t, integers[1], integers[2], integers[3], integers[4], integers[5], integers[6], integers[7], integers[8], integers[9], integers[10], integers[11], integers[12], integers[13]); 
                //printf("%d,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", arr_mvs[line_c] -> id, arr_mvs[line_c] -> identifier.c_str(), arr_mvs[line_c] -> generation_id, arr_mvs[line_c] -> type_id, arr_mvs[line_c] -> power, arr_mvs[line_c] -> pp, arr_mvs[line_c] -> accuracy, arr_mvs[line_c] -> priority , arr_mvs[line_c] -> target_id, arr_mvs[line_c] -> damage_class_id, arr_mvs[line_c] -> effect_id, arr_mvs[line_c] -> effect_chance, arr_mvs[line_c] -> contest_type_id, arr_mvs[line_c] -> contest_effect_id, arr_mvs[line_c] -> super_constest_effect_id); 
                break;
            case 2:
                arr_pkm_mvs[line_c] = new Pokemon_Moves(integers[0], integers[1], integers[2], integers[3], integers[4], integers[5]); 
                //printf("%d,%d,%d,%d,%d,%d\n", arr_pkm_mvs[line_c] -> pokemon_id, arr_pkm_mvs[line_c] -> version_group_id, arr_pkm_mvs[line_c] -> move_id, arr_pkm_mvs[line_c] -> pokemon_move_method_id, arr_pkm_mvs[line_c] -> level, arr_pkm_mvs[line_c] -> order); 
                break;
            case 3:
                arr_pkm_spcs[line_c] = new Pokemon_species(integers[0], identifier_t, integers[1], integers[2], integers[3], integers[4], integers[5], integers[6], integers[7], integers[8], integers[9], integers[10], integers[11], integers[12], integers[13], integers[14], integers[15], integers[16], integers[17], integers[18]); 
                //printf("%d,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", arr_pkm_spcs[line_c] -> id, arr_pkm_spcs[line_c] -> identifier.c_str(), arr_pkm_spcs[line_c] -> generation_id, arr_pkm_spcs[line_c] -> evolves_from_species_id, arr_pkm_spcs[line_c] -> evolution_chain_id, arr_pkm_spcs[line_c] -> color_id, arr_pkm_spcs[line_c] -> shape_id, arr_pkm_spcs[line_c] -> habitat_id, arr_pkm_spcs[line_c] -> gender_rate, arr_pkm_spcs[line_c] -> capture_rate, arr_pkm_spcs[line_c] -> base_happiness, arr_pkm_spcs[line_c] -> is_baby, arr_pkm_spcs[line_c] -> hatch_counter, arr_pkm_spcs[line_c] -> has_gender_differences, arr_pkm_spcs[line_c] -> growth_rate_id, arr_pkm_spcs[line_c] -> forms_switchable, arr_pkm_spcs[line_c] -> is_legendary, arr_pkm_spcs[line_c] -> is_mythical, arr_pkm_spcs[line_c] -> order, arr_pkm_spcs[line_c] -> conquest_order); 
                break;
            case 4:
                arr_xprnc[line_c] = new Experience(integers[0], integers[1], integers[2]);
                //printf("%d,%d,%d\n", arr_xprnc[line_c] -> growth_rate_id, arr_xprnc[line_c] -> level, arr_xprnc[line_c] -> experience);
                break;
            case 5:
                arr_pkm_tps[line_c] = new Pokemon_Types(integers[0], integers[1], integers[2]);
                //printf("%d,%d,%d\n", arr_pkm_tps[line_c] -> pokemon_id, arr_pkm_tps[line_c] -> type_id, arr_pkm_tps[line_c] -> slot);
                break;
            case 6:
                arr_tp_nms[line_c] = new Type_Names(integers[0], integers[1], identifier_t);
                //printf("%d,%d,%s\n", arr_tp_nms[line_c] -> type_id, arr_tp_nms[line_c] -> local_language_id, arr_tp_nms[line_c] -> name.c_str());
                break;
            case 7:
                arr_pkm_stts[line_c] = new Pokemon_Stats(integers[0], integers[1], integers[2], integers[3]);
                //printf("%d,%d,%d,%d\n", arr_pkm_stts[line_c] -> pokemon_id, arr_pkm_stts[line_c] -> stat_id, arr_pkm_stts[line_c] -> base_stat, arr_pkm_stts[line_c] -> effort);
                break;
            case 8:
                arr_stts[line_c] = new Stats(integers[0], integers[1], identifier_t, integers[2], integers[3]);
                //printf("%d,%d,%s,%d,%d\n", arr_stts[line_c] -> id, arr_stts[line_c] -> damage_class_id, arr_stts[line_c] -> identifier.c_str(), arr_stts[line_c] -> is_battle_only, arr_stts[line_c] -> game_index);
                break;
        }

        idx = 0;
        for(i = 0; i < header_size; i++){
            
            if(file_format[i] == 'i'){ 
                if(integers[idx] != INT_MAX)
                    std::cout << integers[idx];
                idx++;
            }
            else
                    std::cout << identifier_t; 
            if(i < header_size - 1)
                std::cout << ',';
        }
        std::cout << std::endl;


        line_c++;
    }
        



    file_strm.close();
    return 1;
}

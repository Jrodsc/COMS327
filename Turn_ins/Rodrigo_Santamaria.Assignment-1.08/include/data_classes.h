#ifndef DATA_CLASSES_H
#define DATA_CLASSES_H

#include <string>


class Pokemon_data{
    public:
        int id;
        std::string identifier;
        int species_id, height, weight, base_experience, order, is_default;
        Pokemon_data(int t1, std::string t2, int t3, int t4, int t5, int t6, int t7, int t8) : id(t1), identifier(t2), species_id(t3), height(t4), weight(t5), base_experience(t6), order(t7), is_default(t8) {}
        Pokemon_data(){}
        ~Pokemon_data(){}
};

class Moves_data{
    public:
        int id;
        std::string identifier;
        int generation_id, type_id, power, pp, accuracy, priority, target_id, damage_class_id, effect_id, effect_chance, contest_type_id, contest_effect_id, super_constest_effect_id;
        Moves_data(int t1, std::string t2, int t3, int t4, int t5, int t6, int t7, int t8, int t9, 
            int t10, int t11, int t12, int t13, int t14, int t15) :
                id(t1), identifier(t2), generation_id(t3), type_id(t4), power(t5), pp(t6),
                accuracy(t7), priority(t8), target_id(t9), damage_class_id(t10),
                effect_id(t11), effect_chance(t12), contest_type_id(t13), contest_effect_id(t14),
                super_constest_effect_id(t15)
        {}
        Moves_data(){}
        ~Moves_data(){}
};

class Pokemon_Moves_data{
    public:
        int pokemon_id, version_group_id, move_id, pokemon_move_method_id, level, order;
        Pokemon_Moves_data(int t1, int t2, int t3, int t4, int t5, int t6) : pokemon_id(t1), 
                version_group_id(t2), move_id(t3), pokemon_move_method_id(t4), level(t5),
                order(t6)
        {}
        Pokemon_Moves_data(){}
        ~Pokemon_Moves_data(){}
};
                            
class Pokemon_species_data{
    public:
        int id;
        std::string identifier;
        int generation_id, evolves_from_species_id, evolution_chain_id, color_id, shape_id, habitat_id, gender_rate, capture_rate, base_happiness, is_baby, hatch_counter, has_gender_differences, growth_rate_id, forms_switchable, is_legendary, is_mythical, order, conquest_order;
        Pokemon_species_data(int t1, std::string t2, int t3, int t4, int t5, int t6, int t7, int t8, 
            int t9, int t10, int t11, int t12, int t13, int t14, int t15, int t16, int t17, int t18,
            int t19, int t20) :
                id(t1), identifier(t2), generation_id(t3), evolves_from_species_id(t4), 
                evolution_chain_id(t5), color_id(t6), shape_id(t7), habitat_id(t8), gender_rate(t9),
                capture_rate(t10), base_happiness(t11), is_baby(t12), hatch_counter(t13), 
                has_gender_differences(t14), growth_rate_id(t15), forms_switchable(t16),
                is_legendary(t17), is_mythical(t18), order(t19), conquest_order(t20) 
        {}
        Pokemon_species_data(){}
        ~Pokemon_species_data(){}
};

class Experience_data{
    public:
        int growth_rate_id, level, experience;
        Experience_data(int t1, int t2, int t3) : growth_rate_id(t1), level(t2), experience(t3) 
        {}
        Experience_data(){}
        ~Experience_data(){}
};

class Type_Names_data{
    public:
        int type_id, local_language_id;
        std::string name;
        Type_Names_data(int t1, int t2, std::string t3) : type_id(t1), local_language_id(t2), name(t3)
        {}
        Type_Names_data(){}
        ~Type_Names_data(){}
};

class Pokemon_Stats_data{
    public:
        int pokemon_id, stat_id, base_stat, effort;
        Pokemon_Stats_data(int t1, int t2, int t3, int t4) : pokemon_id(t1), stat_id(t2), base_stat(t3),
            effort(t4)
        {}
        Pokemon_Stats_data(){}
        ~Pokemon_Stats_data(){}
};

class Stats_data{
    public:
        int id, damage_class_id;
        std::string identifier;
        int is_battle_only, game_index;
        Stats_data(int t1, int t2, std::string t3, int t4, int t5) : id(t1), damage_class_id(t2), identifier(t3),
            is_battle_only(t4), game_index(t5)
        {}
        Stats_data(){}
        ~Stats_data(){}
};

class Pokemon_Types_data{
    public:
        int pokemon_id, type_id, slot;
        Pokemon_Types_data(int t1, int t2, int t3) : pokemon_id(t1), type_id(t2), slot(t3)
        {}
        Pokemon_Types_data(){}
        ~Pokemon_Types_data(){}
};

class Data{
    private:
        std::string files[9] = {  "pokemon",
                            "moves",
                            "pokemon_moves",
                            "pokemon_species",
                            "experience",
                            "type_names",
                            "pokemon_stats",
                            "stats",
                            "pokemon_types"
        };
        int read_file(std::string file);
    public:
        Pokemon_data         * arr_pkm[1093];
        Moves_data           * arr_mvs[845];
        Pokemon_Moves_data   * arr_pkm_mvs[528239];
        Pokemon_species_data * arr_pkm_spcs[899];
        Experience_data      * arr_xprnc[601];
        Type_Names_data      * arr_tp_nms[1676];
        Pokemon_Stats_data   * arr_pkm_stts[6553];
        Stats_data           * arr_stts[9];
        Pokemon_Types_data   * arr_pkm_tps[1676];

        int read_files();
};
#endif

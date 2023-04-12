#ifndef DATA_CLASSES_H
#define DATA_CLASSES_H

#include <string>

class Pokemon{
    public:
        int id;
        std::string identifier;
        int species_id, height, weight, base_experience, order, is_default;
        Pokemon(int t1, std::string t2, int t3, int t4, int t5, int t6, int t7, int t8) : id(t1), identifier(t2), species_id(t3), height(t4), weight(t5), base_experience(t6), order(t7), is_default(t8) {}
};

class Moves{
    public:
        int id;
        std::string identifier;
        int generation_id, type_id, power, pp, accuracy, priority, target_id, damage_class_id, effect_id, effect_chance, contest_type_id, contest_effect_id, super_constest_effect_id;
        Moves(int t1, std::string t2, int t3, int t4, int t5, int t6, int t7, int t8, int t9, 
            int t10, int t11, int t12, int t13, int t14, int t15) :
                id(t1), identifier(t2), generation_id(t3), type_id(t4), power(t5), pp(t6),
                accuracy(t7), priority(t8), target_id(t9), damage_class_id(t10),
                effect_id(t11), effect_chance(t12), contest_type_id(t13), contest_effect_id(t14),
                super_constest_effect_id(t15)
        {}
};

class Pokemon_Moves{
    public:
        int pokemon_id, version_group_id, move_id, pokemon_move_method_id, level, order;
        Pokemon_Moves(int t1, int t2, int t3, int t4, int t5, int t6) : pokemon_id(t1), 
                version_group_id(t2), move_id(t3), pokemon_move_method_id(t4), level(t5),
                order(t6)
        {}
};
                            
class Pokemon_species{
    public:
        int id;
        std::string identifier;
        int generation_id, evolves_from_species_id, evolution_chain_id, color_id, shape_id, habitat_id, gender_rate, capture_rate, base_happiness, is_baby, hatch_counter, has_gender_differences, growth_rate_id, forms_switchable, is_legendary, is_mythical, order, conquest_order;
        Pokemon_species(int t1, std::string t2, int t3, int t4, int t5, int t6, int t7, int t8, 
            int t9, int t10, int t11, int t12, int t13, int t14, int t15, int t16, int t17, int t18,
            int t19, int t20) :
                id(t1), identifier(t2), generation_id(t3), evolves_from_species_id(t4), 
                evolution_chain_id(t5), color_id(t6), shape_id(t7), habitat_id(t8), gender_rate(t9),
                capture_rate(t10), base_happiness(t11), is_baby(t12), hatch_counter(t13), 
                has_gender_differences(t14), growth_rate_id(t15), forms_switchable(t16),
                is_legendary(t17), is_mythical(t18), order(t19), conquest_order(t20) 
        {}
};

class Experience{
    public:
        int growth_rate_id, level, experience;
        Experience(int t1, int t2, int t3) : growth_rate_id(t1), level(t2), experience(t3) 
        {}
};

class Type_Names{
    public:
        int type_id, local_language_id;
        std::string name;
        Type_Names(int t1, int t2, std::string t3) : type_id(t1), local_language_id(t2), name(t3)
        {}
};

class Pokemon_Stats{
    public:
        int pokemon_id, stat_id, base_stat, effort;
        Pokemon_Stats(int t1, int t2, int t3, int t4) : pokemon_id(t1), stat_id(t2), base_stat(t3),
            effort(t4)
        {}
};

class Stats{
    public:
        int id, damage_class_id;
        std::string identifier;
        int is_battle_only, game_index;
        Stats(int t1, int t2, std::string t3, int t4, int t5) : id(t1), damage_class_id(t2), identifier(t3),
            is_battle_only(t4), game_index(t5)
        {}
};

class Pokemon_Types{
    public:
        int pokemon_id, type_id, slot;
        Pokemon_Types(int t1, int t2, int t3) : pokemon_id(t1), type_id(t2), slot(t3)
        {}
};
#endif

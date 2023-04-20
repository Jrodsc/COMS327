#include <stdlib.h>
#include <stdio.h>
#include "tlib.h"

//Comparators


bool cmp_pkm_tps_ids(Pokemon_Types_data * t1, int val){
    return t1 -> pokemon_id < val;
}

//Basic functions

std::string PKM_USE(std::string s, std::string t){
    return s + " used " + t;
}

//Color functions

void red(){
    printf("\033[0;31m");
}

void black(){
    printf("\033[0;30m");
}

void green(){
    printf("\033[0;32m");
}

void yellow(){
    printf("\033[0;33m");
}

void blue(){
    printf("\033[0;34m");
}

void purple(){
    printf("\033[0;35m");
}

void cyan(){
    printf("\033[0;36m");
}

void white(){
    printf("\033[0;37m");
}

void reset(){
    printf("\033[0m");
}

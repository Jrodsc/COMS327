#include <stdio.h>

char * words[26] = {
    "apple",
    "boring",
    "cravat",
    "dinosaur",
    "exentric",
    "fornicate",
    "grasshopper",
    "health",
    "iguana",
    "juxtapose",
    "khaki",
    "limosine",
    "microwave",
    "nuance",
    "ovoid",
    "pterodyctal",
    "quizical",
    "rambunctious",
    "supercalifragilisticexpialidocious",
    "trex",
    "underwhelming",
    "violent",
    "wherewithall",
    "xylophone",
    "y",
    "zoological"
};

int main(int argc, char * argv[]){

    if(argc != 2){
        fprintf(stderr, "Usage: %s <character>\n", argv[0]);

        return 1;
    }

    printf("%c is for %s\n", argv[1][0], words[argv[1][0] - 'a']); 

    return 0;
}

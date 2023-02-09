#include <stdio.h>
#include <stdlib.h>

int main(int arg, char * argv[]){

    char c;
    int x,y;

    printf("Enter a command: ");
    scanf("%c", &c);

    switch(c){
        case 'n':
            printf("Going north\n");
            break;
        case 's':
            printf("Going south\n");
            break;
        case 'e':
            printf("Going east\n");
            break;
        case 'w':
            printf("Going west\n");
            break;
        case 'f':
            scanf("%d %d", &x,&y);
            printf("Flying to <%d,%d>\n", x,y);
            break;
        case 'q':
            exit(0);
            break;
        default:
           printf("Error, invalid input\n");
    }

    return 0;
}

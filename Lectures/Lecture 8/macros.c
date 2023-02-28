#define FOO 10

#define bar(x) (x * 2)

#define min(x,y) (x < y ? x : y);

#define max(x, y) ({ \
        typeof (x) _x = x;      \
        typeof (y) _y = y;      \
        (_x > _y) ? _x : _y;    \
        })


int main(int argc, char * argv[]){
    FOO;

    bar(7);
    
    return 0;
}

#define FOO 10

#define bar(x) (x * 2)

#define min(x,y) (x < y ? x : y);

#define max(x, y) ({            \
        typeof (x) _x = x;      \
        typeof (y) _y = y;      \
        (_x > _y ? _x : _y);    \
})

#define stringify(x) #x

typedef struct {
    const char * name;
    int (*func)(int);
} lutable_entry;

#define entry(x) {#x, x}

lutable_entry table[] {
    entry(foo),
    entry(bar),
    entry(a),
    entry(b),
    entry(c),
    entry(d),
    entry(e),
    entry(f),
    entry(g),
};

int compare(const char **key, lutable_entry *l){
    return strcmp(*key,l -> name);
}

#define sconcat(x) x ##_suffix
#define pconcat(x) x prefix_##
#define terrain(x) ter_## x


int main(int argc, char * argv[]){
    FOO;

    bar(7);

    sconcat(foo);
    pconcat(foo);

    typedef enum{
        ter_water,
        ter_gate,
        ter_mountain,
        ter_path,
        ...
    }terrain_t;

    terrain(gate);
    
    return 0;
}

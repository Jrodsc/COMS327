#include <iostream>
#include <ncurses.h>
#include <vector>
#include <stack>
#include <unistd.h>
#include <time.h>

#define COLOR_BROWN 9
 
using namespace std;
 
stack<int> twr[3];
vector<pair<int,int> > ans;
bool flag;
int moves, save_IQ;
int n, n_save, N;
int pos_arrow[3] = {13, 39, 65};
int best_score[11];
int row_arrow, time_ = 250000, add_time = 125000;
 
WINDOW * st_w[3];

typedef struct DISK{
    int color, size;
} disk_t;

stack<disk_t *> stick_s[3]; 

void hanoi(int ant){
    if((int)twr[2].size() == N) {
        flag = true;
        return;
    }
    for(int i = 0; i < 3 && !flag; i++){
        if(i == ant) continue;
        if(twr[i].empty()) continue;
        for(int j = 1; j < 3 && !flag; j++){
            int t = (i + j + j * (N & 1))%3;
            if(twr[t].empty() || twr[i].top() < twr[t].top()){
                twr[t].push(twr[i].top());
                twr[i].pop();
                ans.push_back(make_pair(i+1,t+1));
                hanoi(t);
            }
        }
    }
}


void wprintcenter(WINDOW * win, int row, string text, short color){
    int adjusted_col = (win -> _maxx + 1)/2 - (int)text.size()/2;

    wattron(win, COLOR_PAIR(color));
    mvwprintw(win, row, adjusted_col, text.c_str());
    wattroff(win, COLOR_PAIR(color));
}

void print_stick(WINDOW * win, int disks, int length){
    for(int i = win -> _maxy - disks - 1; i >= win -> _maxy - length; i--){
        wprintcenter(win, i, " ", 2);
    }
}

int push_disk(int stick, disk_t * disk){
    if(!stick_s[stick].empty() && stick_s[stick].top() -> size < disk -> size)
        return 1;
    stick_s[stick].push(disk);
    wprintcenter(st_w[stick], st_w[stick] -> _maxy - stick_s[stick].size(), 
            string((disk -> size << 1) + 1, ' '), disk -> color);
    //wrefresh(st_w[stick]);
    
    refresh();
    return 0;
}

disk_t * pop_disk(int stick){
    if(stick_s[stick].empty())
        return NULL;

    disk_t * tmp = stick_s[stick].top();

    wprintcenter(st_w[stick], st_w[stick] -> _maxy - stick_s[stick].size(),
            string((tmp -> size << 1) + 1, ' '), 1);

    stick_s[stick].pop();
    print_stick(st_w[stick], stick_s[stick].size(), stick_s[stick].size() + 1);
    //wrefresh(st_w[stick]);

    refresh();
    return tmp;
}

disk_t * create_disk(int len, int color) {
    disk_t * d = (disk_t *)malloc(sizeof(disk_t*));

    d -> size = len;
    d -> color = color;

    return d;
}

void sequence(int i, int j){
    wchar_t c;

    disk_t * tmp = pop_disk(i);
    string disk_text((tmp -> size << 1) + 1, ' ');
    attron(COLOR_PAIR(tmp -> color));
    mvprintw(row_arrow + 1, pos_arrow[i] - tmp -> size, "%s", disk_text.c_str());
    attroff(COLOR_PAIR(tmp -> color));
    refresh();
    mvprintw(22, 7, "%.2f", (double)250000/time_);
    
    c = getch();
    add_time = (double)time_ * 0.1;
    if(c == KEY_RIGHT){
        time_ -= add_time;
    }
    else if(c == KEY_LEFT){
        time_ += add_time;
    }
    flushinp();
    usleep(time_);

    mvprintw(row_arrow + 1, pos_arrow[i] - tmp -> size, "%s", disk_text.c_str());
    attron(COLOR_PAIR(tmp -> color));
    mvprintw(row_arrow + 1, pos_arrow[j] - tmp -> size, "%s", disk_text.c_str());
    attroff(COLOR_PAIR(tmp -> color));
    refresh();
    mvprintw(22, 7, "%.2f", (double)250000/time_);

    c = getch();
    add_time = (double)time_ * 0.1;
    if(c == KEY_RIGHT){
        time_ -= add_time;
    }
    else if(c == KEY_LEFT){
        time_ += add_time;
    }
    flushinp();
    usleep(time_);

    mvprintw(row_arrow + 1, pos_arrow[j] - tmp -> size, "%s", disk_text.c_str());
    disk_text.clear();
    moves++;
    mvprintw(21, 7, "%d", moves);
    mvprintw(22, 7, "%.2f", (double)250000/time_);
    push_disk(j, tmp);
    refresh();
    c = getch();
    add_time = (double)time_ * 0.1;
    if(c == KEY_RIGHT){
        time_ -= add_time;
    }
    else if(c == KEY_LEFT){
        time_ += add_time;
    }
    flushinp();
    usleep(time_);
}

void demonstration(int n){
    clear();
    nodelay(stdscr, true);
    time_ = 250000;
    row_arrow = 14 - n - 2;
    flag = false;
    N = n;
    ans.clear();
    for(int i = n; i > 0; i--) twr[0].push(i);
    hanoi(-1);
    
    for(int i = 0; i < 3; i++){
        while(!twr[i].empty())
            twr[i].pop();
    }
    moves = 0;

    mvprintw(21, 0, "Moves:");
    mvprintw(22, 0, "Speed:");
    print_stick(st_w[0], 0, n + 2);
    print_stick(st_w[1], 0, n + 2);
    print_stick(st_w[2], 0, n + 2);
    wprintcenter(stdscr, 18, string(80, ' '), 8);
 
    for(int i = 1; i <= n; i++){ 
        push_disk(0, create_disk(n + 1 - i, 3 + (i % 5)));
        usleep(100000);
    }

    for(auto x : ans){
        sequence(x.first - 1, x.second - 1);
    }
    nodelay(stdscr, false);

    getch();

    while(stick_s[0].size() || stick_s[1].size() || stick_s[2].size()){
        pop_disk(0);
        pop_disk(1);
        pop_disk(2);
    }
}

void game_loop(int n){
    int sel = 0, from, IQ, tsel;
    wchar_t c;
    bool holding = false;
    string disk_text; 
    disk_t * tmp;

    row_arrow = 14 - n - 2;
    clear();

    n_save = n;
    print_stick(st_w[0], 0, n + 2);
    print_stick(st_w[1], 0, n + 2);
    print_stick(st_w[2], 0, n + 2);
    wprintcenter(stdscr, 18, string(80, ' '), 8);

    if(moves == -1){
        IQ = 90;
        save_IQ = 90;
        moves = 0;
        for(int i = 0; i < 3; i++){
            while(!stick_s[i].empty()){
                stick_s[i].pop();
            }
        }

        for(int i = 1; i <= n; i++){ 
            push_disk(0, create_disk(n + 1 - i, 3 + (i % 5)));
            usleep(100000);
        }
    }else{
        IQ = save_IQ;
        stack<disk_t *> tmp_stack;
        for(int i = 0; i < 3; i++){
            while(!stick_s[i].empty()){
                tmp_stack.push(pop_disk(i));
            }

            while(!tmp_stack.empty()){
                push_disk(i, tmp_stack.top());
                usleep(100000);
                tmp_stack.pop();
            }
        }
    }




    mvprintw(21, 0, "Moves:");
    do{ 
        
        mvprintw(21, 65, "IQ: %d", IQ);
        mvaddch(row_arrow, pos_arrow[sel], 'V');
        mvaddch(row_arrow, pos_arrow[(sel + 1) % 3], ' ');
        mvaddch(row_arrow, pos_arrow[(sel + 2) % 3], ' ');
        mvprintw(21, 7, "%d", moves);

        c = getch();

        if(c == KEY_LEFT){
            if(holding)
                mvprintw(row_arrow + 1, pos_arrow[sel] - tmp -> size, "%s", disk_text.c_str());
            sel = max(0, sel - 1);
            if(holding){
                attron(COLOR_PAIR(tmp -> color));
                mvprintw(row_arrow + 1, pos_arrow[sel] - tmp -> size, "%s", disk_text.c_str());
                attroff(COLOR_PAIR(tmp -> color));
            }
        }else if(c == KEY_RIGHT){
            if(holding)
                mvprintw(row_arrow + 1, pos_arrow[sel] - tmp -> size, "%s", disk_text.c_str());
            sel = min(sel + 1, 2);
            if(holding){
                attron(COLOR_PAIR(tmp -> color));
                mvprintw(row_arrow + 1, pos_arrow[sel] - tmp -> size, "%s", disk_text.c_str());
                attroff(COLOR_PAIR(tmp -> color));
            }
        }else if(c == ' '){
            if(!holding){
                if((tmp = pop_disk(sel))){
                    holding = true;
                    from = sel;
                    disk_text.assign((tmp -> size << 1) + 1, ' ');
                    attron(COLOR_PAIR(tmp -> color));
                    mvprintw(row_arrow + 1, pos_arrow[sel] - tmp -> size, "%s", disk_text.c_str());
                    attroff(COLOR_PAIR(tmp -> color));
                }
            }else{
                if(!push_disk(sel, tmp)){
                    holding = false;
                    mvprintw(row_arrow + 1, pos_arrow[sel] - tmp -> size, "%s", disk_text.c_str());
                    tmp = NULL;
                    disk_text.clear();
                    if(sel != from){
                        moves++;
                        if(n & 1){
                            tsel = (sel + 2)%3;
                            if(tsel == from)
                                IQ += (double)IQ*0.1;
                            else{
                                IQ -= (double)IQ*0.1;
                            }
                        }else{
                            tsel = (sel + 1)%3;
                            if(tsel == from)
                                IQ += (double)IQ*0.1;
                            else{
                                IQ -= (double)IQ*0.1;
                            }
                        }
                    }
                    mvprintw(21, 7, "%d", moves);
                    refresh();
                }
            }
        }

        if(stick_s[2].size() == n){
            wprintcenter(stdscr, 0, "You finished! Press enter to go back to the main menu", 10);
            if(moves < best_score[n]){
                best_score[n] = moves;
                wprintcenter(stdscr, 1, "New record!", 10);
            }
            mvprintw(22, 0, "Best score: %d", best_score[n]);
            mvprintw(23, 0, "Optimal solution: %d", (1 << n) - 1);
            moves = -1;
            getch();
            while(!stick_s[2].empty()){
                stick_s[2].pop();
            }
            break;
        }
        
    }while(c != 27);

    
    stack<disk_t *> st_tmp[3];
    while(stick_s[0].size() || stick_s[1].size() || stick_s[2].size()){
        if(stick_s[0].size()) st_tmp[0].push(pop_disk(0));
        if(stick_s[1].size()) st_tmp[1].push(pop_disk(1));
        if(stick_s[2].size()) st_tmp[2].push(pop_disk(2));
        usleep(125000);
    }
    while(st_tmp[0].size() || st_tmp[1].size() || st_tmp[2].size()){
        if(st_tmp[0].size()){
            stick_s[0].push(st_tmp[0].top());
            st_tmp[0].pop();
        }
        if(st_tmp[1].size()){
            stick_s[1].push(st_tmp[1].top());
            st_tmp[1].pop();
        }
        if(st_tmp[2].size()){
            stick_s[2].push(st_tmp[2].top());
            st_tmp[2].pop();
        }
    }
}

void print_title(){
    mvprintw(0,3,".-. .-.  .--.  .-. .-. .---.  ,-.  _______  .---.  .-.  .-.,---.  ,---.");    
    mvprintw(1,3,"| | | | / /\\ \\ |  \\| |/ .-. ) |(| |__   __|/ .-. ) | |/\\| || .-'  | .-.\\");   
    mvprintw(2,3,"| `-' |/ /__\\ \\|   | || | |(_)(_)   )| |   | | |(_)| /  \\ || `-.  | `-'/");
    mvprintw(3,3,"| .-. ||  __  || |\\  || | | | | |  (_) |   | | | | |  /\\  || .-'  |   (");
    mvprintw(4,3,"| | |)|| |  |)|| | |)|\\ `-' / | |    | |   \\ `-' / |(/  \\ ||  `--.| |\\ \\");   
    mvprintw(5,3,"/(  (_)|_|  (_)/(  (_) )---'  `-'    `-'    )---'  (_)   \\|/( __.'|_| \\)\\");
    mvprintw(6,3,"(__)           (__)    (_)                  (_)            (__)        (__)");
    refresh();
}
 
int main(){
    moves = -1;

    initscr();

    curs_set(0);
    keypad(stdscr, TRUE);
    ESCDELAY = 1;
    noecho();
    //resizeterm(24, 80);

    if(has_colors()){
        start_color();

        init_pair(1, COLOR_BLACK, COLOR_BLACK);
        init_pair(2, COLOR_WHITE, COLOR_WHITE);
        init_pair(3, COLOR_RED, COLOR_RED);
        init_pair(4, COLOR_BLUE, COLOR_BLUE);
        init_pair(5, COLOR_GREEN, COLOR_GREEN);
        init_pair(6, COLOR_MAGENTA, COLOR_MAGENTA);
        init_pair(7, COLOR_CYAN, COLOR_CYAN);
        init_pair(8, COLOR_YELLOW, COLOR_YELLOW);
        init_pair(9, COLOR_BLACK, COLOR_BLUE);
        init_pair(10, COLOR_WHITE, COLOR_BLACK);
    }

    st_w[0] = subwin(stdscr, 17, 25, 2, 1);
    st_w[1] = subwin(stdscr, 17, 25, 2, 27);
    st_w[2] = subwin(stdscr, 17, 25, 2, 53);

    syncok(st_w[0], TRUE);
    syncok(st_w[1], TRUE);
    syncok(st_w[2], TRUE);

    wchar_t c,n_c;
    
    for(int i = 0; i < 11; i++){
        best_score[i] = 1000000000;
    }
                                                                            
                                                                            
    string text[4] = {"New game", "Continue game", "Demo", "Credits"};

    int sel = 0, n_sel;

    print_title();

    do{
        for(int i = 0; i < 4; i++){
            wprintcenter(stdscr, i + 10, text[i], 10 - (sel == i));
        }

        c = getch();

        if(c == KEY_DOWN){
            sel = min(sel + 1, 3);
            if(sel == 1 && moves == -1) sel++;
        }else if(c == KEY_UP){
            sel = max(0, sel - 1);
            if(sel == 1 && moves == -1) sel = 0;
        }else if(c == '\n'){
            switch(sel){
                case 0:
                    moves = -1;
                case 2:
                    n_sel = 3;
                    do{
                        for(int i = 3; i <= 10; i++){
                            if(n_sel == i)
                                attron(COLOR_PAIR(9));
                            mvprintw(7 + i, 60, "%d", i);
                            if(n_sel == i)
                                attroff(COLOR_PAIR(9));
                        }
                        
                        n_c = getch();

                        if(n_c == KEY_UP){
                            n_sel = max(0, n_sel - 1);
                        }else if(n_c == KEY_DOWN){
                            n_sel = min(10, n_sel + 1);
                        }else if(n_c == '\n'){
                            if(sel == 0) 
                                game_loop(n_sel);
                            else{ 
                                demonstration(n_sel);
                                moves = -1;
                            }
                            clear();
                            print_title();
                            break;
                        }else if(n_c == 27){
                            for(int i = 3; i <= 10; i++){
                                mvprintw(7 + i, 60, "  ");
                            }
                            refresh();
                        }
                    }while(n_c != 27);
                    break;
                case 1:
                    game_loop(n_save);
                    clear();
                    print_title();
                    break;
                case 3:
                    break;
            }
        }

    }while(c != 27);

    endwin();
    
    return 0;
}

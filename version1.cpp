#include <iostream>
#include <ncurses.h>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;

#define MAIN_COLOR      1
#define REV_COLOR       2
#define BOTTOM_COLOR    3

void position(WINDOW* w, int collected, int left){
    mvwprintw(w,1,3,"collected:%2d \n   left:%2d",collected,left);
}

void timerpos(WINDOW* w, int seconds){
    mvwprintw(w,1,3,"Time left:%d",seconds);
}

vector<pair<int,int>> generate_star(int numberOfLines, int numberOfColumns) {
    vector<pair<int,int>> coordinates;
    srand(time(0)); // seed random number generator
    while(coordinates.size() < 7) {
        int x = rand() % (COLS - 2) + 1; // generate random x coordinate in range [1, numberOfColumns - 1)
        int y = rand() % (LINES - 4) + 1; // generate random y coordinate in range [1, numberOfLines - 3)
        pair<int,int> coord = make_pair(x,y);
        if(find(coordinates.begin(), coordinates.end(), coord) == coordinates.end()) {
            coordinates.push_back(coord);
        }
    }
    return coordinates;
}

int main()
{
    WINDOW* mainwin,*stat,*timer;
    string C = "*", C_wh;
    int x =20 , y =10 ;
    int collected=0;
    int left=10;
    int seconds=30;

    int key;

    for(int i=0; i < C.size(); i++) C_wh += " ";

    mainwin = initscr();    // initliazzation of screen
    stat=subwin(mainwin,3,20,LINES-4,1);
    timer=subwin(mainwin,3,15,LINES-3,COLS-17);
    curs_set(0);            // do not show the cursor
    noecho();               // do not print anything after key pressed
    keypad(mainwin, true);


    start_color();
    init_pair(MAIN_COLOR, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(REV_COLOR,COLOR_YELLOW,COLOR_BLACK);
    init_pair(BOTTOM_COLOR,COLOR_RED,COLOR_BLACK);

    wattron(mainwin,COLOR_PAIR(MAIN_COLOR));
    mvwprintw(mainwin,y,x,"%s",C.c_str());
    wattron(stat,COLOR_PAIR(BOTTOM_COLOR));
    wattron(timer,COLOR_PAIR(BOTTOM_COLOR));

    box(mainwin,0,0);       // draw box around the window

    wrefresh(stat);
    wrefresh(timer);
    wattron(mainwin,COLOR_PAIR(BOTTOM_COLOR));
   // wattroff(mainwin,COLOR_PAIR(BOTTOM_COLOR));
   // wattron(mainwin,COLOR_PAIR(MAIN_COLOR));

        wattron(mainwin,COLOR_PAIR(BOTTOM_COLOR));
    vector<pair<int,int>> star_coords = generate_star(LINES,COLS);
    for(const auto& coord : star_coords) {
        mvwprintw(mainwin, coord.second, coord.first, "%s", C.c_str());
    }
    wattroff(mainwin,COLOR_PAIR(BOTTOM_COLOR));
    wattron(mainwin,COLOR_PAIR(MAIN_COLOR));


    position(stat,collected,left);
    timerpos(timer,seconds);
    wrefresh(stat);
    wrefresh(timer);
    wrefresh(mainwin);

    while ( 1 ) // MAIN LOOP
    {
        key = wgetch(mainwin);  // wait for key pressed: BLOCKING
        wattron(mainwin,COLOR_PAIR(REV_COLOR));
        mvwprintw(mainwin,y,x,"%s",C_wh.c_str());

        if (key == 'a' || key==KEY_LEFT) // LEFT
        {
            x-=1;
        }
        else if (key == 'd' || key==KEY_RIGHT) // RIGHT
        {
            x+=1;
                }
                else if (key == 'w' || key==KEY_UP) // UP
                {
                        y-=1;
                }
                else if (key=='s' || key==KEY_DOWN) // DOWN
                {
                        y+=1;
                }

                else if (key == 'q')
                {
                        break;
                }

                if(y==0){y=y+4;}
                else if(y==LINES-3){y-=4;}
                if(x==0){x+=4;}
                else if(x==COLS-1){x-=4;}




                wattron(mainwin,COLOR_PAIR(MAIN_COLOR));
                mvwprintw(mainwin,y,x,"%s",C.c_str());
                position(stat,collected,left);
                timerpos(timer,seconds);
                wrefresh(mainwin);      // send from MEMORY -> SCREEN
                wrefresh(stat);
                wrefresh(timer);
        }

        delwin(mainwin);        // remove from MEMORY - WINDOW data structure
        endwin();               // clean up

        return EXIT_SUCCESS;
}

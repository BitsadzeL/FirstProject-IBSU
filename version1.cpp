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
#define OBSCATLE_COLOR  4

void position(WINDOW* w, int collected, int hearts){
    mvwprintw(w,1,1,"Collected:%2d\n Hearts left:%2d",collected,hearts);
}

void timerpos(WINDOW* w, int movements){
    mvwprintw(w,1,2,"Movements:%d",movements);
}


//spawn asterisks on random positions
vector<pair<int,int>> generate_asterisk(int numberOfLines, int numberOfColumns) {
    vector<pair<int,int>> coordinates;
    srand(time(0)); 
    while(coordinates.size() < 500) {
        int x = (rand() % (COLS - 3)) + 1;
        int y = (rand() % (LINES - 5)) +1;

        // Check if x and y match the given values, and generate new random values until they don't match
        while((y >= 1 && y <= 7 && (x >= 5 && x <= 9)) ||
               (y >= 8 && y <= 17 && (x >= 19 && x <= 23)) ||
               (y >= 17 && y <= 18 && (x >= 49 && x <= 53)) ||
               (y >= 25 && y <= 30 && (x >= 69 && x <= 73)) ||
               (y >= 10 && y <= 21 && (x >= 89 && x <= 93)) ||
               (y >= 8 && y <= 27 && (x >= 109 && x <= 114)) ||
               (y >= 4 && y <= 25 && (x >= 129 && x <= 134)) ||
               (y >= 18 && y <= 34 && (x >= 149 && x <= 154))) {
            x = (rand() % (COLS - 3)) + 1;
            y = (rand() % (LINES - 5)) + 1;
        }

        pair<int,int> coord = make_pair(x,y);
        if(find(coordinates.begin(), coordinates.end(), coord) == coordinates.end()) {
            coordinates.push_back(coord);
        }
    }
    return coordinates;
}



//after collecting, spawn a new one
void spawnAsterisk(WINDOW* win, int x_max, int y_max, std::vector<std::pair<int, int>>& asterisk_coords)
{
    // Generate new coordinates for the asterisk
        int new_x = (rand() % (COLS - 3)) + 1;
        int new_y = (rand() % (LINES - 5)) +1;

        while((new_y >= 1 && new_y <= 7 && (new_x >= 5 && new_x <= 9)) ||
               (new_y >= 8 && new_y <= 17 && (new_x >= 19 && new_x <= 23)) ||
               (new_y >= 17 && new_y <= 18 && (new_x >= 49 && new_x <= 53)) ||
               (new_y >= 25 && new_y <= 30 && (new_x >= 69 && new_x <= 73)) ||
               (new_y >= 10 && new_y <= 21 && (new_x >= 89 && new_x <= 93)) ||
               (new_y >= 8 && new_y <= 27 && (new_x >= 109 && new_x <= 114)) ||
               (new_y >= 4 && new_y <= 25 && (new_x >= 129 && new_x <= 134)) ||
               (new_y >= 18 && new_y <= 34 && (new_x >= 149 && new_x <= 154))) {
            new_x = (rand() % (COLS - 3)) + 1;
            new_y = (rand() % (LINES - 5)) + 1;
        }
    // Check if the new coordinates conflict with existing asterisks
    bool conflict = false;
    for (const auto& coord : asterisk_coords)
    {
        if (coord.first == new_x && coord.second == new_y)
        {
            conflict = true;
            break;
        }
 
         
    }

    // If there's a conflict, generate new coordinates until there's no conflict
    while (conflict)
    {
        new_x = rand() % x_max;
        new_y = rand() % y_max;

        conflict = false;
        for (const auto& coord : asterisk_coords)
        {
            if (coord.first == new_x && coord.second == new_y)
            {
                conflict = true;
                break;
            }
        }
    }

    asterisk_coords.push_back(std::make_pair(new_x, new_y));

    mvwprintw(win, new_y, new_x, "*");
    wrefresh(win);
}


int main()
{
	WINDOW* mainwin,*stat,*timer,*obscatle,*obscatle2,*obscatle3,*obscatle4,*obscatle5,*obscatle6,*obscatle7,*obscatle8;
	string C = "*", C_wh;
	int x =18 , y =10 ;
	int collected=0;
    int hearts=18;
    int movements=0;
    int key;




    for(int i=0; i < C.size(); i++) C_wh += " ";

    mainwin = initscr();    
    
    stat=subwin(mainwin,3,20,LINES-4,1);
    timer=subwin(mainwin,3,15,LINES-3,COLS-17);
    obscatle=subwin(mainwin,7,2,1,6);
    obscatle2=subwin(mainwin,10,2,8,20);
    obscatle3=subwin(mainwin,2,2,17,50);
    obscatle4=subwin(mainwin,6,2,25,70);
    obscatle5=subwin(mainwin,12,2,10,90);
    obscatle6=subwin(mainwin,20,2,8,110);
    obscatle7=subwin(mainwin,22,2,4,130);
    obscatle8=subwin(mainwin,17,2,18,150);
    
    curs_set(0);            
    noecho();               
    keypad(mainwin, true);


    start_color();
    init_pair(MAIN_COLOR, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(REV_COLOR,COLOR_YELLOW,COLOR_BLACK);
    init_pair(BOTTOM_COLOR,COLOR_RED,COLOR_BLACK);
    init_pair(OBSCATLE_COLOR,COLOR_BLUE,COLOR_BLUE);

    wattron(mainwin,COLOR_PAIR(MAIN_COLOR));
    mvwprintw(mainwin,y,x,"%s",C.c_str());
    box(mainwin,0,0);   
    wattroff(mainwin,COLOR_PAIR(MAIN_COLOR));

    wattron(obscatle,COLOR_PAIR(OBSCATLE_COLOR));
    wattron(obscatle2,COLOR_PAIR(OBSCATLE_COLOR));
    wattron(obscatle3,COLOR_PAIR(OBSCATLE_COLOR));
    wattron(obscatle4,COLOR_PAIR(OBSCATLE_COLOR));
    wattron(obscatle5,COLOR_PAIR(OBSCATLE_COLOR));
    wattron(obscatle6,COLOR_PAIR(OBSCATLE_COLOR));
    wattron(obscatle7,COLOR_PAIR(OBSCATLE_COLOR));
    wattron(obscatle8,COLOR_PAIR(OBSCATLE_COLOR));
    box(obscatle,0,0);
    box(obscatle2,0,0);
    box(obscatle3,0,0);
    box(obscatle4,0,0);
    box(obscatle5,0,0);
    box(obscatle6,0,0);
    box(obscatle7,0,0);
    box(obscatle8,0,0);    
    wattroff(obscatle,COLOR_PAIR(OBSCATLE_COLOR));
    wattroff(obscatle2,COLOR_PAIR(OBSCATLE_COLOR));
    wattroff(obscatle3,COLOR_PAIR(OBSCATLE_COLOR));
    wattroff(obscatle4,COLOR_PAIR(OBSCATLE_COLOR));
    wattroff(obscatle5,COLOR_PAIR(OBSCATLE_COLOR));
    wattroff(obscatle6,COLOR_PAIR(OBSCATLE_COLOR));
    wattroff(obscatle7,COLOR_PAIR(OBSCATLE_COLOR));
    wattroff(obscatle8,COLOR_PAIR(OBSCATLE_COLOR));    



    wattron(timer,COLOR_PAIR(BOTTOM_COLOR));
    wattron(stat,COLOR_PAIR(BOTTOM_COLOR));

    

    wrefresh(stat);
    wrefresh(timer);
    
    //spawn red asterisks
    wattron(mainwin,COLOR_PAIR(BOTTOM_COLOR));
    vector<pair<int,int>> asterisk_coords = generate_asterisk(LINES,COLS);
    for(const auto& coord : asterisk_coords) {
            mvwprintw(mainwin, coord.second, coord.first, "%s", C.c_str());
    }
    wattroff(mainwin,COLOR_PAIR(BOTTOM_COLOR));
    
    
    wattron(mainwin,COLOR_PAIR(MAIN_COLOR));
    position(stat,collected,hearts);
    timerpos(timer,movements);
    wrefresh(stat);
    wrefresh(timer);
    wrefresh(mainwin);


    while ( 1 ) // MAIN LOOP
    {
        key = wgetch(mainwin); 
        movements++; 
        wattron(mainwin,COLOR_PAIR(REV_COLOR));
        mvwprintw(mainwin,y,x,"%s",C_wh.c_str());

		//CHECK MOVEMENT
        if (key == 'a' || key==KEY_LEFT) {x-=1;}
        else if (key == 'd' || key==KEY_RIGHT) {x+=1;}       
        else if (key == 'w' || key==KEY_UP) {y-=1;}       
        else if (key=='s' || key==KEY_DOWN) {y+=1;}
        else if (key == 'q'){break;}

		
		// CHECK IF PLAYER HIT BORDERS
        if(y==0){y=y+4; hearts--;}
        else if(y==LINES-3){y-=4;hearts--;}
        else if(x==0){x+=4;hearts--;}
        else if(x==COLS-1){x-=4;hearts--;}

        
        else if(y>=1 && y<=7 && x==6){x-=2;hearts--;} //1-lis marcxena
        else if(y>=1 && y<=7 && x==7){x+=2;hearts--;} // 1-lis marjvena

        else if(y>=8 && y<=17 && x==20){x-=2;hearts--;}  // me2es marcxena
        else if(y>=8 && y<=17 && x==21){x+=2;hearts--;}  // me2es marjvena

        else if(y>=17 && y<=18 && x==50){x-=2; hearts--;}  // me3es marcxena
        else if(y>=17 && y<=18 && x==51){x+=2; hearts--;}  // me3es marjvena  

        else if(y>=25 && y<=30 && x==70){x-=2; hearts--;}  // me4es marcxena
        else if(y>=25 && y<=30 && x==71){x+=2; hearts--;}  // me4es marjvena 

        else if(y>=10 && y<=21 && x==90){x-=2; hearts--;}  // me5es marcxena
        else if(y>=10 && y<=21 && x==91){x+=2; hearts--;}  // me5es marjvena         

        else if(y>=8 && y<=27 && x==110){x-=2; hearts--;}  // me6es marcxena
        else if(y>=8 && y<=27 && x==111){x+=2; hearts--;}  // me6es marjvena

        /*
            obscatle7=subwin(mainwin,22,2,4,130);
            obscatle8=subwin(mainwin,17,2,18,150);
        */
        else if(y>=4 && y<=25 && x==130){x-=2; hearts--;}  // me7es marcxena
        else if(y>=4 && y<=25 && x==131){x+=2; hearts--;}  // me7es marjvena


        else if(y>=18 && y<=34 && x==150){x-=2; hearts--;}  // me7es marcxena
        else if(y>=18 && y<=34 && x==151){x+=2; hearts--;}  // me7es marjvena



        
        if(hearts==0 || movements==999){break;}      
 		
 		
 		// CHECK IF PLAYER COLLECTED ASTERISKS
		for (auto it = asterisk_coords.begin(); it != asterisk_coords.end();)
		{
        	if (x == it->first && y == it->second) 
			{
            	collected++;
            	mvwprintw(mainwin, it->second, it->first, " "); 
            	it = asterisk_coords.erase(it); 
                wattroff(mainwin,COLOR_PAIR(MAIN_COLOR));
                wattron(mainwin,COLOR_PAIR(BOTTOM_COLOR));
                spawnAsterisk(mainwin, COLS-1, LINES-3, asterisk_coords);
                wattroff(mainwin,COLOR_PAIR(BOTTOM_COLOR));
                wattron(mainwin,COLOR_PAIR(MAIN_COLOR));
            	position(stat, collected,hearts);
            	wrefresh(stat);
        	} 
			else {++it;}
    	}
    	
        wattron(mainwin,COLOR_PAIR(MAIN_COLOR));
        mvwprintw(mainwin,y,x,"%s",C.c_str());
        position(stat,collected,hearts);
        timerpos(timer,movements);
        wrefresh(mainwin);      
        wrefresh(stat);
        wrefresh(timer);
    }
        delwin(mainwin);

        WINDOW* newmainwin = newwin(LINES, COLS, 0, 0);
    // box(mainwin, 0, 0);
        wattron(newmainwin, COLOR_PAIR(MAIN_COLOR));
        wrefresh(newmainwin);

        // Create dialogue subwindow
        WINDOW* dialogue = subwin(newmainwin, 20, 40, 8, 50);
        box(dialogue, 0, 0);
        wattron(dialogue, COLOR_PAIR(MAIN_COLOR));
        wrefresh(dialogue);
        wattroff(dialogue,COLOR_PAIR(MAIN_COLOR));
        wrefresh(dialogue);

        // Print text in dialogue subwindow
        wattron(dialogue,COLOR_PAIR(BOTTOM_COLOR));
        wrefresh(dialogue);
        mvwprintw(dialogue, 8, 10, "GAME OVER!!!");
        mvwprintw(dialogue,9,10,"You collected %d points",collected);
        mvwprintw(dialogue, 10, 10, "press any key to exit");

        wattroff(dialogue,COLOR_PAIR(BOTTOM_COLOR));
        wrefresh(dialogue);
        wattroff(dialogue, COLOR_PAIR(MAIN_COLOR));
        wrefresh(dialogue);

        int a = wgetch(mainwin); // Wait for key pressed
        wrefresh(newmainwin);      // Refresh mainwin to clear the previous text
        wrefresh(dialogue);     // Refresh dialogue subwindow to show the updated text


        endwin();               
        return EXIT_SUCCESS;
}



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

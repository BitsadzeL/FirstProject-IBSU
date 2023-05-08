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
#define DIALOGUE_COLOR  5

void position(WINDOW* w, int collected, int hearts){
    mvwprintw(w,1,1,"Collected:%2d\n Hearts left:%2d",collected,hearts);
}

void timerpos(WINDOW* w, int movements){
    mvwprintw(w,1,2,"Movements:%d",movements);
}


//spawn asterisks on random positions at the start of the game
vector<pair<int,int>> generate_asterisk(int numberOfLines, int numberOfColumns) {
    vector<pair<int,int>> coordinates;
    srand(time(0)); 
    while(coordinates.size() < 500) {
        int x = (rand() % (COLS - 3)) + 1;
        int y = (rand() % (LINES - 5)) + 1;

        //Generate new random coordinates in case of being inside the obstacle
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



//after collecting, spawn a new asterisk on random position
void spawnAsterisk(WINDOW* win, int x_max, int y_max, vector<pair<int, int>>& asterisk_coords)
{

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

    bool conflict = false;
    for (const auto& coord : asterisk_coords)
    {
        if (coord.first == new_x && coord.second == new_y)
        {
            conflict = true;
            break;
        }
 
         
    }


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

    asterisk_coords.push_back(make_pair(new_x, new_y));

    mvwprintw(win, new_y, new_x, "*");
    wrefresh(win);
}

class Stats{
    private:
        int collected;
        int hearts;
        int movements;
    public:
        Stats(int c, int h, int m){
            collected=c;
            hearts=h;
            movements=m;
        }

	//Get info
        int getCollected(){return collected;}
        int getHearts(){return hearts;}
        int getMovements(){return movements;}

	//Actions
        void collect(){collected++;}
        void damage(){hearts--;}
        void move(){movements++;}
};


class Player{
    private:
        int y;
        int x;
    public:

        Player(int a, int b){y=a;x=b;}

	//Get coordinates
        int getY(){return y;}
        int getX(){return x;}

	//Directions
        void goUp(){y--;}
        void goDown(){y++;}
        void goRight(){x++;}
        void goLeft(){x--;}


	// Bounce when hitting obscatles
        void hitObscLeft(){x-=2;}
        void hitObscRight(){x+=2;}

	//Bounce when hitting borders
        void bounceDown(){y+=4;}
        void bounceUp(){y-=4;}
        void bounceRight(){x+=4;}
        void bounceLeft(){x-=4;}
};


class Game{
    private:
        WINDOW* mainwin,*stat,*timer,*obscatle,*obscatle2,*obscatle3,*obscatle4,*obscatle5,*obscatle6,*obscatle7,*obscatle8;
        
    public:
        void buildObstacles(){
            obscatle=subwin(mainwin,7,2,1,6);
    	    obscatle2=subwin(mainwin,10,2,8,20);
    	    obscatle3=subwin(mainwin,2,2,17,50);
    	    obscatle4=subwin(mainwin,6,2,25,70);
	        obscatle5=subwin(mainwin,12,2,10,90);
    	    obscatle6=subwin(mainwin,20,2,8,110);
    	    obscatle7=subwin(mainwin,22,2,4,130);
    	    obscatle8=subwin(mainwin,17,2,18,150);
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
        }


        void buildStats(){
            stat=subwin(mainwin,3,20,LINES-4,1);
        }

        void buildTimer(){
            timer=subwin(mainwin,3,15,LINES-3,COLS-17);
        }

        int checkKey(int a){
            int c=-1;
            if(a==KEY_UP || a==KEY_DOWN || a==KEY_RIGHT || a==KEY_LEFT || a=='a'|| a=='w' || a=='s' || a=='d'){
                c=1;
            }
            else{c=0;}
            return c;
        }

        void colors(){
            start_color();
            init_pair(MAIN_COLOR, COLOR_YELLOW, COLOR_YELLOW);
            init_pair(REV_COLOR,COLOR_YELLOW,COLOR_BLACK);
            init_pair(BOTTOM_COLOR,COLOR_RED,COLOR_BLACK);
            init_pair(OBSCATLE_COLOR,COLOR_BLUE,COLOR_BLUE);
            init_pair(DIALOGUE_COLOR,COLOR_BLACK,COLOR_BLACK);            
        }


        void gameplay(){
            int key;
            Stats s(0,3,0);
            Player p(10,18);
            string C = "*", C_wh;
            mainwin = initscr(); 
            colors();
            buildStats();
            buildTimer();
            buildObstacles();


            for(int i=0; i < C.size(); i++) C_wh += " ";
            keypad(mainwin, true);
            curs_set(0);
	    noecho();



            wattron(mainwin,COLOR_PAIR(MAIN_COLOR));
            mvwprintw(mainwin,p.getY(),p.getX(),"%s",C.c_str());
            box(mainwin,0,0);   
            wattroff(mainwin,COLOR_PAIR(MAIN_COLOR));
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
            position(stat,s.getCollected(),s.getHearts());
            timerpos(timer,s.getMovements());
            wrefresh(stat);
            wrefresh(timer);
            wrefresh(mainwin);       


            while ( 1 ) 
            {
                key = wgetch(mainwin); 

                if(checkKey(key)==1){s.move();} 
                wattron(mainwin,COLOR_PAIR(REV_COLOR));
                mvwprintw(mainwin,p.getY(),p.getX(),"%s",C_wh.c_str());

            //Check input 
                if (key==KEY_LEFT || key=='a') {p.goLeft();}
                else if (key==KEY_RIGHT || key=='d') {p.goRight();}       
                else if (key==KEY_UP || key=='w') {p.goUp();}       
                else if (key==KEY_DOWN || key=='s') {p.goDown();}
                else if (key == 'q'){break;}

            
            //Chek if player hit borders
                if(p.getY()==0){p.bounceDown();s.damage();}
                else if(p.getY()==LINES-3){p.bounceUp();s.damage();}
                else if(p.getX()==0){p.bounceRight();s.damage();}
                else if(p.getX()==COLS-1){p.bounceLeft();s.damage();}

            
                else if(p.getY()>=1 && p.getY()<=7 && p.getX()==6){p.hitObscLeft();s.damage();} 
                else if(p.getY()>=1 && p.getY()<=7 && p.getX()==7){p.hitObscRight();s.damage();}

                else if(p.getY()>=8 && p.getY()<=17 && p.getX()==20){p.hitObscLeft();s.damage();}
                else if(p.getY()>=8 && p.getY()<=17 && p.getX()==21){p.hitObscRight();s.damage();}

                else if(p.getY()>=17 && p.getY()<=18 && p.getX()==50){p.hitObscLeft(); s.damage();}
                else if(p.getY()>=17 && p.getY()<=18 && p.getX()==51){p.hitObscRight(); s.damage();}

                else if(p.getY()>=25 && p.getY()<=30 && p.getX()==70){p.hitObscLeft(); s.damage();} 
                else if(p.getY()>=25 && p.getY()<=30 && p.getX()==71){p.hitObscRight(); s.damage();} 

                else if(p.getY()>=10 && p.getY()<=21 && p.getX()==90){p.hitObscLeft(); s.damage();} 
                else if(p.getY()>=10 && p.getY()<=21 && p.getX()==91){p.hitObscRight(); s.damage();}  

                else if(p.getY()>=8 && p.getY()<=27 && p.getX()==110){p.hitObscLeft(); s.damage();}  
                else if(p.getY()>=8 && p.getY()<=27 && p.getX()==111){p.hitObscRight(); s.damage();} 

                else if(p.getY()>=4 && p.getY()<=25 && p.getX()==130){p.hitObscLeft(); s.damage();}  
                else if(p.getY()>=4 && p.getY()<=25 && p.getX()==131){p.hitObscRight(); s.damage();} 


                else if(p.getY()>=18 && p.getY()<=34 && p.getX()==150){p.hitObscLeft(); s.damage();}  
                else if(p.getY()>=18 && p.getY()<=34 && p.getX()==151){p.hitObscRight(); s.damage();}  



                //Game break condition
                if(s.getHearts()==0 || s.getMovements()==300){break;}       


                //Payer collected , so delete it and add a new one
                for (auto it = asterisk_coords.begin(); it != asterisk_coords.end();)
                {
                        if (p.getX() == it->first && p.getY() == it->second) 
                    {
                                s.collect();
                                mvwprintw(mainwin, it->second, it->first, " "); 
                                it = asterisk_coords.erase(it); 
                                wattroff(mainwin,COLOR_PAIR(MAIN_COLOR));
                                wattron(mainwin,COLOR_PAIR(BOTTOM_COLOR));
                                spawnAsterisk(mainwin, COLS-1, LINES-3, asterisk_coords);
                                wattroff(mainwin,COLOR_PAIR(BOTTOM_COLOR));
                                wattron(mainwin,COLOR_PAIR(MAIN_COLOR));
                                position(stat, s.getCollected(),s.getHearts());
                                wrefresh(stat);
                        } 
                    else {++it;}
                }
                
                wattron(mainwin,COLOR_PAIR(MAIN_COLOR));
                mvwprintw(mainwin,p.getY(),p.getX(),"%s",C.c_str());
                position(stat,s.getCollected(),s.getHearts());
                timerpos(timer,s.getMovements());
                wrefresh(mainwin);      
                wrefresh(stat);
                wrefresh(timer);
            }

            delwin(mainwin);

            
            // Create dialogue subwindow
            WINDOW* newmainwin = newwin(LINES, COLS, 0, 0);
            curs_set(0);
            wattron(newmainwin, COLOR_PAIR(MAIN_COLOR));
            wrefresh(newmainwin);            
            WINDOW* dialogue = subwin(newmainwin, 20, 40, 8, 50);
            curs_set(0);
            box(dialogue, 0, 0);
            wattron(dialogue, COLOR_PAIR(MAIN_COLOR));
            wrefresh(dialogue);
            wattroff(dialogue,COLOR_PAIR(MAIN_COLOR));
            wrefresh(dialogue);

            // Print text in dialogue subwindow
            wattron(dialogue,COLOR_PAIR(BOTTOM_COLOR));
            wrefresh(dialogue);
            mvwprintw(dialogue, 8, 10, "GAME OVER!!!");
            mvwprintw(dialogue,9,10,"You collected %d points",s.getCollected());
            mvwprintw(dialogue, 10, 10, "press 'o' to exit");

            wattroff(dialogue,COLOR_PAIR(BOTTOM_COLOR));
            wattroff(dialogue, COLOR_PAIR(MAIN_COLOR));
            wrefresh(dialogue);

            wattroff(newmainwin, COLOR_PAIR(MAIN_COLOR));
            wattron(newmainwin,COLOR_PAIR(DIALOGUE_COLOR));
            wrefresh(newmainwin);   

            while(1)
            {
                
                int a = wgetch(newmainwin); // Wait for key pressed
                if (a == 'o') {
                    delwin(newmainwin); 
                    break; 
                } else {
                    wrefresh(newmainwin); 
                    wrefresh(dialogue); 
                }
            }  
        };

        Game(){
            gameplay();
        }
        ~Game(){
            endwin();      
            delwin(mainwin);
            
        }    

};

int main()
{
    Game game;
    return EXIT_SUCCESS;
}

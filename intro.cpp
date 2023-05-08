#include <iostream>
#include <ncurses.h>
#include <cstdlib>

#define MAIN_COLOR 		1
#define FONT_COLOR      2

using namespace std;

int main()
{
        initscr();
        start_color();
        cbreak();
        noecho();
        keypad(stdscr, TRUE);
        curs_set(0);

        init_pair(MAIN_COLOR, COLOR_YELLOW, COLOR_GREEN);
        init_pair(FONT_COLOR,COLOR_RED,COLOR_BLACK);

        WINDOW* mainwin = newwin(LINES, COLS, 0, 0);
        wattron(mainwin, COLOR_PAIR(MAIN_COLOR));
        wrefresh(mainwin);

        // Create dialogue subwindow
        WINDOW* dialogue = subwin(mainwin, 30, 90, 8, 30);
        box(dialogue, 0, 0);
        wattron(dialogue, COLOR_PAIR(MAIN_COLOR));
        wrefresh(dialogue);
        wattroff(dialogue,COLOR_PAIR(MAIN_COLOR));
        wrefresh(dialogue);

        wattron(dialogue,COLOR_PAIR(FONT_COLOR));
        wrefresh(dialogue);
        mvwprintw(dialogue, 5, 30, "Collect if you can!");
        mvwprintw(dialogue, 10, 10, "Welcome to my game!");
        mvwprintw(dialogue, 11, 10, "Here are the rules:");
        mvwprintw(dialogue, 12, 10, "Collect as many asterisks as possible without hitting obstacles or borders!");
        mvwprintw(dialogue, 13, 10, "You can make 300 movements at maximum, use them wisely!");
        mvwprintw(dialogue, 14, 10, "If you hit borders or obstacles, one of your hearts is gone");
        mvwprintw(dialogue, 15, 10, "Bottom border of the game is upper line of statistics part");
        mvwprintw(dialogue, 16, 10, "Game ends when quantity of hearts reaches 0 or you reach movement limit!");
        mvwprintw(dialogue, 17, 10, "Are you ready? (y/n)");
        wattroff(dialogue,COLOR_PAIR(FONT_COLOR));
        wrefresh(dialogue);
        wattroff(dialogue, COLOR_PAIR(MAIN_COLOR));
        wrefresh(dialogue);

        int a = wgetch(mainwin);
        wrefresh(mainwin);
        wrefresh(dialogue);

        if (a == 'y') {
                system("./gameplay");
        }

        // Clean up
        delwin(dialogue);
        delwin(mainwin);
        endwin();

        return 0;
}


#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>

struct room{
    int width;
    int height;
    int door[2][2];
    WINDOW *win;
};

void drawWindow(struct room room, int x, int y){
    box(room.win,0,0);
    mvwaddch(room.win,room.door[0][0],room.door[0][1],'D');
    mvwaddch(room.win,room.door[1][0],room.door[1][1],'D');
    mvwaddch(room.win,y,x,'X');
    wrefresh(room.win);
}

void textDisplay(char* text, WINDOW *rootwin){
    WINDOW *tempwin = newwin(4, getmaxx(rootwin), getmaxy(rootwin)-4, 0);
    wmove(tempwin,1,1);
    box(tempwin,0,0);
    wprintw(tempwin,text);
    wrefresh(tempwin);
}

void textDisplayWS(char* text, WINDOW *rootwin, char* name){
    WINDOW *tempwin = newwin(4, getmaxx(rootwin), getmaxy(rootwin)-4, 0);
    wmove(tempwin,1,1);
    box(tempwin,0,0);
    wprintw(tempwin,text, name);
    wrefresh(tempwin);
}

WINDOW* newRandomRoom(int mx, int my, struct room *Room){
    Room->width = rand() % 20 + 1;
    Room->height = rand() % 20 + 1;
    Room->door[0][0] = 0;
    Room->door[0][1] = rand() % Room->width + 1;
    Room->door[1][0] = Room->height+1;
    Room->door[1][1] = rand() % Room->width + 1;
    return newwin(Room->height+2, Room->width+2, my/2-Room->height/2, mx/2-Room->width/2);
}
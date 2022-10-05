#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include "engine.h"

int main(int argc, char** argv){
    srand(time(0));
    // Set playername
    char* playername = argv[0];
    playername = argv[1] != NULL ? argv[1]: "Default Name";

    //window setup functions;
    int x,y,nx,ny;
    int mx=0,my=0;
    WINDOW *win = initscr();
    getmaxyx(win,my,mx);
    struct room rooms[50] = {0,0};
    int currentRoom = 0;
    rooms[0].width = 10;
    rooms[0].height = 10;
    rooms[0].door[0][0] = 0;
    rooms[0].door[0][1] = 5;
    rooms[0].door[1][0] = 0;
    rooms[0].door[1][1] = 5;
    rooms[0].win = newwin(12, 12, my/2-6, mx/2-6);
    wmove(rooms[0].win, 6,6);
    box(rooms[0].win,0,0);
    waddch(rooms[0].win,'x');
    refresh();
    wrefresh(rooms[0].win);
    getch();

    //Welcome Code
    textDisplayWS("Welcome Knight %s",win, playername);

    //game loop
    for (;;){
        //Movement/Action Code
        x = getcurx(rooms[currentRoom].win)-1; nx = x;
        y = getcury(rooms[currentRoom].win); ny = y;
	    getmaxyx(rooms[currentRoom].win,my,mx);	
        switch(getch()){
            //Move Up
        case ',':{
                wclear(rooms[currentRoom].win);
                ny--;
                break;
            }
            //Move Left
        case 'a':{
                wclear(rooms[currentRoom].win);
                nx--;
                break;
            }
            //Move Down
        case 'o':{
                wclear(rooms[currentRoom].win);
                ny++;
                break;
            }
            //Move Right
        case 'e':{
                wclear(rooms[currentRoom].win);
                nx++;
                break;
            }
            //Open A Door
        case '.':{
                if (x == rooms[currentRoom].door[0][1] && y - 1 == rooms[currentRoom].door[0][0]){
                    wclear(win);
                    currentRoom++;
                    if(rooms[currentRoom].height == 0){
                        rooms[currentRoom].win = newRandomRoom(getmaxx(win),getmaxy(win), &rooms[currentRoom]);
                        textDisplay("test",win);
                    }
                    ny = rooms[currentRoom].door[1][0]-1; nx = rooms[currentRoom].door[1][1];
                    y=ny;x=nx;
                    wrefresh(rooms[currentRoom-1].win);
                    wrefresh(win);
                    getmaxyx(rooms[currentRoom].win,my,mx);
                    textDisplay("The Door slowly opens, into another cavernous room",win);
                }
                else if (x == rooms[currentRoom].door[1][1] && y + 1 == rooms[currentRoom].door[1][0]){
                    wclear(win);
                    currentRoom--;
                    ny = rooms[currentRoom].door[0][0]+1; nx = rooms[currentRoom].door[0][1];
                    y=ny;x=nx;
                    wrefresh(win);
                    getmaxyx(rooms[currentRoom].win,my,mx);
                    textDisplay("The Door slowly opens, into another cavernous room",win);
                }
                break;
        }
        }
        if (nx >= mx-1 || nx < 1 || ny >= my-1 || ny < 1)
            drawWindow(rooms[currentRoom],x,y);
        else
            drawWindow(rooms[currentRoom],nx,ny);
    }

    endwin();
    return 0;
}

WINDOW *createRoom(struct room room){

}

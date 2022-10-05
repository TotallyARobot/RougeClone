#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include "engine.h"

int main(int argc, char** argv){
    srand(time(0));
    // Set playername
    struct player player;
    player.health = 20;
    player.name = argv[1] != NULL ? argv[1]: "Default Name";

    //window setup functions;
    int nx,ny;
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
    textDisplayWS("Welcome Knight %s",win, player.name);

    int enemymove = 0;
    //game loop
    for (;;){
        //Movement/Action Code
        player.x = getcurx(rooms[currentRoom].win)-1; nx = player.x;
        player.y = getcury(rooms[currentRoom].win); ny = player.y;
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
                if (player.x == rooms[currentRoom].door[0][1] && player.y - 1 == rooms[currentRoom].door[0][0]){
                    wclear(win);
                    currentRoom++;
                    if(rooms[currentRoom].height == 0){
                        rooms[currentRoom].win = newRandomRoom(getmaxx(win),getmaxy(win), &rooms[currentRoom]);
                        textDisplay("test",win);
                    }
                    ny = rooms[currentRoom].door[1][0]-1; nx = rooms[currentRoom].door[1][1];
                    player.y=ny;player.x=nx;
                    wrefresh(rooms[currentRoom-1].win);
                    wrefresh(win);
                    getmaxyx(rooms[currentRoom].win,my,mx);
                    textDisplay("The Door slowly opens, into another cavernous room",win);
                }
                else if (player.x == rooms[currentRoom].door[1][1] && player.y + 1 == rooms[currentRoom].door[1][0]){
                    wclear(win);
                    currentRoom--;
                    ny = rooms[currentRoom].door[0][0]+1; nx = rooms[currentRoom].door[0][1];
                    player.y=ny;player.x=nx;
                    wrefresh(win);
                    getmaxyx(rooms[currentRoom].win,my,mx);
                    textDisplay("The Door slowly opens, into another cavernous room",win);
                }
                break;
            }
            //Attack
        case '\'':{
                wclear(rooms[currentRoom].win);
                for (int i = 0; i < rooms[currentRoom].enemyAmount; i++)
                    if(rooms[currentRoom].enemies[i].x <= player.x + 1 && rooms[currentRoom].enemies[i].x >= player.x - 1
                    && rooms[currentRoom].enemies[i].y <= player.y + 1 && rooms[currentRoom].enemies[i].y >= player.y + 1)
                        rooms[currentRoom].enemies[i].health-=5;
                enemiesRemove(&rooms[currentRoom]);
                textDisplay("You attack and hit a monster",win);
            } 
        }
        enemymove++;
        if (enemymove == 2){
            enemymove = 0;
            enemyMovement(&rooms[currentRoom], &player);
        }
        if (nx >= mx-1 || nx < 1 || ny >= my-1 || ny < 1)
            drawWindow(rooms[currentRoom],player.x,player.y);
        else
            drawWindow(rooms[currentRoom],nx,ny);
        inventoryDisplay(player, win);
        

        if(player.health <= 0){
            textDisplay("YOU DIED.",win);
            getch();
            break;
        }

    }

    endwin();
    return 0;
}


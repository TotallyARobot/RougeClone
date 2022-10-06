#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>

struct enemy{
    int x,y,health;
    char* name;
    char icon;
};

struct room{
    int width;
    int height;
    int door[2][2];
    WINDOW *win;
    int enemyAmount;
    struct enemy enemies[5];
};

struct player{
    int x,y,health;
    char* name;
    unsigned short int Inventory[16];
    unsigned short int Inventoryp;
};

char* readItem(int item){
    if(item < 0x0200 && item > 0x0100)
        return "Heal Pot";
}

void drawWindow(struct room room, int x, int y){
    box(room.win,0,0);
    mvwaddch(room.win,room.door[0][0],room.door[0][1],'D');
    mvwaddch(room.win,room.door[1][0],room.door[1][1],'D');
    for(int i = 0; i < room.enemyAmount; i++)
        mvwaddch(room.win,room.enemies[i].y,room.enemies[i].x,room.enemies[i].icon);
    mvwaddch(room.win,y,x,'X');
    wrefresh(room.win);
}

void enemyMovement(struct room *room, struct player *player){
    for(int i = 0; i < room->enemyAmount; i++){
        if(abs(player->x-room->enemies[i].x) > abs(player->y-room->enemies[i].y))
            if (player->x-room->enemies[i].x < 0)
                room->enemies[i].x--;
            else
                room->enemies[i].x++;
        else
            if (player->y-room->enemies[i].y < 0)
                room->enemies[i].y--;
            else
                room->enemies[i].y++;

        mvwaddch(room->win,room->enemies[i].y,room->enemies[i].x,room->enemies[i].icon);
        if(room->enemies[i].x <= player->x + 1 && room->enemies[i].x >= player->x - 1
        && room->enemies[i].y <= player->y + 1 && room->enemies[i].y >= player->y - 1)
                            player->health--;
    }
}

void textDisplay(char* text, WINDOW *rootwin){
    WINDOW *tempwin = newwin(4, getmaxx(rootwin), getmaxy(rootwin)-4, 0);
    wmove(tempwin,1,1);
    box(tempwin,0,0);
    wprintw(tempwin,text);
    wrefresh(tempwin);
}

void inventoryDisplay(struct player player, WINDOW *rootwin){
    WINDOW *tempwin = newwin(getmaxy(rootwin)-4, 15, 0, 0);
    wmove(tempwin,1,1);
    box(tempwin,0,0);
    wprintw(tempwin,"Health: %i", player.health);
    for (int i = 0; i < player.Inventoryp; i++){
        wmove(tempwin,i+2,1);
        wprintw(tempwin,"%s: %i", readItem(player.Inventory[i]), player.Inventory[i]-0x0100);
    }
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

    Room->enemyAmount = rand() % 5 + 1;
    for(int i = 0; i < Room->enemyAmount; i++){
        Room->enemies[i].x = rand() % Room->width + 1;
        Room->enemies[i].y = rand() % Room->height + 1;
        Room->enemies[i].health = rand() % 7 + 5;
        Room->enemies[i].icon = rand() % 5 + 97;
        Room->enemies[i].name = "Goblin";
    }

    return newwin(Room->height+2, Room->width+2, my/2-Room->height/2, mx/2-Room->width/2);
}

void enemiesRemove(struct room *room){
    for(int i = 0; i < room->enemyAmount; i++){
        if(room->enemies[i].health <= 0){
            for(int w = i; w < room->enemyAmount; w++){
                room->enemies[w].health = room->enemies[w+1].health;
                room->enemies[w].icon = room->enemies[w+1].icon;
                room->enemies[w].name = room->enemies[w+1].name;
                room->enemies[w].x = room->enemies[w+1].x;
                room->enemies[w].y = room->enemies[w+1].y; 
            }
            room->enemyAmount--;
        }

    }
}

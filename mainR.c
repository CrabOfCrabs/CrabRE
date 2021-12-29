#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/ioctl.h>
#include "crlib.h"
#include <math.h>
#include <curses.h>

int pointr(){

	initscr();
	clear();
	
	tri tb[] = { 0 }; //declares  an array for all triangles in cube
	mkcube(tb); //makes triangulated cube and puts triangles to tb
	int offs = 50; //screen offset

	for(double xr=0;xr<=5000;xr+=0.001){ //rotate in radianos
		for(int tr=0;tr<=11;tr++){ //for all triangols
		mvprintw(rent(tb[tr],xr).p1.y/2+offs, rent(tb[tr],xr).p1.x+offs,"#");mvprintw( rent(tb[tr],xr).p2.y/2+offs, rent(tb[tr],xr).p2.x+offs,"#");mvprintw( rent(tb[tr],xr).p3.y/2+offs, rent(tb[tr],xr).p3.x+offs,"#"); //render my points
		}	
		nanosleep((const struct timespec[]){{0, 500000L}}, NULL); //wait some time between frames
		refresh();		
		clear();
	}
	refresh();
	getch();
	endwin();
	exit(0);

return 0;
}

int liner(){

	initscr();
	clear();
	
	tri tb[12] = {0}; //declares  an array for all triangles in cube
	mkcube(tb); //makes triangulated cube and puts triangles to tb
	int offs = 20; //screen offset

	for(double xr=0;xr<=5000;xr+=0.001){ //rotate in radianos
		for(int tr=0;tr<=11;tr++){ //for all triangols
	
		line l[3] = {0};
		lnfy(rent(tb[tr],xr),l);

			for(double y = l[0].p2.y;y<= l[0].p1.y;y++){
			mvprintw(y/2+offs,inpolx(l[0],y)+offs,"#");
			}
			for(double y = l[1].p2.y;y<= l[1].p1.y;y++){
			mvprintw(y/2+offs,inpolx(l[1],y)+offs,"#");
			}
			for(double y = l[2].p2.y;y<= l[2].p1.y;y++){
			mvprintw(y/2+offs,inpolx(l[2],y)+offs,"#");
			}

		}	
		nanosleep((const struct timespec[]){{0, 500000L}}, NULL); //wait some time between frames
		refresh();		
		clear();
	}
	refresh();
	getch();
	endwin();
	exit(0);

return 0;
}

int main(){
	liner();	


return 0;
}	

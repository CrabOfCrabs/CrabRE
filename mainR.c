#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/ioctl.h>
#include <math.h>
#include <stdbool.h>
#include <curses.h>
#include "crlib.h"


void trir(){
int offs = 0;
	initscr();
	clear();
	curs_set(0);
	printf("f");
	tri tb[12] = {0}; //declares  an array for all triangles in cube
	 //makes triangulated cube and puts triangles to tbi
	mkcube(tb,1);
	point sun = mkp(0,0,1);
	point campo = mkp(0,0,0);
	struct winsize sz;
	for(double xr=0;xr<=2000;xr+=0.1){ //rotate in radianosi
		point campos = {-xr/30,2,-xr/30,1};
		point vt = {0,0,0,0};
		atto_cam(campos,vt);
		for(int tr=0;tr<=11;tr++){ //for all triangols
			ioctl( 0, TIOCGWINSZ, &sz );
			tri att = ziprentc(tb[tr],mkp(sz.ws_col,sz.ws_row,0));
			if(dot(trinorm(att),subp(att.p1,campo)) <0 ){	
			double shade = calcshade(tb[tr],campos);
			scanln(tb[tr],shade,mkp(sz.ws_col,sz.ws_row,0));
			}
		}	
		nanosleep((const struct timespec[]){{0, 5000000L}}, NULL); //wait some time between frames
		refresh();		
		clear();
	}
	refresh();
	getch();
	endwin();
	exit(0);

}



int main(){
	trir();	


return 0;
}	

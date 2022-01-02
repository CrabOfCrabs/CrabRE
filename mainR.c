#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/ioctl.h>
#include "crlib.h"
#include <math.h>
#include <curses.h>
/*
void printons(point p){
	float y = p.y;
	float x = p.x;
	int offs = 20;
	mvprintw(y/2+offs,x+offs,"#");
}
*/
void pointr(){

	initscr();
	clear();
	
	tri tb[] = { 0 }; //declares  an array for all triangles in cube
	mkcube(tb,1); //makes triangulated cube and puts triangles to tb
	int offs = 50; //screen offset

	for(float xr=0;xr<=5000;xr+=0.001){ //rotate in radianos
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

}

void liner(){

	initscr();
	clear();
	
	tri tb[12] = {0}; //declares  an array for all triangles in cube
	mkcube(tb,1); //makes triangulated cube and puts triangles to tb
	int offs = 20; //screen offset

	for(float xr=0;xr<=5000;xr+=0.001){ //rotate in radianos
		for(int tr=0;tr<=11;tr++){ //for all triangols
	
		line l[3] = {0};
		lnfy(rent(tb[tr],xr),l);

			for(float y = l[0].p2.y;y<= l[0].p1.y;y++){
			mvprintw(y/2+offs,inpolx(l[0],y)+offs,"#");
			}
			for(float y = l[1].p2.y;y<= l[1].p1.y;y++){
			mvprintw(y/2+offs,inpolx(l[1],y)+offs,"#");
			}
			for(float y = l[2].p2.y;y<= l[2].p1.y;y++){
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

}
void linpr(tri t,float xr,point s){
	int offs = 0;
	line l[3] = {0};
	lnfy(projt(rent(t,xr),mkp(s.x,s.y,0)),l);

	for(float y = l[0].p2.y;y<= l[0].p1.y;y++){
		mvprintw(y/2+offs,inpolx(l[0],y)+offs," ");
			}
	for(float y = l[1].p2.y;y<= l[1].p1.y;y++){
		mvprintw(y/2+offs,inpolx(l[1],y)+offs," ");
			}
	for(float y = l[2].p2.y;y<= l[2].p1.y;y++){
		mvprintw(y/2+offs,inpolx(l[2],y)+offs," ");}
}
void trir(){
int offs = 0;
	initscr();
	clear();
	curs_set(0);
	printf("f");
	tri tb[12] = {0}; //declares  an array for all triangles in cube
	mkcube(tb,1); //makes triangulated cube and puts triangles to tbi
	point sun = mkp(0,0,1);
	point cm = mkp(0,0,5);
	struct winsize sz;
	for(float xr=0;xr<=5000;xr+=0.01){ //rotate in radianos
		
		point cmr = multm(multm(multm(cm,crmz(xr)),crmy(xr)),crmx(xr));
		
	
		for(int tr=0;tr<=11;tr++){ //for all triangols
			ioctl( 0, TIOCGWINSZ, &sz );
			tri tt = rent(tb[tr],xr);
			if(dot(trinorm(tt),subp(tricenp(tt),cm)) <0 ){	
			float shade = calcshade(tt,sun);
			scanln(tb[tr],xr,shade,mkp(sz.ws_col,sz.ws_row,0));
		//	linpr(tb[tr],xr,mkp(sz.ws_col,sz.ws_row,0));
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

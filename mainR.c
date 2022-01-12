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
	tri tb[6] = {0}; //declares  an array for all triangles in cube
	 //makes triangulated cube and puts triangles to tbi
	mkpir(tb,1);
	point sun = mkp(0,0,1);
	point campo = mkp(0,0,0);
	struct winsize sz;
	for(double xr=0;xr<=2000;xr+=1){ //rotate in radianosi
		point campos = {-xr/300,2,-xr/100,1};
		point vt = {0.5,1,0.5,0};
		atto_cam(campos,vt);
		for(int tr=0;tr<=5;tr++){ //for all triangols
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

void trirf(){
int offs = 0;
        initscr();
        clear();
        curs_set(0);
        printf("f");
        tri tb[6] = {0}; //declares  an array for all triangles in cube
         //makes triangulated cube and puts triangles to tbi
        mkpir(tb,1);
        point sun = mkp(-1,3,-1);
        point campo = mkp(-2,1,-2);
	
	double yr = 0;
        char c;
	struct winsize sz;
        while(1){ //rotate in radianosi
		system("/bin/stty raw");

		c=getchar();
		if(c == 'w'){campo.z = campo.z + 0.05;}
		if(c == 's'){campo.z = campo.z - 0.05;}
		if(c == 'a'){campo.x = campo.x + 0.05;}
                if(c == 'd'){campo.x = campo.x - 0.05;}
		if(c == 'o'){yr = yr + 1;}
                if(c == 'l'){yr = yr - 1;}

		if(c == 'e'){system("clear");system("/bin/stty sane");exit(1);}
		point vt = mkp( 0,0,1 );
		mat4 mcr = crmy(yr);
		point vld = multm(vt,mcr);
		vt = addp(campo, vld);
                atto_cam(campo,vt);
		point campos = getcamp(campo);
                for(int tr=0;tr<=5;tr++){ //for all triangols
                        ioctl( 0, TIOCGWINSZ, &sz );
                        tri att = ziprentc(tb[tr],mkp(sz.ws_col,sz.ws_row,0));
                        if(dot(trinorm(att),subp(att.p1,campo)) <0 ){
                        double shade = calcshade(tb[tr],sun);
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
	trirf();	


return 0;
}	

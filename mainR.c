#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/ioctl.h>
#include <math.h>
#include <stdbool.h>
#include <curses.h>
#include "crlib.h"

/*
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
			tri att = ziprentc(tb[tr],mkp(sz.ws_col,sz.ws_row/2,0));
			if(dot(trinorm(att),subp(att.p1,campo)) <0 ){	
			double shade = calcshade(tb[tr],campos);
			scanln(tb[tr],shade,mkp(sz.ws_col,sz.ws_row/2,0));
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
*/

void trirf(){
int offs = 0;
        initscr();
        clear();
        curs_set(0);
        printf("f");
        tri tb[300] = {0}; //declares  an array for all triangles in cube
         //makes triangulated cube and puts triangles to tbi
        mkfile(tb);
	point up = mkp(0,-1,0);
        point sun = mkp(-1,3,-1);
        point campo = mkp(0,0,100);
	point vld = mkp(0,0,1);	
	double yr = 0;
	double xr = 0;
	double zr = 0;
        char c;
	struct winsize sz;
        while(1){ //rotate in radianosi
		system("/bin/stty raw");

		c=getchar();
		if(c == 'w'){campo = subp(campo,divpn(vld,5));}
		else if(c == 's'){campo = addp(campo,divpn(vld,5));}
		else if(c == 'd'){campo = subp(campo,divpn(normp(crossp(vld,up)),5));}
		else if(c == 'a'){campo = addp(campo,divpn(normp(crossp(vld,up)),5));}
		else if(c == 'r'){campo.y = campo.y - 0.5;}
		else if(c == 'f'){campo.y = campo.y + 0.5;}
		else if(c == 'l'){yr -=0.05;}
		else if(c == 'j'){yr +=0.05;}
		else if(c == 'k'){xr -=0.05;}
		else if(c == 'i'){xr +=0.05;}
	
		else if(c == 'e'){system("clear");system("/bin/stty sane");exit(1);}
		point vt = mkp( 0,0,1 );
		vld = multm(multm(vt,crmx(xr)),crmy(yr));
		vt = addp(campo, vld);
		mat4 camm = qinvm(camtr(campo,vt,up)); 
		tri tristack[300] = {0};
                for(int tr=0;tr<=300;tr++){ //for all triangols
                        ioctl( 0, TIOCGWINSZ, &sz );
			point screen = mkp(sz.ws_col,sz.ws_row*2,0);
		if(dot(trinorm(tb[tr]),subp(tricenp(tb[tr]),campo)) < 0 ){
				tri fint;

				fint.p1 = multm(tb[tr].p1,camm);
				fint.p2 = multm(tb[tr].p2,camm);
				fint.p3 = multm(tb[tr].p3,camm);

				fint.p1 = multm(fint.p1,ppm(screen));
				fint.p2 = multm(fint.p2,ppm(screen));
				fint.p3 = multm(fint.p3,ppm(screen));


				fint.p1 = normdcp(fint.p1,screen);
				fint.p2 = normdcp(fint.p2,screen);
				fint.p3 = normdcp(fint.p3,screen);
				double shade = calcshade(tb[tr],campo);
				fint.gs = shade;
		tristack[tr] = fint;
		}	}
		qsort(tristack,300,sizeof(tri),compare_function);
		for(int tt = 0;tt<=300;tt++){
			scanln(tristack[tt],tristack[tt].gs);

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

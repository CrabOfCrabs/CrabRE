#include "crlib.h"

void trirf(){
int offs = 0;
        initscr();
        clear();
        curs_set(0);
        tri tb[400] = {0}; //declares  an array for all triangles in cube
         //makes triangulated cube and puts triangles to tbi
        mkfile(tb);
	point up;
        point sun = mkp(0,1,-1);
        point campo = mkp(0,0,100);
	point vld;
	point r = mkp(0,0,0);	
	double yr = 0;
	double xr = 0;
	double zr = 0;
        char c;
	struct winsize sz;
        while(1){ //rotate in radianosi
		system("/bin/stty raw");
		point up = {0,1,0,1};
		point vt = {0,0,-1,1};
		c=getchar();
		switch(c){
			case'w' : campo = subp(campo,divpn(vld,5));break;
			case's' : campo = addp(campo,divpn(vld,5));break;
			case'a' : campo = subp(campo,divpn(normp(crossp(vld,up)),5));break;
			case'd' : campo = addp(campo,divpn(normp(crossp(vld,up)),5));break;
			case'l' : yr -=0.05;break;
			case'j' : yr +=0.05;break;
			case'k' : xr -=0.05;break;
			case'i' : xr +=0.05;break;
	
			case'e' : system("clear");system("/bin/stty sane");exit(1);
			default : break;}

		vld = multm(vt,multms(crmx(xr),crmy(yr)));
		vt = addp(campo, vld);
		mat4 camm = qinvm(camtr(campo,vt,up)); 
		tri tristack[400] = {0};
                for(int tr=0;tr<=400;tr++){ //for all triangols
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
				double shade = calcshade(tb[tr],subp(tricenp(tb[tr]),campo));
				fint.gs = -shade;
			tristack[tr] = fint;}}

		qsort(tristack,400,sizeof(tri),compare_function);
		clear();
		for(int tt = 0;tt<=400;tt++){
			scanln(tristack[tt],tristack[tt].gs);}
                
                nanosleep((const struct timespec[]){{0, 5000000L}}, NULL); //wait some time between frames
                refresh();}
        refresh();
        getch();
        endwin();
        exit(0);}

int main(){
	trirf();
return 0;
}	

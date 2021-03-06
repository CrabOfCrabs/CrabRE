#include "crlib.h"
#include "crl_ncurses.h"
void trirf(){
       // initscr();
        clear();
        curs_set(0);
        tri tb[6500] = {0}; //declares  an array for all triangles in cube
         //makes triangulated cube and puts triangles to tbi
        mkfile(tb);
        Vec4 campo = mkp(0,0,5);
	Vec4 vld;
	Vec4 r = mkp(0,0,0);	
        char c;
	struct winsize sz;
	double timeS = 0;
        while(1){ //rotate in radianosi
		system("/bin/stty raw");
		Vec4 up = {0,1,0,0};
		Vec4 vt = {0,0,1,0};
		//c=getchar();
		/*switch(c){
			case'w' : campo = subp(campo,divpn(vld,5));break;
			case's' : campo = addp(campo,divpn(vld,5));break;
			case'a' : campo = subp(campo,divpn(normp(crossp(vld,up)),5));break;
			case'd' : campo = addp(campo,divpn(normp(crossp(vld,up)),5));break;
			case'l' : yr -=0.05;break;
			case'j' : yr +=0.05;break;
			case'k' : xr -=0.05;break;
			case'i' : xr +=0.05;break;
	
			case'e' : system("clear");system("/bin/stty sane");exit(1);
			default : break;}*/

		vld = multm(vt,multms(crmx(r.x),multms(crmy(r.y),crmz(r.z))));
		

        vt = addp(campo, vld);
		mat4 camm = qinvm(camtr(campo,vt,up)); 
		tri tristack[6500] = {0};
                for(int tr=0;tr<=6500;tr++){ //for all triangols
                        ioctl( 0, TIOCGWINSZ, &sz );
			Vec4 screen = mkp(sz.ws_col,sz.ws_row*2,0);
			
			tri fint;
			fint = trant(tb[tr],mkp(0,timeS,0),mkp(0,0,0),mkp(0,0,0),mkp(1,1,1));	
			
			if(dot(trinorm(fint),subp(tricenp(fint),campo)) < 0 ){		
				
				fint.gs = -calcshade(fint,subp(tricenp(fint),campo));	

				fint.p1 = multm(fint.p1,camm);
				fint.p2 = multm(fint.p2,camm);
				fint.p3 = multm(fint.p3,camm);
				
				fint.p1 = multm(fint.p1,ppm(screen));
				fint.p2 = multm(fint.p2,ppm(screen));
				fint.p3 = multm(fint.p3,ppm(screen));


				fint.p1 = normdcp(fint.p1,screen);
				fint.p2 = normdcp(fint.p2,screen);
				fint.p3 = normdcp(fint.p3,screen);

				tristack[tr] = fint;}}

		qsort(tristack,6500,sizeof(tri),compare_function);
		clear();
		for(int tt = 0;tt<=6500;tt++){
			triRaster(tristack[tt],tristack[tt].gs);}
                sleep(0.100);
                //nanosleep((const struct timespec[]){{0, 5000000L}}, NULL); //wait some time between frames
                timeS += 0.1;
		refresh();}
        refresh();
        getch();
        endwin();
        exit(0);}

int main(){
	trirf();
return 0;
}	

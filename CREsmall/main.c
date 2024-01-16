#include <sys/ioctl.h>
#include <unistd.h>
#include <time.h>
#include <curses.h>
#include "CRE_T.h"

void fchshade(int x,int y,float lg){
  char *shade[9]={".",":","*","o","#","&","8","@","?"};
  mvprintw(y/2,x,shade[(int)floor((lg*8))]);
}


int main(){
   initscr();
   clear();struct winsize sz;
M mesh={0};CRE_M_load(&mesh,"./obj/utah.obj");
int rot=45,chng=1;

  curs_set(0);
        while(1){ ioctl(0,TIOCGWINSZ,&sz);
rot+=chng;
  if(rot>=90 || rot <=-0){chng*=-1;}
Cam Lcam = {90,1000,0.1,{0,0,20},{0,1,0},{0,0,1}};
    Lcam.VT=CRE_Q_rotate((V3){0,1,0},(rot-45)*3.1415/180,(V3){0,0,1});
    Lcam.VT=CRE_Q_rotate((V3){1,0,0},(rot-45)*3.1415/270,Lcam.VT);
  Cam Lcam2 = Lcam;
  Lcam2.CAM_V.d[y]=6;

int S[2]={sz.ws_col,sz.ws_row*2};
    clear();
          CRE_M_render(mesh,Lcam,S,&fchshade);
CRE_M_render(mesh,Lcam2,S,&fchshade);
   //sleep(1);
   refresh();}
   refresh();
   getch();
   endwin();
   exit(0);
   return 0;
}

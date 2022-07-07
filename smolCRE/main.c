#include <sys/ioctl.h>
#include <unistd.h>
#include <time.h>
#include <curses.h>
#include "CRE_small.h"

int main(){
   initscr();
   clear();
   curs_set(0);
   struct winsize sz;
    M mesh;Cam Lcam = {90,1000,0.1,{0,0,5},{0,1,0},{0,0,1}};
   CRE_M_load(&mesh,"./crab1.obj");
   while(1){ioctl(0,TIOCGWINSZ,&sz);

   CRE_M_render(&mesh,Lcam,(int[2]){sz.ws_col,sz.ws_row*2},NULL);
   sleep(1);refresh();}
   refresh();
   getch();
   endwin();
   exit(0);
   return 0;
}

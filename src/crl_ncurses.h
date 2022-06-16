#include <curses.h>

/*	
	RENDER FUNCTIONS 
*/	


/*	
	DRAWING FUNCTIONS //will be moved to a seperate graphis handling library or 2d objects
*/

//scanline top and down triangle projection
void chshaded(point p){
	 
	double y=p.y, x=p.x, lg=p.z; //z is shade val
	
	if(lg>1){
	mvprintw(y,x,"M");}
	else if(lg<=1 && lg>0.7){
	mvprintw(y/2,x,"#");}
	else if(lg<=0.7 && lg>0.6){
	mvprintw(y/2,x,"@");}
	else if(lg<=0.6 && lg>0.5){
	mvprintw(y/2,x,"8");}
	else if(lg<=0.5 && lg>0.4){
	mvprintw(y/2,x,"&");}
	else if(lg<=0.4 && lg>0.3){
	mvprintw(y/2,x,"o");}
	else if(lg<=0.3 && lg>0.2){
	mvprintw(y/2,x,"*");}
	else if(lg<=0.2 && lg>0.1){
	mvprintw(y/2,x,":");}
	else if(lg<=0.1 && lg>=0){
	mvprintw(y/2,x,".");}}

void drawline2d(point p1,point p2){ //p1 z is the shade of tri
	double x=0, y=0, x1 = p1.x, x2 = p2.x, x2l=0;

	if(x1<x2){x=x1;x2l = x2;}else{x=x2;x2l = x1;}y=p1.y;
	while(x<=x2l){
		chshaded(mkp(x,y,p1.z));x=x+1;}}

void drawtup(tri t,double lg){ //draws the line parallel to y up
	point p1 = t.p1,p2 = t.p2,p3 = t.p3;p1.y = floor(p1.y);
	double invslope1 = (p2.x - p1.x) / (p2.y - p1.y),
	       invslope2 = (p3.x - p1.x) / (p3.y - p1.y),
	       curx1 = p1.x, 
	       curx2 = p1.x;
	for(int Y = p1.y; Y < p2.y; Y++){
		drawline2d(mkp(curx1,Y,lg),mkp(curx2,Y,0));
		curx1 += invslope1;
		curx2 += invslope2;}}
void drawtdown(tri t,double lg){//draws the line parallel to y down
	point p1 = t.p1,p2 = t.p2,p3 = t.p3;p3.y = ceil(p3.y);
	double invslope1 = (p3.x - p1.x) / (p3.y - p1.y), 
	       invslope2 = (p3.x - p2.x) / (p3.y - p2.y), 
	       curx1 = p3.x, 
	       curx2 = p3.x;	
	for(int Y = p3.y; Y > p1.y; Y--){
		drawline2d(mkp(curx1,Y,lg),mkp(curx2,Y,0));
		curx1 -= invslope1;
		curx2 -= invslope2;}}
void triRaster(tri tr,double lg){
	if(tr.p2.y < tr.p1.y){ swapp(&tr.p2, &tr.p1); }if(tr.p3.y <tr.p1.y){ swapp(&tr.p3, &tr.p1); }if(tr.p3.y < tr.p2.y){ swapp(&tr.p3, &tr.p2); }
	
	if(tr.p3.y == tr.p2.y){drawtup(tr,lg);}
	else if(tr.p1.y == tr.p2.y){drawtdown(tr,lg);}
	else{point p4 = mkp(inpolx(mkline(tr.p1,tr.p3),tr.p2.y),tr.p2.y,0);
	tri tu = mktri(tr.p1,tr.p2,p4);tri td = mktri(tr.p2,p4,tr.p3);
	drawtup(tu,lg);drawtdown(td,lg);}}

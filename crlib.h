#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/ioctl.h>
#include <math.h>
#include <stdbool.h>
#include <curses.h>
#include <string.h>
#include "typegr.h"
#include "constgr.h"


/*	
	HEADER CONSTANT //will be move to main toggles file in future
*/
bool inToggle = false;


/*	
	GENERAL PURPOSE FUNCTIONS //will be move to general use header
*/	

double clamp(double n,double max,double min){if(n<min){n=min;}else if(n>max){n=max;}return n;}

int compare_function(const void *a,const void *b){
tri *x = (tri *) a;
tri *y = (tri *) b;

double z1 = (x->p1.z + x->p2.z + x->p3.z)/3;
double z2 = (y->p1.z + y->p2.z + y->p3.z)/3;
if(z1 < z2){
	return -1;}
else if(z1 > z2){
	return 1;}
else{return 0;}
}





/*	
	RENDER FUNCTIONS 
*/	

//this shit moves your tringle / point to world space but doesnt have to you can just predefine the rotation
point tranp(point p,point xyzr,point objor,point objoff,point objsize){
	point transformed = multm(p,multms(multms(multms(multms(multms(multms(
	//shittone f matrix multiplications and then multiply p by final matrix
	spos(objor),//origin
	pscale(objsize)),//rescale
	crmz(xyzr.z)),crmy(xyzr.y)),crmx(xyzr.x)),//zyx rotation
	spos(objoff)),//final offset
	idm()));//identity matrix
return transformed;}
tri trant(tri t,point rc,point pc,point co,point os){
	tri t1 = mktri(tranp(t.p1,rc,pc,co,os),tranp(t.p2,rc,pc,co,os),tranp(t.p3,rc,pc,co,os));
return t1;}

/*
//this shit does all the things for you but sometimes its doing some weird shit
point fastproj(point p,point screen){
	point cor =  multm(p,qinvm(camtr(lcam.atV,lcam.toV,lcam.upV)));
	point coff = multm(cor,ppm(screen));	
	point enp = normdcp(coff,screen);
return enp;}
tri ziprent(tri t,point screen){
	tri t1 = mktri( fastproj(t.p1,screen),
			fastproj(t.p2,screen),
			fastproj(t.p3,screen));
return t1;}
*/
point getcampo(point p,camera rcam){point pct = multm(p,camtr(rcam.atV,rcam.toV,rcam.upV));return pct;}
/*	
	DRAWING FUNCTIONS //will be moved to a seperate graphis handling library or 2d objects
*/

//scanline top and down triangle projection
void chshaded(point p){
	 
	double y=p.y, x=p.x, lg=p.z; //z is shade val
	
	if(lg>1){
	mvprintw(y/2,x,"M");}
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
	double dx, dy=0, p, x, y, x1 = p1.x, x2 = p2.x, x2l;

	if(x1<=x2){x=x1;dx=x2-x1;x2l = x2;}else{x=x2;dx=x1-x2;x2l = x1;}y=p1.y;p=2*dy-dx;
	while(x<x2l){
		if(p>=0){chshaded(mkp(x,y,p1.z));y=y+1;p=p+2*dy-2*dx;}else{chshaded(mkp(x,y,p1.z));p=p+2*dy;}x=x+1;}}

void drawtup(tri t,double lg){
	point p1 = t.p1,p2 = t.p2,p3 = t.p3;
	double invslope1 = (p2.x - p1.x) / (p2.y - p1.y),
	       invslope2 = (p3.x - p1.x) / (p3.y - p1.y),
	       curx1 = p1.x, 
	       curx2 = p1.x;
	for(double Y = p1.y; Y <= p2.y; Y++){
		drawline2d(mkp(curx1,Y,lg),mkp(curx2,Y,0));
		curx1 += invslope1;
		curx2 += invslope2;}}
void drawtdown(tri t,double lg){
	point p1 = t.p1,p2 = t.p2,p3 = t.p3;
	double invslope1 = (p3.x - p1.x) / (p3.y - p1.y), 
	       invslope2 = (p3.x - p2.x) / (p3.y - p2.y), 
	       curx1 = p3.x, 
	       curx2 = p3.x;
	for(double Y = p3.y; Y >p1.y; Y--){
		drawline2d(mkp(curx1,Y,lg),mkp(curx2,Y,0));
		curx1 -= invslope1;
		curx2 -= invslope2;}}
void scanln(tri tr,double lg){
	//tri tr = projt(rent(t,xr),screen);

	if(tr.p2.y < tr.p1.y){ swapp(&tr.p2, &tr.p1); }if(tr.p3.y <tr.p1.y){ swapp(&tr.p3, &tr.p1); }if(tr.p3.y < tr.p2.y){ swapp(&tr.p3, &tr.p2); }
	
	if(tr.p3.y == tr.p2.y){drawtup(tr,lg);}
	else if(tr.p1.y == tr.p2.y){drawtdown(tr,lg);}
	else{point p4 = mkp(inpolx(mkline(tr.p1,tr.p3),tr.p2.y),tr.p2.y,0);
	tri tu = mktri(tr.p1,tr.p2,p4);tri td = mktri(tr.p2,p4,tr.p3);
	drawtup(tu,lg);drawtdown(td,lg);}}

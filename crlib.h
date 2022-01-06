#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/ioctl.h>
#include <stdbool.h>
#include <math.h>
#include <curses.h>
#include "constgr.h"


/*	
	HEADER CONSTANT //will be move to main toggles file in future
*/

bool inToggle = false;


/*	
	GENERAL PURPOSE FUNCTIONS //will be move to general use header
*/	

double clamp(double n,double max,double min){if(n<min){n=min;}else if(n>max){n=max;}return n;}



/*	
	POINT OPERATION FUNCTIONS
*/	

//functions for point operations
point mkp(double x,double y,double z){point p = {x,y,z,1};return p;} //makes a point

void swapp(point *p1,point *p2){point tmp = *p1;*p1 = *p2;*p2 = tmp;}

bool chckp(point p1,point p2){if(p1.x==p2.x || p1.y==p2.y || (p1.z==p2.z && p1.z!=0 && p2.z!=0)){return true;}return false;} //checks if points are the same or not

double magp(point p){double mg = sqrt((p.x*p.x) + (p.y*p.y) + (p.z*p.z));return mg;}
double sign (point p1, point p2, point p3){return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);}
double dot(point p1,point p2){double dot = (p1.x*p2.x) + (p1.y*p2.y) + (p1.z*p2.z);return dot;}

point subp(point p1,point p2){point p3=mkp(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);return p3;} //subctracts second point from the first
point addp(point p1,point p2){point p3=mkp(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z);return p3;}
point divp(point p1,point p2){point p3=mkp(p1.x / p2.x, p1.y / p2.y, p1.z / p2.z);return p3;}
point mulp(point p1,point p2){point p3=mkp(p1.x * p2.x, p1.y * p2.y, p1.z * p2.z);return p3;}

point subpn(point p,double d){point p1=mkp(p.x-d,p.y-d,p.z-d); return p1;}
point addpn(point p,double d){point p1=mkp(p.x+d,p.y+d,p.z+d); return p1;}
point divpn(point p,double d){point p1=mkp(p.x/d,p.y/d,p.z/d); return p1;}
point mulpn(point p,double d){point p1=mkp(p.x*d,p.y*d,p.z*d); return p1;}

void chngx(point *p,double d){ p->x = d;}
void chngy(point *p,double d){ p->y = d;}
void chngz(point *p,double d){ p->z = d;}
void chngw(point *p,double d){ p->w = d;}

point negp(point p){point np = mkp(-p.x,-p.y,-p.z);return np;}

point normp(point p){
	double mg = magp(p);
	point pn = mkp(p.z/mg,p.y/mg,p.z/mg);
	return pn;}//normailize a vector
point tricenp(tri t){
	point p1 = t.p1,p2 = t.p2,p3 = t.p3;
	point p = mkp((p1.x+p2.x+p3.x)/3,(p1.y+p2.y+p3.y)/3,(p1.z+p2.z+p3.z)/3);
	return p;}
point crossp(point U ,point V){point c = mkp((U.y*V.z)-(U.z*V.y), (U.z*V.x)-(U.x*V.z), (U.x*V.y)-(U.y*V.x));return c;}

point normdcp(point p,point screen){point pw = {p.x/p.w,p.y/p.w,p.z/p.w,0};point op = mkp(( screen.x / 2 )*((-1*pw.x)+1),(screen.y / 2 )*((-1*pw.y)+1),0.5*pw.z + 0.5);return op;}

point multm(point p,mat4 m){
	point o;
	o.x = p.x * m.m[0][0] + p.y * m.m[1][0] + p.z * m.m[2][0] + p.w * m.m[3][0];
	o.y = p.x * m.m[0][1] + p.y * m.m[1][1] + p.z * m.m[2][1] + p.w * m.m[3][1];
	o.z = p.x * m.m[0][2] + p.y * m.m[1][2] + p.z * m.m[2][2] + p.w * m.m[3][2];
	o.w = p.x * m.m[0][3] + p.y * m.m[1][3] + p.z * m.m[2][3] + p.w * m.m[3][3];
	return o;}

mat4 qinvm(mat4 m){
		mat4 matrix;
		matrix.m[0][0] = m.m[0][0]; matrix.m[0][1] = m.m[1][0]; matrix.m[0][2] = m.m[2][0]; matrix.m[0][3] = 0;
		matrix.m[1][0] = m.m[0][1]; matrix.m[1][1] = m.m[1][1]; matrix.m[1][2] = m.m[2][1]; matrix.m[1][3] = 0;
		matrix.m[2][0] = m.m[0][2]; matrix.m[2][1] = m.m[1][2]; matrix.m[2][2] = m.m[2][2]; matrix.m[2][3] = 0;
		matrix.m[3][0] = -(m.m[3][0] * matrix.m[0][0] + m.m[3][1] * matrix.m[1][0] + m.m[3][2] * matrix.m[2][0]);
		matrix.m[3][1] = -(m.m[3][0] * matrix.m[0][1] + m.m[3][1] * matrix.m[1][1] + m.m[3][2] * matrix.m[2][1]);
		matrix.m[3][2] = -(m.m[3][0] * matrix.m[0][2] + m.m[3][1] * matrix.m[1][2] + m.m[3][2] * matrix.m[2][2]);
		matrix.m[3][3] = 1;
		return matrix;}
mat4 multms(mat4 m1,mat4 m2){
	mat4 matrix;
		for (int c = 0; c < 4; c++)
			for (int r = 0; r < 4; r++)
				matrix.m[r][c] = m1.m[r][0] * m2.m[0][c] + m1.m[r][1] * m2.m[1][c] + m1.m[r][2] * m2.m[2][c] + m1.m[r][3] * m2.m[3][c];
		return matrix;}




/*	
	LINE OPERATION FUNCTIONS
*/	

//functions for line opertaions p1 decided on y cordintate
line mkline(point p1,point p2){if(p1.y>p2.y){line l =  {p1,p2};return l;}else{line l = {p2,p1};return l;}} // makes a point

double inpolx(line l,double y){double x = l.p1.x + (l.p2.x - l.p1.x)*(y - l.p1.y)/(l.p2.y - l.p1.y);return x;}




/*	
	TRI OPERATION FUNCTIONS
*/	

//functions for triangle operations
tri mktri(point p1,point p2,point p3){tri t = {p1,p2,p3};return t;}
//makes tri with 9 numbers
tri amktri(double tarr[9]){tri t = mktri(mkp(tarr[0],tarr[1],tarr[2]),mkp(tarr[3],tarr[4],tarr[5]),mkp(tarr[6],tarr[7],tarr[8]));return t;}

void lnfy(tri t,line *l){
	line ll1 = mkline(t.p1,t.p2);
	line ll2 = mkline(t.p2,t.p3);
	line ll3 = mkline(t.p3,t.p1);	
	l[0] = ll1;
	l[1] = ll3;
	l[2] = ll2;}
void offsett(tri *t,point off){
	t->p1 = addp(t->p1,off);
	t->p2 = addp(t->p2,off);
	t->p3 = addp(t->p3,off);}

bool PointInTriangle (tri t, point p){
	point p1 = t.p1, p2 = t.p2, p3 = t.p3;
	double d1, d2, d3;
	bool has_neg, has_pos;
	d1 = sign(p, p1, p2);
	d2 = sign(p, p2, p3);
	d3 = sign(p, p3, p1);
	has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
	has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);
	return !(has_neg && has_pos);}

double calcshade(tri t,point s){
	point v = normp(s);
	point tn = trinorm(t);
	double out = dot(tn,v);
	return out;}

point trinorm(tri t){
	point U = subp(t.p2,t.p1),V = subp(t.p3,t.p1);
	point N = crossp(U,V);
	point Nn = normp(N);
	return Nn;}

tri psort(tri t){
	tri tr = t;
	if(tr.p2.y < tr.p1.y){ swapp(&tr.p2, &tr.p1); }
	if(tr.p3.y <tr.p1.y){ swapp(&tr.p3, &tr.p1); }
	if(tr.p3.y < tr.p2.y){ swapp(&tr.p3, &tr.p2); }
	if(tr.p1.y == tr.p2.y && tr.p1.x<tr.p2.x){ swapp(&tr.p1, &tr.p2); }
	if(tr.p3.y == tr.p2.y && tr.p3.x<tr.p2.x){ swapp(&tr.p3, &tr.p2); }
	return tr;}
//mesh mkmesh(point &pa,tri &ta){mesh n;n.ta = calloc(sizeof ta / sizeof ta[0] ,sizeof(tri*));n.pa = calloc(sizeof pa /sizeof pa[0],sizeof(point*));return n;} //i really need to implement meshes
	


/*	
	RENDER FUNCTIONS OPERATION FUNCTIONS
*/	

//rendering stuff or something
point projp(point p,point xyzr,point camor,point camoff){
	point cor = multm(p,spos(camor)); //projection with camera offset
	point camrot = multm(multm(multm(cor,crmz(xyzr.z)),crmy(xyzr.y)),crmx(xyzr.x));//projection with camera rotation
	point coff = multm(camrot,spos(camoff));
	return coff;}
	
point projpw(point p,point eye,point cen ,point up,point screen){
	point camrot = multm(p,multms(multms(multms(multms(multms(spos(mkp(0,0,0)),pscale(mkp(1,1,1))),crmz(0)),crmx(0)),spos(mkp(0,0,0))),idm()));
	point cor = multm(camrot,qinvm(camtr(eye,cen,up))); //projection with camera offset
	point coff = multm(cor,ppm(screen));	
	point enp = normdcp(coff,screen);
	return enp;}

point rend(point p,point screen){

	point prp = multm(p,pmm(screen)); //final point projection
	point pvp = multm(prp,pvm(prp.z));
	point fop = multm(pvp,voff(mkp(screen.x/2,screen.y/2,0)));
	return fop;}

tri rent(tri t,double r){
	point rc = mkp(r,r/2,r/3);
	point pc = mkp(0,0,0);
	point co = mkp(0,0,5);
	tri t1 = mktri(projp(t.p1,rc,pc,co),projp(t.p2,rc,pc,co),projp(t.p3,rc,pc,co));
	return t1;}
tri projt(tri t,point screen){
	tri t1 = mktri(rend(t.p1,screen),rend(t.p2,screen),rend(t.p3,screen));
	return t1;}

tri rentsym(tri t,point screen,double r){
	point rc = mkp(1.5,r/10,1.5);
	point pc = mkp(0,0,0);
	point co = mkp(0,-1,0);

	tri t1 = mktri(projpw(t.p1,rc,pc,co,screen),projpw(t.p2,rc,pc,co,screen),projpw(t.p3,rc,pc,co,screen));
	return t1;}


/*	
	DRAWING FUNCTIONS //will be moved to a seperate graphis handling library or 2d objects
*/

//scanline top and down triangle projection
void chshaded(point p){
	int offs = 0;
	double y=p.y, x=p.x, lg=p.z; //z is shade val
	
	if(lg>1){
	mvprintw(y/2+offs,x+offs,"@");}
	else if(lg<=1 && lg>0.9){
	mvprintw(y/2+offs,x+offs,"$");}
	else if(lg<=0.9 && lg>0.8){
	mvprintw(y/2+offs,x+offs,"#");}
	else if(lg<=0.8 && lg>0.7){
	mvprintw(y/2+offs,x+offs,"c");}
	else if(lg<=0.7 && lg>0.6){
	mvprintw(y/2+offs,x+offs,"+");}
	else if(lg<=0.6 && lg>0.5){
	mvprintw(y/2+offs,x+offs,"=");}
	else if(lg<=0.5 && lg>0.4){
	mvprintw(y/2+offs,x+offs,"-");}
	else if(lg<=0.4 && lg>0.3){
	mvprintw(y/2+offs,x+offs,":");}
	else if(lg<=0.3 && lg>0){
	mvprintw(y/2+offs,x+offs,".");}}
void drawline2d(point p1,point p2){
	double dx, dy, p, x, y;int offs = 20;double x1 = p1.x;double x2 = p2.x;dy=0;double x2l;
	if(x1<=x2){x=x1;dx=x2-x1;x2l = x2;}else{x=x2;dx=x1-x2;x2l = x1;}y=p1.y;p=2*dy-dx;
	double lg = p1.z;
	while(x<x2l){
		if(p>=0){chshaded(mkp(x,y,lg));y=y+1;p=p+2*dy-2*dx;}else{chshaded(mkp(x,y,lg));p=p+2*dy;}x=x+1;}}
void drawtup(tri t,double lg){
	point p1 = t.p1,p2 = t.p2,p3 = t.p3;
	double invslope1 = (p2.x - p1.x) / (p2.y - p1.y);
	double invslope2 = (p3.x - p1.x) / (p3.y - p1.y);
	
	double curx1 = p1.x;
	double curx2 = p1.x;
double offs = 20;
	for(double Y = p1.y; Y <= p2.y; Y++){
		drawline2d(mkp(curx1,Y,lg),mkp(curx2,Y,0));
		curx1 += invslope1;
		curx2 += invslope2;}}
void drawtdown(tri t,double lg){
	point p1 = t.p1,p2 = t.p2,p3 = t.p3;
	double invslope1 = (p3.x - p1.x) / (p3.y - p1.y);
	double invslope2 = (p3.x - p2.x) / (p3.y - p2.y);
	
	double curx1 = p3.x;
	double curx2 = p3.x;
	double offs = 20;
	for(double Y = p3.y; Y >p1.y; Y--){
		drawline2d(mkp(curx1,Y,lg),mkp(curx2,Y,0));
		curx1 -= invslope1;
		curx2 -= invslope2;}}
void scanln(tri t,double xr,double lg,point screen){
	//tri tr = projt(rent(t,xr),screen);
	tri tr = rentsym(t,screen,xr);
	if(tr.p2.y < tr.p1.y){ swapp(&tr.p2, &tr.p1); }if(tr.p3.y <tr.p1.y){ swapp(&tr.p3, &tr.p1); }if(tr.p3.y < tr.p2.y){ swapp(&tr.p3, &tr.p2); }
	
	if(tr.p3.y == tr.p2.y){drawtup(tr,lg);}
	else if(tr.p1.y == tr.p2.y){drawtdown(tr,lg);}
	else{point p4 = mkp(inpolx(mkline(tr.p1,tr.p3),tr.p2.y),tr.p2.y,0);
	tri tu = mktri(tr.p1,tr.p2,p4);tri td = mktri(tr.p2,p4,tr.p3);
	drawtup(tu,lg);drawtdown(td,lg);}}

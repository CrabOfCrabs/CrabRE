#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/ioctl.h>
#include <stdbool.h>
#include <math.h>
#include <curses.h>

// left handed system




//new data type for a 2d point cords
typedef struct points{float x;float y;float z;}point;
point subp(point p1,point p2);point normp(point p);float magp(point p);bool chckp(point p1,point p2);//declarations of point functions
//data type of a line
typedef struct lines{point p1;point p2;}line;
//data type of a triangle
typedef struct triangle{point p1;point p2;point p3;}tri;
point trinorm(tri t);
//data for mesh contains all points and tris with pointers to poiters to tris and pointers to pointers to poitnts define with mesh.pa = calloc(size,sizeof(point*)) same for tris
typedef struct meshes{tri** ta; point** pa;}mesh;

typedef struct mat4{float m[4][4];}mat4;



mat4 campos(point campos){
	mat4 pm;pm.m[0][1] = 0;pm.m[0][2] = 0;pm.m[0][3] = 0;pm.m[1][0] = 0;pm.m[1][2] = 0;pm.m[1][3] = 0;pm.m[2][0] = 0;pm.m[2][1] = 0;pm.m[2][3] = 0;
	pm.m[0][0] = 1;
	pm.m[1][1] = 1;
	pm.m[2][2] = 1;
	pm.m[3][3] = 1;
	pm.m[3][0] = -campos.x;
	pm.m[3][1] = -campos.y;
	pm.m[3][2] = -campos.z;
return pm;}mat4 crmx(float theta){
	mat4 pm;pm.m[0][1] = 0;pm.m[0][2] = 0;pm.m[0][3] = 0;pm.m[1][0] = 0;pm.m[1][3] = 0;pm.m[2][0] = 0;pm.m[2][3] = 0;pm.m[3][0] = 0;pm.m[3][1] = 0;pm.m[3][2] = 0;
	pm.m[0][0] = 1;
	pm.m[1][1] = cos(theta);
	pm.m[1][2] = sin(theta);
	pm.m[2][1] = -sin(theta);
	pm.m[2][2] = cos(theta);
	pm.m[3][3] = 1;
return pm;}mat4 crmy(float theta){
	mat4 pm;pm.m[0][1] = 0;pm.m[0][3] = 0;pm.m[1][0] = 0;pm.m[1][2] = 0;pm.m[1][3] = 0;pm.m[2][1] = 0;pm.m[2][3] = 0;pm.m[3][0] = 0;pm.m[3][1] = 0;pm.m[3][2] = 0;
	pm.m[1][1] = 1;
	pm.m[0][0] = cos(theta);
	pm.m[2][0] = sin(theta);
	pm.m[0][2] = -sin(theta);
	pm.m[2][2] = cos(theta);
	pm.m[3][3] = 1;

return pm;}mat4 crmz(float theta){
	mat4 pm;pm.m[0][2] = 0;pm.m[0][3] = 0;pm.m[1][2] = 0;pm.m[1][3] = 0;pm.m[2][0] = 0;pm.m[2][1] = 0;pm.m[2][3] = 0;pm.m[3][0] = 0;pm.m[3][1] = 0;pm.m[3][2] = 0;
	pm.m[0][0] = cos(theta);
	pm.m[0][1] = sin(theta);
	pm.m[1][0] = -sin(theta);
	pm.m[1][1] = cos(theta);
	pm.m[2][2] = 1;
	pm.m[3][3] = 1;
return pm;}
mat4 ppm(point screen){
	mat4 pm;pm.m[0][1] = 0;pm.m[0][2] = 0;pm.m[0][3] = 0;pm.m[1][2] = 0;pm.m[1][3] = 0;pm.m[2][0] = 0;pm.m[2][1] = 0;pm.m[2][3] = 0;pm.m[3][0] = 0;pm.m[3][1] = 0;pm.m[3][2] = 0;
	float f    = 0.1;
	float px   = screen.x/10000;
	float py   = screen.y/10000;
	float offx = screen.x;
	float offy = screen.y;
	float skew = 0;

	pm.m[0][0] = (f*screen.x)/(2*px);
	pm.m[1][0] = skew;
	pm.m[1][1] = (f*screen.y)/(2*py);
	pm.m[2][2] = -1;
	pm.m[3][3] = 1;
return pm;}
mat4 pvm(float z){
	mat4 pm;pm.m[0][1] = 0;pm.m[0][2] = 0;pm.m[0][3] = 0;pm.m[1][0] = 0;pm.m[1][2] = 0;pm.m[1][3] = 0;pm.m[2][0] = 0;pm.m[2][1] = 0;pm.m[2][3] = 0;pm.m[3][0] = 0;pm.m[3][1] = 0;pm.m[3][2] = 0;
	pm.m[0][0]=1/z;
	pm.m[1][1]=1/z;
	pm.m[2][2]=1;
	pm.m[3][3]=1;
return pm;}
mat4 voff(point off){
	mat4 pm;pm.m[0][1] = 0;pm.m[0][2] = 0;pm.m[0][3] = 0;pm.m[1][0] = 0;pm.m[1][2] = 0;pm.m[1][3] = 0;pm.m[2][0] = 0;pm.m[2][1] = 0;pm.m[2][3] = 0;pm.m[3][2] = 0;
	pm.m[0][0] = 1;
	pm.m[1][1] = -1;
	pm.m[2][2] = 1;
	pm.m[3][3] = 1;
	pm.m[3][0] = off.x;
	pm.m[3][1] = off.y;
return pm;}


point multm(point p,mat4 m){
	point o;
	o.x = p.x * m.m[0][0] + p.y * m.m[1][0] + p.z * m.m[2][0] + m.m[3][0];
	o.y = p.x * m.m[0][1] + p.y * m.m[1][1] + p.z * m.m[2][1] + m.m[3][1];
	o.z = p.x * m.m[0][2] + p.y * m.m[1][2] + p.z * m.m[2][2] + m.m[3][2];
	float w = p.x * m.m[0][3] + p.y * m.m[1][3] + p.z * m.m[2][3] + m.m[3][3];
	if (w != 0){o.x /= w; o.y /= w; o.z /= w;}
	return o;}




float clamp(float n,float max,float min){if(n<min){n=min;}else if(n>max){n=max;}return n;}





//functions for point operations
point mkp(float x,float y,float z){point p = {x,y,z};return p;} //makes a point

void swapp(point *p1,point *p2){point tmp = *p1;*p1 = *p2;*p2 = tmp;}

bool chckp(point p1,point p2){if(p1.x==p2.x || p1.y==p2.y || (p1.z==p2.z && p1.z!=0 && p2.z!=0)){return true;}return false;} //checks if points are the same or not

float magp(point p){float mg = sqrt((p.x*p.x) + (p.y*p.y) + (p.z*p.z));return mg;}
float sign (point p1, point p2, point p3){return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);}
float dot(point p1,point p2){float dot = (p1.x*p2.x) + (p1.y*p2.y) + (p1.z*p2.z);return dot;}

point subp(point p1,point p2){point p3=mkp(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);return p3;} //subctracts second point from the first
point addp(point p1,point p2){point p3=mkp(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z);return p3;}

point subpn(point p,float d){point p1=mkp(p.x-d,p.y-d,p.z-d); return p1;}
point addpn(point p,float d){point p1=mkp(p.x+d,p.y+d,p.z+d); return p1;}
point divpn(point p,float d){point p1=mkp(p.x/d,p.y/d,p.z/d); return p1;}
point mulpn(point p,float d){point p1=mkp(p.x*d,p.y*d,p.z*d); return p1;}

point chngx(point p,float d){point p1=mkp(d,p.y,p.z); return p1;}
point chngy(point p,float d){point p1=mkp(p.x,d,p.z); return p1;}
point chngz(point p,float d){point p1=mkp(p.x,p.y,d); return p1;}

point negp(point p){point np = mkp(-p.x,-p.y,-p.z);return np;}

point normp(point p){
	float mg = magp(p);
	point pn = mkp(p.z/mg,p.y/mg,p.z/mg);
	return pn;}//normailize a vector
point tricenp(tri t){
	point p1 = t.p1,p2 = t.p2,p3 = t.p3;
	point p = mkp((p1.x+p2.x+p3.x)/3,(p1.y+p2.y+p3.y)/3,(p1.z+p2.z+p3.z)/3);
	return p;}
point crossp(point U ,point V){point c = mkp((U.y*V.z)-(U.z*V.y), (U.z*V.x)-(U.x*V.z), (U.x*V.y)-(U.y*V.x));return c;}





//functions for line opertaions p1 decided on y cordintate
line mkline(point p1,point p2){if(p1.y>p2.y){line l =  {p1,p2};return l;}else{line l = {p2,p1};return l;}} // makes a point

float inpolx(line l,float y){float x = l.p1.x + (l.p2.x - l.p1.x)*(y - l.p1.y)/(l.p2.y - l.p1.y);return x;}







//functions for triangle operations
tri mktri(point p1,point p2,point p3){tri t = {p1,p2,p3};return t;}
//makes tri with 9 numbers
tri amktri(float tarr[9]){tri t = mktri(mkp(tarr[0],tarr[1],tarr[2]),mkp(tarr[3],tarr[4],tarr[5]),mkp(tarr[6],tarr[7],tarr[8]));return t;}

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
	float d1, d2, d3;
	bool has_neg, has_pos;
	d1 = sign(p, p1, p2);
	d2 = sign(p, p2, p3);
	d3 = sign(p, p3, p1);
	has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
	has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);
	return !(has_neg && has_pos);}

float calcshade(tri t,point s){
	point v = normp(s);
	point tn = trinorm(t);
	float out = dot(tn,v);
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

//mesh mkmesh(point &pa,tri &ta){mesh n;n.ta = calloc(sizeof ta / sizeof ta[0] ,sizeof(tri*));n.pa = calloc(sizeof pa /sizeof pa[0],sizeof(point*));return n;}

	



//predefined object 	
void mkcube(tri *obj,float size){
	
	float smax = size/2;
	float smin = -size/2;
	float t1[9]={ smin,smin,smin,smin,smax,smin,smax,smax,smin };
	float t2[9]={ smin,smin,smin,smax,smax,smin,smax,smin,smin };
	float t3[9]={ smax,smin,smin,smax,smax,smin,smax,smax,smax };
	float t4[9]={ smax,smin,smin,smax,smax,smax,smax,smin,smax};
	float t5[9]={ smax,smin,smax,smax,smax,smax,smin,smax,smax };
	float t6[9]={ smax,smin,smax,smin,smax,smax,smin,smin,smax };
	float t7[9]={ smin,smin,smax,smin,smax,smax,smin,smax,smin };
	float t8[9]={ smin,smin,smax,smin,smax,smin,smin,smin,smin };
	float t9[9]={ smin,smax,smin,smin,smax,smax,smax,smax,smax };
	float t10[9]={ smin,smax,smin,smax,smax,smax,smax,smax,smin };
	float t11[9]={ smax,smin,smax,smin,smin,smax,smin,smin,smin };
	float t12[9]={ smax,smin,smax,smin,smin,smin,smax,smin,smin };
	                                                      	
	obj[0] = amktri(t1);
	obj[1] = amktri(t2);
	obj[2] = amktri(t3);
	obj[3] = amktri(t4);
	obj[4] = amktri(t5);
	obj[5] = amktri(t6);
	obj[6] = amktri(t7);
	obj[7] = amktri(t8);
	obj[8] = amktri(t9);
	obj[9] = amktri(t10);
	obj[10] = amktri(t11);
	obj[11] = amktri(t12);}



//rendering stuff or something
point projp(point p,point xyzr,point camor,point camoff){
	point cor = multm(p,campos(camor)); //projection with camera offset
	point camrot = multm(multm(multm(cor,crmz(xyzr.z)),crmy(xyzr.y)),crmx(xyzr.x));//projection with camera rotation
	point coff = multm(camrot,campos(camoff));
	return coff;}
point rend(point p,point screen){

	point prp = multm(p,ppm(screen)); //final point projection
	point pvp = multm(prp,pvm(prp.z));
	point fop = multm(pvp,voff(mkp(screen.x/2,screen.y/2,0)));
	return fop;}

tri rent(tri t,float r){
	point rc = mkp(r,r/2,r/3);
	point pc = mkp(0,0,0);
	point co = mkp(0,0,5);
	tri t1 = mktri(projp(t.p1,rc,pc,co),projp(t.p2,rc,pc,co),projp(t.p3,rc,pc,co));
	return t1;}
tri projt(tri t,point screen){
	tri t1 = mktri(rend(t.p1,screen),rend(t.p2,screen),rend(t.p3,screen));
	return t1;}






//scanline top and down triangle projection

void chshaded(point p){
	int offs = 0;
	float y=p.y, x=p.x, lg=p.z; //z is shade val
	
	if(lg>1){
	mvprintw(y/2+offs,x+offs,"@");}
	else if(lg<=1 && lg>0.9){
	mvprintw(y/2+offs,x+offs,"b");}
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
	mvprintw(y/2+offs,x+offs,".");}
}

void drawline2d(point p1,point p2){
	float dx, dy, p, x, y;int offs = 20;float x1 = p1.x;float x2 = p2.x;dy=0;float x2l;
	if(x1<=x2){x=x1;dx=x2-x1;x2l = x2;}else{x=x2;dx=x1-x2;x2l = x1;}y=p1.y;p=2*dy-dx;
	float lg = p1.z;
	while(x<x2l){
		if(p>=0){chshaded(mkp(x,y,lg));y=y+1;p=p+2*dy-2*dx;}else{chshaded(mkp(x,y,lg));p=p+2*dy;}x=x+1;}}
void drawtup(tri t,float lg){
	point p1 = t.p1,p2 = t.p2,p3 = t.p3;
	float invslope1 = (p2.x - p1.x) / (p2.y - p1.y);
	float invslope2 = (p3.x - p1.x) / (p3.y - p1.y);
	
	float curx1 = p1.x;
	float curx2 = p1.x;
float offs = 20;
	for(float Y = p1.y; Y <= p2.y; Y++){
		drawline2d(mkp(curx1,Y,lg),mkp(curx2,Y,0));
		curx1 += invslope1;
		curx2 += invslope2;}}
void drawtdown(tri t,float lg){
	point p1 = t.p1,p2 = t.p2,p3 = t.p3;
	float invslope1 = (p3.x - p1.x) / (p3.y - p1.y);
	float invslope2 = (p3.x - p2.x) / (p3.y - p2.y);
	
	float curx1 = p3.x;
	float curx2 = p3.x;
	float offs = 20;
	for(float Y = p3.y; Y >p1.y; Y--){
		drawline2d(mkp(curx1,Y,lg),mkp(curx2,Y,0));
		curx1 -= invslope1;
		curx2 -= invslope2;}}
void scanln(tri t,float xr,float lg,point screen){
	tri tr = projt(rent(t,xr),screen);
	if(tr.p2.y < tr.p1.y){ swapp(&tr.p2, &tr.p1); }if(tr.p3.y <tr.p1.y){ swapp(&tr.p3, &tr.p1); }if(tr.p3.y < tr.p2.y){ swapp(&tr.p3, &tr.p2); }
	
	if(tr.p3.y == tr.p2.y){drawtup(tr,lg);}
	else if(tr.p1.y == tr.p2.y){drawtdown(tr,lg);}
	else{point p4 = mkp(inpolx(mkline(tr.p1,tr.p3),tr.p2.y),tr.p2.y,0);
	tri tu = mktri(tr.p1,tr.p2,p4);tri td = mktri(tr.p2,p4,tr.p3);
	drawtup(tu,lg);drawtdown(td,lg);}}

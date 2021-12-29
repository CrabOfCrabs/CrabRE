#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/ioctl.h>
#include <stdbool.h>
#include <math.h>

//new data type for a 2d point cords
typedef struct points{double x;double y;double z;}point;
point subp(point p1,point p2);point normp(point p);point trinorm(point p1,point p2,point p3);double magp(point p);bool chckp(point p1,point p2);//declarations of point functions
//data type of a line
typedef struct lines{point p1;point p2;}line;
//data type of a triangle
typedef struct triangle{point p1;point p2;point p3;point n;bool exists;}tri;
//data for mesh contains all points and tris with pointers to poiters to tris and pointers to pointers to poitnts define with mesh.pa = calloc(size,sizeof(point*)) same for tris
typedef struct meshes{tri** ta; point** pa;}mesh;

typedef struct mat4{double m[4][4];}mat4;

mat4 campos(point campos){
	mat4 pm;
	
	pm.m[0][1] = 0;
	pm.m[0][2] = 0;
	pm.m[0][3] = 0;
	pm.m[1][0] = 0;
	pm.m[1][2] = 0;
	pm.m[1][3] = 0;
	pm.m[2][0] = 0;
	pm.m[2][1] = 0;
	pm.m[2][3] = 0;


	pm.m[0][0] = 1;
	pm.m[1][1] = 1;
	pm.m[2][2] = 1;
	pm.m[3][3] = 1;
	pm.m[3][0] = -campos.x;
	pm.m[3][1] = -campos.y;
	pm.m[3][2] = -campos.z;
return pm;
}
//camera rotate matrix x
mat4 crmx(double theta){
	mat4 pm;	
	
	pm.m[0][1] = 0;
	pm.m[0][2] = 0;
	pm.m[0][3] = 0;
	pm.m[1][0] = 0;
	pm.m[1][3] = 0;
	pm.m[2][0] = 0;
	pm.m[2][3] = 0;
	pm.m[3][0] = 0;
	pm.m[3][1] = 0;
	pm.m[3][2] = 0;

	pm.m[0][0] = 1;
	pm.m[1][1] = cos(theta);
	pm.m[1][2] = sin(theta);
	pm.m[2][1] = -sin(theta);
	pm.m[2][2] = cos(theta);
	pm.m[3][3] = 1;
return pm;
}
//camera rotate matrix x
mat4 crmy(double theta){
	mat4 pm;
	
	pm.m[0][1] = 0;
	pm.m[0][3] = 0;
	pm.m[1][0] = 0;
	pm.m[1][2] = 0;
	pm.m[1][3] = 0;
	pm.m[2][1] = 0;
	pm.m[2][3] = 0;
	pm.m[3][0] = 0;
	pm.m[3][1] = 0;
	pm.m[3][2] = 0;

	pm.m[1][1] = 1;
	pm.m[0][0] = cos(theta);
	pm.m[2][0] = sin(theta);
	pm.m[0][2] = -sin(theta);
	pm.m[2][2] = cos(theta);
	pm.m[3][3] = 1;

return pm;
}
//camera rotate matrix x
mat4 crmz(double theta){
	mat4 pm;

	pm.m[0][2] = 0;
	pm.m[0][3] = 0;
	pm.m[1][2] = 0;
	pm.m[1][3] = 0;
	pm.m[2][0] = 0;
	pm.m[2][1] = 0;
	pm.m[2][3] = 0;
	pm.m[3][0] = 0;
	pm.m[3][1] = 0;
	pm.m[3][2] = 0;

	pm.m[0][0] = cos(theta);
	pm.m[0][1] = sin(theta);
	pm.m[1][0] = -sin(theta);
	pm.m[1][1] = cos(theta);
	pm.m[2][2] = 1;
	pm.m[3][3] = 1;
return pm;
}

//perspective projection matrix
mat4 ppm(){
	mat4 pm;

	pm.m[0][1] = 0;
	pm.m[0][2] = 0;
	pm.m[0][3] = 0;
	pm.m[1][2] = 0;
	pm.m[1][3] = 0;
	pm.m[2][0] = 0;
	pm.m[2][1] = 0;
	pm.m[2][3] = 0;
	pm.m[3][0] = 0;
	pm.m[3][1] = 0;
	pm.m[3][2] = 0;

	double f    = 0.1;
	double px   = 0.01;
	double py   = 0.01;
	double offx = 100/2;
	double offy = 100/2;
	double skew = 0;

	pm.m[0][0] = (f*100)/(2*px);
	pm.m[1][0] = skew;
	pm.m[1][1] = (f*100)/(2*py);
	pm.m[2][2] = -1;

	pm.m[3][3] = 1;

return pm;
}

mat4 pvm(double z){
	mat4 pm;

	pm.m[0][1] = 0;
	pm.m[0][2] = 0;
	pm.m[0][3] = 0;
	pm.m[1][0] = 0;
	pm.m[1][2] = 0;
	pm.m[1][3] = 0;
	pm.m[2][0] = 0;
	pm.m[2][1] = 0;
	pm.m[2][3] = 0;
	pm.m[3][0] = 0;
	pm.m[3][1] = 0;
	pm.m[3][2] = 0;

	pm.m[0][0]=1/z;
	pm.m[1][1]=1/z;
	pm.m[2][2]=1;
	pm.m[3][3]=1;
return pm;
}

mat4 voff(point off){
	mat4 pm;

	pm.m[0][1] = 0;
	pm.m[0][2] = 0;
	pm.m[0][3] = 0;
	pm.m[1][0] = 0;
	pm.m[1][2] = 0;
	pm.m[1][3] = 0;
	pm.m[2][0] = 0;
	pm.m[2][1] = 0;
	pm.m[2][3] = 0;
	pm.m[3][2] = 0;

	pm.m[0][0] = 1;
	pm.m[1][1] = -1;
	pm.m[2][2] = 1;
	pm.m[3][3] = 1;
	pm.m[3][0] = off.x;
	pm.m[3][1] = off.y;
return pm;
}


//functions for point operations
point mkp(double x,double y,double z){point p = {x,y,z};return p;} //makes a point

point subp(point p1,point p2){point p3=mkp(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);return p3;} //subctracts second point from the first
point addp(point p1,point p2){point p3=mkp(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z);return p3;}

point subpn(point p,double d){point p1=mkp(p.x-d,p.y-d,p.z-d); return p1;}
point addpn(point p,double d){point p1=mkp(p.x+d,p.y+d,p.z+d); return p1;}
point divpn(point p,double d){point p1=mkp(p.x/d,p.y/d,p.z/d); return p1;}
point mulpn(point p,double d){point p1=mkp(p.x*d,p.y*d,p.z*d); return p1;}

point chngx(point p,double d){point p1=mkp(d,p.y,p.z); return p1;}
point chngy(point p,double d){point p1=mkp(p.x,d,p.z); return p1;}
point chngz(point p,double d){point p1=mkp(p.x,p.y,d); return p1;}

point normp(point p){
	double mg = magp(p);
	point pn = mkp(p.z/mg,p.y/mg,p.z/mg);
	return pn;}//normailize a vector

point tricenp(point p1,point p2,point p3){
	point p = mkp((p1.x+p2.x+p3.x)/3,(p1.y+p2.y+p3.y)/3,(p1.z+p2.z+p3.z)/3);
	return p;}

point crossp(point U ,point V){point c = mkp((U.y*V.z)-(U.z*V.y), (U.z*V.x)-(U.x*V.z), (U.x*V.y)-(U.y*V.x));return c;}

double magp(point p){double mg = sqrt((p.x*p.x) + (p.y*p.y) + (p.z*p.z));return mg;}
bool chckp(point p1,point p2){if(p1.x==p2.x || p1.y==p2.y || (p1.z==p2.z && p1.z!=0 && p2.z!=0)){return true;}return false;} //checks if points are the same or not





//functions fr line opertaions
line mkline(point p1,point p2){line l = {p1,p2};return l;} // makes a point






//functions for triangle operations
tri mktri(point p1,point p2,point p3){if(!(chckp(p1,p2)) && !(chckp(p2,p3)) && !(chckp(p1,p3))){tri t1 = {p1,p2,p3,trinorm(p1,p2,p3),true};return t1;}tri t2 = {p1,p2,p3,trinorm(p1,p2,p3),false};return t2;} //makes a triangle
tri amktri(double tarr[9]){tri t = mktri(mkp(tarr[0],tarr[1],tarr[2]),mkp(tarr[3],tarr[4],tarr[5]),mkp(tarr[6],tarr[7],tarr[8]));return t;}

double inpolx(line l,double y){
	double x = l.p1.x + (l.p2.x - l.p1.x)*(y - l.p1.y)/(l.p2.y - l.p1.y);
	return x;
}

void lnfy(tri t,line *l){
	(t.p1.y>t.p2.y) ? l[0] = mkline(t.p1,t.p2) : mkline(t.p2,t.p1);
	(t.p2.y>t.p3.y) ? l[1] = mkline(t.p2,t.p3) : mkline(t.p3,t.p2);
	(t.p3.y>t.p1.y) ? l[2] = mkline(t.p3,t.p1) : mkline(t.p1,t.p3);
}

//mesh mkmesh(point &pa,tri &ta){mesh n;n.ta = calloc(sizeof ta / sizeof ta[0] ,sizeof(tri*));n.pa = calloc(sizeof pa /sizeof pa[0],sizeof(point*));return n;}
double clamp(double n,double max,double min){if(n<min){n=min;}else if(n>max){n=max;}return n;}
		

double sign (point p1, point p2, point p3){
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool PointInTriangle (tri t, point p){
	point p1 = t.p1, p2 = t.p2, p3 = t.p3;
	double d1, d2, d3;
	bool has_neg, has_pos;
	d1 = sign(p, p1, p2);
	d2 = sign(p, p2, p3);
	d3 = sign(p, p3, p1);
	has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
	has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);
	return !(has_neg && has_pos);
}

double dot(point p1,point p2){
	double dot = (p1.x*p2.x) + (p1.y*p2.y) + (p1.z*p2.z);
	return dot;
}

//triangle normal now for free!!!
point trinorm(point p1,point p2,point p3){
	point U = subp(p2,p1),V = subp(p3,p1);
	point N = crossp(U,V);
	point Nn = normp(N);
	return Nn;
}

double calcshade(tri t,point s){
	point v = normp(subp(s,tricenp(t.p1,t.p2,t.p3)));
	point tn = trinorm(t.p1,t.p2,t.p3);
	double out = dot(tn,v);
	return out;
}
point multm(point p,mat4 m){
	point o;
	o.x = p.x * m.m[0][0] + p.y * m.m[1][0] + p.z * m.m[2][0] + m.m[3][0];
	o.y = p.x * m.m[0][1] + p.y * m.m[1][1] + p.z * m.m[2][1] + m.m[3][1];
	o.z = p.x * m.m[0][2] + p.y * m.m[1][2] + p.z * m.m[2][2] + m.m[3][2];
	double w = p.x * m.m[0][3] + p.y * m.m[1][3] + p.z * m.m[2][3] + m.m[3][3];

	if (w != 0)
	{
		o.x /= w; o.y /= w; o.z /= w;
	}
	return o;
}
void mkcube(tri *obj){

	double t1[9]={ -0.5, -0.5, -0.5,    -0.5, 0.5, -0.5,    0.5, 0.5, -0.5 };
	double t2[9]={ -0.5, -0.5, -0.5,    0.5, 0.5, -0.5,    0.5, -0.5, -0.5 };
	double t3[9]={ 0.5, -0.5, -0.5,    0.5, 0.5, -0.5,    0.5, 0.5, 0.5 };
	double t4[9]={ 0.5, -0.5, -0.5,    0.5, 0.5, 0.5,    0.5, -0.5, 0.5 };
	double t5[9]={ 0.5, -0.5, 0.5,    0.5, 0.5, 0.5,    -0.5, 0.5, 0.5 };
	double t6[9]={ 0.5, -0.5, 0.5,    -0.5, 0.5, 0.5,    -0.5, -0.5, 0.5 };
	double t7[9]={ -0.5, -0.5, 0.5,    -0.5, 0.5, 0.5,    -0.5, 0.5, -0.5 };
	double t8[9]={ -0.5, -0.5, 0.5,    -0.5, 0.5, -0.5,    -0.5, -0.5, -0.5 };
	double t9[9]={ -0.5, 0.5, -0.5,    -0.5, 0.5, 0.5,    0.5, 0.5, 0.5 };
	double t10[9]={ -0.5, 0.5, -0.5,    0.5, 0.5, 0.5,    0.5, 0.5, -0.5 };
	double t11[9]={ 0.5, -0.5, 0.5,    -0.5, -0.5, 0.5,    -0.5, -0.5, -0.5 };
	double t12[9]={ 0.5, -0.5, 0.5,    -0.5, -0.5, -0.5,    0.5, -0.5, -0.5 };

	                                                      
	
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
	obj[11] = amktri(t12);
}

point projp(point p,point xyzr,point camor,point camoff){

	//point pointrot = multm(multm(multm(subp(p,orgin),crmz(xyzr.z)),crmy(xyzr.y)),crmx(xyzr.x))//point rotation
	point cor = multm(p,campos(camor)); //projection with camera offset
	point camrot = multm(multm(multm(cor,crmz(xyzr.z)),crmy(xyzr.y)),crmx(xyzr.x));//projection with camera rotation
	point coff = multm(camrot,campos(camoff));
	point prp = multm(coff,ppm()); //final point projection
	point pvp = multm(prp,pvm(prp.z));
	point fop = multm(pvp,voff(mkp(50,50,0)));
	return fop;
}


tri rent(tri t,double r){
	point rc = mkp(r,r,r);
	point pc = mkp(0,0,0);
	point co = mkp(0,0,5);
	tri t1 = mktri(projp(t.p1,rc,pc,co),projp(t.p2,rc,pc,co),projp(t.p3,rc,pc,co));
	return t1;
}



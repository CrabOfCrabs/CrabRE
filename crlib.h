#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/ioctl.h>
#include <math.h>
#include <stdbool.h>
#include <curses.h>
#include <string.h>





/*
           STRUCTURES FOR GRAPHICS
*/

//new data type for a 2d point cords
typedef struct points{double x;double y;double z;double w;}point;
//data type of a line
typedef struct lines{point p1;point p2;}line;

typedef struct triangle{point p1;point p2;point p3;double gs;}tri;
//data for mesh contains all points and tris with pointers to poiters to tris and pointers to pointers to poitnts define
//   with mesh.pa = calloc(size,sizeof(point*)) same for tris
typedef struct meshes{int n;tri tap[];}mesh; //mesh made of points and triangles that have pointers to points)

typedef struct mat4{double m[4][4];}mat4;

typedef struct lookatcamera{point atV;point toV;point upV;}camera;



point mkp(double x,double y,double z);
void swapp(point *p1,point *p2);
bool chckp(point p1,point p2);//checks if points are the same or not


double magp(point p);double sign(point p1, point p2, point p3);double dot(point p1,point p2);
point subp(point p1,point p2);point addp(point p1, point p2);point divp(point p1,point p2);point mulp(point p1, point p2);
point addp(point p1, point p2);point subpn(point p, double d);point divpn(point p, double d);point mulpn(point p,double d);
void chngx(point *p,double d);void chngy(point *p,double d);void chngz(point *p,double d);void chngw(point *p,double d);
point negp(point p);
point normp(point p);
point tricenp(tri t);
point crossp(point U, point V);//declarations of point functions
point normdcp(point p,point screen);
point multm(point p,mat4 m);

mat4 qinvm(mat4 m);
mat4 multms(mat4 m1,mat4 m2);
line mkline(point p1,point p2);
double inpolx(line l,double y);
tri mktri(point p1,point p2,point p3);
tri amktri(double tarr[9]);
void lnfy(tri t,line *l);
void offsett(tri *t,point off);
bool PointInTriangle (tri t, point p);
int compare_function(const void *a,const void *b);
double calcshade(tri t,point s);
point trinorm(tri t);
tri psort(tri t);
mesh* mkmesh(tri ta[]);


mat4 idm();
mat4 spos(point pos);mat4 inspos(point pos);
mat4 pscale(point scale);
mat4 crmx(double theta);mat4 crmy(double theta);mat4 crmz(double theta);
mat4 camtr(point eye,point cen,point up);
mat4 ppm(point screen);
void mkcube(tri *obj);
void mkpir(tri *obj);
void mkfile(tri *obj);

double clamp(double n,double max,double min);
point tranp(point p,point xyzr,point objor,point objoff,point objsize);
tri trant(tri t,point rc,point pc,point co,point os);
point getcampo(point p,camera rcam);
void chshaded(point p);
void drawline2d(point p1,point p2);
void drawtup(tri t,double lg);
void drawtdown(tri t,double lg);
void scanln(tri tr,double lg);

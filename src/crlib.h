#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/ioctl.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>




/*
           STRUCTURES FOR GRAPHICS
*/

//new data type for a 2d Vec4 cords
typedef struct Vec4s{double x;double y;double z;double w;}Vec4;
//data type of a line
typedef struct lines{Vec4 p1;Vec4 p2;}line;

typedef struct {Vec4 p1;Vec4 p2;Vec4 p3;double gs;}tri;
//data for mesh contains all Vec4s and tris with Vec4ers to poiters to tris and Vec4ers to Vec4ers to poitnts define
//   with mesh.pa = calloc(size,sizeof(Vec4*)) same for tris
typedef struct meshes{int n;tri tap[];}mesh; //mesh made of Vec4s and triangles that have Vec4ers to Vec4s)

typedef struct mat4{double m[4][4];}mat4;

typedef struct lookatcamera{Vec4 atV;Vec4 toV;Vec4 upV;}camera;



Vec4 mkp(double x,double y,double z);
void swapp(Vec4 *p1,Vec4 *p2);
bool chckp(Vec4 p1,Vec4 p2);//checks if Vec4s are the same or not


double magp(Vec4 p);double sign(Vec4 p1, Vec4 p2, Vec4 p3);double dot(Vec4 p1,Vec4 p2);
Vec4 subp(Vec4 p1,Vec4 p2);Vec4 addp(Vec4 p1, Vec4 p2);Vec4 divp(Vec4 p1,Vec4 p2);Vec4 mulp(Vec4 p1, Vec4 p2);
Vec4 addp(Vec4 p1, Vec4 p2);Vec4 subpn(Vec4 p, double d);Vec4 divpn(Vec4 p, double d);Vec4 mulpn(Vec4 p,double d);
void chngx(Vec4 *p,double d);void chngy(Vec4 *p,double d);void chngz(Vec4 *p,double d);void chngw(Vec4 *p,double d);
Vec4 negp(Vec4 p);
Vec4 normp(Vec4 p);
Vec4 tricenp(tri t);
Vec4 crossp(Vec4 U, Vec4 V);//declarations of Vec4 functions
Vec4 normdcp(Vec4 p,Vec4 screen);
Vec4 multm(Vec4 p,mat4 m);

mat4 qinvm(mat4 m);
mat4 multms(mat4 m1,mat4 m2);
line mkline(Vec4 p1,Vec4 p2);
double inpolx(line l,double y);
tri mktri(Vec4 p1,Vec4 p2,Vec4 p3);
tri amktri(double tarr[9]);
void lnfy(tri t,line *l);
void offsett(tri *t,Vec4 off);
bool Vec4InTriangle (tri t, Vec4 p);
int compare_function(const void *a,const void *b);
double calcshade(tri t,Vec4 s);
Vec4 trinorm(tri t);
tri psort(tri t);
mesh* mkmesh(tri ta[]);


mat4 idm();
mat4 spos(Vec4 pos);mat4 inspos(Vec4 pos);
mat4 pscale(Vec4 scale);
mat4 crmx(double theta);mat4 crmy(double theta);mat4 crmz(double theta);
mat4 camtr(Vec4 eye,Vec4 cen,Vec4 up);
mat4 ppm(Vec4 screen);
void mkcube(tri *obj);
void mkpir(tri *obj);
void mkfile(tri *obj);

double clamp(double n,double max,double min);
Vec4 tranp(Vec4 p,Vec4 xyzr,Vec4 objor,Vec4 objoff,Vec4 objsize);
tri trant(tri t,Vec4 rc,Vec4 pc,Vec4 co,Vec4 os);
Vec4 getcampo(Vec4 p,camera rcam);

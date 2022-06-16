#include "crlib.h"

/*	
	Vec4 OPERATION FUNCTIONS
*/	


double clamp(double n,double max,double min){if(n<min){n=min;}else if(n>max){n=max;}return n;}

//functions for Vec4 operations
Vec4 mkp(double x,double y,double z){Vec4 p = {x,y,z,1};return p;} //makes a Vec4

void swapp(Vec4 *p1,Vec4 *p2){Vec4 tmp = *p1;*p1 = *p2;*p2 = tmp;}

bool chckp(Vec4 p1,Vec4 p2){if(p1.x==p2.x || p1.y==p2.y || (p1.z==p2.z && p1.z!=0 && p2.z!=0)){return true;}return false;} //checks if Vec4s are the same or not

double magp(Vec4 p){double mg = sqrt((p.x*p.x) + (p.y*p.y) + (p.z*p.z));return mg;}
double sign (Vec4 p1, Vec4 p2, Vec4 p3){return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);}
double dot(Vec4 p1,Vec4 p2){double dot = (p1.x*p2.x) + (p1.y*p2.y) + (p1.z*p2.z);return dot;}

Vec4 subp(Vec4 p1,Vec4 p2){Vec4 p3=mkp(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);return p3;} //subctracts second Vec4 from the first
Vec4 addp(Vec4 p1,Vec4 p2){Vec4 p3=mkp(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z);return p3;}
Vec4 divp(Vec4 p1,Vec4 p2){Vec4 p3=mkp(p1.x / p2.x, p1.y / p2.y, p1.z / p2.z);return p3;}
Vec4 mulp(Vec4 p1,Vec4 p2){Vec4 p3=mkp(p1.x * p2.x, p1.y * p2.y, p1.z * p2.z);return p3;}

Vec4 subpn(Vec4 p,double d){Vec4 p1=mkp(p.x-d,p.y-d,p.z-d); return p1;}
Vec4 addpn(Vec4 p,double d){Vec4 p1=mkp(p.x+d,p.y+d,p.z+d); return p1;}
Vec4 divpn(Vec4 p,double d){Vec4 p1=mkp(p.x/d,p.y/d,p.z/d); return p1;}
Vec4 mulpn(Vec4 p,double d){Vec4 p1=mkp(p.x*d,p.y*d,p.z*d); return p1;}

void chngx(Vec4 *p,double d){ p->x = d;}
void chngy(Vec4 *p,double d){ p->y = d;}
void chngz(Vec4 *p,double d){ p->z = d;}
void chngw(Vec4 *p,double d){ p->w = d;}

Vec4 negp(Vec4 p){Vec4 np = mkp(-p.x,-p.y,-p.z);return np;}

Vec4 normp(Vec4 p){
	double mg = magp(p);
	Vec4 pn = mkp(p.x/mg,p.y/mg,p.z/mg);
	return pn;}//normailize a vector
Vec4 tricenp(tri t){
	Vec4 p = mkp((t.p1.x+t.p2.x+t.p3.x)/3,(t.p1.y+t.p2.y+t.p3.y)/3,(t.p1.z+t.p2.z+t.p3.z)/3);
	return p;}
Vec4 crossp(Vec4 U ,Vec4 V){Vec4 c = mkp((U.y*V.z)-(U.z*V.y), (U.z*V.x)-(U.x*V.z), (U.x*V.y)-(U.y*V.x));return c;}

Vec4 normdcp(Vec4 p,Vec4 screen){Vec4 pw = mkp(p.x/p.w,p.y/p.w,p.z/p.w);Vec4 op = { screen.x * 0.5 *(pw.x+1),screen.y * 0.5 * (pw.y+1),0.5*pw.z + 0.5,0};return op;}

Vec4 multm(Vec4 p,mat4 m){
	Vec4 o;
	o.x = p.x * m.m[0][0] + p.y * m.m[1][0] + p.z * m.m[2][0] + p.w * m.m[3][0];
	o.y = p.x * m.m[0][1] + p.y * m.m[1][1] + p.z * m.m[2][1] + p.w * m.m[3][1];
	o.z = p.x * m.m[0][2] + p.y * m.m[1][2] + p.z * m.m[2][2] + p.w * m.m[3][2];
	o.w = p.x * m.m[0][3] + p.y * m.m[1][3] + p.z * m.m[2][3] + p.w * m.m[3][3];
	return o;}
mat4 qinvm(mat4 m){//only for lookat inverse
		mat4 mat;
		mat.m[0][0] = m.m[0][0]; mat.m[0][1] = m.m[1][0]; mat.m[0][2] = m.m[2][0]; mat.m[0][3] = 0;
		mat.m[1][0] = m.m[0][1]; mat.m[1][1] = m.m[1][1]; mat.m[1][2] = m.m[2][1]; mat.m[1][3] = 0;
		mat.m[2][0] = m.m[0][2]; mat.m[2][1] = m.m[1][2]; mat.m[2][2] = m.m[2][2]; mat.m[2][3] = 0;
		mat.m[3][0] = -(m.m[3][0] * mat.m[0][0] + m.m[3][1] * mat.m[1][0] + m.m[3][2] * mat.m[2][0]);
		mat.m[3][1] = -(m.m[3][0] * mat.m[0][1] + m.m[3][1] * mat.m[1][1] + m.m[3][2] * mat.m[2][1]);
		mat.m[3][2] = -(m.m[3][0] * mat.m[0][2] + m.m[3][1] * mat.m[1][2] + m.m[3][2] * mat.m[2][2]);
		mat.m[3][3] = 1;
		return mat;}		
mat4 multms(mat4 m1,mat4 m2){
	mat4 mat;
		for (int c = 0; c < 4; c++)
			for (int r = 0; r < 4; r++)
				mat.m[r][c] = m1.m[r][0] * m2.m[0][c] + m1.m[r][1] * m2.m[1][c] + m1.m[r][2] * m2.m[2][c] + m1.m[r][3] * m2.m[3][c];
return mat;}





/*	
	LINE OPERATION FUNCTIONS
*/	

//functions for line opertaions p1 decided on y cordintate
line mkline(Vec4 p1,Vec4 p2){if(p1.y>p2.y){line l =  {p1,p2};return l;}else{line l = {p2,p1};return l;}} // makes a Vec4

double inpolx(line l,double y){double x = l.p1.x + (l.p2.x - l.p1.x)*(y - l.p1.y)/(l.p2.y - l.p1.y);return x;}






/*	
	TRI OPERATION FUNCTIONS
*/	

//functions for triangle operations
tri mktri(Vec4 p1,Vec4 p2,Vec4 p3){tri t = {p1,p2,p3,0};return t;}
//makes tri with 9 numbers
tri amktri(double tarr[9]){tri t = mktri(mkp(tarr[0],tarr[1],tarr[2]),mkp(tarr[3],tarr[4],tarr[5]),mkp(tarr[6],tarr[7],tarr[8]));return t;}

void lnfy(tri t,line *l){
	line ll1 = mkline(t.p1,t.p2);
	line ll2 = mkline(t.p2,t.p3);
	line ll3 = mkline(t.p3,t.p1);	
	l[0] = ll1;
	l[1] = ll3;
	l[2] = ll2;}
void offsett(tri *t,Vec4 off){
	t->p1 = addp(t->p1,off);
	t->p2 = addp(t->p2,off);
	t->p3 = addp(t->p3,off);}

bool Vec4InTriangle (tri t, Vec4 p){
	Vec4 p1 = t.p1, p2 = t.p2, p3 = t.p3;
	double d1, d2, d3;
	bool has_neg, has_pos;
	d1 = sign(p, p1, p2);
	d2 = sign(p, p2, p3);
	d3 = sign(p, p3, p1);
	has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
	has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);
	return !(has_neg && has_pos);}

int compare_function(const void *a,const void *b){
	tri *x = (tri *) a;
	tri *y = (tri *) b;

	double z1 = (x->p1.z + x->p2.z + x->p3.z)/3;
	double z2 = (y->p1.z + y->p2.z + y->p3.z)/3;
	if(z1 < z2){
		return -1;}
	else if(z1 > z2){
		return 1;}
	else{return 0;}}

double calcshade(tri t,Vec4 s){
	Vec4 v = normp(s);
	Vec4 tn = trinorm(t);
	double out = dot(v,tn);
	return out;}

Vec4 trinorm(tri t){
	Vec4 U = subp(t.p2,t.p1),V = subp(t.p3,t.p1);
	Vec4 N = crossp(U,V);
	Vec4 Nn = normp(N);
	return Nn;}

tri psort(tri t){
	tri tr = t;
	if(tr.p2.y < tr.p1.y){ swapp(&tr.p2, &tr.p1); }
	if(tr.p3.y <tr.p1.y){ swapp(&tr.p3, &tr.p1); }
	if(tr.p3.y < tr.p2.y){ swapp(&tr.p3, &tr.p2); }
	if(tr.p1.y == tr.p2.y && tr.p1.x<tr.p2.x){ swapp(&tr.p1, &tr.p2); }
	if(tr.p3.y == tr.p2.y && tr.p3.x<tr.p2.x){ swapp(&tr.p3, &tr.p2); }
return tr;}
                                                                        
//this shit moves your tringle / Vec4 to world space but doesnt have to you can just predefine the rotation
Vec4 tranp(Vec4 p,Vec4 xyzr,Vec4 objor,Vec4 objoff,Vec4 objsize){             
    Vec4 transformed = multm(p,multms(multms(multms(multms(multms(multms(
        //shittone f matrix multiplications and then multiply p by final matrix
        spos(objor),//origin
        pscale(objsize)),//rescale
        crmz(xyzr.z)),crmy(xyzr.y)),crmx(xyzr.x)),//zyx rotation
        spos(objoff)),//final offset
        idm()));//identity matrix
return transformed;}
tri trant(tri t,Vec4 rc,Vec4 pc,Vec4 co,Vec4 os){
        tri t1 = mktri(tranp(t.p1,rc,pc,co,os),tranp(t.p2,rc,pc,co,os),tranp
(t.p3,rc,pc,co,os));
return t1;}

Vec4 getcampo(Vec4 p,camera rcam){Vec4 pct = multm(p,camtr(rcam.atV,rcam.toV,rcam.upV));return pct;}

/*	
	MESH OPERATION FUNCTIONS
*/

//mesh* mkmesh(tri ta[]){size_t st = sizeof(*ta)/sizeof(ta[0]);mesh *n = malloc(sizeof(mesh) + sizeof(tri [st]));return n;} //i really need to implement meshes



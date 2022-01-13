


/*	
	STRUCTURES FOR GRAPHICS
*/	

//new data type for a 2d point cords
typedef struct points{double x;double y;double z;double w;}point;
double dot(point p1,point p2);point subp(point p1,point p2);point divp(point p1,point p2);point mulpn(point p,double d);point normp(point p);double magp(point p);bool chckp(point p1,point p2);point crossp(point U, point V);//declarations of point functions
//data type of a line
typedef struct lines{point p1;point p2;}line;
//data type of a triangle
typedef struct triangle{point p1;point p2;point p3;}tri;
tri amktri(double tarr[8]);point trinorm(tri t);
//data for mesh contains all points and tris with pointers to poiters to tris and pointers to pointers to poitnts define with mesh.pa = calloc(size,sizeof(point*)) same for tris
typedef struct meshes{int n;tri tap[];}mesh; //mesh made of points and triangles that have pointers to points)

typedef struct mat4{double m[4][4];}mat4;

typedef struct lookatcamera{point atV;point toV;point upV;}camera;





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

point normdcp(point p,point screen){point pw = mkp(p.x/p.w,p.y/p.w,p.z/p.w);point op = { screen.x * 0.5 *(-1*pw.x+1),screen.y * 0.5 * (-1*pw.y+1),0.5*pw.z + 0.5,0};return op;}

point multm(point p,mat4 m){
	point o;
	o.x = p.x * m.m[0][0] + p.y * m.m[1][0] + p.z * m.m[2][0] + p.w * m.m[3][0];
	o.y = p.x * m.m[0][1] + p.y * m.m[1][1] + p.z * m.m[2][1] + p.w * m.m[3][1];
	o.z = p.x * m.m[0][2] + p.y * m.m[1][2] + p.z * m.m[2][2] + p.w * m.m[3][2];
	o.w = p.x * m.m[0][3] + p.y * m.m[1][3] + p.z * m.m[2][3] + p.w * m.m[3][3];
	return o;}
mat4 qinvm(mat4 m){//only for lookat inverse
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
}
/*	
	MESH OPERATION FUNCTIONS
*/

mesh* mkmesh(tri ta[]){size_t st = sizeof(*ta)/sizeof(ta[0]);mesh *n = malloc(sizeof(mesh) + sizeof(tri [st]));return n;} //i really need to implement meshes



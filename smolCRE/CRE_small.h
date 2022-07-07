#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define V3loop(int) for(int=0;int<=2;int++)

typedef struct {double d[3];}V3; //3 dimensional vector because that's all we'll need
typedef struct {V3 *v[3];}T; //Triangle with reference because memory less
typedef struct {unsigned int ta,va;T *t;V3 *v;V3 transform[3];}M; //mesh transform 1 -> offset ,2 -> rotation ,3 -> scale 
typedef struct {double x[3],y[3];double z,shade;}Tp; //primitive triangle
typedef struct {double FOV,FAR,NEAR;V3 CAM_V,UP,VT;}Cam;

typedef struct {double x,y,z,w;}Vec4;
typedef struct {Vec4 p1,p2,p3;}Tri;
typedef struct {Vec4 p1,p2;}line;
Vec4 mkp(double x,double y,double z){Vec4 p = {x,y,z,1};return p;} //makes a Vec4

void swapp(Vec4 *p1,Vec4 *p2){Vec4 tmp = *p1;*p1 = *p2;*p2 = tmp;}
line mkline(Vec4 p1,Vec4 p2){if(p1.y>p2.y){line l =  {p1,p2};return l;}else{line l = {p2,p1};return l;}} // makes a Vec4
Tri mkTri(Vec4 p1,Vec4 p2,Vec4 p3){Tri t = {p1,p2,p3};return t;}
double inpolx(line l,double y){double x = l.p1.x + (l.p2.x - l.p1.x)*(y - l.p1.y)/(l.p2.y - l.p1.y);return x;}

/*	
	RENDER FUNCTIONS 
*/	


/*	
	DRAWING FUNCTIONS //will be moved to a seperate graphis handling library or 2d objects
*/

//scanline top and down Triangle projection
void chshaded(Vec4 p){
	 
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

void drawline2d(Vec4 p1,Vec4 p2){ //p1 z is the shade of Tri
	double x=0, y=0, x1 = p1.x, x2 = p2.x, x2l=0;

	if(x1<x2){x=x1;x2l = x2;}else{x=x2;x2l = x1;}y=p1.y;
	while(x<=x2l){
		chshaded(mkp(x,y,p1.z));x=x+1;}}

void drawtup(Tri t,double lg){ //draws the line parallel to y up
	Vec4 p1 = t.p1,p2 = t.p2,p3 = t.p3;p1.y = floor(p1.y);
	double invslope1 = (p2.x - p1.x) / (p2.y - p1.y),
	       invslope2 = (p3.x - p1.x) / (p3.y - p1.y),
	       curx1 = p1.x, 
	       curx2 = p1.x;
	for(int Y = p1.y; Y < p2.y; Y++){
		drawline2d(mkp(curx1,Y,lg),mkp(curx2,Y,0));
		curx1 += invslope1;
		curx2 += invslope2;}}
void drawtdown(Tri t,double lg){//draws the line parallel to y down
	Vec4 p1 = t.p1,p2 = t.p2,p3 = t.p3;p3.y = ceil(p3.y);
	double invslope1 = (p3.x - p1.x) / (p3.y - p1.y), 
	       invslope2 = (p3.x - p2.x) / (p3.y - p2.y), 
	       curx1 = p3.x, 
	       curx2 = p3.x;	
	for(int Y = p3.y; Y > p1.y; Y--){
		drawline2d(mkp(curx1,Y,lg),mkp(curx2,Y,0));
		curx1 -= invslope1;
		curx2 -= invslope2;}}
void TriRaster(Tri tr,double lg){
	if(tr.p2.y < tr.p1.y){ swapp(&tr.p2, &tr.p1); }if(tr.p3.y <tr.p1.y){ swapp(&tr.p3, &tr.p1); }if(tr.p3.y < tr.p2.y){ swapp(&tr.p3, &tr.p2); }
	
	if(tr.p3.y == tr.p2.y){drawtup(tr,lg);}
	else if(tr.p1.y == tr.p2.y){drawtdown(tr,lg);}
	else{Vec4 p4 = mkp(inpolx(mkline(tr.p1,tr.p3),tr.p2.y),tr.p2.y,0);
	Tri tu = mkTri(tr.p1,tr.p2,p4);Tri td = mkTri(tr.p2,p4,tr.p3);
	drawtup(tu,lg);drawtdown(td,lg);}}





void CRE_M_load(M *m,char *F_p){
    m->va=0;m->ta=0;
    m->v = (V3*)malloc(sizeof(V3));
    m->t = (T*)malloc(sizeof(T));
    FILE *fp = fopen(F_p,"r");
    if(fp == NULL){exit(-1);}
    char lb[512];
    bool nchk = false,tchk = false;
    short int cnt = 0,p[3],tr;
    while(fgets(lb,512,fp)){if(lb[0]!=' ' && lb[1]==' '){switch(lb[0]){
                case 'v':
                    if(m->va==0){}else{m->v = (V3*)realloc(m->v,sizeof(V3)*(m->va+1));}
                    sscanf(lb,"v %lf %lf %lf",&m->v[m->va].d[0],&m->v[m->va].d[1],&m->v[m->va].d[2]);
                    m->va++; break;
                case 'f':
                    switch(cnt){
                    case 0:sscanf(lb,"f %d %d %d"
                    ,&p[0],&p[1],&p[2]);break;
                    case 1:sscanf(lb,"f %d/%d %d/%d %d/%d"
                    ,&p[0],&tr,&p[1],&tr,&p[2],&tr);break;
                    case 2:sscanf(lb,"f %d/%d/%d %d/%d/%d %d/%d/%d"
                    ,&p[0],&tr,&tr,&p[1],&tr,&tr,&p[2],&tr,&tr);break;}
                    if(m->ta == 0){}else{m->t = (T*)realloc(m->t,sizeof(T)*(m->ta+1));}
                    m->t[m->ta]=(T){&m->v[p[0]-1],&m->v[p[1]-1],&m->v[p[2]-1]};
                    m->ta++;break;
                default:break;}
        }else if(lb[0]=='v' &&  lb[2]==' '){
            if(lb[1]=='n' && nchk==false){nchk = true;cnt++;} 
            else if(lb[1]=='t' && tchk==false){tchk = true;cnt++;}}}
}


void CRE_M_render(M *m,Cam cam,int S[2],void(*put_pixel)(int,int,double)){
    double ZLM,FOV=cam.FOV,FAR=cam.FAR,NEAR=cam.NEAR;V3 CAM_V=cam.CAM_V,UP=cam.UP,VT=cam.VT;
    Tp TtR_A[m->ta];
    unsigned int TtR = 0,i1,i2;
    //TO CAMERA SPACE MATRIX 
    V3 yc,zc,wc;
    V3loop(i1){zc.d[i1] = VT.d[i1];}
    double asprt,fovr,ycl,zcd=0,zcl=sqrt(zc.d[0]*zc.d[0]+zc.d[1]*zc.d[1]+zc.d[2]*zc.d[2]);
    V3loop(i1){zc.d[i1]/= zcl;zcd+=(UP.d[i1]*zc.d[i1]);}
    V3loop(i1){yc.d[i1] = UP.d[i1]-(zc.d[i1]*zcd);ycl+=yc.d[i1]*yc.d[i1];}
    ycl=sqrt(ycl);yc.d[0]/= ycl;yc.d[1]/= ycl;yc.d[2]/= ycl;
    
    V3 xc = {{yc.d[1]*zc.d[2]-yc.d[2]*zc.d[1],
        yc.d[2]*zc.d[0]-yc.d[0]*zc.d[2],
        yc.d[0]*zc.d[1]-yc.d[1]*zc.d[0]}};
    V3loop(i1){wc.d[0]+=CAM_V.d[i1]*xc.d[i1];}
    V3loop(i1){wc.d[1]+=CAM_V.d[i1]*yc.d[i1];}
    V3loop(i1){wc.d[2]+=CAM_V.d[i1]*zc.d[i1];}

    double CAM_M[4][4]={{xc.d[0],yc.d[0],zc.d[0],0},
        {xc.d[1],yc.d[1],zc.d[1],0},
        {xc.d[2],yc.d[2],zc.d[2],0},
        {-wc.d[0],-wc.d[1],-wc.d[2],1}};

    fovr  = 1/tan(FOV*0.5/180*M_PI);
    asprt = (double)S[0]/S[1];

    for(int Ti = 0;Ti<m->ta;Ti++){
        V3 p[3] = {*m->t[Ti].v[0],*m->t[Ti].v[1],*m->t[Ti].v[2]},w,
        Tcrs,Tcv; 
        
        V3loop(i1){ //TRIANGLE NORMAL
            int U=(i1+1)%3,V=(i1+2)%3;
            Tcrs.d[i1]= (p[1].d[U]-p[0].d[U])*(p[2].d[V]-p[0].d[V])-
                        (p[1].d[V]-p[0].d[V])*(p[2].d[U]-p[0].d[U]);
            Tcv.d[i1]= ((p[0].d[i1]+p[1].d[i1]+p[2].d[i1])/3)-CAM_V.d[i1];}
        double depth,tshade,Tcm=sqrt(Tcrs.d[0]*Tcrs.d[0]+Tcrs.d[1]*Tcrs.d[1]+Tcrs.d[2]*Tcrs.d[2]),
               Tcvm=sqrt(Tcv.d[0]*Tcv.d[0]+Tcv.d[1]*Tcv.d[1]+Tcv.d[2]*Tcv.d[2]);
        Tcrs.d[0]/=Tcm;Tcrs.d[1]/=Tcm;Tcrs.d[2]/=Tcm;Tcv.d[0]/=Tcvm;Tcv.d[1]/=Tcvm;Tcv.d[2]/=Tcvm;   
        tshade = (Tcrs.d[0]*Tcv.d[0]+Tcrs.d[1]*Tcv.d[1]+Tcrs.d[2]*Tcv.d[2]);  
        
        if(tshade<0){TtR_A[TtR].shade=-tshade; //BACKFACE CULLING & FLAT SHADE
                V3loop(i1){V3 pcp = p[i1];V3loop(i2){ //WORLD TO CAMERA
                    p[i1].d[i2] = pcp.d[0]*CAM_M[0][i2]+pcp.d[1]*CAM_M[1][i2]+
                    pcp.d[2]*CAM_M[2][i2]+CAM_M[3][i2];}
                w.d[i1] = pcp.d[0]*CAM_M[0][3]+pcp.d[1]*CAM_M[1][3]+
                pcp.d[2]*CAM_M[2][3]+CAM_M[3][3];}
            
            V3loop(i1){ //FUCK ME IS THIS THE PROJECTION MATRIX!111!!!1112!!
                ZLM=p[i1].d[2]; // Z LIVES MATTER
                p[i1].d[0] = p[i1].d[0]*(asprt*fovr);
                p[i1].d[1] = p[i1].d[1]*fovr;
                p[i1].d[2] = (p[i1].d[2]*(FAR/(FAR-NEAR)))+w.d[i1]*((-FAR*NEAR)/(FAR-NEAR));
                w.d[i1]=ZLM; // Z LIVES MATTER
                }

            V3loop(i1){ //TO NORMAL SPACE NIGGA
                p[i1].d[0]=S[0]*0.5*((p[i1].d[0]/w.d[i1])+1);
                p[i1].d[1]=S[1]*0.5*((p[i1].d[1]/w.d[i1])+1);
                p[i1].d[2]=0.5*(p[i1].d[2]/w.d[i1])+0.5;w.d[i1]=0;}
            
            V3loop(i1){ //CALCULATE AVERAGE DISTANCE AND PUT 2D TRIANGLE INTO ARRAY
                depth+=p[i1].d[2];
                TtR_A[TtR].x[i1]=p[i1].d[0];TtR_A[TtR].y[i1]=p[i1].d[1];}
            TtR_A[TtR].z = depth/3;
            TtR++;}}
    
    for(int i=1;i<TtR;i++){ //INSERTION SORT INSPIRED BY small3dlib
        Tp tmp = TtR_A[i];int j=i-1;
        while(j>=0&&TtR_A[j].z<tmp.z){TtR_A[j+1]=TtR_A[j];j--;}
    TtR_A[j+1]=tmp;}

    for(int i=0;i<TtR;i++){
        Tri trend = {{TtR_A[i].x[0],TtR_A[i].y[0],0,0},{TtR_A[i].x[1],TtR_A[i].y[1],0,0},{TtR_A[i].x[2],TtR_A[i].y[2],0,0}};
        TriRaster(trend,TtR_A[i].shade);
        



        /* 
           V3loop(i1){
           int pst=(i1+1)%3,pnd=(i1+2)%3;
           if(lT.y[pst]<lT.y[pnd]){
           int tmpV[2]={lT.x[pst],lT.y[pst]};
           lT.x[pst]=lT.x[pnd];lT.y[pst]=lT.y[pnd];
           lT.x[pnd]=tmpV[0];lT.y[pnd]=tmpV[1];}}
        
        if(lT.y[2]=lT.y[1]){}
        else if(lT.y[0]==lT.y[1]){}
        else{double np}
        
         void drawline2d(Vec4 p1,Vec4 p2){ //p1 z is the shade of Tri
	double x=0, y=0, x1 = p1.x, x2 = p2.x, x2l=0;

	if(x1<x2){x=x1;x2l = x2;}else{x=x2;x2l = x1;}y=p1.y;
	while(x<=x2l){
		chshaded(mkp(x,y,p1.z));x=x+1;}}

void drawtup(Tri t,double lg){ //draws the line parallel to y up
	Vec4 p1 = t.p1,p2 = t.p2,p3 = t.p3;p1.y = floor(p1.y);
	double invslope1 = (p2.x - p1.x) / (p2.y - p1.y),
	       invslope2 = (p3.x - p1.x) / (p3.y - p1.y),
	       curx1 = p1.x, 
	       curx2 = p1.x;
	for(int Y = p1.y; Y < p2.y; Y++){
		drawline2d(mkp(curx1,Y,lg),mkp(curx2,Y,0));
		curx1 += invslope1;
		curx2 += invslope2;}}
void drawtdown(Tri t,double lg){//draws the line parallel to y down
	Vec4 p1 = t.p1,p2 = t.p2,p3 = t.p3;p3.y = ceil(p3.y);
	double invslope1 = (p3.x - p1.x) / (p3.y - p1.y), 
	       invslope2 = (p3.x - p2.x) / (p3.y - p2.y), 
	       curx1 = p3.x, 
	       curx2 = p3.x;	
	for(int Y = p3.y; Y > p1.y; Y--){
		drawline2d(mkp(curx1,Y,lg),mkp(curx2,Y,0));
		curx1 -= invslope1;
		curx2 -= invslope2;}}
	if(tr.p2.y < tr.p1.y){ swapp(&tr.p2, &tr.p1); }if(tr.p3.y <tr.p1.y){ swapp(&tr.p3, &tr.p1); }if(tr.p3.y < tr.p2.y){ swapp(&tr.p3, &tr.p2); }
	
	if(tr.p3.y == tr.p2.y){drawtup(tr,lg);}
	else if(tr.p1.y == tr.p2.y){drawtdown(tr,lg);}
	else{Vec4 p4 = mkp(inpolx(mkline(tr.p1,tr.p3),tr.p2.y),tr.p2.y,0);
	Tri tu = mkTri(tr.p1,tr.p2,p4);Tri td = mkTri(tr.p2,p4,tr.p3);
	drawtup(tu,lg);drawtdown(td,lg);}       
    
    V3loop(i1){int U=(i1+1)%3,V=(i1+2)%3;
    if(TtR[i].y[U]<TtR[i].y[V]){
        double tmp[2]={TtR[i].x[U],TtR[i].y[U]};
        TtR[i].x[U]=TtR[i].x[V];TtR[i].y[U]=TtR[i].y[V];
        TtR[i].x[V]=tmp[0];TtR[i].y[V]=tmp[1];}}
    */
    }}

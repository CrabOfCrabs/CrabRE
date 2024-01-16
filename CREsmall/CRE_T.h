#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>


#define FOR(n) for(int i=0;i<n;i+=1)
#define V3l(int) for(int=0;int<=2;int++)
enum dimension{x=0,y,z,w};
typedef struct {float d[3];}V3; //3 dimensional vector because that's all we'll need
typedef struct {V3 *v[3];}T; //Triangle with reference because memory less
typedef struct {unsigned int ta,va;T *t;V3 *v;}M; //groups triangles and vectors into meshes 
typedef struct {float txy[3][2];float z,shade;}Tp; //primitive triangle
typedef struct {float FOV,FAR,NEAR;V3 CAM_V,UP,VT;}Cam;

size_t CRE_LF=sizeof(float);

//main functions
void CRE_M_load(M *m,char *F_p);
void CRE_M_render(M m,Cam cam,int S[2],void(*drawPixel)(int,int,float));

//extra functions
V3 CRE_Q_rotate(V3 u,float a,V3 v);




V3 CRE_Q_rotate(V3 u,float a,V3 v){    // u - rotation axis , a - rotation in radians , v - vector to rotate
	float mdir = (1-cos(a))*(u.d[x]*v.d[x]+u.d[y]*v.d[y]+u.d[z]*v.d[z]);
	return (V3){
    cos(a)*v.d[x]+mdir*u.d[x]+sin(a)*(u.d[y]*v.d[z]-u.d[z]*v.d[y]),
		cos(a)*v.d[y]+mdir*u.d[y]+sin(a)*(u.d[z]*v.d[x]-u.d[x]*v.d[z]),
		cos(a)*v.d[z]+mdir*u.d[z]+sin(a)*(u.d[x]*v.d[y]-u.d[y]*v.d[x])
	};
}



void CRE_M_load(M *m,char *F_p){
    m->va=0;m->ta=0; 
    FILE *fp = fopen(F_p,"r");
    if(fp == NULL){exit(-1);}
    char lb[512];
    bool nchk = false,tchk = false;
    int cnt = 0,p[3];
    while(fgets(lb,512,fp)){if(lb[0]!=' ' && lb[1]==' '){switch(lb[0]){
                case 'v':
                    if(m->va==0){m->v = (V3*)malloc(sizeof(V3));}else{m->v = (V3*)realloc(m->v,sizeof(V3)*(m->va+1));}
                    sscanf(lb,"v %f %f %f",&m->v[m->va].d[0],&m->v[m->va].d[1],&m->v[m->va].d[2]);
                    m->va++; break;
                case 'f':
                    sscanf(lb,"f %d %d %d",&p[0],&p[1],&p[2]);
                    if(m->ta == 0){m->t = (T*)malloc(sizeof(T));}else{m->t = (T*)realloc(m->t,sizeof(T)*(m->ta+1));}
                    m->t[m->ta]=(T){&m->v[p[0]-1],&m->v[p[1]-1],&m->v[p[2]-1]};
                    m->ta++;break;
                default:break;}}}
    fclose(fp);}


void CRE_M_render(M m,Cam cam,int S[2],void(*drawPixel)(int,int,float)){
    V3 CAM_V=cam.CAM_V,UP=cam.UP,VT=cam.VT;
    float CAM_M[4][4]={0},ycl=0,zcd=0,zcl=0,FOV=cam.FOV,FAR=cam.FAR,NEAR=cam.NEAR,fovr=1/tan(FOV*0.5/180*3.14159);
    Tp TtR_A[m.ta];
    unsigned int TtR = 0,i1,i2,i3;
    //TO CAMERA SPACE MATRIX  
    V3 dc[4]={0};
    V3l(i1){dc[z].d[i1] = VT.d[i1];zcl+=VT.d[i1]*VT.d[i1];}
    zcl=sqrt(zcl);
    V3l(i1){dc[z].d[i1]/= zcl;zcd+=(UP.d[i1]*dc[z].d[i1]);}
    V3l(i1){dc[y].d[i1] = UP.d[i1]-(dc[z].d[i1]*zcd);ycl+=dc[y].d[i1]*dc[y].d[i1];}
    ycl=sqrt(ycl);V3l(i1){dc[y].d[i1]/= ycl;}
    
    V3l(i1){int U=(i1+1)%3,V=(i1+2)%3;
        dc[x].d[i1]=dc[y].d[U]*dc[z].d[V]-dc[y].d[V]*dc[z].d[U];}
    V3l(i1){V3l(i2){dc[w].d[i1]+=CAM_V.d[i2]*dc[i1].d[i2];}
        V3l(i2){CAM_M[i1][i2]=dc[i2].d[i1];}CAM_M[3][i1]=-dc[w].d[i1];}
    CAM_M[3][3]=1;
    float ProjM[3]={S[y]/(float)S[x]*fovr,fovr,FAR/(FAR-NEAR)};

    FOR(m.ta){
        V3 p[3]={*m.t[i].v[x],*m.t[i].v[y],*m.t[i].v[z]},w,
        Tcrs,Tcv={0};
        V3l(i1){ //TRIANGLE NORMAL
            int U=(i1+1)%3,V=(i1+2)%3;
            Tcrs.d[i1]= (p[1].d[U]-p[0].d[U])*(p[2].d[V]-p[0].d[V])-
                        (p[1].d[V]-p[0].d[V])*(p[2].d[U]-p[0].d[U]);
            V3l(i2){Tcv.d[i1]+=p[i2].d[i1]/3;}
            Tcv.d[i1]-=CAM_V.d[i1];}
        float ZLM,tshade=0,Tcm=0,Tcvm=0;
        V3l(i1){Tcvm+=Tcv.d[i1]*Tcv.d[i1];Tcm+=Tcrs.d[i1]*Tcrs.d[i1];}
        V3l(i1){tshade+=(Tcrs.d[i1]/sqrt(Tcm))*(Tcv.d[i1]/sqrt(Tcvm));}   
        
        if(tshade<0){
        TtR_A[TtR].shade=-tshade; //BACKFACE CULLING & FLAT SHADE
                V3l(i1){V3 pcp = p[i1];V3l(i2){ //WORLD TO CAMERA
                    p[i1].d[i2] = pcp.d[0]*CAM_M[0][i2]+pcp.d[1]*CAM_M[1][i2]+
                    pcp.d[2]*CAM_M[2][i2]+CAM_M[3][i2];}
                w.d[i1] = pcp.d[0]*CAM_M[0][3]+pcp.d[1]*CAM_M[1][3]+
                pcp.d[2]*CAM_M[2][3]+CAM_M[3][3];
                ZLM=p[i1].d[z]; // Z LIVES MATTER
                V3l(i2){p[i1].d[i2]*=ProjM[i2];} 
                p[i1].d[z]+=w.d[i1]*((-FAR*NEAR)/(FAR-NEAR));
                w.d[i1]=ZLM;
                p[i1].d[x]=(float)S[x]/2*((p[i1].d[x]/w.d[i1])+1);
                p[i1].d[y]=(float)S[y]/2*((p[i1].d[y]/w.d[i1])+1);
                p[i1].d[z]=(p[i1].d[z]/w.d[i1])/2+0.5;
                memcpy(TtR_A[TtR].txy[i1],p[i1].d,2*CRE_LF);
                TtR_A[TtR].z=0;
                TtR_A[TtR].z+=p[i1].d[z];}
            TtR_A[TtR].z/=3;TtR++;}}
   FOR(TtR-1){ //INSERTION SORT INSPIRED BY small3dlib
        Tp tmp = TtR_A[i+1];int j=i;
        while(j>=0&&TtR_A[j].z>tmp.z){TtR_A[j+1]=TtR_A[j];j--;}
        TtR_A[j+1]=tmp;}

 

    FOR(TtR){
        float lT[3][2];
        memcpy(lT,TtR_A[i].txy,6*CRE_LF);
        V3l(i1){int dI[3]={1,1,0};
            int pst=dI[2-i1]+1,pnd=(dI[i1]-1)*-1;
            if(lT[pst][y]<lT[pnd][y]){
                float tmpV[2]={lT[pst][x],lT[pst][y]};
                lT[pst][x]=lT[pnd][x];lT[pst][y]=lT[pnd][y];
                lT[pnd][x]=tmpV[x];lT[pnd][y]=tmpV[y];}}
        int udn=-1;
        float tud[2][3][2]; //[2] <-- two possible triangles ;[3] <-- three points ;[2] <-- 2d vector
        if(lT[2][y]==lT[1][y]){udn=0;}
        else if(lT[0][y]==lT[1][y]){udn=1;}
        else{float p4x=lT[0][x]+(lT[2][x]-lT[0][x])*(lT[1][y]-lT[0][y])/(lT[2][y]-lT[0][y]); 
            memcpy(tud,(float[2][3][2]){{{lT[0][x],lT[0][y]},{lT[1][x],lT[1][y]},{p4x,lT[1][y]}},
                    {{lT[1][x],lT[1][y]},{p4x,lT[1][y]},{lT[2][x],lT[2][y]}}},12*CRE_LF);}
       
        if(udn!=-1){memcpy(tud[udn],lT,6*CRE_LF);}

        if(udn==-1||udn==0){
            tud[0][0][y]=floor(tud[0][0][y]);
            float invs1=(tud[0][1][x]-tud[0][0][x])/(tud[0][1][y]-tud[0][0][y]),
                   invs2=(tud[0][2][x]-tud[0][0][x])/(tud[0][2][y]-tud[0][0][y]);
            for(int cruxm=0,Y=tud[0][0][y];Y<tud[0][1][y];Y++,cruxm++){
                float x1=0,x2=0,tmx1=tud[0][0][x]+cruxm*invs1,tmx2=tud[0][0][x]+cruxm*invs2;
                if(tmx1<tmx2){x1=tmx1;x2=tmx2;}
                else{x2=tmx1;x1=tmx2;}
                while(x1<=x2){drawPixel(x1,Y,TtR_A[i].shade);x1=x1+1;}}}

        if(udn==-1||udn==1){
            tud[1][2][y]=ceil(tud[1][2][y]);
            float invs1=(tud[1][2][x]-tud[1][0][x])/(tud[1][2][y]-tud[1][0][y]),
                   invs2=(tud[1][2][x]-tud[1][1][x])/(tud[1][2][y]-tud[1][1][y]);
            for(int cruxm=0,Y=tud[1][2][y];Y>tud[1][0][y];Y--,cruxm--){
                float x1=0,x2=0,tmx1=tud[1][2][x]+cruxm*invs1,tmx2=tud[1][2][x]+cruxm*invs2;
                if(tmx1<tmx2){x1=tmx1;x2=tmx2;}
                else{x2=tmx1;x1=tmx2;}
                while(x1<=x2){drawPixel(x1,Y,TtR_A[i].shade);x1=x1+1;}}}}}

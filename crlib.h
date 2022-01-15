#include "typegr.h"
#include "constgr.h"


/*	
	HEADER CONSTANT //will be move to main toggles file in future
*/
point zero = {0,0,0,1};
point def_target = {0,0,1,1};
point camSpace_up = {0,1,0,1};

bool inToggle = false;
camera lcam;

/*	
	GENERAL PURPOSE FUNCTIONS //will be move to general use header
*/	

double clamp(double n,double max,double min){if(n<min){n=min;}else if(n>max){n=max;}return n;}




//void update_cam(){camera *c; c = &lcam;c->atV = at; c->toV = to;c->upV = camSpace_up;}
void atto_cam(point at,point to){camera *c; c = &lcam;c->atV = at; c->toV = to;c->upV = camSpace_up;}



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

//rotate camur
point ctranp(point p,point xyzr,point objoff){
	mat4 idtran = idm();
	idtran = multms(crmz(xyzr.z),crmx(xyzr.x));
	idtran = multms(idtran,spos(objoff));
	point pfinal = multm(p,idtran);
return pfinal;}
tri ctrant(tri t,point rc,point co){
	tri t1 = mktri(ctranp(t.p1,rc,co),ctranp(t.p2,rc,co),ctranp(t.p3,rc,co));
return t1;}

point allinoneprojc(point p,point screen){
	point cor = multm(p,qinvm(camtr(lcam.atV,lcam.toV,lcam.upV))); 
return cor;}
tri ziprentc(tri t,point screen){
	tri t1 = mktri( allinoneprojc(t.p1,screen),
			allinoneprojc(t.p2,screen),
			allinoneprojc(t.p3,screen));
return t1;}

//this shit does all the things for you but sometimes its doing some weird shit
point allinoneproj(point p,point screen){
	point cor = multm(p,qinvm(camtr(lcam.atV,lcam.toV,lcam.upV))); 
	point coff = multm(cor,ppm(screen));	
	point enp = normdcp(coff,screen);
return enp;}
tri ziprent(tri t,point screen){
	tri t1 = mktri( allinoneproj(t.p1,screen),
			allinoneproj(t.p2,screen),
			allinoneproj(t.p3,screen));
return t1;}


//some shitty projection i found on internet don't know how it works and dont wanna
point fastproj(point p,point screen){
	point cor = multm(p,qinvm(camtr(lcam.atV,lcam.toV,lcam.upV))); 
	point prp = multm(cor,ppm(screen)); //final point projectio
//	point pvp = multm(prp,pvm(prp.z));
	point enp = normdcp(prp,screen);
//	point fop = multm(pvp,voff(mkp(screen.x/2,screen.y/2,0)));
return enp;}
tri fastrent(tri t,point screen){
	tri t1 = mktri(fastproj(t.p1,screen),fastproj(t.p2,screen),fastproj(t.p3,screen));
return t1;}


point getcamp(point p){point camp = multm(p,camtr(lcam.atV,lcam.toV,lcam.upV));return camp;}


/*	
	DRAWING FUNCTIONS //will be moved to a seperate graphis handling library or 2d objects
*/

//scanline top and down triangle projection
void chshaded(point p){
	int offs = 0;
	double y=p.y, x=p.x, lg=p.z; //z is shade val
		
	if(lg>1){
	mvprintw(y/2+offs,x+offs,"M");}
	else if(lg<=1 && lg>0.9){
	mvprintw(y/2+offs,x+offs,"#");}
	else if(lg<=0.9 && lg>0.8){
	mvprintw(y/2+offs,x+offs,"@");}
	else if(lg<=0.8 && lg>0.7){
	mvprintw(y/2+offs,x+offs,"8");}
	else if(lg<=0.7 && lg>0.6){
	mvprintw(y/2+offs,x+offs,"&");}
	else if(lg<=0.6 && lg>0.5){
	mvprintw(y/2+offs,x+offs,"o");}
	else if(lg<=0.5 && lg>0.4){
	mvprintw(y/2+offs,x+offs,"*");}
	else if(lg<=0.4 && lg>0.3){
	mvprintw(y/2+offs,x+offs,":");}
	else if(lg<=0.3 && lg>=0){
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
void scanln(tri tr,double lg){
	//tri tr = projt(rent(t,xr),screen);

	if(tr.p2.y < tr.p1.y){ swapp(&tr.p2, &tr.p1); }if(tr.p3.y <tr.p1.y){ swapp(&tr.p3, &tr.p1); }if(tr.p3.y < tr.p2.y){ swapp(&tr.p3, &tr.p2); }
	
	if(tr.p3.y == tr.p2.y){drawtup(tr,lg);}
	else if(tr.p1.y == tr.p2.y){drawtdown(tr,lg);}
	else{point p4 = mkp(inpolx(mkline(tr.p1,tr.p3),tr.p2.y),tr.p2.y,0);
	tri tu = mktri(tr.p1,tr.p2,p4);tri td = mktri(tr.p2,p4,tr.p3);
	drawtup(tu,lg);drawtdown(td,lg);}}


// left handed system




/*	
	HARD CODED MATRICES
*/	

mat4 idm(){
	mat4 pm;pm.m[0][1] = 0;pm.m[0][2] = 0;pm.m[0][3] = 0;pm.m[1][0] = 0;pm.m[1][2] = 0;pm.m[1][3] = 0;pm.m[2][0] = 0;pm.m[2][1] = 0;pm.m[2][3] = 0;	
	pm.m[0][0] = 1;
	pm.m[1][1] = 1;
	pm.m[2][2] = 1;
	pm.m[3][3] = 1;
	pm.m[3][0] = 0;
	pm.m[3][1] = 0;
	pm.m[3][2] = 0;

return pm;}
mat4 spos(point pos){
	mat4 pm;pm.m[0][1] = 0;pm.m[0][2] = 0;pm.m[0][3] = 0;pm.m[1][0] = 0;pm.m[1][2] = 0;pm.m[1][3] = 0;pm.m[2][0] = 0;pm.m[2][1] = 0;pm.m[2][3] = 0;
	
	pm.m[0][0] = 1;
	pm.m[1][1] = 1;
	pm.m[2][2] = 1;
	pm.m[3][3] = 1;
	pm.m[3][0] = -pos.x;
	pm.m[3][1] = -pos.y;
	pm.m[3][2] = -pos.z;

return pm;}mat4 inspos(point pos){
	mat4 pm;pm.m[0][1] = 0;pm.m[0][2] = 0;pm.m[0][3] = 0;pm.m[1][0] = 0;pm.m[1][2] = 0;pm.m[1][3] = 0;pm.m[2][0] = 0;pm.m[2][1] = 0;pm.m[2][3] = 0;
	
	pm.m[0][0] = 1;
	pm.m[1][1] = 1;
	pm.m[2][2] = 1;
	pm.m[3][3] = 1;
	pm.m[3][0] = pos.x;
	pm.m[3][1] = pos.y;
	pm.m[3][2] = pos.z;

return pm;}mat4 pscale(point scale){
	mat4 pm;pm.m[0][1] = 0;pm.m[0][2] = 0;pm.m[0][3] = 0;pm.m[1][0] = 0;pm.m[1][2] = 0;pm.m[1][3] = 0;pm.m[2][0] = 0;pm.m[2][1] = 0;pm.m[2][3] = 0;pm.m[3][0] = 0;pm.m[3][1] = 0;pm.m[3][2] = 0;
	
	pm.m[0][0] = scale.x;
	pm.m[1][1] = scale.y;
	pm.m[2][2] = scale.z;
	pm.m[3][3] = 1;

return pm;}mat4 crmx(double theta){
	mat4 pm;pm.m[0][1] = 0;pm.m[0][2] = 0;pm.m[0][3] = 0;pm.m[1][0] = 0;pm.m[1][3] = 0;pm.m[2][0] = 0;pm.m[2][3] = 0;pm.m[3][0] = 0;pm.m[3][1] = 0;pm.m[3][2] = 0;
	
	pm.m[0][0] = 1;
	pm.m[1][1] = cos(theta);
	pm.m[1][2] = sin(theta);
	pm.m[2][1] = -sin(theta);
	pm.m[2][2] = cos(theta);
	pm.m[3][3] = 1;

return pm;}mat4 crmy(double theta){
	mat4 pm;pm.m[0][1] = 0;pm.m[0][3] = 0;pm.m[1][0] = 0;pm.m[1][2] = 0;pm.m[1][3] = 0;pm.m[2][1] = 0;pm.m[2][3] = 0;pm.m[3][0] = 0;pm.m[3][1] = 0;pm.m[3][2] = 0;
	
	pm.m[1][1] = 1;
	pm.m[0][0] = cos(theta);
	pm.m[0][2] = sin(theta);
	pm.m[2][0] = -sin(theta);
	pm.m[2][2] = cos(theta);
	pm.m[3][3] = 1;

return pm;}mat4 crmz(double theta){
	mat4 pm;pm.m[0][2] = 0;pm.m[0][3] = 0;pm.m[1][2] = 0;pm.m[1][3] = 0;pm.m[2][0] = 0;pm.m[2][1] = 0;pm.m[2][3] = 0;pm.m[3][0] = 0;pm.m[3][1] = 0;pm.m[3][2] = 0;

	pm.m[0][0] = cos(theta);
	pm.m[0][1] = sin(theta);
	pm.m[1][0] = -sin(theta);
	pm.m[1][1] = cos(theta);
	pm.m[2][2] = 1;
	pm.m[3][3] = 1;

return pm;}mat4 camtr(point eye,point cen,point up){//tri p1 = eye , p2 = center ,p3 = upvector
	mat4 pm;pm.m[0][3] = 0;pm.m[1][3] = 0;pm.m[2][3] = 0;	

	point zc = normp(subp(cen,eye));
	point xc = normp(subp(up,mulpn(zc,dot(up,zc))));
	point yc = crossp(xc,zc);

	pm.m[0][0] = yc.x;
	pm.m[1][0] = xc.x;
	pm.m[2][0] = zc.x;
	pm.m[0][1] = yc.y;
	pm.m[1][1] = xc.y;
	pm.m[2][1] = zc.y;
	pm.m[0][2] = yc.z;
	pm.m[1][2] = xc.z;
	pm.m[2][2] = zc.z;
	pm.m[3][0] = eye.x;
	pm.m[3][1] = eye.y;
	pm.m[3][2] = eye.z;
	pm.m[3][3] = 1;/*
	pm.m[0][0] = xc.x;
	pm.m[0][1] = yc.x;
	pm.m[0][2] = zc.x;
	pm.m[1][0] = xc.y;
	pm.m[1][1] = yc.y;
	pm.m[1][2] = zc.y;
	pm.m[2][0] = xc.z;
	pm.m[2][1] = yc.z;
	pm.m[2][2] = zc.z;
	pm.m[3][0] = -xc.x*eye.x - xc.y*eye.y - xc.z*eye.z;
	pm.m[3][1] = -yc.x*eye.x - yc.y*eye.y - xc.z*eye.z;
	pm.m[3][2] = -zc.x*eye.x - zc.y*eye.y - zc.z*eye.z;
	pm.m[3][3] = 1;*/
return pm;}mat4 pmm(point screen){
	mat4 pm;pm.m[0][1] = 0;pm.m[0][2] = 0;pm.m[0][3] = 0;pm.m[1][2] = 0;pm.m[1][3] = 0;pm.m[2][0] = 0;pm.m[2][1] = 0;pm.m[2][3] = 0;pm.m[3][0] = 0;pm.m[3][1] = 0;pm.m[3][2] = 0;
	double f    = 0.1;
	double px   = screen.x/10000;
	double py   = screen.y/10000;
	double offx = screen.x;
	double offy = screen.y;
	double skew = 0;

	pm.m[0][0] = (f*screen.x)/(2*px);
	pm.m[1][0] = skew;
	pm.m[1][1] = (f*screen.y)/(2*py);
	pm.m[2][2] = -1;
	pm.m[3][3] = 1;

return pm;}mat4 ppm(point screen){
	mat4 pm;pm.m[0][1] = 0;pm.m[0][2] = 0;pm.m[0][3] = 0;pm.m[1][0] = 0;pm.m[1][2] = 0;pm.m[1][3] = 0;pm.m[2][0] = 0;pm.m[2][1] = 0;pm.m[3][0] = 0;pm.m[3][1] = 0;
	double near = 0.1;
	double far = 1000;
	double fov = 90;
	double asprt = screen.y/screen.x;
	double fovr = 1 / tan(fov * 0.5 / 180 * 3.14159);

	pm.m[0][0] = asprt * fovr;
	pm.m[1][1] = fovr;
	pm.m[2][2] = far / (far - near);
	pm.m[2][3] = 1;
	pm.m[3][2] = (-far * near) / (far - near);
	pm.m[3][3] = 0;
return pm;}mat4 symppm(point screen){
	mat4 pm;pm.m[0][1] = 0;pm.m[0][2] = 0;pm.m[0][3] = 0;pm.m[1][0] = 0;pm.m[1][2] = 0;pm.m[1][3] = 0;pm.m[2][0] = 0;pm.m[2][1] = 0;pm.m[3][0] = 0;pm.m[3][1] = 0;
	double near = 0.1;
	double far = 100;
	double fov = 45;
	double asprt = screen.x/screen.y;
	double fovr = 1/(tan(fov/2));

	pm.m[0][0] = fovr/asprt;
	pm.m[1][1] = fovr;
	pm.m[2][2] = -(far+near) / (far - near);
	pm.m[2][3] = -1;
	pm.m[3][2] = -(2*far * near) / (far - near);
	pm.m[3][3] = 0;
return pm;}
mat4 asymppm(point screen){
	mat4 pm;pm.m[0][1] = 0;pm.m[0][2] = 0;pm.m[0][3] = 0;pm.m[1][0] = 0;pm.m[1][2] = 0;pm.m[1][3] = 0;pm.m[2][0] = 0;pm.m[2][1] = 0;pm.m[3][0] = 0;pm.m[3][1] = 0;
	double near = 0.1;
	double far = 1000;
	double fov = 45;
	double asprt = screen.x/screen.y;
	double fovr = 1/(tan(fov/2));

	pm.m[0][0] = fovr/asprt;
	pm.m[1][1] = fovr;
	pm.m[2][2] = -(far+near) / (far - near);
	pm.m[2][3] = -1;
	pm.m[3][2] = -(2*far * near) / (far - near);
	pm.m[3][3] = 0;
return pm;}
mat4 pvm(double z){
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



/*	
	SHAPE TEMPLATES
*/	

//predefined square with size as edge lenght its center will be at 0,0,0 	
void mkcube(tri *obj,double size){
	
	double smax = size/2;
	double smin = -size/2;
	double t1[9]={ smin,smin,smin,smin,smax,smin,smax,smax,smin };
	double t2[9]={ smin,smin,smin,smax,smax,smin,smax,smin,smin };
	double t3[9]={ smax,smin,smin,smax,smax,smin,smax,smax,smax };
	double t4[9]={ smax,smin,smin,smax,smax,smax,smax,smin,smax};
	double t5[9]={ smax,smin,smax,smax,smax,smax,smin,smax,smax };
	double t6[9]={ smax,smin,smax,smin,smax,smax,smin,smin,smax };
	double t7[9]={ smin,smin,smax,smin,smax,smax,smin,smax,smin };
	double t8[9]={ smin,smin,smax,smin,smax,smin,smin,smin,smin };
	double t9[9]={ smin,smax,smin,smin,smax,smax,smax,smax,smax };
	double t10[9]={ smin,smax,smin,smax,smax,smax,smax,smax,smin };
	double t11[9]={ smax,smin,smax,smin,smin,smax,smin,smin,smin };
	double t12[9]={ smax,smin,smax,smin,smin,smin,smax,smin,smin };
	                                                      	
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




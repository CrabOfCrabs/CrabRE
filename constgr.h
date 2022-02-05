
// left handed system




/*	
	HARD CODED MATRICES
*/	

mat4 idm(){
	mat4 pm;pm.m[0][1] = 0;pm.m[0][2] = 0;pm.m[0][3] = 0;pm.m[1][0] = 0;pm.m[1][2] = 0;pm.m[1][3] = 0;pm.m[2][0] = 0;pm.m[2][1] = 0;pm.m[2][3] = 0;pm.m[3][0] = 0;pm.m[3][1] = 0;pm.m[3][2] = 0;

	pm.m[0][0] = 1;
	pm.m[1][1] = 1;
	pm.m[2][2] = 1;
	pm.m[3][3] = 1;

return pm;}
mat4 spos(point pos){
	mat4 pm;pm.m[0][1] = 0;pm.m[0][2] = 0;pm.m[0][3] = 0;pm.m[1][0] = 0;pm.m[1][2] = 0;pm.m[1][3] = 0;pm.m[2][0] = 0;pm.m[2][1] = 0;pm.m[2][3] = 0;
	
	pm.m[0][0] = 1;
	pm.m[1][1] = 1;
	pm.m[2][2] = 1;
	pm.m[3][3] = 1;
	pm.m[3][0] = pos.x;
	pm.m[3][1] = pos.y;
	pm.m[3][2] = pos.z;

return pm;}mat4 inspos(point pos){
	mat4 pm;pm.m[0][1] = 0;pm.m[0][2] = 0;pm.m[0][3] = 0;pm.m[1][0] = 0;pm.m[1][2] = 0;pm.m[1][3] = 0;pm.m[2][0] = 0;pm.m[2][1] = 0;pm.m[2][3] = 0;
	
	pm.m[0][0] = 1;
	pm.m[1][1] = 1;
	pm.m[2][2] = 1;
	pm.m[3][3] = 1;
	pm.m[3][0] = -pos.x;
	pm.m[3][1] = -pos.y;
	pm.m[3][2] = -pos.z;

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
	point yc = normp(subp(up,mulpn(zc,dot(up,zc))));
	point xc = crossp(yc,zc);

	pm.m[0][0] = xc.x;
	pm.m[1][0] = yc.x;
	pm.m[2][0] = zc.x;
	pm.m[0][1] = xc.y;
	pm.m[1][1] = yc.y;
	pm.m[2][1] = zc.y;
	pm.m[0][2] = xc.z;
	pm.m[1][2] = yc.z;
	pm.m[2][2] = zc.z;
	pm.m[3][0] = eye.x;
	pm.m[3][1] = eye.y;
	pm.m[3][2] = eye.z;
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
return pm;}


/*	
	SHAPE TEMPLATES
*/	

//predefined square with size as edge lenght its center will be at 0,0,0 	
void mkcube(tri *obj){
	
	double t1[9]={ -0.5,-0.5,-0.5,-0.5,0.5,-0.5,0.5,0.5,-0.5 };
	double t2[9]={ -0.5,-0.5,-0.5,0.5,0.5,-0.5,0.5,-0.5,-0.5 };
	double t3[9]={ 0.5,-0.5,-0.5,0.5,0.5,-0.5,0.5,0.5,0.5 };
	double t4[9]={ 0.5,-0.5,-0.5,0.5,0.5,0.5,0.5,-0.5,0.5};
	double t5[9]={ 0.5,-0.5,0.5,0.5,0.5,0.5,-0.5,0.5,0.5 };
	double t6[9]={ 0.5,-0.5,0.5,-0.5,0.5,0.5,-0.5,-0.5,0.5 };
	double t7[9]={ -0.5,-0.5,0.5,-0.5,0.5,0.5,-0.5,0.5,-0.5 };
	double t8[9]={ -0.5,-0.5,0.5,-0.5,0.5,-0.5,-0.5,-0.5,-0.5 };
	double t9[9]={ -0.5,0.5,-0.5,-0.5,0.5,0.5,0.5,0.5,0.5 };
	double t10[9]={ -0.5,0.5,-0.5,0.5,0.5,0.5,0.5,0.5,-0.5 };
	double t11[9]={ 0.5,-0.5,0.5,-0.5,-0.5,0.5,-0.5,-0.5,-0.5 };
	double t12[9]={ 0.5,-0.5,0.5,-0.5,-0.5,-0.5,0.5,-0.5,-0.5 };
	                                                      	
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

void mkpir(tri *obj){

	double t1[9]={ -0.5,-0.5,0.5,-0.5,-0.5,-0.5,0.5,-0.5,-0.5 };
	double t2[9]={ -0.5,-0.5,0.5,0.5,-0.5,-0.5,0.5,-0.5,0.5};
	double t3[9]={ -0.5,-0.5,-0.5,-0.5,-0.5,0.5,0,1,0 };
	double t4[9]={ -0.5,-0.5,-0.5,0,1,0,0.5,-0.5,-0.5};
	double t5[9]={ -0.5,-0.5,0.5,0.5,-0.5,0.5,0,1,0 };
	double t6[9]={ 0,1,0,0.5,-0.5,0.5,0.5,-0.5,-0.5 };

	obj[0] = amktri(t1);
	obj[1] = amktri(t2);
	obj[2] = amktri(t3);
	obj[3] = amktri(t4);
	obj[4] = amktri(t5);
	obj[5] = amktri(t6);}

void mkfile2(tri *obj){
	FILE *fp = fopen("testSuperPOLY (1).obj","r");
	point pbuff[2028];
	int pcnt = 0;
	int tricnt = 0;
	if (fp == NULL) 
            {   
              printf("Error! Could not open file\n"); 
              exit(-1); 
            }     double x,y,z;

		    char a;
		while(fscanf(fp,"%c %lf %lf %lf ",&a,&x,&y,&z)!= EOF){

			if(a == 'v'){
				pbuff[pcnt]=mkp(x,y,z);
				pcnt++;
			}
			if(a == 'f'){
				obj[tricnt]=mktri(pbuff[(int)x],pbuff[(int)y],pbuff[(int)z]);
				tricnt++;
			}
		}
	fclose(fp);


}

void mkfile(tri *obj){
	FILE *fp = fopen("testSuperPOLY (1).obj","r");

	point pbuff[2028];
	char lbuff[64];
	int pcnt = 1,tricnt = 0;
	int i,j,n;
	if(fp == NULL){exit(-1);}
	while(fgets(lbuff,sizeof(lbuff),fp)){
		char strbuff[3][64];
		j = 0;
		n = 0;
		if(lbuff[0] == 'f'){
			for(i=2;i<=(strlen(lbuff));i++){
				if(lbuff[i]==' ' || lbuff[i] == '\b'){strbuff[n][j] = '\0';n++;j = 0;} //seperate if space and add str end
				else{strbuff[n][j] = lbuff[i];j++;}} //add letter from line buff to string under index n	
			obj[tricnt] = mktri(pbuff[atoll(strbuff[0])],pbuff[atoll(strbuff[1])],pbuff[atoll(strbuff[2])]);
			tricnt++;}
		else if(lbuff[0] == 'v'){
			for(i=2;i<=(strlen(lbuff));i++){
				if(lbuff[i]==' ' || lbuff[i] == '\b'){strbuff[n][j] = '\0';n++;j = 0;} //seperate if space and add str end
				else{strbuff[n][j] = lbuff[i];j++;}} //add letter from line buff to string under index n	
			pbuff[pcnt] = mkp(strtof(strbuff[0],NULL),strtof(strbuff[1],NULL),strtof(strbuff[2],NULL));
			pcnt++;}}
	fclose(fp);}




#pragma once

#include "hemisphere.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


void Hemisphere::setup(){	
    
	int i,j,index = 0;
	int i1,i2,i3,i4;
	double theta,phi,r;
	double len;
	double x;
	double y;
	double z;
	double nx;
	double ny;
	double nz;
	radius = 1;
	N = 128;  // Mesh resolution, must be multiple of 4

	double u;
	double v;
	mesh.enableTextures();
	mesh.enableNormals();

	mesh.addVertex(ofVec3f(0,0,0));
	mesh.addNormal(ofVec3f(0,0,0));
	mesh.addTexCoord(ofVec2f(0,0));

	for (j=0;j<=N/4;j++) {
		for (i=0;i<=N;i++) {
			theta = i * 2 * PI / N;
			phi = PI/2  - PI/2 * j / (N/4);
			x = radius * cos(phi) * cos(theta);
			y = radius * cos(phi) * sin(theta);
			z = radius * sin(phi);
			mesh.addVertex(ofVec3f(x,y,z));

			len = sqrt(x*x + y*y + z*z);
			nx = x/len;
   			ny = y/len;
   			nz = z/len;
			mesh.addNormal(ofVec3f(nx,ny,nz));

			phi = atan2(sqrt(x*x+y*y),z); // 0 ... pi/2
			theta = atan2(y,x); // -pi ... pi
			r = phi / PI/2 * 4; // 0 ... 1
			u = 0.5 * (r * cos(theta) + 1);
			v = 0.5 * (r * sin(theta) + 1);
			mesh.addTexCoord(ofVec2f(u,v));

			index++;
		}        
    }


	for (j=0;j<N/4;j++) {
		for (i=0;i<N;i++) {
			i1 =  j    * (N+1) + i       + 1;
			i2 =  j    * (N+1) + (i + 1) + 1;
			i3 = (j+1) * (N+1) + (i + 1) + 1;
			i4 = (j+1) * (N+1) + i       + 1;
			mesh.addTriangle(i1, i2, i3);
			mesh.addTriangle(i1, i2, i3);
			mesh.addTriangle(i1, i2, i3);
			mesh.addTriangle(i1, i3, i4);
			mesh.addTriangle(i1, i3, i4);
			mesh.addTriangle(i1, i3, i4);
		}
	}


}

void Hemisphere::update(){
}	

void Hemisphere::draw(){
	ofRotateX(90);
	mesh.draw();
}

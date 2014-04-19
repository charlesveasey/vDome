#include "dome.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
namespace vd {

/******************************************

 CONSTRUCTOR

 ********************************************/

Dome::Dome(){
	radius = 20;
	N = 256;  // Mesh resolution, must be multiple of 4
    textureScale = 1;
}

/******************************************

 SETUP

 ********************************************/

void Dome::setup(){
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
	double u;
	double v;
    vbo.clear();
	vbo.enableTextures();
	vbo.enableNormals();

	vbo.addVertex(ofVec3f(0,0,0));
	vbo.addNormal(ofVec3f(0,0,0));
	vbo.addTexCoord(ofVec2f(0,0));

	for (j=0;j<=N/4;j++) {
		for (i=0;i<=N;i++) {
			theta = i * 2 * PI / N;
			phi = PI/2  - PI/2 * j / (N/4);
			x = radius * cos(phi) * cos(theta);
			y = radius * cos(phi) * sin(theta);
			z = radius * sin(phi);
			vbo.addVertex(ofVec3f(x,y,z));

			len = sqrt(x*x + y*y + z*z);
			nx = x/len;
   			ny = y/len;
   			nz = z/len;
			vbo.addNormal(ofVec3f(nx,ny,nz));

			phi = atan2(sqrt(x*x+y*y),z); // 0 ... pi/2
			theta = atan2(y,x); // -pi ... pi
			r = phi / PI/2 * 4 * textureScale; // 0 ... 1 --->
			u = 0.5 * (r * cos(theta) + 1);
			v = 0.5 * (r * sin(theta) + 1);
			vbo.addTexCoord(ofVec2f(u,1-v)); // reverse
			index++;
		}
    }

	for (j=0;j<N/4;j++) {
		for (i=0;i<N;i++) {
			i1 =  j    * (N+1) + i       + 1;
			i2 =  j    * (N+1) + (i + 1) + 1;
			i3 = (j+1) * (N+1) + (i + 1) + 1;
			i4 = (j+1) * (N+1) + i       + 1;
			vbo.addTriangle(i1, i2, i3);
			vbo.addTriangle(i1, i2, i3);
			vbo.addTriangle(i1, i2, i3);
			vbo.addTriangle(i1, i3, i4);
			vbo.addTriangle(i1, i3, i4);
			vbo.addTriangle(i1, i3, i4);
		}
	}
}

/******************************************

 DRAW

 ********************************************/

void Dome::draw(){
	ofRotateX(90);
    glEnable(GL_CULL_FACE);
    glCullFace( GL_BACK );
	vbo.draw();
    glDisable(GL_CULL_FACE);
}


/******************************************

 KEYBOARD

 ********************************************/

void Dome::keyPressed(int key) {
    switch (key) {
        case OF_KEY_RIGHT:
            switch (editMode) {
                case RADIUS:
                    radius += value;
                    setup();
                    break;
            }
            break;
        case OF_KEY_LEFT:
            switch (editMode) {
                case RADIUS:
                    radius -= value;
                    setup();
                    break;
            }
    }

}

void Dome::keyPressedInput(int key) {
    switch (key) {
        case OF_KEY_UP:  // up = switch on mode
            switch (editMode) {
                case 1: // mesh texture scale
                    textureScale += value;
                    setup();
                    break;
            }
            break;
        case OF_KEY_DOWN:  // up = switch on mode
            switch (editMode) {
                case 1: // mesh texture scale
                    textureScale -= value;
                    setup();
                    break;
            }
            break;
    }
}

/******************************************

 SETTINGS

 ********************************************/

void Dome::loadXML(ofXml &xml) {
    if (xml.exists("dome[@radius]"))
        radius = ofToDouble( xml.getAttribute("dome[@radius]") );

    if (xml.exists("input[@scale"))
        textureScale = ofToFloat( xml.getAttribute("input[@scale]") );

    setup();
}

void Dome::saveXML(ofXml &xml) {
    xml.setTo("dome");
    xml.setAttribute("radius", ofToString(radius));
    xml.setToParent();
    xml.setTo("input");
    xml.setAttribute("scale", ofToString(textureScale));
    xml.setToParent();
}


/******************************************

 SAVE MESH

 ********************************************/

void Dome::saveMesh(string file) {
    vbo.save(file);
}

}

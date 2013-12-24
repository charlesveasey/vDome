#include "plane.h"

bool shift;
ofPoint lastM;
map<int, bool> sel;
bool group = false;
bool drawBox = false;
ofPoint boxOrigin;
ofPoint boxUpdate;
int xRes = 10;
int yRes = 10;

vector<ofVec3f> orgVerts;
vector<ofVec3f> keyVerts;
vector<ofVec3f> gridVerts;
ofMatrix4x4 mat;

void Plane::init(){
}


void Plane::setup(){
    int w = 1024;
    int h = 768;
    int x = 0 + (w * indx);
    int y = 0;
    
    position.push_back(x);
    position.push_back(y);
    
    cout << indx << endl;
    
    mesh = ofMesh::plane(1024, 768, xRes, yRes, OF_PRIMITIVE_TRIANGLES);
    
    vector<ofVec3f> v = mesh.getVertices();
    
    for (int i=0; i<v.size(); i++) {
        mesh.setTexCoord(i, ofVec2f( v[i].x + w/2 , v[i].y +h/2   ));
        mesh.setVertex(i, ofVec3f(v[i].x + w/2 + x, v[i].y + h/2, v[i].z) );
    }
    
    //mesh.load("models/plane-mesh-" + ofToString(indx));
    
    topLeft.set(0, 0);
    topRight.set(1, 0);
    bottomLeft.set(0, 1);
    bottomRight.set(1, 1);

    ofPoint tl(topLeft.x * w + x, topLeft.y * h + y);
    ofPoint tr(topRight.x * w + x, topRight.y * h + y);
    ofPoint bl(bottomLeft.x * w + x, bottomLeft.y * h + y);
    ofPoint br(bottomRight.x * w + x, bottomRight.y * h + y);
    
    keystone.setAnchorSize(w/2, h/2);
    keystone.setSourceRect( ofRectangle( 0, 0, w, h ) );    // this is the source rectangle which is the size of the image and located at ( 0, 0 )
    keystone.setTopLeftCornerPosition( tl );                // this is position of the quad warp corners, centering the image on the screen.
    keystone.setTopRightCornerPosition( tr );               // this is position of the quad warp corners, centering the image on the screen.
    keystone.setBottomLeftCornerPosition( bl );             // this is position of the quad warp corners, centering the image on the screen.
    keystone.setBottomRightCornerPosition( br );            // this is position of the quad warp corners, centering the image on the screen.
    keystone.setup();
   
    for (int i=0; i<v.size(); i++) {
        orgVerts.push_back(mesh.getVertex(i));
        keyVerts.push_back(ofVec3f(0,0,0));
        gridVerts.push_back(ofVec3f(0,0,0));
    }
}


void Plane::draw(){
    vector<ofVec3f> v = mesh.getVertices();
    vector<ofVec3f> v2 = mesh.getVertices();
    vector<ofVec3f> v3 = mesh.getVertices();
    
    for (int i=0; i<v.size(); i++) {
        v[i] = orgVerts[i];
        v2[i] = gridVerts[i] + orgVerts[i];
        v3[i] = keystone.getMatrix().preMult( v2[i] );
    }
    
    for (int i=0; i<v.size(); i++) {
        mesh.setVertex(i, v3[i]);
    }
    
    mesh.draw();
}


void Plane::drawConfig(){
    if (drawBox) {
        ofNoFill();
        glLineWidth(2);
        ofSetHexColor(0xFFFFFF);
        ofRect(boxOrigin.x, boxOrigin.y, boxUpdate.x-boxOrigin.x, boxUpdate.y-boxOrigin.y);
    }
        
    vector<ofVec3f> v = mesh.getVertices();
    
    float rad = 5;
    for (int i=0; i<v.size(); i++) {
        
        if(sel[i])
            ofSetHexColor(0xFFF000);
        else
            ofSetHexColor(0xFFFFFF);
        
        ofDrawSphere(v[i].x, v[i].y, v[i].z, rad );
    }
}

//--------------------------------------------------------------
void Plane::keyPressed(int key){
    if (key == OF_KEY_SHIFT) {
        shift = true;
    }
    
    else if (key == OF_KEY_UP) {
        
        for (int i=0; i<sel.size(); i++) {
            if (sel[i]) {
                float x = mesh.getVertex(i).x;
                float y = mesh.getVertex(i).y;
                float z = mesh.getVertex(i).z;
                y--;
                mesh.setVertex( i, ofVec3f(x,y,z) );
            }
        }
    }
    else if (key == OF_KEY_DOWN) {
        
        for (int i=0; i<sel.size(); i++) {
            if (sel[i]) {
                float x = mesh.getVertex(i).x;
                float y = mesh.getVertex(i).y;
                float z = mesh.getVertex(i).z;
                y++;
                mesh.setVertex( i, ofVec3f(x,y,z) );
            }
        }
    }
    else if (key == OF_KEY_LEFT) {
        
        for (int i=0; i<sel.size(); i++) {
            if (sel[i]) {
                float x = mesh.getVertex(i).x;
                float y = mesh.getVertex(i).y;
                float z = mesh.getVertex(i).z;
                x--;
                mesh.setVertex( i, ofVec3f(x,y,z) );
            }
        }
    }
    else if (key == OF_KEY_RIGHT) {
        
        for (int i=0; i<sel.size(); i++) {
            if (sel[i]) {
                float x = mesh.getVertex(i).x;
                float y = mesh.getVertex(i).y;
                float z = mesh.getVertex(i).z;
                x++;
                mesh.setVertex( i, ofVec3f(x,y,z) );
            }
        }
    }
}


//--------------------------------------------------------------
void Plane::keyReleased(int key){
    if (key == OF_KEY_SHIFT) {
        shift = false;
    }
}


//--------------------------------------------------------------
void Plane::onMouseDragged(ofMouseEventArgs& mouseArgs){
    
    ofPoint mousePoint(mouseArgs.x, mouseArgs.y);

    if (keystoneActive) {
        keystone.onMouseDragged(mouseArgs);
    }
    else if (gridActive) {
        if (group) {
            drawBox = true;
            boxUpdate = ofPoint(mousePoint.x, mousePoint.y);
            return;
        }
        
        if (indx == -1)
            return;
        
        for (int i=0; i<sel.size(); i++) {
            if (sel[i]) {
                ofPoint newPoint = ofPoint(gridVerts[i].x, gridVerts[i].y) - ((lastM - mousePoint) * value);
                gridVerts[i] = ofVec3f(newPoint.x, newPoint.y, newPoint.z);
            }
        }
    }
    
    lastM = mousePoint;    
}

//--------------------------------------------------------------
void Plane::onMousePressed(ofMouseEventArgs& mouseArgs){

    int x = mouseArgs.x;
    int y = mouseArgs.y;
    
    if (keystoneActive) {
        keystone.onMousePressed(mouseArgs);
    }
    else if (gridActive) {
        vector<ofVec3f> v = mesh.getVertices();
        float rad = 20;

        for (int i=0; i<v.size(); i++) {
            float distance = ofDist(v[i].x, v[i].y, x, y);
            
            if (distance < rad) {
                indx = i;
                
                if (!sel[i]) {
                    
                    if (shift) {
                        sel[i] = true;
                    }
                    else {
                        sel.clear();
                        sel[i] = true;
                    }
                }
                group = false;
                break;
            }
            else {
                indx = -1;
                group = true;
                drawBox = true;
                boxOrigin = ofPoint(x,y);
                boxUpdate = boxOrigin;
            }
        }
    }

    lastM = ofPoint(x,y);
}

//--------------------------------------------------------------
void Plane::onMouseReleased(ofMouseEventArgs& mouseArgs){
    
    int x = mouseArgs.x;
    int y = mouseArgs.y;
    
    if (gridActive && group) {
        vector<ofVec3f> v = mesh.getVertices();
        
        float rad = 20;
        
        ofRectangle rect;
        rect = ofRectangle(boxOrigin.x, boxOrigin.y, boxUpdate.x-boxOrigin.x, boxUpdate.y-boxOrigin.y);
        
        
        for (int i=0; i<v.size(); i++) {
            
            if ( (v[i].x) + rad    >= rect.getTopLeft().x
                && (v[i].x) - rad   <= rect.getBottomRight().x) {
                
                if ( (v[i].y) + rad >= rect.getTopLeft().y
                    && (v[i].y) - rad    <= rect.getBottomRight().y) {
                    
                    sel[i] = true;
                }
                else  {
                    sel[i] = false;
                }
            }
            else  {
                sel[i] = false;
            }
        }
        
        drawBox = false;
        group = false;
    }
}

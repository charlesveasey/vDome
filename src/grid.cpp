#include "grid.h"

int ind;
bool shift;
ofPoint lastM;
map<int, bool> sel;
bool group = false;
bool drawBox = false;
ofPoint boxOrigin;
ofPoint boxUpdate;



void Grid::init(){
}


void Grid::setup(){
    mesh = ofMesh::plane(1024, 768, 5, 5, OF_PRIMITIVE_TRIANGLES);
    vector<ofVec3f> v = mesh.getVertices();
    
    for (int i=0; i<v.size(); i++) {        
        mesh.setTexCoord(i, ofVec2f( v[i].x + 1024/2 , v[i].y +768/2   ));
    }
    
}


void Grid::draw(){
    //tex.bind();
    
    mesh.draw();
    //tex.unbind();
}


void Grid::drawConfig(){
    if (drawBox) {
        ofNoFill();
        glLineWidth(2);
        ofSetHexColor(0xFFFFFF);
        ofRect(boxOrigin.x, boxOrigin.y, boxUpdate.x-boxOrigin.x, boxUpdate.y-boxOrigin.y);
    }
    
    ofTranslate(1024/2, 768/2, 0);
    
    vector<ofVec3f> v = mesh.getVertices();
    
    float rad = 10;
    for (int i=0; i<v.size(); i++) {
        
        if(sel[i])
            ofSetHexColor(0xFFF000);
        else
            ofSetHexColor(0xFFFFFF);
        
        ofDrawPlane(v[i].x + (1024 * indx), v[i].y, rad, rad );
    }
    
    ofTranslate(-1024/2, -768/2);
}

//--------------------------------------------------------------
void Grid::keyPressed(int key){
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
void Grid::keyReleased(int key){
    if (key == OF_KEY_SHIFT) {
        shift = false;
    }
    
}


//--------------------------------------------------------------
void Grid::onMouseDragged(int x, int y, int button){
        
    if (group) {
        drawBox = true;
        boxUpdate = ofPoint(x,y);
        return;
    }
    
    if (ind == -1)
        return;
    int i = ind;
    
    ofPoint mousePoint(x, y);
    
    for (int i=0; i<sel.size(); i++) {
        if (sel[i]) {
            mesh.setVertex( i, ofPoint(mesh.getVertex(i).x, mesh.getVertex(i).y) - (lastM - mousePoint) );
        }
    }
    lastM = mousePoint;
}

//--------------------------------------------------------------
void Grid::onMousePressed(int x, int y, int button){

    vector<ofVec3f> v = mesh.getVertices();
    
    float rad = 20;
    
    for (int i=0; i<v.size(); i++) {
        float distance = ofDist(v[i].x+(1024 * indx), v[i].y, x-1024/2, y-768/2);
        
        if (distance < rad) {
            ind = i;
            
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
            ind = -1;
            group = true;
            drawBox = true;
            boxOrigin = ofPoint(x,y);
            boxUpdate = boxOrigin;
        }
        
    }
    
    lastM = ofPoint(x,y);
}

//--------------------------------------------------------------
void Grid::onMouseReleased(int x, int y, int button){
    
    if (group) {
        
        vector<ofVec3f> v = mesh.getVertices();
        
        float rad = 20;
        
        ofRectangle rect;
        rect = ofRectangle(boxOrigin.x, boxOrigin.y, boxUpdate.x-boxOrigin.x, boxUpdate.y-boxOrigin.y);
        
        
        for (int i=0; i<v.size(); i++) {
            
            if ( (v[i].x+(1024 * indx) + 1024/2) + rad    >= rect.getTopLeft().x   &&
                 (v[i].x+(1024 * indx) + 1024/2) - rad   <= rect.getBottomRight().x) {
                
                if ( (v[i].y + 768/2) + rad >= rect.getTopLeft().y  &&
                     (v[i].y + 768/2) - rad    <= rect.getBottomRight().y) {
                    
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

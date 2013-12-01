#include "keystone.h"


void Keystone::setup(){
    
    int w = 1024;
    int h = 768;
    int x = 0;
    int y = 0;
    
    warper.setSourceRect( ofRectangle( 0, 0, w, h ) );              // this is the source rectangle which is the size of the image and located at ( 0, 0 )
    warper.setTopLeftCornerPosition( ofPoint( x, y ) );             // this is position of the quad warp corners, centering the image on the screen.
    warper.setTopRightCornerPosition( ofPoint( x + w, y ) );        // this is position of the quad warp corners, centering the image on the screen.
    warper.setBottomLeftCornerPosition( ofPoint( x, y + h ) );      // this is position of the quad warp corners, centering the image on the screen.
    warper.setBottomRightCornerPosition( ofPoint( x + w, y + h ) ); // this is position of the quad warp corners, centering the image on the screen.
    warper.setup();
}


void Keystone::draw(){
    warper.draw();
}
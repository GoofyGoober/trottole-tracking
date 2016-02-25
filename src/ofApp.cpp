#include "ofApp.h"

#define MONITOR_4 0
#define DISEGNO 1

int status = 0;
int w = 640;
int h = 480;
int wh = w*h;
//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(30);
    setupAllocation();
}

void ofApp::setupAllocation(){
    image.allocate(w,h);
    filtered.allocate(w,h);
    red.allocate(w,h);
    green.allocate(w,h);
    blue.allocate(w,h);
    webcam.setup(w,h);
}

//--------------------------------------------------------------
void ofApp::update(){
    webcam.update();
    calcolaContorno(webcam,w,h);
}

void ofApp::calcolaContorno(ofVideoGrabber& source, int& _w, int& _h){

    if (source.isFrameNew()){
        image.setFromPixels( source.getPixels() );
        image.mirror(false, true);
        image.convertToGrayscalePlanarImages(red, green, blue);
        red+=blue;
        green-=red;
        for (int i=0; i<wh; i++) {
            filtered.getPixels()[i] = ofInRange(green.getPixels()[i],0,10) ? 0 : 255;
        }
        filtered.flagImageChanged();
        finder.findContours(filtered, 50, wh/2, 10, false);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(status==MONITOR_4){
        green.draw(640,0);
        filtered.draw(0,480);
        finder.draw(640,480);
        
        //inverte webcam image
        webcam.draw(webcam.getWidth(),0,-webcam.getWidth(),webcam.getHeight());
    
        //draw red circles for found blobs
        for (int i=0; i<finder.nBlobs; i++) {
            ofDrawCircle(finder.blobs[i].centroid.x, finder.blobs[i].centroid.y, 5);
        }
        
    } else if (status==DISEGNO) {
        
        for (int i=0; i<finder.nBlobs; i++) {
            
            ofDrawCircle(finder.blobs[i].centroid.x, finder.blobs[i].centroid.y, 5);
        }
        
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'x'){
        setupAllocation();
        
        img.grabScreen(0, 0 , ofGetWidth(), ofGetHeight());
        img.save("screenshot.png");
    }
    
    if(key== 's'){
        if (status==MONITOR_4){
            ofLogNotice("monitor 4");
            w = 1280;
            h = 960;
            wh = w*h;
            status = DISEGNO;
        } else if (status==DISEGNO){
            ofLogNotice("disegno");
            w = 640;
            h = 480;
            wh = w*h;
            status = MONITOR_4;
        }
        setupAllocation();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

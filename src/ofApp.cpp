#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);

    ofSetFrameRate(60);
    setupAllocation();;
    sender.setup(OSC_IP, OSC_PORT);

    toggle.addListener(this, &ofApp::toggleButtonPressed);

    setupGui();

}

void ofApp::setupGui(){
    gui.setup();
    toggle.setup("show all visualization",true);
    sliderMinArea.setup("min-area", 10,1,300);
    sliderMaxArea.setup("max-area", wh/2,10,300);
    sliderNConsidered.setup("n-considered", 1,1,20);
    sliderColorSensibility.setup("color-sensibility",20,0,255);
    toggleUseApproximation.setup("approximation",true);
    gui.add(&sliderMinArea);
    gui.add(&sliderMaxArea);
    gui.add(&sliderNConsidered);
    gui.add(&sliderColorSensibility);
    gui.add(&toggleUseApproximation);
    gui.add(&toggle);
}


bool ofApp::toggleButtonPressed(bool & inval){
    if (!inval){
        w = 1280;
        h = 960;
    } else {
        w = 640;
        h = 480;
    }
    wh = w*h;
    setupAllocation();
}

void ofApp::setupAllocation(){
    webcam.setup(w,h);
    image.allocate(w,h);
    imageHSV.allocate(w,h);
    filtered.allocate(w,h);
    red.allocate(w,h);
    green.allocate(w,h);
    blue.allocate(w,h);
    hue.allocate(w, h);
    sat.allocate(w, h);
    bri.allocate(w, h);
}

//--------------------------------------------------------------
void ofApp::update(){
    webcam.update();
    calcolaContornoDaWebCam();
    bundle.clear();
}

void ofApp::calcolaContornoDaWebCam(){
    if (webcam.isFrameNew()){
        image.setFromPixels( webcam.getPixels() );
        image.mirror(false, true);
        imageHSV = image;
        imageHSV.convertRgbToHsv();
        imageHSV.convertToGrayscalePlanarImages(hue, sat, bri);

        verdi = estremizzaBianchiNeri(hue, 115);
        contorniVerdi = calcolaContorno(verdi);
        
    }
}


ofxCvGrayscaleImage ofApp::estremizzaBianchiNeri(ofxCvGrayscaleImage _imagebw, int hueSearching){
    ofPixels   pixels = _imagebw.getPixels();
    ofPixels   pixelsGreen =  _imagebw.getPixels();
    
    int min = hueSearching-sliderColorSensibility;
    int max = hueSearching+sliderColorSensibility;

    for (int i=0; i<wh; i++) {
        pixels[i] = ofInRange(pixelsGreen[i],min,max) ? 255 : 0;
    }
    _imagebw.setFromPixels(pixels);
    return _imagebw;
    
}

ofxCvContourFinder ofApp::calcolaContorno(ofxCvGrayscaleImage _filtered){
    ofxCvContourFinder _finder;
    _finder.findContours(_filtered,
                        sliderMinArea,
                        sliderMaxArea,
                        sliderNConsidered,
                        toggleUseApproximation);
    return _finder;
}




//--------------------------------------------------------------
void ofApp::draw(){
    if(toggle){
        green.draw(640,0);
        imageHSV.draw(0,480);
        contorniVerdi.draw(640,480);
        webcam.draw(webcam.getWidth(),0,-webcam.getWidth(),webcam.getHeight());

        drawBlobs();
        
    } else {
        drawBlobs();
    }
    
    if (finder.blobs.size() > 0){
        sender.sendBundle(bundle);
    }

    
    gui.draw();
}


void ofApp::drawBlobs(){
    for (int i=0; i<finder.nBlobs; i++) {
        sendOsc(i);
        int x = finder.blobs[i].centroid.x;
        int y = finder.blobs[i].centroid.y;
        ofDrawCircle(x, y, 5);
    }
    
}


void ofApp::sendOsc(int i){
    m.clear();
    m.setAddress("/blob_"+ofToString(i));
    newX = float(finder.blobs[i].centroid.x/w)*100;
    newY = float(finder.blobs[i].centroid.y/h)*100;
    m.addInt32Arg(newX);
    m.addInt32Arg(newY);
    m.addInt32Arg(1);
    bundle.addMessage(m);
}



//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(key == 'x'){
        img.grabScreen(0, 0 , ofGetWidth(), ofGetHeight());
        img.save("screenshot.png");
    }
}

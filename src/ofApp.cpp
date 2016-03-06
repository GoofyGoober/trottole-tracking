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
    sliderHue1.setup("hue 1",115, 0,255);
    sliderHue2.setup("hue 2",0, 0,255);
    sliderHue3.setup("hue 3",240, 0,255);
    gui.add(&sliderMinArea);
    gui.add(&sliderMaxArea);
    gui.add(&sliderNConsidered);
    gui.add(&sliderColorSensibility);
    gui.add(&toggleUseApproximation);
    gui.add(&sliderHue1);
    gui.add(&sliderHue2);
    gui.add(&sliderHue3);
    gui.add(&toggle);
    gui.setPosition(ofGetWindowWidth()-gui.getWidth(), 0);
}


bool ofApp::toggleButtonPressed(bool & inval){
    if (!inval){
        w = 1280;
        h = 960;
    } else {
        w = 640/2;
        h = 480/2;
    }
    wh = w*h;
    setupAllocation();
}

void ofApp::setupAllocation(){
    webcam.setup(w,h);
    image.allocate(w,h);
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
        image.convertRgbToHsv();
        image.convertToGrayscalePlanarImages(hue, sat, bri);

        contorniVerdi = calcolaContorno(estremizzaBianchiNeri(hue, sliderHue1));
        contorniRossi = calcolaContorno(estremizzaBianchiNeri(hue, sliderHue2));
        contorniBlue  = calcolaContorno(estremizzaBianchiNeri(hue, sliderHue3));
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

ofColor coloreDaSlider(ofxIntSlider slider ){
    ofColor color;
    color.setHsb(slider, 255, 255);
    return color;
}


//--------------------------------------------------------------
void ofApp::draw(){
    if(toggle){
        //webcam.draw(0, 0, webcam.getWidth(), webcam.getHeight());
        webcam.draw(webcam.getWidth(),0,-webcam.getWidth(),webcam.getHeight());
        webcam.draw(320,0,-webcam.getWidth(),webcam.getHeight());

        drawBlobs(contorniVerdi);
        drawBlobs(contorniRossi);
        drawBlobs(contorniBlue);
        
        
        // update gui
        sliderHue1.setFillColor(coloreDaSlider(sliderHue1));
        sliderHue1.draw();
        sliderHue2.setFillColor(coloreDaSlider(sliderHue2));
        sliderHue2.draw();
        sliderHue3.setFillColor(coloreDaSlider(sliderHue3));
        sliderHue3.draw();
        
        
        // mostra analisi
        contorniVerdi.draw(0,0,320,240);
        contorniRossi.draw(320,0,320,240);
        contorniBlue.draw(0,320,320,240);


    } else {
        drawBlobs(contorniVerdi);
    }
    
    if (contorniVerdi.blobs.size() > 0){
        sender.sendBundle(bundle);
    }

    
    gui.draw();
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));

}

bool ofApp::contorniHannoBlob(){
    if (contorniVerdi.blobs.size()>0 || contorniRossi.blobs.size()>0|| contorniBlue.blobs.size()>0){
        return true;
    }
    return false;
}


void ofApp::drawBlobs(ofxCvContourFinder &contorno){
    for (int i=0; i<contorno.nBlobs; i++) {
        sendOsc(i, contorno);
        int x = contorno.blobs[i].centroid.x;
        int y = contorno.blobs[i].centroid.y;
        ofDrawCircle(x, y, 5);
    }
    
}


void ofApp::sendOsc(int i, ofxCvContourFinder &contorno){
    m.clear();
    m.setAddress("/blob_"+ofToString(i));
    newX = float(contorno.blobs[i].centroid.x/w)*100;
    newY = float(contorno.blobs[i].centroid.y/h)*100;
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

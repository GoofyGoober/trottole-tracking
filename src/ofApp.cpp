#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);

    ofSetFrameRate(20);
    setupAllocation();;
    sender.setup(OSC_IP, OSC_PORT);

    toggle.addListener(this, &ofApp::toggleButtonPressed);

    setupGui();
    

}

void ofApp::setupGui(){
    gui.setup();
    toggle.setup("show all visualization",true);
    sliderMinArea.setup("min-area", 0,1,800);
    sliderMaxArea.setup("max-area", 100,10,1000);
    sliderNConsidered.setup("n-considered", 1,1,20);
    sliderColorSensibility.setup("color-sensibility",20,0,255);
    toggleUseApproximation.setup("approximation",true);
    
    toggleHue1.setup("enable 1", true);
    sliderHue1.setup("hue 1",  64, 0, 255);
    sliderHue1Sat.setup("sat", 255,0,255);
    sliderHue1Lum.setup("lum", 255,0,255);
    sliderHue1LumSensibility.setup("lum-sensibility",20,0,255);
    sliderHue1SatSensibility.setup("sat-sensibility",20,0,255);

    toggleHue2.setup("enable 2", true);
    sliderHue2.setup("hue 2", 183, 0, 255);
    sliderHue2Sat.setup("sat", 255,0,255);
    sliderHue2Lum.setup("lum", 255,0,255);
    sliderHue2LumSensibility.setup("lum-sensibility",20,0,255);
    sliderHue2SatSensibility.setup("sat-sensibility",20,0,255);
    
    toggleHue3.setup("enable 3", true);
    sliderHue3.setup("hue 3", 255, 0, 255);
    sliderHue3Sat.setup("sat", 255,0,255);
    sliderHue3Lum.setup("lum", 255,0,255);
    sliderHue3LumSensibility.setup("lum-sensibility",20,0,255);
    sliderHue3SatSensibility.setup("sat-sensibility",20,0,255);
    
    //webcam setup gui
    sliderWebCamLum.setup("brightness",0,0,255);
    sliderWebCamExp.setup("exp",0,0,255);
    sliderWebCamGain.setup("gain",0,0,255);
    sliderWebCamFrameRate.setup("framerate",1,1,30);
    sliderWebCamSharpness.setup("sharpness",0,0,255);
    sliderWebCamGreen.setup("sliderWebCamGreen",0,0,255);
        sliderWebCamRed.setup("sliderWebCamRed",0,0,255);
    sliderWebCamBlue.setup("sliderWebCamBlue",0,0,255);
    gui.add(&sliderMinArea);
    gui.add(&sliderMaxArea);
    gui.add(&sliderNConsidered);
    gui.add(&sliderColorSensibility);

    gui.add(&toggleUseApproximation);
    
    gui.add(&sliderHue1);
    gui.add(&toggleHue1);
    gui.add(&sliderHue1Lum);
    gui.add(&sliderHue1LumSensibility);
    gui.add(&sliderHue1Sat);
    gui.add(&sliderHue1SatSensibility);

    gui.add(&sliderHue2);
    gui.add(&toggleHue2);
    gui.add(&sliderHue2Lum);
    gui.add(&sliderHue2LumSensibility);
    gui.add(&sliderHue2Sat);
    gui.add(&sliderHue2SatSensibility);

    gui.add(&sliderHue3);
    gui.add(&toggleHue3);
    gui.add(&sliderHue3Lum);
    gui.add(&sliderHue3LumSensibility);
    gui.add(&sliderHue3Sat);
    gui.add(&sliderHue3SatSensibility);
    
    // webcam gui
    gui.add(&sliderWebCamLum);
    gui.add(&sliderWebCamExp);
    gui.add(&sliderWebCamGain);
    gui.add(&sliderWebCamFrameRate);
    gui.add(&sliderWebCamSharpness);
    gui.add(&sliderWebCamGreen);
    gui.add(&sliderWebCamRed);
    gui.add(&sliderWebCamBlue);
    
    gui.setPosition(ofGetWindowWidth()-gui.getWidth(), 0);
//    gui.load("settings.xml");
    gui.loadFromFile("settings.xml");
}


bool ofApp::toggleButtonPressed(bool & inval){
        w = 640;
        h = 480;
   
    wh = w*h;
    setupAllocation();
}

void ofApp::setupAllocation(){
//    webcam.setup(w,h);
    image.allocate(w,h);
    hue.allocate(w, h);
    sat.allocate(w, h);
    bri.allocate(w, h);
    
    webcam.setGrabber(std::make_shared<ofxPS3EyeGrabber>());
    
    webcam.setup(w, h);
//    webcam.getGrabber<ofxPS3EyeGrabber>()->setAutogain(false);
    webcam.getGrabber<ofxPS3EyeGrabber>()->setAutoWhiteBalance(true);
    webcam.getGrabber<ofxPS3EyeGrabber>()->setDesiredFrameRate(20);
    webcam.getGrabber<ofxPS3EyeGrabber>()->setBrightness(0);

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

        contorniVerdi = calcolaContorno(estremizzaBianchiNeri(hue,
                                                              sat,
                                                              bri,
                                                              sliderHue1,
                                                              sliderHue1Sat,
                                                              sliderHue1Lum,
                                                              sliderColorSensibility,
                                                              sliderHue1SatSensibility,
                                                              sliderHue1LumSensibility));
        
        contorniRossi = calcolaContorno(estremizzaBianchiNeri(hue,
                                                              sat,
                                                              bri,
                                                              sliderHue2,
                                                              sliderHue2Sat,
                                                              sliderHue2Lum,
                                                              sliderColorSensibility,
                                                              sliderHue2SatSensibility,
                                                              sliderHue2LumSensibility));
        
        contorniBlue  = calcolaContorno(estremizzaBianchiNeri(hue,
                                                              sat,
                                                              bri,
                                                              sliderHue3,
                                                              sliderHue3Sat,
                                                              sliderHue3Lum,
                                                              sliderColorSensibility,
                                                              sliderHue3SatSensibility,
                                                              sliderHue3LumSensibility));
    }
}




ofxCvGrayscaleImage ofApp::estremizzaBianchiNeri(ofxCvGrayscaleImage _hue,
                                                 ofxCvGrayscaleImage _sat,
                                                 ofxCvGrayscaleImage _lum,
                                                 int hueSearching,
                                                 int satSearching,
                                                 int lumSearching,
                                                 int hueSensibility,
                                                 int satSensiblity,
                                                 int lumSensibility) {
    

    ofPixels   pixels = image.getPixels();
    ofPixels   hue    = _hue.getPixels();
    ofPixels   sat    = _sat.getPixels();
    ofPixels   lum    = _lum.getPixels();
    
    int minHue = hueSearching-hueSensibility;
    int maxHue = hueSearching+hueSensibility;
    
    int minSat = satSearching-satSensiblity;
    int maxSat = satSearching+satSensiblity;

    int minLum = lumSearching-lumSensibility;
    int maxLum = lumSearching+lumSensibility;

    for (int i=0; i<wh; i++) {
        if (ofInRange(hue[i], minHue, maxHue) and
            ofInRange(sat[i], minSat, maxSat) and
            ofInRange(lum[i], minLum, maxLum) ){
            pixels[i] = 255;
        } else {
            pixels[i] = 0;
        }
    }
    ofxCvGrayscaleImage _imagebw;
    _imagebw.allocate(w,h);
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

ofColor coloreDaSlider(ofxIntSlider sliderHue, ofxIntSlider sliderSat, ofxIntSlider sliderLum){
    ofColor color;
    color.setHsb(sliderHue, sliderSat, sliderLum);
    return color;
}


//--------------------------------------------------------------
void ofApp::draw(){
        webcam.draw(webcam.getWidth(),0,-webcam.getWidth(),webcam.getHeight());
        
        // update gui
        sliderHue1.setFillColor(coloreDaSlider(sliderHue1, sliderHue1Sat, sliderHue1Lum));
        sliderHue2.setFillColor(coloreDaSlider(sliderHue2, sliderHue2Sat, sliderHue2Lum));
        sliderHue3.setFillColor(coloreDaSlider(sliderHue3, sliderHue3Sat, sliderHue3Lum));
        sliderHue1.draw();
        sliderHue2.draw();
        sliderHue3.draw();
        
        // mostra analisi
        
        if (toggleHue1){
            contorniVerdi.draw(0, 0, w, h);
            drawBlobs(contorniVerdi, coloreDaSlider(sliderHue1, sliderHue1Sat, sliderHue1Lum));
        }
        if (toggleHue2){
            contorniRossi.draw(0, 0, w, h);
            drawBlobs(contorniRossi, coloreDaSlider(sliderHue2, sliderHue2Sat, sliderHue2Lum));
        }
        if (toggleHue3){
            contorniBlue.draw( 0, 0, w, h);
            drawBlobs(contorniBlue,  coloreDaSlider(sliderHue3, sliderHue3Sat, sliderHue3Lum));
        }


    
    if (contorniVerdi.blobs.size() > 0){
        sender.sendBundle(bundle);
    }

    
    gui.draw();
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    
    webcam.getGrabber<ofxPS3EyeGrabber>()->setBrightness(sliderWebCamLum);
    webcam.getGrabber<ofxPS3EyeGrabber>()->setExposure(sliderWebCamExp);
    webcam.getGrabber<ofxPS3EyeGrabber>()->setGain(sliderWebCamGain);
    webcam.getGrabber<ofxPS3EyeGrabber>()->setDesiredFrameRate(sliderWebCamFrameRate);
    webcam.getGrabber<ofxPS3EyeGrabber>()->setGreenBalance(sliderWebCamGreen);
    webcam.getGrabber<ofxPS3EyeGrabber>()->setRedBalance(sliderWebCamRed);
    webcam.getGrabber<ofxPS3EyeGrabber>()->setBlueBalance(sliderWebCamBlue);
    webcam.getGrabber<ofxPS3EyeGrabber>()->setGreenBalance(sliderWebCamGreen);
    webcam.getGrabber<ofxPS3EyeGrabber>()->setSharpness(sliderWebCamSharpness);

}

bool ofApp::contorniHannoBlob(){
    if (contorniVerdi.blobs.size()>0 || contorniRossi.blobs.size()>0|| contorniBlue.blobs.size()>0){
        return true;
    }
    return false;
}


void ofApp::drawBlobs(ofxCvContourFinder &contorno, ofColor color){
    ofSetColor(color);
    for (int i=0; i<contorno.nBlobs; i++) {
        sendOsc(i, contorno);
        int x = contorno.blobs[i].centroid.x;
        int y = contorno.blobs[i].centroid.y;
        ofDrawCircle(x, y, 5);
    }
    ofSetColor(255);
    
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

void ofApp::mousePressed(int x, int y, int button){
ofColor color =     webcam.getPixels().getColor(w-x, y);
    ofLog()<<"---";
    ofLog()<<color.getHue();
    ofLog()<<"saturation";
    ofLog()<<color.getSaturation();
        ofLog()<<"lum";
    ofLog()<<color.getLightness();
    
    sliderHue1 = color.getHue();
    sliderHue1Lum = color.getLightness();
    
    sliderHue1Sat = color.getSaturation();

    
}

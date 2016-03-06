#pragma once

#include "ofMain.h"
#include "ofxCvColorImage.h"
#include "ofxCvGrayscaleImage.h"
#include "ofxCvFloatImage.h"
#include "ofxCvContourFinder.h"
#include "ofxOsc.h"
#include "ofxGui.h"

#define MONITOR_4 0
#define DISEGNO 1

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void keyPressed(int key);
        void calcolaContornoDaWebCam();
        void calcolaContornoDaIphone();
        ofxCvContourFinder calcolaContorno(ofxCvGrayscaleImage _filtered);
        ofxCvGrayscaleImage estremizzaBianchiNeri(ofxCvGrayscaleImage _imagebw, int hueSearching);
        void setupAllocation();
        void setupGui();
        void drawBlobs(ofxCvContourFinder &contorno);
        void sendOsc(int i, ofxCvContourFinder &contorno);
        bool toggleButtonPressed(bool & inval);
    bool contorniHannoBlob();
    
    //osc
    ofxOscSender sender;
    ofxOscMessage m;
    
    // gui
    ofxPanel gui;
    ofxToggle toggle;
    ofxToggle toggleUseApproximation;
    //gui for findContours
    ofxIntSlider sliderMinArea;
    ofxIntSlider sliderMaxArea;
    ofxIntSlider sliderNConsidered;
    ofxIntSlider sliderColorSensibility;
    ofxIntSlider sliderHue1;
    ofxIntSlider sliderHue2;
    ofxIntSlider sliderHue3;

    // webacam
    ofVideoGrabber webcam;
    ofxCvColorImage image;
    
    // colors
    ofxCvGrayscaleImage hue;
    ofxCvGrayscaleImage sat;
    ofxCvGrayscaleImage bri;
    
    ofxCvContourFinder contorniVerdi;
    ofxCvContourFinder contorniRossi;
    ofxCvContourFinder contorniBlue;
    ofImage img;
    
    
    int newX;
    int newY;
    
    
    int status = 0;
    int w = 640;
    int h = 480;
    int wh = w*h;
    
    ofImage imageIphone;
    ofFile file;
    string OSC_IP = "127.0.0.1";
    int    OSC_PORT = 12345;
    int    WEB_SERVER_SOCKET_PORT = 8080;
    ofxOscBundle bundle;

};

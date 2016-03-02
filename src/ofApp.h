#pragma once

#include "ofMain.h"
#include "ofxCvColorImage.h"
#include "ofxCvGrayscaleImage.h"
#include "ofxCvFloatImage.h"
#include "ofxCvContourFinder.h"
#include "ofxOsc.h"
#include "ofxLibwebsockets.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();


		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void keyPressed(int key);
        void calcolaContornoDaWebCam(ofVideoGrabber& source);
        void calcolaContornoDaIphone();
        void calcolaContorno();
        void setupAllocation();
        void setupWebSockets();  
        void onConnect(ofxLibwebsockets::Event& args);
        void onOpen(ofxLibwebsockets::Event& args);
        void onClose(ofxLibwebsockets::Event& args);
        void onIdle(ofxLibwebsockets::Event& args);
        void onMessage(ofxLibwebsockets::Event& args);
        void onBroadcast(ofxLibwebsockets::Event& args);

    ofxOscSender sender;
    
    ofxLibwebsockets::Server server;

    ofVideoPlayer video;
    ofxCvColorImage image;
    ofxCvGrayscaleImage grayImage, grayImagePrev;
    ofxCvGrayscaleImage diff;    //Absolute difference of the frames
    ofxCvFloatImage diffFloat;   //Amplified difference images
    ofxCvFloatImage bufferFloat; //Buffer image
    ofVideoGrabber webcam;
    ofxCvGrayscaleImage filtered,red,green,blue;
    ofxCvContourFinder finder;
    ofImage     img;
    
    int w = 640;
    int h = 480;
		
};

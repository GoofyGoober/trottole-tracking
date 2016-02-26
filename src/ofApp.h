#pragma once

#include "ofMain.h"
#include "ofxCvColorImage.h"
#include "ofxCvGrayscaleImage.h"
#include "ofxCvFloatImage.h"
#include "ofxCvContourFinder.h"
#include "ofxOsc.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void calcolaContorno(ofVideoGrabber& source, int& _w, int& _h);
        void setupAllocation();
    
    
    ofxOscSender sender;
    

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

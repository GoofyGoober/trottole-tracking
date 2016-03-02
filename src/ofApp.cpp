#include "ofApp.h"

#define MONITOR_4 0
#define DISEGNO 1

int status = 0;
int w = 640;
int h = 480;
int wh = w*h;

ofImage imageIphone;
ofFile file;
ofxLibwebsockets::Server  server;
ofxLibwebsockets::ServerOptions options;
string OSC_IP = "169.254.7.175";
int    OSC_PORT = 12345;
int    WEB_SERVER_SOCKET_PORT = 8080;

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(24);
    setupAllocation();
    webcam.setup(w,h);
    setupWebSockets();
    sender.setup(OSC_IP, OSC_PORT);
}

void ofApp::setupAllocation(){
    image.allocate(w,h);
    filtered.allocate(w,h);
    red.allocate(w,h);
    green.allocate(w,h);
    blue.allocate(w,h);
}

//--------------------------------------------------------------
void ofApp::update(){
    webcam.update();
    //calcolaContornoDaWebCam(webcam);
    file.open("/Users/ale/test.jpg", ofFile::ReadOnly, false);

    //imageIphone.load(file);
}

void ofApp::calcolaContornoDaIphone(){
    
}
void ofApp::calcolaContornoDaWebCam(ofVideoGrabber& source){

    if (source.isFrameNew()){
        image.setFromPixels( source.getPixels() );
        calcolaContorno();
    }
}

void ofApp::calcolaContorno(){
    image.mirror(false, true);
    image.convertToGrayscalePlanarImages(red, green, blue);
    red+=blue;
    green-=red;
    for (int i=0; i<wh; i++) {
        filtered.getPixels()[i] = ofInRange(green.getPixels()[i],0,10) ? 0 : 255;
    }
    filtered.flagImageChanged();
    finder.findContours(filtered, 50, wh/2, 3, false);
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofxOscBundle bundle;
    
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
            ofxOscMessage m;
            m.setAddress("blob_"+ofToString(i)+"/");
            int newX = float(finder.blobs[i].centroid.x/w)*100;
            int newY = float(finder.blobs[i].centroid.y/h)*100;
            m.addInt32Arg(newX);
            m.addInt32Arg(newY);
            m.addInt32Arg(1);

            ofDrawCircle(finder.blobs[i].centroid.x, finder.blobs[i].centroid.y, 5);
            bundle.addMessage(m);
        }
        
    }
    
    // Invio OSC
    sender.sendBundle(bundle);
    
    //int imgWidth = imageIphone.getWidth();
    //int imgHeight = imageIphone.getHeight();
    //imageIphone.draw(10, 10, imgWidth, imgHeight);
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

void ofApp::setupWebSockets()
{
    ofLogNotice("--- SETUP ----");
    options = ofxLibwebsockets::defaultServerOptions();
    options.port = WEB_SERVER_SOCKET_PORT;
    options.bUseSSL = false;
    server.setup( options );
    server.addListener(this);
    ofLogNotice("WebSocket server setup at "+ofToString(server.getPort()));
}


//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    ofLogNotice("--- MESSAGE---");
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    ofLogNotice("----> f");
}

void ofApp::onConnect(ofxLibwebsockets::Event& args)
{
    ofLogNotice("ASD");
    cout<<"on connected"<<endl;
}

void ofApp::onOpen(ofxLibwebsockets::Event& args)
{
    ofLogNotice("AsD - onOpen");
    cout<<"new connection open"<<endl;
}

void ofApp::onClose(ofxLibwebsockets::Event& args)
{
        ofLogNotice("close");
    cout<<"on close"<<endl;
}

void ofApp::onIdle(ofxLibwebsockets::Event& args)
{
        ofLogNotice("idle");
    cout<<"on idle"<<endl;
}

void ofApp::onMessage(ofxLibwebsockets::Event& args)
{
        ofLogNotice("messaggggino");
    cout<<"got message "<<args.message<<endl;

    args.conn.send(args.message);
}

void ofApp::onBroadcast(ofxLibwebsockets::Event& args)
{
            ofLogNotice("broadcast");
    cout<<"got broadcast "<<args.message<<endl;
}

#pragma once

#include "ofMain.h"
#include "ofxSlitScan.h"
#include "ofxUI.h"
#define VIDEO_WIDTH 640
#define VIDEO_HEIGHT 480
#define DRAW_SPACING 10

class testApp : public ofBaseApp{

	public:
        //app
		void setup();
		void update();
		void draw();
		void keyPressed(int key);
        int videoX;
        int videoY;
    

        //cam grabber
        ofVideoGrabber grabber;
        void setupGrabber();
        bool drawGrabber;
    
        //slit scan
        ofxSlitScan scanner;
        void setupScanner();
        bool drawScanner;
    
        //rutt etra
        ofPixels vidPixels;
        ofMesh mesh;
        int xStep;
        int yStep;
        void drawMesh();
        void setupRuttEtra();
        bool drawRuttEtra;
    
        //fbo
        ofFbo fbo;
        int fadeAmount;
        void setupFBO();
    
        //ui
        ofxUICanvas* gui;
        void exit();
        void guiEvent(ofxUIEventArgs &e);
        void setupGUI();
		
};

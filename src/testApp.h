#pragma once

#include "ofMain.h"
#include "ofxSlitScan.h"
#include "ofxUI.h"
#define VIDEO_WIDTH 640
#define VIDEO_HEIGHT 480
#define DRAW_SPACING 10

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);

        //cam grabber
        ofVideoGrabber grabber;
    
        //slit scan
        ofxSlitScan scanner;
    
        //rutt etra
        ofPixels vidPixels;
        ofMesh mesh;
        int xStep;
        int yStep;
        void drawMesh();
    
        //fbo
        ofFbo fbo;
        int fadeAmount;
    
        //ui
        ofxUICanvas* gui;
        void exit();
        void guiEvent(ofxUIEventArgs &e);
		
};

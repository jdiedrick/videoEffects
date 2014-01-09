#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    //init some global shit for the app
    ofEnableAlphaBlending();
    ofBackground(0);
    
    //setup grabber
    grabber.initGrabber(VIDEO_WIDTH, VIDEO_HEIGHT);
    
    //setup slitscanner
    ofImage distortionMap;
    distortionMap.loadImage("distortion.png");
    scanner.setup(VIDEO_WIDTH, VIDEO_HEIGHT, 30, OF_IMAGE_COLOR);
    scanner.setDelayMap(distortionMap);
    scanner.setBlending(true);
    scanner.setTimeDelayAndWidth(30, 30);
    
    //setup rutt etra
    xStep = 20;
    yStep = 20;

    //setup fbo
    fbo.allocate(VIDEO_WIDTH, VIDEO_HEIGHT, GL_RGBA32F_ARB);
    fbo.begin();
    ofClear(0,0,0,0);
    fbo.end();
}

//--------------------------------------------------------------
void testApp::update(){
    
    grabber.update();
    
    //slitscan update
    if(grabber.isFrameNew()){
        
        //add image to scanner
        scanner.addImage(grabber);
        
        //rutt etra update
        vidPixels = grabber.getPixelsRef();
    }
    
    //draw into fbo
    fbo.begin();
    drawMesh();
    fbo.end();
    

}

//--------------------------------------------------------------
void testApp::draw(){
    
    //draw scanner
    scanner.getOutputImage().draw(0,0);
    
    //draw grabber
    grabber.draw(VIDEO_WIDTH+DRAW_SPACING, 0);
    
    //draw fbo
    fbo.draw(0, VIDEO_HEIGHT+DRAW_SPACING);
    

}

//--------------------------------------------------------------
void testApp::drawMesh(){
    
    //most of this is based off the fbo trails example :)
    
    //we clear the fbo if c is pressed.
    //this completely clears the buffer so you won't see any trails
    if( ofGetKeyPressed('c') ){
        ofClear(0,0,0, 0);
    }
    
    //some different alpha values for fading the fbo
    //the lower the number, the longer the trails will take to fade away.
    fadeAmount = 30;
    if(ofGetKeyPressed('1')){
        fadeAmount = 1;
    }else if(ofGetKeyPressed('2')){
        fadeAmount = 5;
    }else if(ofGetKeyPressed('3')){
        fadeAmount = 15;
    }
    
    ofPushStyle();
    ofFill();
    ofSetColor(0,0,0, fadeAmount);
    ofRect(0,0,VIDEO_WIDTH,VIDEO_HEIGHT);
    ofPopStyle();
    
    ofMesh mesh;
    
    int rowCount = 0;
    for (int y = 0; y<VIDEO_HEIGHT; y+=yStep){
        //   ofNoFill();
        mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
        
        if (rowCount % 2 == 0) {
            for (int x = 0; x < VIDEO_WIDTH; x += xStep){
                ofColor curColor = vidPixels.getColor(x, y);
                mesh.addColor(ofColor(curColor, 255));
                // mesh.addVertex(ofVec3f(x, y, z+curColor.getBrightness() * 0.3 + fftSmoothed[y]*30.f));
                if(y<VIDEO_WIDTH/2){
                    mesh.addVertex(ofVec3f(x, y, curColor.getBrightness() * 0.3));
                } else {
                    mesh.addVertex(ofVec3f(x, y, curColor.getBrightness() * 0.3));
                }
            }
        } else {
            for (int x = VIDEO_WIDTH-1; x >= 0; x -= xStep){
                ofColor curColor = vidPixels.getColor(x, y);
                mesh.addColor(ofColor(curColor, 255));
                if(y<VIDEO_WIDTH/2){
                    mesh.addVertex(ofVec3f(x, y, curColor.getBrightness() * 0.3 ));
                } else {
                    mesh.addVertex(ofVec3f(x, y, curColor.getBrightness() * 0.3));
                }
            }
        }
        rowCount++;
        
    }
    mesh.draw();
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    //
}

//--------------------------------------------------------------
void testApp::exit(){
    //
}

//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e){
    //
}


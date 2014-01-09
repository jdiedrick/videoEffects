#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    //setup grabber
    grabber.initGrabber(640, 480);
    
    //setup slitscanner
    ofImage distortionMap;
    distortionMap.loadImage("distortion.png");
    scanner.setup(640, 480, 30, OF_IMAGE_COLOR);
    scanner.setDelayMap(distortionMap);
    scanner.setBlending(true);
    scanner.setTimeDelayAndWidth(30, 30);
    
    

}

//--------------------------------------------------------------
void testApp::update(){
    
    grabber.update();
    
    if(grabber.isFrameNew()){
        
        //add image to scanner
        scanner.addImage(grabber);
    }
    

}

//--------------------------------------------------------------
void testApp::draw(){
    
    //draw scanner
    scanner.getOutputImage().draw(0,0);
    
    //draw grabber
    grabber.draw(650, 0);

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

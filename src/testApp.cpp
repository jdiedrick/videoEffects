#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    //init some global shit for the app
    ofEnableAlphaBlending();
    ofBackground(0);
    
    //setup gui
    setupGUI();
    
    //setup grabber
    setupGrabber();
    
    //setup slitscanner
    setupScanner();
    
    //setup rutt etra
    setupRuttEtra();

    //setup fbo
    setupFBO();
    
    //more global app shit after other things have been set up
    videoX = ofGetWindowWidth()/2-grabber.width/2;
    videoY = ofGetWindowHeight()/2-grabber.height/2;
    
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
    
    //draw grabber
    if(drawGrabber) grabber.draw(videoX, videoY);

    //draw scanner
    if(drawScanner) scanner.getOutputImage().draw(videoX, videoY);
    
    //draw fbo
    if(drawRuttEtra) fbo.draw(videoX, videoY);
    
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
    if(key=='d'){
        gui->toggleVisible();
    }
    else if(key=='f'){
        ofToggleFullscreen();
    }
}

//--------------------------------------------------------------
void testApp::setupGrabber(){
    drawGrabber = true;
    grabber.initGrabber(VIDEO_WIDTH, VIDEO_HEIGHT);
}

//--------------------------------------------------------------
void testApp::setupScanner(){
    drawScanner = false;
    ofImage distortionMap;
    distortionMap.loadImage("distortion.png");
    scanner.setup(VIDEO_WIDTH, VIDEO_HEIGHT, 30, OF_IMAGE_COLOR);
    scanner.setDelayMap(distortionMap);
    scanner.setBlending(true);
    scanner.setTimeDelayAndWidth(30, 30);
}

//--------------------------------------------------------------
void testApp::setupRuttEtra(){
    drawRuttEtra = false;
    xStep = 10;
    yStep = 10;
}

//--------------------------------------------------------------
void testApp::setupFBO(){
    fbo.allocate(VIDEO_WIDTH, VIDEO_HEIGHT, GL_RGBA32F_ARB);
    fbo.begin();
    ofClear(0,0,0,0);
    fbo.end();
}

//--------------------------------------------------------------
void testApp::setupGUI(){
    
    float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 320-xInit;
    float dim = 16;

    vector<string> video_effects;
	video_effects.push_back("Grabber");
	video_effects.push_back("Slit Scan");
	video_effects.push_back("Rutt-Etra");
    
    gui = new ofxUICanvas(0,0,320,320);
    //add ui widgets
    gui->addWidgetDown(new ofxUILabel("Video Effects", OFX_UI_FONT_LARGE));
    gui->addSpacer(length, 5);
    gui->addWidgetDown(new ofxUISlider(304,16,0.0,255.0,100.0,"Background Value"));
    gui->addWidgetDown(new ofxUIToggle(32, 32, false, "Fullscreen"));
    
    gui->addSpacer(length, 10);
    gui->addWidgetDown(new ofxUILabel("Video Effects", OFX_UI_FONT_MEDIUM));
    gui->addRadio("Video Effects", video_effects, OFX_UI_ORIENTATION_HORIZONTAL, dim, dim);

    gui->addSpacer(length, 10);
    gui->addWidgetDown(new ofxUILabel("Slit Scan", OFX_UI_FONT_MEDIUM));
    gui->addWidgetDown(new ofxUILabel("UI TBD", OFX_UI_FONT_SMALL));

    gui->addSpacer(length, 10);
    gui->addWidgetDown(new ofxUILabel("Rutt-Etra", OFX_UI_FONT_MEDIUM));
    gui->addWidgetDown(new ofxUISlider(304,16,1,100,100,"X Step"));
    gui->addWidgetDown(new ofxUISlider(304,16,1,100,100,"Y Step"));
    
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);
    gui->loadSettings("GUI/guiSettings.xml");
}

//--------------------------------------------------------------
void testApp::exit(){
    gui->saveSettings("GUI/guiSettings.xml");
    delete gui;
}

//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e){
    if(e.widget->getName() == "Background Value")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        ofBackground(slider->getScaledValue());
    }
    
    else if(e.widget->getName() == "Fullscreen")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        ofSetFullscreen(toggle->getValue());
    }
    

    else if(e.widget->getName() == "Grabber")
    {
        drawGrabber=true;
        drawRuttEtra=false;
        drawScanner=false;
        cout << "draw grabber" <<endl;
        
    }
    
    else if(e.widget->getName() == "Slit Scan")
    {
        drawGrabber=false;
        drawScanner=true;
        drawRuttEtra=false;
        cout << "draw scan" <<endl;

    }
    
    else if(e.widget->getName() == "Rutt-Etra")
    {
        drawGrabber=false;
        drawScanner=false;
        drawRuttEtra=true;
        cout << "draw rutt etra" <<endl;

    }
    
    else if(e.widget->getName() == "X Step")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        xStep = (int)slider->getScaledValue();
    }
    
    else if(e.widget->getName() == "Y Step")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        yStep = (int)slider->getScaledValue();
    }
}


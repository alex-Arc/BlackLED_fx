#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync( true );
    ofSetBackgroundAuto(false);
    
    wave.setup(NUM_ARRAY);
    wave.updateResponse(ATTACK, DAMPING);
    
    
    graficWave.allocate(RENDER_WIDTH, RENDER_HEIGHT,GL_RGBA32F);
    fboTexWaves.allocate(RENDER_WIDTH, RENDER_HEIGHT);
    render.allocate(RENDER_WIDTH, RENDER_HEIGHT);
    syphonRenderOut.setName("Waves");
    
    graficWave.begin();
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    graficWave.end();
    
    fboTexWaves.begin();
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    fboTexWaves.end();
    
    render.begin();
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    render.end();
    
    visualControl.setName("visualControl");
    visualControl.add(inLeft.set("inLeft", 0., -1., 1.));
    visualControl.add(inRight.set("inRight", 0., -1., 1.));
    visualControl.add(noiseAmt.set("noiseAmt", 0.15, 0., 1));
    
    visualControl.add(posWave.set("posWave", 0.5, 0., 1));
    visualControl.add(posLine.set("posLine", 0.5, 0, 1));
    
    visualControl.add(colorLine.set("colorLine", ofColor(0,0), ofColor(0,0),ofColor(255)));
    visualControl.add(colorBGTop.set("colorBGTop", ofColor(0,255), ofColor(0,0),ofColor(255)));
    visualControl.add(colorBGBot.set("colorBGBot", ofColor(0,255), ofColor(0,0),ofColor(255)));
    visualControl.add(colorTopTop.set("colorTopTop", ofColor(0,0), ofColor(0,0),ofColor(255)));
    visualControl.add(colorTopBot.set("colorTopBot", ofColor(0,0), ofColor(0,0),ofColor(255)));
    visualControl.add(colorBotTop.set("colorBotTop", ofColor(0,0), ofColor(0,0),ofColor(255)));
    visualControl.add(colorBotBot.set("colorBotBot", ofColor(0,0), ofColor(0,0),ofColor(255)));
    
    
    
    
    
    // Blinkende Lygter
    graficBlinkendeLygter.allocate(RENDER_WIDTH, RENDER_HEIGHT);
    
    // Control
    paramBlinkendeLyter.setName("BlinkendeLygter");
    paramBlinkendeLyter.add(blinkIntensity.set("blinkIntensity", 0.0, 0.0, 1.0));
    paramBlinkendeLyter.add(tempo.set("tempo", 0.02, 0.0001, 0.1));
    paramBlinkendeLyter.add(hard_soft.set("hard/soft", true));
    //paramBlinkendeLyter.add(colorLygter.set("colorLygter", ofColor(255,255), ofColor(0,0), ofColor(255,255)));
    
    visualControl.add(paramBlinkendeLyter);
    
    // Boubbles
    graficBoubbles.allocate(RENDER_WIDTH, RENDER_HEIGHT);
    
    // Control
    paramBoubbles.setName("Boubbles");
    paramBoubbles.add(boubblesIntensity.set("boubblesIntensity", 0.0, 0.0, 1.0));
    paramBoubbles.add(velMin.set("velMin", 0.5, 0.0, 5.0));
    paramBoubbles.add(velMax.set("velMax", 2., 0.0, 5.0));
    //paramBoubbles.add(colorBoubbles.set("colorBoubbles", ofColor(255,255), ofColor(0,0), ofColor(255,255)));
    
    visualControl.add(paramBoubbles);
    
    
    
    
    
    // Dancing Lines
    graficDancingLines.allocate(RENDER_WIDTH, RENDER_HEIGHT);
    
    // Control
    paramLines.setName("DancingLines");
    paramLines.add(speed.set("speed", .2, 0.0, 1));
    paramLines.add(syncLines.set("syncLines", .9, 0.5, 1));
    paramLines.add(numOfLines.set("numOfLines", 5, 0, 50));
    
    paramLines.add(colorDLines.set("colorDLines", ofColor(0,255), ofColor(0,0), ofColor(255,255)));
    
    counterDLines = 0;
    numOfLinesLast = 0;
    for(int i = 0; i < numOfLines; i++){
        DancingLine newLine;
        newLine.location1 = ofVec2f(0, RENDER_HEIGHT/2);
        newLine.location2 = ofVec2f(RENDER_WIDTH, RENDER_HEIGHT/2);
        newLine.lineColor = colorDLines;
        newLine.freedom = RENDER_HEIGHT*2;
        
        dancingLines.push_back(newLine);
        numOfLinesLast++;
    }
    
    visualControl.add(paramLines);
    
    
    gui.setup(visualControl);
    
    
    // SETUP FOR OSC
    syncOSC.setup((ofParameterGroup&)gui.getParameter(),OSCRECEIVEPORT,"localhost",OSCSENDPORT);
    
    colorLygter = ofColor(255);
    colorBoubbles = ofColor(255);
    
    gui.minimizeAll();
    
    
    port = midiIn.getNumPorts()-1;
    numPorts =midiIn.getNumPorts();
    sync.setup(port, gui.getParameter());
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    syncOSC.update();
    
    
    // Blinkedne blinks
    for(int i = 0; i < 10 ; i++){
        if(ofRandom(100000)/100000 < blinkIntensity){
            Blink blink;
            blink.blinkColor = ofColor(255);
            blink.location = ofVec2f(((int)ofRandom(RENDER_WIDTH)),(int)ofRandom(RENDER_HEIGHT));
            blink.tempo = tempo;
            blink.hard_soft = hard_soft;
            
            blinks.push_back(blink);
        }
    }
    
    for (vector<Blink>::iterator it=blinks.begin(); it!=blinks.end();)    {
        it->update();
        if(it->isDead())
            it = blinks.erase(it);
        else
            ++it;
    }
    
    graficBlinkendeLygter.begin();
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for(int i = 0; i < blinks.size(); i++){
        blinks[i].draw();
    }
    graficBlinkendeLygter.end();
    
    
    // Boubbles
    if(ofRandom(100000)/100000 < boubblesIntensity){
        Boubble boubble;
        boubble.boubbleColor = colorBoubbles;
        boubble.location = ofVec2f(((int)ofRandom(RENDER_WIDTH)),RENDER_HEIGHT);
        boubble.velocity = ofVec2f(0., - ofRandom(velMin   , velMax));
        
        boubbles.push_back(boubble);
    }
    
    for (vector<Boubble>::iterator it=boubbles.begin(); it!=boubbles.end();)    {
        it->update();
        
        if(it->isDead())
            it = boubbles.erase(it);
        else
            ++it;
    }
    
    graficBoubbles.begin();
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for(int i = 0; i < boubbles.size(); i++){
        boubbles[i].draw();
    }
    graficBoubbles.end();
    
    
    // DANCING LINES
    if(numOfLines > numOfLinesLast){
        for (int n = 1; n<numOfLines-numOfLinesLast; n++){
            DancingLine newLine;
            newLine.location1 = ofVec2f(0, RENDER_HEIGHT/2);
            newLine.location2 = ofVec2f(RENDER_WIDTH, RENDER_HEIGHT/2);
            newLine.lineColor = colorDLines;
            newLine.freedom = RENDER_HEIGHT*2;
            
            dancingLines.push_back(newLine);
            numOfLinesLast++;
            
        }
        numOfLinesLast = dancingLines.size();
    }
    else if(numOfLines < numOfLinesLast){
        for (int n = 0; n<numOfLinesLast-numOfLines; n++){
            dancingLines.pop_back();
            numOfLinesLast--;
        }
        
    }

    
    counterDLines += speed/100;
    
    
    for (vector<DancingLine>::iterator it=dancingLines.begin(); it!=dancingLines.end();)    {
        it->lineColor = colorDLines;
        it->update(counterDLines);
        it->sync = syncLines;
        ++it;
    }
    
    
    
    /*    // OSC soundWaveReceive
     // check for waiting messages
     while(receiveOscSoundWave.hasWaitingMessages()){
     // get the next message
     ofxOscMessage m;
     receiveOscSoundWave.getNextMessage(m);
     cout << "oscReceive: "<< m.getAddress() << " " << ofToString(m.getArgAsFloat(0)) << "\n";
     
     // check for mouse moved message
     if(m.getAddress() == "/soundWaves/impulseR/"){
     float  strength = m.getArgAsFloat(0);
     
     sWSystem.addSoundWave(ofVec2f(RENDER_WIDTH_SW, RENDER_HEIGHT_SW/2), strength);
     }
     // check for mouse button message
     else if(m.getAddress() == "/soundWaves/impulseL/"){
     float  strength = m.getArgAsFloat(0);
     
     sWSystem.addSoundWave(ofVec2f(0, RENDER_HEIGHT_SW/2), strength);
     }
     
     }
     // Update soundWaves
     sWSystem.update();*/
    
    
    
    fboTexWaves.begin();
    ofRectGradient(0, fboTexWaves.getHeight()/2, fboTexWaves.getWidth(), fboTexWaves.getHeight()/2, colorBotTop, colorBotBot,
                   OF_GRADIENT_LINEAR);
    ofRectGradient(0,0, fboTexWaves.getWidth(), fboTexWaves.getHeight()/2, colorTopTop, colorTopBot, OF_GRADIENT_LINEAR);
    for(int i = 0; i < dancingLines.size(); i++){
        dancingLines[i].draw();
    }
    fboTexWaves.end();
    
    
    // draw the Waves
    float noiseLeft = noiseAmt*(ofNoise(ofGetElapsedTimef()*4)-0.5);
    float noiseRight = noiseAmt*(ofNoise(ofGetElapsedTimef()*4+300.)-0.5);
    float noiseRight_ramp = noiseAmt*(ofNoise(ofGetElapsedTimef()*4+600.)-0.5);
    float noiseLeft_ramp = noiseAmt*(ofNoise(ofGetElapsedTimef()*4+900.)-0.5);
    
    wave.update(inLeft+noiseLeft, inRight+noiseRight);
    
    
    
    //    wave_poles.updateResponse(ATTACK, DAMPING);
    //    wave_ramp.updateResponse(ATTACK, DAMPING);
    
    
    // Draw the content into the renderer for syphon publishing.
    
    render.begin();
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ofSetColor(255,255);
    ofRectGradient(0, 0, RENDER_WIDTH, RENDER_HEIGHT, colorBGTop, colorBGBot, OF_GRADIENT_LINEAR);
    
    wave.drawGradient(0,0,                RENDER_WIDTH, RENDER_HEIGHT, posWave, fboTexWaves.getTexture());
    
    // Line
    wave.drawLine(0,0,                RENDER_WIDTH, RENDER_HEIGHT, colorLine, posLine, LINE_WIDTH);
    
    
    
    graficBlinkendeLygter.draw(0,0);//RENDER_HEIGHT_POLE);
    graficBoubbles.draw(0,0);//RENDER_HEIGHT_POLE);
    
    
    //ofRectGradient(,RENDER_HEIGHT_RAMP, RENDER_WIDTH_RAMP, RENDER_HEIGHT_POLE-RENDER_HEIGHT_RAMP, colorTopTop, colorTopBot, OF_GRADIENT_LINEAR);
    
    
    render.end();
    
    
    
    syphonRenderOut.publishTexture(&render.getTexture());
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(150);
    
    //ofSetColor(255);
    //render.draw(0,0, RENDER_WIDTH, RENDER_HEIGHT);
    gui.draw();
    sync.drawDebug();
    
    
    
    ofSetWindowTitle("LEDFX FPS: "+ofToString((int)ofGetFrameRate()));
    
    
    
    
}

void ofApp::ofRectGradient(int px, int py, int w, int h,const ofColor& start, const ofColor& end, ofGradientMode mode){
    ofVboMesh gradientMesh;
    gradientMesh.clear();
    gradientMesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
#ifndef TARGET_EMSCRIPTEN
#ifdef TARGET_OPENGLES
    if(ofIsGLProgrammableRenderer()) gradientMesh.setUsage(GL_STREAM_DRAW);
#else
    gradientMesh.setUsage(GL_STREAM_DRAW);
#endif
#endif
    if(mode == OF_GRADIENT_CIRCULAR) {
        // this could be optimized by building a single mesh once, then copying
        // it and just adding the colors whenever the function is called.
        ofVec2f center(w / 2 + px, h / 2 + py);
        gradientMesh.addVertex(center);
        gradientMesh.addColor(start);
        int n = 32; // circular gradient resolution
        float angleBisector = TWO_PI / (n * 2);
        float smallRadius = ofDist(0, 0, w / 2, h / 2);
        float bigRadius = smallRadius / cos(angleBisector);
        for(int i = 0; i <= n; i++) {
            float theta = i * TWO_PI / n;
            gradientMesh.addVertex(center + ofVec2f(sin(theta), cos(theta)) * bigRadius);
            gradientMesh.addColor(end);
        }
    } else if(mode == OF_GRADIENT_LINEAR) {
        gradientMesh.addVertex(ofVec2f(px, py));
        gradientMesh.addVertex(ofVec2f(px+w, py));
        gradientMesh.addVertex(ofVec2f(px+w, py+h));
        gradientMesh.addVertex(ofVec2f(px, py+h));
        gradientMesh.addColor(start);
        gradientMesh.addColor(start);
        gradientMesh.addColor(end);
        gradientMesh.addColor(end);
    } else if(mode == OF_GRADIENT_BAR) {
        gradientMesh.addVertex(ofVec2f(px+w / 2, py+h / 2));
        gradientMesh.addVertex(ofVec2f(px, py+h / 2));
        gradientMesh.addVertex(ofVec2f(px, py));
        gradientMesh.addVertex(ofVec2f(px+w, py));
        gradientMesh.addVertex(ofVec2f(px+w, py+h / 2));
        gradientMesh.addVertex(ofVec2f(px+w, py+h));
        gradientMesh.addVertex(ofVec2f(px, py+h));
        gradientMesh.addVertex(ofVec2f(px, py+h / 2));
        gradientMesh.addColor(start);
        gradientMesh.addColor(start);
        gradientMesh.addColor(end);
        gradientMesh.addColor(end);
        gradientMesh.addColor(start);
        gradientMesh.addColor(end);
        gradientMesh.addColor(end);
        gradientMesh.addColor(start);
    }
    GLboolean depthMaskEnabled;
    glGetBooleanv(GL_DEPTH_WRITEMASK,&depthMaskEnabled);
    glDepthMask(GL_FALSE);
    gradientMesh.draw();
    if(depthMaskEnabled){
        glDepthMask(GL_TRUE);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 's') {
        gui.saveToFile("settings.xml");
        sync.save();
    }
    if(key == 'l') {
        gui.loadFromFile("settings.xml");
        sync.load();
    }
    
    if(key == ' ') {
        sync.learn();
    }
    if(key == 'u') {
        sync.unlearn();
    }
    if(key=='m'){
        port ++;
        port=port%numPorts;
        sync.reset();
        cout<<"listening to midi Port "+ofToString(port)<<endl;
        sync.setup(port, gui.getParameter());
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

//--------------------------------------------------------------

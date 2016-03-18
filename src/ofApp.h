#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxSyphon.h"
#include "ofxOscParameterSync.h"

#include "ofxParameterMidiSync.h"
#include "ofxMidiIn.h"

#include "defines.h"
//#include "soundWaveSystem.h"
#include "blink.h"
#include "boubble.h"
#include "dancingLine.h"
#include "waveSystem.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void ofRectGradient(int px, int py, int w, int h,const ofColor& start, const ofColor& end, ofGradientMode mode);
    
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    
    // Midisync
    ofxParameterMidiSync sync;
    ofxMidiIn midiIn;
    int port;
    int numPorts;
    
    // GUI
    ofxPanel gui;
    ofParameterGroup visualControl;
    ofParameter<float> inLeft;
    ofParameter<float> inRight;
    ofParameter<float> noiseAmt;
    
    ofParameter<float> posWave;
    ofParameter<float> posLine;
    
    
    
    
    ofParameter<ofColor> colorBG;
    ofParameter<ofColor> colorLine;
    ofParameter<ofColor> colorBGTop;
    ofParameter<ofColor> colorBGBot;
    ofParameter<ofColor> colorTopTop;
    ofParameter<ofColor> colorTopBot;
    ofParameter<ofColor> colorBotTop;
    ofParameter<ofColor> colorBotBot;
    
    
    ofxOscParameterSync syncOSC;
    
    
    

    ofFbo graficWave;
    
    ofFbo render;
    ofxSyphonServer syphonRenderOut;
    
    //WaveArray wave;
    
    WaveParticleSystem wave;
    
    // Blinkende Lyger
    // GUI / Controlpanel
    ofParameterGroup paramBlinkendeLyter;
    ofParameter<float> tempo;
    ofParameter<float> blinkIntensity;
    ofParameter<bool> hard_soft;
    ofParameter<ofColor> colorLygter;
    // BlinkendeLygter
    vector<Blink> blinks;
    ofFbo graficBlinkendeLygter;

    
    // Boubbles Lyger
    // GUI / Controlpanel
    ofParameterGroup paramBoubbles;
    ofParameter<float> boubblesIntensity;
    ofParameter<float> velMin;
    ofParameter<float> velMax;

    ofParameter<ofColor> colorBoubbles;
    // BlinkendeLygter
    vector<Boubble> boubbles;
    ofFbo graficBoubbles;

    
    // dancing Lines
    // GUI / Controlpanel
    ofParameterGroup paramLines;
    ofParameter<float> speed;
    ofParameter<float> syncLines;

    ofParameter<float> numOfLines;
    int numOfLinesLast;

    ofParameter<ofColor> colorDLines;

    vector<DancingLine> dancingLines;

        ofFbo graficDancingLines;

    float counterDLines;
    
    ofFbo fboTexWaves;

    
};


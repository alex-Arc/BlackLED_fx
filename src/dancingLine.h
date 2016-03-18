//
//  dancingLine.h
//  VardeWave_1
//
//  Created by Jonas Fehr on 11/12/15.
//
//

#include "ofMain.h"

class DancingLine{
public:
    ofVec2f location1;
    ofVec2f location2;
    float offset1, offset2;
    float   freedom;
    float sync;
    float seedOffset;
    
    ofColor lineColor;
    
    DancingLine() {
        lineColor = ofColor(0);
        seedOffset = ofRandom(1000);
        freedom = 100;
        sync = 1;
    }
    
    // Method to update location
    void update(float _timer) {
        offset1 = freedom * ((ofNoise(_timer+seedOffset))*1.8-0.4);
        offset2 = freedom * ((ofNoise(_timer*sync+seedOffset))*1.8-0.4);
        
    }
    // Method to display
    void draw() {
        
        
        ofSetColor(lineColor);
        ofSetLineWidth(1.);
        ofDrawLine(location1.x, location1.y+offset1,location2.x, location2.y+offset2);
        
    }
};
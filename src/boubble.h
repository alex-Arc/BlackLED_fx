//
//  bloubble.h
//  VardeWave_1
//
//  Created by Jonas Fehr on 11/12/15.
//
//

#include "ofMain.h"

class Boubble{
public:
    ofVec2f location;
    ofVec2f velocity;
    ofColor boubbleColor;
    
    Boubble() {
        velocity = ofVec2f(0., - ofRandom(0.1, 2.));
    }
    
    // Method to update location
    void update() {
        location.operator+=(velocity);
        
    }
    // Method to display
    void draw() {
        
        
        ofSetColor(boubbleColor);
        ofFill();
        ofSetLineWidth(0.);
        ofDrawRectangle(location.x,location.y, 1,1);
        
    }
    
    // Is the particle still useful?
    bool isDead() {
        if(location.y < 0){
            return true;
        } else {
            return false;
        }
    }
    
};
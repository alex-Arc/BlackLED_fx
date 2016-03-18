//
//  defines.h
//  springArray
//
//  Created by Jonas Fehr on 26/08/15.
//
//

// Rednering
#define RENDER_WIDTH 1920
#define RENDER_HEIGHT 1080

// Physics

#define NUM_ARRAY 100
#define DAMPING 0.96
#define ATTACK  0.04


// OSC
#define OSCRECEIVEPORT 7000
#define OSCSENDPORT 7001
#define OSCHOSTIP   "127.0.0.1"

// Visual
#define LINE_WIDTH 4
 

// UNUSED CODE
/*
 class Delay{
 public:
 void setup(int _length);
 void update();
 float get();
 float getIdx(int _idx);
 void set(float _valueIn);
 void addDamping(int _damping);
 
 vector<float> values;
 };
 
 
 
 class WaveArray : public Delay{
 public:
 void setup();
 void update(float _inLeft, float _inRight, float _damping);
 void drawLine(ofColor colorLine);
 void drawGradient(ofColor colorWaveTop, ofColor colorWaveBot);
 
 vector<float> values;
 
 Delay RL_delay;
 Delay LR_delay;
 };
 
 
 //---------------------------DELAY------------------------------
 void Delay::setup(int _length){
 
 for(int i = 0; i<_length; i++){
 values.push_back(0.);
 }
 }
 
 void Delay::set(float _valueIn){
 values.erase(values.begin());
 values.push_back(_valueIn);
 }
 
 float Delay::get(){
 return values.front();
 }
 
 float Delay::getIdx(int _idx){
 return values[_idx];
 }
 
 void Delay::addDamping(int _damping){
 for(int i = 0; i<values.size(); i++){
 values[i] = values[i] * _damping;
 }
 
 }
 
 
 
 
 //------------------------SPRING ARRAY--------------------------
 void WaveArray::setup(){
 for(int i = 0; i<NUM_ARRAY; i++){
 values.push_back(0.);
 
 }
 LR_delay.setup(NUM_ARRAY);
 RL_delay.setup(NUM_ARRAY);
 }
 
 void WaveArray::update(float _inLeft, float _inRight, float _damping){
 float valLR_delay = LR_delay.get();
 float valRL_delay = RL_delay.get();
 LR_delay.set(_inLeft - _damping * RL_delay.get());
 RL_delay.set(- _inRight - _damping * valLR_delay);
 
 LR_delay.addDamping(_damping);
 RL_delay.addDamping(_damping);
 
 for(int i = 0; i<NUM_ARRAY; i++){
 
 values[i]=LR_delay.getIdx(NUM_ARRAY-i)+RL_delay.getIdx(i);
 }
}

void WaveArray::drawLine(ofColor colorLine){
    vector <ofPoint> points;
    
    for(int i = 0; i < NUM_ARRAY-1; i++){
        
        if(false){
            ofFill();
            ofSetColor(80);
            ofSetCircleResolution(20);
            //ofCircle(p[i]->position, 10);
        }
        if(false){
            ofSetColor(255);
            ofSetLineWidth(2);
        }
    }
    
    if(true){
        ofSetLineWidth(4);
        ofSetColor(colorLine);
        ofBeginShape();
        ofNoFill();
        vector<ofPoint> points;
        float dist = RENDER_WIDTH/(NUM_ARRAY-1);
        for(int i = 0; i < NUM_ARRAY; i++){
            ofPoint point = ofVec2f(dist*i, (values[i]+1)/2*RENDER_HEIGHT);
            points.push_back(point);
        }
        
        ofCurveVertices(points);
        ofEndShape();
    }
    
    
}

void WaveArray::drawGradient(ofColor colorWaveTop, ofColor colorWaveBot){
    float dist = RENDER_WIDTH/(NUM_ARRAY-1);
    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    
    ofFloatColor colorTopTop_f = colorTopTop;
     ofFloatColor colorTopBot_f = colorTopBot;
     ofFloatColor colorBotTop_f = colorBotTop;
     ofFloatColor colorBotBot_f = colorBotBot;
     
     
     for(int i = 0; i < NUM_ARRAY; i++){
     mesh.addVertex( ofPoint(dist*i, 0));
     mesh.addColor(colorTopTop_f);
     
     mesh.addVertex(ofPoint(dist*i, (values[i]+1)/2*RENDER_HEIGHT));
     mesh.addColor(colorTopBot_f);
     
     mesh.addVertex(ofPoint(dist*i, (values[i]+1)/2*RENDER_HEIGHT));
     mesh.addColor(colorBotTop_f);
     
     mesh.addVertex(ofPoint(dist*i, RENDER_HEIGHT));
     mesh.addColor(colorBotBot_f);
     
     }
     for(int i = 0; i < NUM_ARRAY-1; i++){
     mesh.addIndex(i*4);
     mesh.addIndex(i*4+1);
     mesh.addIndex(i*4+4);
     
     mesh.addIndex(i*4+1);
     mesh.addIndex(i*4+5);
     mesh.addIndex(i*4+4);
     
     mesh.addIndex(i*4+2);
     mesh.addIndex(i*4+3);
     mesh.addIndex(i*4+6);
     
     mesh.addIndex(i*4+3);
     mesh.addIndex(i*4+7);
     mesh.addIndex(i*4+6);
     
     }
 
    mesh.draw();
    
}
 */
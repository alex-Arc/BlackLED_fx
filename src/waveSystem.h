//
//  waveSystem.h
//  VardeWave_1
//
//  Created by Jonas Fehr on 11/12/15.
//
//

#include "ofMain.h"


class WaveParticle {
    
    // Inspired by http://vvvv.org/documentation/wave-simulation
public:
    float p;
    float v;
    float a;
    float attack;
    float damping;
    
    WaveParticle(){
        p = v = a = 0;
        attack = 0;
        damping = 0;
    }
    
    void setResponse(float _attack, float _damping){
        attack = _attack;
        damping = _damping;
    }
    
    void calcAccel(float p_left, float p_right);
    
    void update( float p_left, float p_right ){
        float delta_p = p_left + p_right - 2*p;
        a = attack * delta_p;
        v = (v + a) * damping;
        p = p + v;
    }
};


class WaveParticleSystem{
public:
    vector<WaveParticle> waveParticles;
    
    void setup(int _numArray){
        for(int i = 0; i<_numArray; i++){
            WaveParticle wP;
            wP.p = 0.;
            waveParticles.push_back(wP);
        }
    }
    
    void updateResponse(float _attack, float _damping){
        for(int i = 0; i<waveParticles.size(); i++){
            waveParticles[i].damping = _damping;
            waveParticles[i].attack  = _attack;
        }
    }
    
    void update(float _inLeft, float _inRight){
        waveParticles[0].p = _inLeft;
        waveParticles.back().p = _inRight;
        
        for(int i = 1; i<waveParticles.size()-1; i++){
            waveParticles[i].update(waveParticles[i-1].p, waveParticles[i+1].p);
            int i2 = waveParticles.size()-1-i;
            waveParticles[i2].update(waveParticles[i2-1].p, waveParticles[i2+1].p);
        }
    }
    
    void drawLine(int _x, int _y,int _w, int _h, ofColor colorLine, float _posHLine, int _lineWidth){
        ofPushMatrix();
        ofTranslate(_x, _y);
        ofSetLineWidth(_lineWidth);
        ofSetColor(colorLine);
        float inc = waveParticles.size()/_w;
        float distPoints = _w/waveParticles.size();

        ofPolyline line;
        for(int i = 0; i <= NUM_ARRAY; i++){
            float x = i*distPoints;
            float y = (waveParticles[i].p/2+1-_posHLine)*_h;
            line.addVertex(ofVec2f(x,y));
        }
        line.draw();
        ofPopMatrix();
    }
    
    void drawGradient(int _x, int _y, int _w, int _h, float _posHWave , ofTexture& _texture){
        ofPushMatrix();
        ofTranslate(_x, _y);
        
        float inc = waveParticles.size()/(float)_w;
        float distPoints = _w/waveParticles.size();

        float incTex = _texture.getWidth()/_w;
        float texHeight = _texture.getHeight();
        float texHalfHeight = texHeight/2;
        
        ofMesh mesh;
        mesh.clear();
        mesh.enableTextures();
        mesh.setMode(OF_PRIMITIVE_TRIANGLES);
        
        float min = 0;
        float max = 0;
        
        for(int i = 0; i <= _w; i++){
            float pValue = waveParticles[i*inc].p*-1;
            if(pValue < min) min = pValue;
            if(pValue > max) max = pValue;
            
        }
        
        for(int i = 0; i <= NUM_ARRAY; i++){
            float x = i*distPoints;
            float y = (waveParticles[i].p/2+1-_posHWave)*_h;
            ofFloatColor pColor = ofFloatColor(1., ofMap( waveParticles[i*inc].p*-1, min, max, 0.8, 1, true));

            mesh.addVertex(ofVec3f(x, 0, 0));
            mesh.addTexCoord(ofVec2f(x,0));
            mesh.addColor(pColor);
            
            mesh.addVertex(ofVec3f(x, y, 0));
            mesh.addTexCoord(ofVec2f(x,texHalfHeight));
            mesh.addColor(pColor);
            
            mesh.addVertex(ofVec3f(x, _h, 0));
            mesh.addTexCoord(ofVec2f(x,texHeight));
            mesh.addColor(pColor);
        }
        
        for(int i = 0; i < mesh.getNumVertices()/3-1; i++){
            mesh.addIndex(i*3+1);
            mesh.addIndex(i*3+3);
            mesh.addIndex(i*3+0);
            
            mesh.addIndex(i*3+1);
            mesh.addIndex(i*3+4);
            mesh.addIndex(i*3+3);
            
            mesh.addIndex(i*3+2);
            mesh.addIndex(i*3+4);
            mesh.addIndex(i*3+1);
            
            mesh.addIndex(i*3+2);
            mesh.addIndex(i*3+5);
            mesh.addIndex(i*3+4);
        }
        ofSetColor( 255, 255, 255 );  //Set white color
        _texture.bind();
        mesh.draw();
        _texture.unbind();
        
        ofPopMatrix();
    }
    
};

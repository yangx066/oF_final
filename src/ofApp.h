#pragma once

#include "ofMain.h"


class agent {
public:
    agent();
    void update(agent *arr);
    void draw();
    void start(float X, float Y);
    void calcGravityWell(float x,float y);
private:
    float getRelAngle(ofVec2f neighbor);
    float calcAlignment(agent *arr, int max);
    ofVec2f calcCohesion(agent *arr, int max);
    ofVec2f calcDispersion(agent *arr, int max);
    ofVec2f location;
    ofVec2f velDir; // velocity & direction for each step
    ofVec2f mouse;  //
    
    bool active = false;	// if this agent is active
    
};



class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    int r=225;
    int g=225;
    int b=225;
    
    int r1=225;
    int g1=225;
    int b1=225;
    
    ofTrueTypeFont ins;
    ofTrueTypeFont goodF;
    ofTrueTypeFont badF;
    
    ofSoundPlayer good;
    ofSoundPlayer bad;

    ofImage girlStill;
    ofImage gt;
    ofImage bt;
    ofVideoPlayer girl;
    ofVideoPlayer goodtouch;
    ofPoint loc;
    float sTime;
    float sWeight;
    float fallSpeed;
    float fallWiggle;
    float rX;
    int pressState = 0;
    int pressState1 = 0;

int count = 0;

    
    ofArduino	ard;
private:
    
    void setupArduino(const int & version);
    void updateArduino();
    bool		bSetupArduino;
    
    
    
		
};

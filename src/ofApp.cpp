#include "ofApp.h"
#include <iostream>
int byteData;
using namespace std;
#define MAX_AGENTS 1000
agent Agents[MAX_AGENTS];

// constructor
agent::agent(){
    velDir.set(0, ofRandom(.5, 2));
    velDir.rotate(ofRandom(360));
    mouse.set(0,0);
}


void agent::update(agent *arr){
    
    int count = 0;
    ofVec2f noVec(-1,-1);
    
    if (!active)
        return;
    
    float alignDelta = calcAlignment(arr, MAX_AGENTS);	// set up aligment
    velDir.rotate(alignDelta*.05);			// apply alignment delta (weighted)
    
    count ++;
    
    // For agents nearby, get centroid, head that way
    ofVec2f middle = calcCohesion(arr, MAX_AGENTS);	// get centroid of nearby mass of agents
    if (middle != noVec) {
        float alignMouse = velDir.angle(middle - location);	// delta between centroid & location gives us a VECTOR
        velDir.rotate(alignMouse*.02);			// apply centroid gravity (weighted)
    }
    
    // For too-close agents try to move away from them
    ofVec2f tooClose = calcDispersion(arr, MAX_AGENTS);			// get avg heading of too-close agents
    if (tooClose != noVec) {
        float tooCloseDelta;
        if (velDir.angle((tooClose - location)) < 0) 			// if it is on our left side
            tooCloseDelta = velDir.angle((tooClose - location).rotate(90));	// move opposite to mass
        else
            tooCloseDelta = velDir.angle((tooClose - location).rotate(-90));  // move opposite to mass
        velDir.rotate(tooCloseDelta*.03);					// apply centroid gravity (weighted)
    }
    
    //mouse gravity
    float tooFarfromMouse = mouse.distance(location);
    float angle;
    float h;
    if(tooFarfromMouse<500 && tooFarfromMouse >300)
    {
        angle = velDir.angle(mouse-location);
        velDir.rotate(angle*0.06);
    }
    else if(tooFarfromMouse<300 && tooFarfromMouse>100)
    {
        if(velDir.angle(location-mouse)<0)
            angle = velDir.angle((location-mouse).rotate(90));
        else
            angle = velDir.angle((location-mouse).rotate(-90));
        velDir.rotate(angle*0.06);
    }
    else if(tooFarfromMouse <100)
    {
        h = 0.4;
        velDir += (0,h);
    }
    float * locationPtr = location.getPtr();
    float y = *(locationPtr+1);
    if(y > ofGetHeight()) y = ofRandom(.5,2);
    else location += velDir;	// move to current location
    
    
}

void agent::draw(){
    if (!active)  		// if not active, do nothing
        return;
    
    ofVec2f top(0,-1);		// top of origin used as control
    ofVec2f p1(6.25,  12);	// triangle pointing up w middle at origin
    ofVec2f p2(0, -12);
    ofVec2f p3(-6.25,  12);
    
    
    float angle = top.angle(velDir);	// get direction angle
    
    p1.rotate(angle);	// point in direction of movement
    p2.rotate(angle);
    p3.rotate(angle);
    
    // if it moves off the board, move it to opposite side
    if (location.x > ofGetWidth()) location.x = 0;
    if (location.x < 0) location.x = ofGetWidth();
    if (location.y > ofGetHeight()) location.y = 0;
    if (location.y < 0) location.y = ofGetHeight();
    
    p1 += location;	// move to current location
    //    p2 += location+(ofRandom(0,60),ofRandom(0,30));
    //    p3 += location+(ofRandom(0,30),ofRandom(0,60));
    
    
    
    
    
    ofDrawCircle(p1, 2);
    //    ofDrawCircle(p2, 3);
    //    ofDrawCircle(p3, 3);
    
    
    
}

void agent::start(float x, float y){
    active = true; // turn this one on!
    location.set(x,y);
}


// Get relative angle from me -180 - 180
float agent::getRelAngle(ofVec2f neighbor){
    float angle = velDir.angle(neighbor);
    return (angle);
}


// get mouse location for gravity
void agent::calcGravityWell(float x,float y){
    mouse.set(x,y);
}

// average & return neighbor's VECTORS
float agent::calcAlignment(agent *arr, int max){
    float sum = 0;  // sum of angles
    int sumNum = 0; // num of angles summed
    
    for (int i=0; i < max; i++) {
        float distance = location.distance(arr[i].location);
        if (distance < 600 && distance > 0 && arr[i].active) { 	// if a neighbor is close & active
            //cout  << " Dist: " << distance << "\n";
            sum += getRelAngle(arr[i].velDir);			// add its angle to sum
            sumNum++;							// count how many are summed
        }
    }
    if (sumNum > 0) {   // avoid div by 0
        return(sum/sumNum);
    } else
        return(0);
}

// average and return neighbor's LOCATIONS
ofVec2f agent::calcCohesion(agent *arr, int max){
    ofVec2f centroid, nearArr[MAX_AGENTS];				// centroid & arr of Neighbors near us
    int lenArr = 0;
    
    for (int i=0; i < max; i++) {
        float distance = location.distance(arr[i].location);
        
        if (distance < 70 && distance > 20 && arr[i].active) { 	// if a neighbor is close & active
            nearArr[lenArr] = arr[i].location;			// add its location to array
            lenArr++;							// count how many are added
        }
    }
    
    if (lenArr > 0) {
        centroid.average(nearArr, lenArr);	// average the location of all neighbor agents
        return (centroid);
    } else {
        ofVec2f noVec(-1,-1);
        return (noVec);  			// no nearby agents
    }
}

// average and return too-close neighbor's LOCATIONS
ofVec2f agent::calcDispersion(agent *arr, int max){
    ofVec2f centroid, nearArr[MAX_AGENTS];		// centroid & arr of Neighbors near us
    int lenArr = 0;
    
    for (int i=0; i < max; i++) {
        float distance = location.distance(arr[i].location);
        
        if (distance < 20 && arr[i].active) { 	// if a neighbor is close & active
            nearArr[lenArr] = arr[i].location;	// add its location to array
            lenArr++;					// count how many are added
        }
    }
    
    if (lenArr > 0) {
        centroid.average(nearArr, lenArr);		// average the location of all too-close neighbor agents
        return (centroid);
    } else {
        ofVec2f noVec(-1,-1);
        return (noVec);				// no nearby agents
    }
}




int curAgent = 0;			// what agent we are dealing with











//--------------------------------------------------------------
void ofApp::setup(){
    ard.connect("/dev/cu.usbmodem43",57600);
    ofAddListener(ard.EInitialized, this, &ofApp::setupArduino);
    ofAddListener(ard.EInitialized, this, &ofApp::setupArduino);
    bSetupArduino	= false;
    
    
    
    ofBackground(255, 255, 255);
    girlStill.load("girl.png");
    gt.load("gt.png");
    bt.load("bt.png");
    girl.load("movies/girl.mp4");
    goodtouch.load("movies/goodtouch.mp4");
    girl.play();
    goodtouch.play();
    good.load("sound/good.mp3");
    bad.load("sound/bad.mp3");
    sTime = ofGetElapsedTimef();
    sWeight = ofRandom(0.1,0.4);
    
    for (curAgent; curAgent < MAX_AGENTS; curAgent++) {  // only enable if we disable mouse gravity
        Agents[curAgent].start(ofRandom(ofGetWidth()),ofRandom(ofGetHeight()));
    }

}

//--------------------------------------------------------------
void ofApp::update(){
    updateArduino();
    
    girl.update();
    goodtouch.update();
    float time = sTime + ofGetElapsedTimef();
    rX = ofSignedNoise(time * 1)*10.0;

    fallWiggle = ofRandom(-5,5);
    
    for (int i=0; i < MAX_AGENTS; i++) {
        Agents[i].update(Agents);
    }
    

}

void ofApp::setupArduino(const int & version) {
    
    ofRemoveListener(ard.EInitialized, this, &ofApp::setupArduino);
    
    // it is now safe to send commands to the Arduino
    bSetupArduino = true;
    
    ofLogNotice() << ard.getFirmwareName();
    ofLogNotice() << "firmata v" << ard.getMajorFirmwareVersion() << "." << ard.getMinorFirmwareVersion();
    
    ard.sendAnalogPinReporting(0, ARD_ANALOG);
    ard.sendAnalogPinReporting(3, ARD_ANALOG);

    
}

void ofApp::updateArduino(){
    
    ard.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    int data0 = ard.getAnalog(0);
    int data1 = ard.getAnalog(3);

    cout << data0 << endl;
    cout << data1 << endl;

    ofSetColor(255,255,255);
    girlStill.draw(-48,10,510,700);

    float z = ofSignedNoise((ofGetElapsedTimef() + sTime) * 0.9) * 10;

    bool goodT = true;
    
    if(data0>5)
    {
        pressState ++;
    }
    else pressState = 0;

    if(pressState == 1){
        bad.play();
    }
    if(pressState >0) {
        ofBackground(255, 255, 255);
        girl.draw(-435+fallWiggle,0);
        ofPopMatrix();
        
        r=0;
        g=0;
        b=0;
        r1=0;
        g1=0;
        b1=0;
        bt.draw(50+ofRandom(0,fallWiggle),500,250,50);


    }
    
    if(data1>5)
    {
        pressState1 ++;
    }
    else pressState1 = 0;
    if(pressState1 == 1){
        good.play();
    }
    
    if(pressState1 >0) {
        ofBackground(255, 255, 255);
        goodtouch.draw(-435,0);
        
        r=242;
        g=167;
        b=184;
        r1=118;
        g1=180;
        b1=227;
        gt.draw(50+fallWiggle,500,250,50);
        ofPopMatrix();

        
    }
    

    
    for (int i=0; i < MAX_AGENTS; i++) {
        Agents[i].calcGravityWell(ofGetWidth()/2+20,ofGetHeight()/2);
    }
    
    int alpha = 255;
    
//    ofSetColor(ofRandom(118, 242),ofRandom(167,180),ofRandom(184,227));
    

    ofColor aqua(r,g,b);
    ofColor purple(r1,g1,b1,alpha);
    ofColor inbetween = aqua.getLerped(purple, ofRandom(1.0));  // linear interpolation between colors color
    
    for (int i=0; i < MAX_AGENTS; i++) {
        ofSetColor(inbetween);
        Agents[i].draw();
        
    }
    ins.load("FuturBoo",15);
    ofSetColor(49, 76, 123);
    ins.drawString("TOUCH DIFFERENT PART OF HER",45,90);
    ins.drawString("BODY AND SEE WHAT WILL HAPPEN!",25, 120);
    
    goodF.load("FuturBoo",15);
    badF.load("FuturBoo",15);




        
        
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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

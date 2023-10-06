#include "ofApp.h"
#include "Particle.h"


//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
	
	int num = 1500;
	p.assign(num, Particle());
	currentMode = PARTICLE_MODE_ATTRACT;

	currentModeStr = "1 - PARTICLE_MODE_ATTRACT: attracts to mouse"; 

	resetParticles();
}

//--------------------------------------------------------------
void ofApp::resetParticles(){

	//these are the attraction points used in the fourth demo 
	attractPoints.clear();
	for(int i = 0; i < 4; i++){
		attractPoints.push_back( glm::vec3( ofMap(i, 0, 4, 100, ofGetWidth()-100) , ofRandom(100, ofGetHeight()-100) , 0) );
	}
	
	attractPointsWithMovement = attractPoints;
	
	for(unsigned int i = 0; i < p.size(); i++){
		p[i].setMode(currentMode);		
		p[i].setAttractPoints(&attractPointsWithMovement);;
		p[i].reset();
	}	
}

//--------------------------------------------------------------
void ofApp::update(){
	if(!isPaused){
		for(unsigned int i = 0; i < p.size(); i++){
			p[i].setMode(currentMode);
			p[i].update();
			p[i].new_rect = rect;
		}
	
		//lets add a bit of movement to the attract points
		for(unsigned int i = 0; i < attractPointsWithMovement.size(); i++){
			attractPointsWithMovement[i].x = attractPoints[i].x + ofSignedNoise(i * 10, ofGetElapsedTimef() * 0.7) * 12.0;
			attractPointsWithMovement[i].y = attractPoints[i].y + ofSignedNoise(i * -10, ofGetElapsedTimef() * 0.7) * 12.0;
		}

		if(isReplaying==true && fpscounter<=61){
			fpscounter++;
			if(keys[index]=='1'){
				currentMode = PARTICLE_MODE_ATTRACT;
				currentModeStr = "1 - PARTICLE_MODE_ATTRACT: attracts to mouse";
			}
			if(keys[index]=='2'){
				currentMode = PARTICLE_MODE_REPEL;
				currentModeStr = "2 - PARTICLE_MODE_REPEL: repels to mouse";
			}
			if(keys[index]=='3'){
				currentMode = PARTICLE_MODE_NEAREST_POINTS;
				currentModeStr = "3 - PARTICLE_MODE_NEAREST_POINTS: attracts to nearest points";
			}
			if(keys[index]=='4'){
				currentMode = PARTICLE_MODE_NOISE;
				currentModeStr = "4 - PARTICLE_MODE_NOISE: snow particle simulation";
				resetParticles();
			}
			if( keys[index] == ' ' && fpscounter==1 ){
				resetParticles();
			}

			if(keys[index] == 's' && fpscounter==1){
				isPaused=!isPaused;
			}

			if (keys[index] == 'd' && fpscounter==1){
				for (unsigned int i=0; i<p.size();i++){
					p[i].vel*= 2.0;
				}
			}

			if(keys[index] == 'a' && fpscounter==1){
				for (unsigned int i=0;i<p.size();i++){
					p[i].vel*= 0.5;
				}
			}

			if (keys[index] == 't' && fpscounter==1){
				for(unsigned int i=0;i<p.size();i++){
					p[i].colorchange=true;
					p[i].t_count+=1;
				}
			}
			if(fpscounter==60){
				index++;
				fpscounter=0;
			}
			if(index==keys.size()){
				isReplaying=false;
				index=0;
			}
		}
	}	
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackgroundGradient(ofColor(60,60,60), ofColor(10,10,10));

	for(unsigned int i = 0; i < p.size(); i++){
		p[i].draw();
	}
	ofSetColor(ofColor::yellow);
	ofNoFill();
	ofDrawRectangle(rect);
	
	ofSetColor(190);
	if( currentMode == PARTICLE_MODE_NEAREST_POINTS ){
		for(unsigned int i = 0; i < attractPoints.size(); i++){
			ofNoFill();
			ofDrawCircle(attractPointsWithMovement[i], 10);
			ofFill();
			ofDrawCircle(attractPointsWithMovement[i], 4);
		}
	}

	ofSetColor(230);	
	ofDrawBitmapString(currentModeStr + "\n\nSpacebar to reset. \nKeys 1-4 to change mode.", 10, 20);

	if(isRecording==true){
		ofSetColor(255,0,0);
		ofDrawBitmapString("Is recording...",40,100);
	}
	if(isReplaying==true){
		ofSetColor(0,255,0);
		if (index < keys.size()) {
            string keyString = "Replaying: " + ofToString(keys[index]);
            ofDrawBitmapString(keyString, 40, 100);
        }
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(!isReplaying){
		if( key == '1'){
			currentMode = PARTICLE_MODE_ATTRACT;
			currentModeStr = "1 - PARTICLE_MODE_ATTRACT: attracts to mouse"; 		
		}
		if( key == '2'){
			currentMode = PARTICLE_MODE_REPEL;
			currentModeStr = "2 - PARTICLE_MODE_REPEL: repels from mouse"; 				
		}
		if( key == '3'){
			currentMode = PARTICLE_MODE_NEAREST_POINTS;
			currentModeStr = "3 - PARTICLE_MODE_NEAREST_POINTS: attracts to nearest points"; 						
		}
		if( key == '4'){
			currentMode = PARTICLE_MODE_NOISE;
			currentModeStr = "4 - PARTICLE_MODE_NOISE: snow particle simulation"; 						
			resetParticles();
		}	
		
		if( key == ' ' ){
			resetParticles();
		}

		if(key == 's'){
			isPaused=!isPaused;
		}

		if (key == 'd'){
			for (unsigned int i=0; i<p.size();i++){
				p[i].vel*= 2.0;
			}
		}

		if(key == 'a'){
			for (unsigned int i=0;i<p.size();i++){
				p[i].vel*= 0.5;
			}
		}

		if (key == 't'){
			for(unsigned int i=0;i<p.size();i++){
				p[i].colorchange=true;
				p[i].t_count+=1;
			}
		}

		if(key=='r'){
			if(isRecording==false){
				isRecording=true;
				keys.clear();
			}
			else{isRecording=false;}
		}
		if(isRecording==true && key != 'p'){
			keys.push_back(key);
		}
		if(key == 'p' && !isRecording && !keys.empty()){
			isReplaying=true;
		}
	}
		if(key =='c'){
			isReplaying=false;
					index=0;
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
if (button == 0){
	x1 = x;
	y1 = y;
}
else if (button == 2 && rect.inside(x,y)){
	rect = ofRectangle(0, 0, 0, 0);
}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
if (button == 0){
	x2 = x;
	y2 = y;
	rect = ofRectangle(x1,y1, x2-x1, y2-y1);
}
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

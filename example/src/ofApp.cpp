#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
//	ofSetFrameRate(60);
//	ofSetVerticalSync(true);
	
	ofBackground(0);
	fbo.allocate(1920, 1080, GL_RGB);
}

//--------------------------------------------------------------
void ofApp::update() {
    
}

//--------------------------------------------------------------
void ofApp::draw() {
    // draw to the fbo //
    fbo.begin(); {
        int c = (sin(ofGetElapsedTimef()) * 0.5 + 0.5) * 100 + 150;
        ofClear(c, c, c);
        ofSetColor( 20 );
        ofDrawRectangle( (cos(ofGetElapsedTimef()*3.f) * 0.5f + 0.5f) * fbo.getWidth(), 0, 50, fbo.getHeight() );
        ofDrawBitmapStringHighlight("App frame: "+ofToString(ofGetFrameNum(), 0), fbo.getWidth()/2, fbo.getHeight()/2 );
    } fbo.end();
    
    if (mode == 0) {
        reader.readToPixels(fbo, pix);
    } else if (mode == 1) {
        fbo.readToPixels(pix);
    }
    
    if(bSave) {
        if( pix.getWidth() > 0 && pix.getHeight() > 0 ) {
            ofSaveImage( pix, ofGetTimestampString()+".png" );
        }
    }
    
    ofSetColor( 255 );
    fbo.draw(0,0,ofGetWidth(), ofGetHeight() );
    
    stringstream ss;
    ss << "press space to save screen: " << (bSave ? "saving" : "ready") << endl;
    ss << "mode (m): " << (mode==0 ? "Fast" : "default") << endl;
    ss << "Async (a): " << (reader.getAsync() ? "yes" : "no");
    ofDrawBitmapStringHighlight( ss.str(), 20, 24);
    
    bSave = false;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == 'm') {
		mode++;
		mode = mode % 2;
	} else if (key == 'a') {
        reader.setAsync(!reader.getAsync());
	} else if( key == ' ' ) {
        bSave = true;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{

}


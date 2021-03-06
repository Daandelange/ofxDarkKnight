#include "DKWebCam.h"

void DKWebCam::setup()
{
	camWidth = 1920;
	camHeight = 1080;

	//get back a list of devices.
	vector<ofVideoDevice> devices = vidGrabber.listDevices();

	for (size_t i = 0; i < devices.size(); i++) {
		if (devices[i].bAvailable) {
			//log the device
			ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
		}
		else {
			//log the device and note it as unavailable
			ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
		}
	}

	vidGrabber.setDeviceID(0);
	vidGrabber.setDesiredFrameRate(60);
	vidGrabber.initGrabber(camWidth, camHeight);
	
	drawFbo = false;
	fbo = nullptr;
	scaleX = scaleY = 0.25;

	addOutputConnection(DKConnectionType::DK_FBO);


	fbo = new ofFbo;
	fbo->allocate(camWidth, camHeight, GL_RGB);
	fbo->begin();
	ofClear(0, 0, 0, 0);
	fbo->end();


	gui->setWidth(camWidth * scaleX);

}

void DKWebCam::update()
{
	vidGrabber.update();
	
	fbo->begin();
	vidGrabber.draw(0, 0);
	fbo->end();

}

void DKWebCam::draw()
{
	//if (drawFbo)
	//{
		ofPushMatrix();
		ofTranslate(gui->getPosition().x, gui->getPosition().y + 50);
		ofScale(scaleX, scaleY);
		fbo->draw(0, 0);
		ofPopMatrix();
	//}
}

void DKWebCam::addModuleParameters()
{
	int numDevices = vidGrabber.listDevices().size();
	ofxDatGuiMatrix* matrix = gui->addMatrix("cam index", numDevices, true);
	matrix->onMatrixEvent(this, &DKWebCam::onInputChange);
	matrix->setRadioMode(true);
	matrix->setSelected({ 0 });
}
	
ofFbo* DKWebCam::getFbo()
{
	return fbo;
}

void DKWebCam::onInputChange(ofxDatGuiMatrixEvent e)
{
	vidGrabber.close();
	vidGrabber.setDeviceID(e.child);
	vidGrabber.setDesiredFrameRate(60);
	vidGrabber.initGrabber(camWidth, camHeight);
}

#include "ofApp.h"
#include <ostream>

#define kMoveInc 10
#define kRotInc 5

ofApp::ofApp()
{
	renderer2d = nullptr;
	rendererModel = nullptr;
    renderer3d = nullptr;
}

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60);
	ofSetWindowTitle("Paint3D+");

    m_firstTimeSelection = 1;
	m_grid = false;
	m_canSetPanelPos = false;
	renderer2d = new Renderer2D();
    renderer2d->setup("Parameters", this);

    renderer3d = new Renderer3D();
    renderer3d->setup("3D", this);

	rendererModel = new RendererModel(); //*
	rendererModel->setup();

	//Panels params
	menuBarParams.setName("Menu");
	menuBarParams.add(b2D.set("2D", true));
	menuBarParams.add(b3D.set("3D", false));
	menuBarParams.add(bModelMode.set("Model Mode", false)); //*
	importButton = new ofxButton();
	exportButton = new ofxButton();
	displacementMapButton = new ofxButton();
	cubeMapButton = new ofxButton();
	mergeButton = new ofxButton();
	unmergeButton = new ofxButton();
    next = new ofxButton();
    unselect = new ofxButton();
    catmullRomCurveButton = new ofxButton();
    nurbsButton = new ofxButton();
    bezierCurveButton = new ofxButton();
    bezierSurfaceButton = new ofxButton();
    surfaceCoonsButton = new ofxButton();

    shapesParams.setName("2D Shapes");
	shapesParams.add(bLine.set("Line", false));
	shapesParams.add(bTriangle.set("Triangle", false));
	shapesParams.add(bRectangle.set("Rectangle", false));
	shapesParams.add(bCircle.set("Circle", false));

    shapes3DParams.setName("3D Shapes");
    shapes3DParams.add(bCube.set("Cube", false));
    shapes3DParams.add(bSphere.set("Sphere", false));
    shapes3DParams.add(bCamera.set("Camera", false));
    shapes3DParams.add(bControlPoint.set("Control Points", false));

	shapesSettingsParams.setName("2D Settings");
	shapesSettingsParams.add(vSync.set("vSync",true));
	shapesSettingsParams.add(bgrid.set("Grid", false));
	shapesSettingsParams.add(renderer2d->parameters);

    //cameraParams.add();

	importButton->setup("Import");
	exportButton->setup("Export");
	displacementMapButton->setup("Apply Displacement Map");
	cubeMapButton->setup("Apply Cube Map");

	mergeButton->setup("Merge Shapes");
	unmergeButton->setup("Unmerge Shapes");
    next->setup("Select next 3D shape");
    unselect->setup("Unselect");
    catmullRomCurveButton->setup("CatmullRom Curve");
    nurbsButton->setup("Nurbs Curve");
    bezierCurveButton->setup("Bezier Cubique Curve");
    bezierSurfaceButton->setup("Bezier Cubique Surface");
    surfaceCoonsButton->setup("Coons Surface");

	//Setup panels
	menuPanel.setup(menuBarParams);
	menuPanel.add(importButton);
	menuPanel.add(exportButton);
	menuPanel.add(bSelect.set("Selection", false));
	menuPanel.add(bAntialiasing.set("Antialiasing", false));
	shapesPanel.setup(shapesParams);
	shapesPanel.add(mergeButton);
	shapesPanel.add(unmergeButton);
    shapesParamsPanel.setup(shapesSettingsParams);
	shapesParamsPanel.setName("Shapes parameters");
    shapes3DPanel.setup(shapes3DParams);
    shapes3DPanel.add(catmullRomCurveButton);
    shapes3DPanel.add(nurbsButton);
    shapes3DPanel.add(bezierCurveButton);
    shapes3DPanel.add(bezierSurfaceButton);
    shapes3DPanel.add(surfaceCoonsButton);
    shapes3DPanel.add(next);
    shapes3DPanel.add(unselect);
	shapes3DPanel.add(bHeightMap.set("Apply Height Map", false));
	shapes3DPanel.add(displacementMapButton);
	shapes3DPanel.add(cubeMapButton);
    shapes3DPanel.add(renderer3d->parameters3D);
    cameraPanel.setup(renderer3d->cameraSettings);
    cameraPanel.setPosition(shapes3DPanel.getWidth(), 0);
	menuPanel.setPosition(0, 0);
	shapesPanel.setPosition(0, 5 + menuPanel.getHeight());
	shapesParamsPanel.setPosition(0, 5 + shapesPanel.getHeight() + menuPanel.getHeight());
    shapes3DPanel.setPosition(0, 5 + menuPanel.getHeight());

	//Listeners
	b2D.addListener(this, &ofApp::b2DChanged);
	b3D.addListener(this, &ofApp::b3DChanged);
	bModelMode.addListener(this, &ofApp::bModelModeChanged); //*
	importButton->addListener(this, &ofApp::buttonPressed);
	exportButton->addListener(this, &ofApp::buttonPressed);
	mergeButton->addListener(this, &ofApp::buttonPressed);
	unmergeButton->addListener(this, &ofApp::buttonPressed);
    catmullRomCurveButton->addListener(this, &ofApp::buttonPressed);
    nurbsButton->addListener(this, &ofApp::buttonPressed);
    bezierCurveButton->addListener(this, &ofApp::buttonPressed);
    bezierSurfaceButton->addListener(this, &ofApp::buttonPressed);
    surfaceCoonsButton->addListener(this, &ofApp::buttonPressed);
    next->addListener(this, &ofApp::buttonPressed);
    unselect->addListener(this, &ofApp::buttonPressed);
	bHeightMap.addListener(this, &ofApp::bHeightMapChanged);
	displacementMapButton->addListener(this, &ofApp::buttonPressed);
	cubeMapButton->addListener(this, &ofApp::buttonPressed);
	bLine.addListener(this, &ofApp::bLineChanged);
	bTriangle.addListener(this, &ofApp::bTriangleChanged);
	bRectangle.addListener(this, &ofApp::bRectangleChanged);
	bCircle.addListener(this, &ofApp::bCircleChanged);
	bSelect.addListener(this, &ofApp::bSelectChanged);
    bCube.addListener(this, &ofApp::bCubeChanged);
    bSphere.addListener(this, &ofApp::bSphereChanged);
    bCamera.addListener(this, &ofApp::bCameraChanged);
    bControlPoint.addListener(this, &ofApp::bControlChanged);
	bgrid.addListener(this, &ofApp::bGridChanged);

    //gui.loadFromFile("settings.xml");

    font.load(OF_TTF_SANS, 10, true, true);
    ofEnableAlphaBlending();

	//Default Parameters
	m_state = AppState::ACTION_SELECT;
	m_mode = MODE_2D;
	m_clickRadius = 5;
	isTakingScreenshot = false;
	isClearingButtonsShapes = false;
	isClearingButtonsModes = false;
    m_selectionIndex = 0;
    m_addPan = 0;
    m_addTilt = 0;
    m_changeCameraOrientation = 0;
    showCamera = 0;

	showGui2D = true;
	showGui3D = false;
	ofSetLogLevel(OF_LOG_VERBOSE);

    s = string("") +
            "\n" +
            "Camera Mode, Keys:\n" +
            "Mouse and click to look\n"
            "LEFT pan left\n" +
            "RIGHT pan right\n" +
            "UP tilt up\n" +
            "DOWN tilt down\n" +
            ", roll left\n" +
            ". roll right\n" +
            "a truck left\n" +
            "d truck right\n" +
            "w dolly forward\n" +
            "s dolly backward\n" +
            "r boom up\n" +
            "f boom down\n" +
            "o: Orthogonal Mode\n" +
            "p: Projection Mode\n"+
            "c: Add a new camera\n"+
            "v: Switch between cameras\n"+
            "b: Delete current camera";

}

void ofApp::b2DChanged(bool & p_2D) {
	if (!isClearingButtonsModes) {
		m_canSetPanelPos = true;
		isClearingButtonsModes = true;
        b3D.set(false);
		bModelMode.set(false); //*
        b2D.set(p_2D);
		isClearingButtonsModes = false;
		m_mode = MODE_2D;
		showGui2D = true; //*
		showGui3D = false; //*
		showCamera = false;
	}
}

void ofApp::b3DChanged(bool & p_3D) {
	if (!isClearingButtonsModes) {
		m_canSetPanelPos = true;
		isClearingButtonsModes = true;
        b2D.set(false);
		bModelMode.set(false); //*
        b3D.set(p_3D);
		isClearingButtonsModes = false;
		m_mode = MODE_3D;
		showGui3D = true; //*
		showGui2D = false;
	}
}

void ofApp::bModelModeChanged(bool & p_modelMode) { //*
	if (!isClearingButtonsModes) {
		isClearingButtonsModes = true;
		b2D.set(false);
		b3D.set(false);
		bModelMode.set(true);
		isClearingButtonsModes = false;
		m_mode = MODE_MODEL;
		showGui2D = false;
		showGui3D = false;
	}
}

void ofApp::vSyncChanged(bool & vSync){
	ofSetVerticalSync(vSync);
}

void ofApp::bLineChanged(bool & pLine) {
	if (!isClearingButtonsShapes) {
        b3D.set(false);
        b2D.set(true);
		clearButtons();
		isClearingButtonsShapes = false;
		bLine.set(true);
		m_buffer.clear();
		m_state = AppState::BUILD_LINE;
	}
}
void ofApp::bTriangleChanged(bool & pTriangle) {
	if (!isClearingButtonsShapes) {
		clearButtons();
		isClearingButtonsShapes = false;
		bTriangle.set(true);
        b3D.set(false);
        b2D.set(true);
		m_buffer.clear();
		m_state = AppState::BUILD_TRIANGLE;
	}
}
void ofApp::bRectangleChanged(bool & pRectangle) {
	if (!isClearingButtonsShapes) {
		clearButtons();
		isClearingButtonsShapes = false;
		bRectangle.set(true);
        b3D.set(false);
        b2D.set(true);
		m_buffer.clear();
		m_state = AppState::BUILD_RECTANGLE;
	}
}
void ofApp::bCircleChanged(bool & pCircle) {
	if (!isClearingButtonsShapes) {
		clearButtons();
		isClearingButtonsShapes = false;
		bCircle.set(true);
        b3D.set(false);
        b2D.set(true);
		m_buffer.clear();
		m_state = AppState::BUILD_CIRCLE;
	}
}
void ofApp::bSelectChanged(bool & pSelect) {
	if (!isClearingButtonsShapes) {
		clearButtons();
		isClearingButtonsShapes = false;
		bSelect.set(true);
		m_buffer.clear();
		m_state = AppState::ACTION_SELECT;
	}
}

void ofApp::bGridChanged(bool & pGrid) {
	bgrid.set(pGrid);
	m_grid = pGrid;
}

void ofApp::bHeightMapChanged(bool & pHeightMap) {
	if (!isClearingButtonsShapes) {
		clearButtons();
		bHeightMap.set(true);
		isClearingButtonsShapes = false;
		b2D.set(false);
		b3D.set(true);
		m_buffer.clear();
		m_state = AppState::BUILD_TERRAIN;
	}
}

void ofApp::bCubeChanged(bool & p_cube) {
    if (!isClearingButtonsShapes) {
        clearButtons();
        isClearingButtonsShapes = false;
        bCube.set(true);
        b2D.set(false);
        b3D.set(true);
        m_buffer.clear();
        m_state = AppState::BUILD_CUBE;
    }
}

void ofApp::bSphereChanged(bool & p_sphere) {
    if (!isClearingButtonsShapes) {
        clearButtons();
        isClearingButtonsShapes = false;
        bSphere.set(true);
        b2D.set(false);
        b3D.set(true);
        m_buffer.clear();
        m_state = AppState::BUILD_SPHERE;
    }
}

void ofApp::bCameraChanged(bool & p_camera) {
    if (!isClearingButtonsShapes) {
        clearButtons();
		bCamera.set(false);
        isClearingButtonsShapes = false;
        bCamera.set(true);
        m_buffer.clear();
        m_state = AppState::CAMERA;
        showCamera = true;
    }
}

void ofApp::clearButtons() {
	isClearingButtonsShapes = true;
	bSelect.set(false);
	bLine.set(false);
	bTriangle.set(false);
	bRectangle.set(false);
	bCircle.set(false);
    bCube.set(false);
    bSphere.set(false);
    bCamera.set(false);
	bHeightMap.set(false);
}

//--------------------------------------------------------------
void ofApp::update(){
	// frameNum is a readonly parameter so this will fail to compile
	// unless we are inside the CirclesRenderer class
	// renderer.frameNum = 5;

    if (m_mode == MODE_3D && !mouseIsOverPanel() && m_state == CAMERA
            && m_changeCameraOrientation){

        m_currentPosition2X = ofGetMouseX();
        m_currentPosition2Y = ofGetMouseY();
        m_addPan = m_currentPosition2X - m_currentPositionX;
        m_addTilt = m_currentPosition2Y - m_currentPositionY;

        renderer3d->cam[renderer3d->m_camSelection].pan(-m_addPan);
        renderer3d->cam[renderer3d->m_camSelection].tilt(m_addTilt);

        m_currentPositionX = m_currentPosition2X;
        m_currentPositionY = m_currentPosition2Y;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackgroundGradient(ofColor::white, ofColor::gray);
	font.drawString("fps: " + ofToString((int)ofGetFrameRate()), ofGetWidth() - 150, 0);

	if (bAntialiasing) {
		ofEnableAntiAliasing();
	}
	else {
		ofDisableAntiAliasing();
	}

	if (m_mode == MODE_2D) {
		if (m_grid) {
			drawGrid();
		}
        renderer2d->draw();
	}
	else if (m_mode == MODE_MODEL) {
		rendererModel->draw();
	}
    else if (m_mode == MODE_3D){
        //ofPushMatrix();
        renderer3d->draw();
        //ofPopMatrix();
    }

    ofSetColor(255);
	if (!isTakingScreenshot) {
		menuPanel.draw();
        if (showGui2D) shapesPanel.draw();
		if (showGui2D) {
			if (m_canSetPanelPos) {
				shapesParamsPanel.setPosition(0, 5 + shapesPanel.getHeight() + menuPanel.getHeight());
				m_canSetPanelPos = false;
			}
			shapesParamsPanel.draw();
		}
		if (showGui3D) {
			if (m_canSetPanelPos) {
				shapesParamsPanel.setPosition(ofGetWindowWidth() - shapesParamsPanel.getWidth() - 5, 0);
				m_canSetPanelPos = false;
			}
			shapesParamsPanel.draw();
			shapes3DPanel.draw();
		}
        if (showCamera){
            ofDrawBitmapString(s, ofPoint(ofGetWidth()-202, ofGetHeight()-270));
            cameraPanel.setPosition(shapes3DPanel.getWidth()+5, 0);
            cameraPanel.draw();
        }
	}

	drawCursor();
}

void ofApp::drawCursor() {
	int mouseX = ofGetMouseX();
	int mouseY = ofGetMouseY();

	if (mouseIsOverPanel()) {
		ofShowCursor();
		return;
	}

	ofHideCursor();
	ofSetColor(0);
	ofSetLineWidth(4);
	ofNoFill();
	switch (m_state) {
	case AppState::ACTION_SELECT:
	case AppState::ACTION_GROUPSELECT:
		ofShowCursor();
		break;
	case AppState::BUILD_RECTANGLE:
		ofDrawRectangle(mouseX, mouseY, 16, 16);
		break;
	case AppState::BUILD_TRIANGLE:
		ofDrawTriangle(mouseX, mouseY, mouseX + 16, mouseY, mouseX, mouseY + 16);
		ofFill();
		ofDrawCircle(mouseX, mouseY, 2);
		ofDrawCircle(mouseX + 16, mouseY, 2);
		ofDrawCircle(mouseX, mouseY + 16, 2);
		break;
	case AppState::BUILD_CIRCLE:
		ofCircle(mouseX + 6, mouseY + 6, 8);
		break;
	case AppState::BUILD_LINE:
		ofDrawLine(mouseX, mouseY, mouseX + 16, mouseY + 16);
		break;
	case AppState::ACTION_TRANSLATE:
		ofSetLineWidth(2);
		ofDrawArrow(ofVec3f(mouseX + 8, mouseY + 8), ofVec3f(mouseX, mouseY + 8), 2);
		ofDrawArrow(ofVec3f(mouseX + 8, mouseY + 8), ofVec3f(mouseX + 16, mouseY + 8), 2);
		ofDrawArrow(ofVec3f(mouseX + 8, mouseY + 8), ofVec3f(mouseX + 8, mouseY), 2);
		ofDrawArrow(ofVec3f(mouseX + 8, mouseY + 8), ofVec3f(mouseX + 8, mouseY + 16), 2);
		break;
	case AppState::ACTION_ROTATE:
		ofDrawBezier(mouseX, mouseY, mouseX + 8, mouseY + 2, mouseX + 14, mouseY + 8, mouseX + 16, mouseY + 16);
		break;
	case AppState::ACTION_RESIZE:
		ofSetLineWidth(2);
		ofDrawArrow(ofVec3f(mouseX + 4, mouseY + 4), ofVec3f(mouseX, mouseY), 2);
		ofDrawArrow(ofVec3f(mouseX + 12, mouseY + 12), ofVec3f(mouseX + 16, mouseY + 16), 2);
		ofDrawRectangle(mouseX + 4, mouseY + 4, 8, 8);
		break;
	default:
		ofShowCursor();
		break;
	}
	ofFill();
}

bool ofApp::mouseIsOverPanel() {
	int mouseX = ofGetMouseX();
	int mouseY = ofGetMouseY();

	if ((menuPanel.getPosition().x <= mouseX) && (mouseX <= (menuPanel.getPosition().x + menuPanel.getWidth()))
		&& (menuPanel.getPosition().y <= mouseY) && (mouseY <= (menuPanel.getPosition().y + menuPanel.getHeight()))) {
		return true;
	}

	 if ((shapesPanel.getPosition().x <= mouseX) && (mouseX <= (shapesPanel.getPosition().x + shapesPanel.getWidth()))
		&& (shapesPanel.getPosition().y <= mouseY) && (mouseY <= (shapesPanel.getPosition().y + shapesPanel.getHeight()))) {
		return true;
	}

	else if ((shapesParamsPanel.getPosition().x <= mouseX) && (mouseX <= (shapesParamsPanel.getPosition().x + shapesParamsPanel.getWidth()))
		&& (shapesParamsPanel.getPosition().y <= mouseY) && (mouseY <= (shapesParamsPanel.getPosition().y + shapesParamsPanel.getHeight()))) {
		return true;
	}
	else if ((shapes3DPanel.getPosition().x <= mouseX) && (mouseX <= (shapes3DPanel.getPosition().x + shapes3DPanel.getWidth()))
		&& (shapes3DPanel.getPosition().y <= mouseY) && (mouseY <= (shapes3DPanel.getPosition().y + shapes3DPanel.getHeight()))) {
		return true;
	}

	else if ((shapes3DParamsPanel.getPosition().x <= mouseX) && (mouseX <= (shapes3DParamsPanel.getPosition().x + shapes3DParamsPanel.getWidth()))
		&& (shapes3DParamsPanel.getPosition().y <= mouseY) && (mouseY <= (shapes3DParamsPanel.getPosition().y + shapes3DParamsPanel.getHeight()))) {
		return true;
	}

	return false;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (m_mode == MODE_2D) {
		if (ofGetKeyPressed(OF_KEY_DEL)) {
			deleteSelection();
			m_buffer.clear();
		}

		if (key == '+') {
			for (Obj2D* o : m_obj2DVector) {
				o->resize(2);
			}
			m_buffer.clear();
		}
		if (key == '-') {
			for (Obj2D* o : m_obj2DVector) {
				o->resize(0.5);
			}
			m_buffer.clear();
		}
	}
	else if (m_mode == MODE_MODEL) { //*
		if (ofGetKeyPressed('l')) {
			rendererModel->switchLight = !rendererModel->switchLight;
		}
		if (ofGetKeyPressed('m')) {
			rendererModel->switchMaterials = !rendererModel->switchMaterials;
			if (rendererModel->switchMaterials) rendererModel->model.disableMaterials();
			if (!rendererModel->switchMaterials) rendererModel->model.enableMaterials();
		}
		if (ofGetKeyPressed('t')) {
			rendererModel->switchTextures = !rendererModel->switchTextures;
			if (rendererModel->switchTextures) rendererModel->model.disableTextures();
			if (!rendererModel->switchTextures) rendererModel->model.enableTextures();
		}
	}
    else if (m_mode == MODE_3D){
		//3D
        if (ofGetKeyPressed(OF_KEY_DEL)) {
            deleteSelection3D();
            m_buffer.clear();
        }

        else{
            if(key == OF_KEY_LEFT){
                renderer3d->cam[renderer3d->m_camSelection].pan(kRotInc);
            }
            if(key == OF_KEY_RIGHT){
                renderer3d->cam[renderer3d->m_camSelection].pan(-kRotInc);
            }
            if(key == OF_KEY_UP){
                renderer3d->cam[renderer3d->m_camSelection].tilt(kRotInc);
            }
            if(key == OF_KEY_DOWN){
                renderer3d->cam[renderer3d->m_camSelection].tilt(-kRotInc);
            }
            if(key == ','){
                renderer3d->cam[renderer3d->m_camSelection].roll(kRotInc);
            }
            if(key == '.'){
                renderer3d->cam[renderer3d->m_camSelection].roll(-kRotInc);
            }
            if(key == 'a'){
                renderer3d->cam[renderer3d->m_camSelection].truck(-kMoveInc);
            }
            if(key == 'd'){
                renderer3d->cam[renderer3d->m_camSelection].truck(kMoveInc);
            }
            if(key == 'w'){
                renderer3d->cam[renderer3d->m_camSelection].dolly(-kMoveInc);
            }
            if(key == 's'){
                renderer3d->cam[renderer3d->m_camSelection].dolly(kMoveInc);
            }
            if(key == 'r'){
                renderer3d->cam[renderer3d->m_camSelection].boom(kMoveInc);
            }
            if(key == 'f'){
                renderer3d->cam[renderer3d->m_camSelection].boom(-kMoveInc);
            }
            if(key == 'o'){
                renderer3d->cam[renderer3d->m_camSelection].enableOrtho();
            }
            if(key == 'p'){
                renderer3d->cam[renderer3d->m_camSelection].disableOrtho();
            }
            if(key == 'c'){
                renderer3d->cam.push_back(ofCamera());
                renderer3d->m_camSelection = renderer3d->cam.size() - 1;
                renderer3d->cam[renderer3d->m_camSelection].resetTransform();
                renderer3d->cam[renderer3d->m_camSelection].setFov(60);
                renderer3d->cam[renderer3d->m_camSelection].clearParent();

                renderer3d->cam[renderer3d->m_camSelection].setPosition(ofGetWidth()/2, ofGetHeight()/2, 1000);
                renderer3d->cam[renderer3d->m_camSelection].lookAt(ofVec3f(ofGetWidth()/2, ofGetHeight()/2, -1));

            }
            if(key == 'v'){
                renderer3d->m_camSelection = renderer3d->m_camSelection + 1;
                if(renderer3d->m_camSelection == renderer3d->cam.size()){
                    renderer3d->m_camSelection = 0;
                }
            }
            if(key == 'b'){
                if(renderer3d->cam.size() > 1){
                    renderer3d->cam.erase(renderer3d->cam.begin() + renderer3d->m_camSelection);
                    renderer3d->m_camSelection = renderer3d->m_camSelection - 1;

                    if(renderer3d->m_camSelection < 0){
                        renderer3d->m_camSelection = renderer3d->cam.size() - 1;
                    }
                }
            }
        }
	}
}

void ofApp::deleteSelection() {
	int size = m_obj2DVector.size();
	for (int i = 0; i < m_obj2DVector.size(); i++) {
		if (m_obj2DVector[i]->isSelected()) {
			//DELETE OBJ
			m_obj2DVector.erase(m_obj2DVector.begin() + i);
			i--;
			size--;
		}
	}
}

void ofApp::deleteSelection3D() {
    int size = m_obj3DVector.size();
    for (int i = 0; i < m_obj3DVector.size(); i++) {
        if (m_obj3DVector[i]->isSelected()) {
            //DELETE OBJ
            Obj3D *ptr_obj3d;
            ptr_obj3d = m_obj3DVector[i];
            m_obj3DVector.erase(m_obj3DVector.begin() + i);
            delete ptr_obj3d;
            i--;
            size--;
            m_selectionIndex--;
        }
    }
}

void ofApp::buttonPressed(const void * sender){
    ofxButton * button = (ofxButton*)sender;
	string btnName = button->getName();

	if (m_mode == MODE_2D) {
		if (btnName == "Import") {
			ofFileDialogResult file = ofSystemLoadDialog("Load Image", false);
			if (file.getPath() != "") {
				app::Image2D *newImage = new app::Image2D(file.getPath(), 0, renderer2d->strokeWidth.get(), renderer2d->colorStroke.get(), renderer2d->colorSelected.get(), renderer2d->colorFill.get());
				if (newImage->getImage().getTextureReference().isAllocated()) {
					m_obj2DVector.push_back(newImage);
				}
			}
		}

		else if (btnName == "Export") {
			isTakingScreenshot = true;
			draw();
			isTakingScreenshot = false;

			ofFileDialogResult file = ofSystemSaveDialog("Save", "Export");

			if (file.getPath() != "") {
				renderer2d->imageExport(file.getPath(), "png");
			}
		}

		else if (btnName == "Merge Shapes") {
			std::vector<Obj2D*> glued;
			int size = m_obj2DVector.size();
			for (int i = 0; i < m_obj2DVector.size(); i++) {
				if (m_obj2DVector[i]->isSelected()) {
					glued.push_back(m_obj2DVector[i]);
					m_obj2DVector.erase(m_obj2DVector.begin() + i);
					i--;
					size--;
				}
			}
			m_obj2DVector.push_back(new app::Obj2DCollection(glued));
			m_obj2DVector.back()->setSelected(true);
			cout << "Created" << endl;
			m_buffer.clear();
		}

		else if (btnName == "Unmerge Shapes") {
			std::vector<Obj2D*> glued;
			int size = m_obj2DVector.size();
			for (int i = 0; i < m_obj2DVector.size(); i++) {
				if (m_obj2DVector[i]->getType() == EnumVectorDrawMode::VECTOR_PRIMITIVE_COLLECTION && m_obj2DVector[i]->isSelected()) {
					app::Obj2DCollection* j = dynamic_cast<app::Obj2DCollection*>(m_obj2DVector[i]);
					for (Obj2D* o : getCollectionObjects(j)) {
						glued.push_back(o);
					}
					m_obj2DVector.erase(m_obj2DVector.begin() + i);
					i--;
					size--;
				}
			}
			for (Obj2D* o : glued) {
				m_obj2DVector.push_back(o);
			}
			m_buffer.clear();
		}
	}
	else if (m_mode == MODE_MODEL) { //*
		if (btnName == "Import") {
			ofFileDialogResult file = ofSystemLoadDialog("Select a model file to load");
			if (file.bSuccess) {
				rendererModel->processOpenFileSelection(file);
			}
		}
	}
	else {
		//3D
        if (btnName == "Select next 3D shape") {
            if(m_obj3DVector.size() > 0){

                if (m_firstTimeSelection == 1){
                    m_firstTimeSelection = 0;
                    m_selectionIndex = 0;
                    m_obj3DVector[m_selectionIndex]->setSelected(false);
                    m_selectionIndex = m_selectionIndex % m_obj3DVector.size();
                    m_obj3DVector[m_selectionIndex]->setSelected(true);
                    renderer3d->bCloud.set(m_obj3DVector[m_selectionIndex]->isCloud());
                    bControlPoint.set(m_obj3DVector[m_selectionIndex]->isControl());
                }
                else{
                    m_obj3DVector[m_selectionIndex]->setSelected(false);
                    m_selectionIndex = (m_selectionIndex + 1) % m_obj3DVector.size();
                    m_obj3DVector[m_selectionIndex]->setSelected(true);
                    renderer3d->bCloud.set(m_obj3DVector[m_selectionIndex]->isCloud());
                    bControlPoint.set(m_obj3DVector[m_selectionIndex]->isControl());
                    renderer3d->tesselation.set(m_obj3DVector[m_selectionIndex]->getTesselation());
                }
            }
        }
        else if (btnName == "Unselect") {
            if(m_firstTimeSelection == 0){
                m_obj3DVector[m_selectionIndex]->setSelected(false);

            }
        }

		else if (btnName == "Export") {
			isTakingScreenshot = true;
			draw();
			isTakingScreenshot = false;

			ofFileDialogResult file = ofSystemSaveDialog("Save", "Export");

			if (file.getPath() != "") {
				renderer3d->imageExport(file.getPath(), "png");
			}
		}

		else if (btnName == "Apply Displacement Map") {
			for (Obj3D* o : m_obj3DVector) {
				if (o->isSelected()) {
					o->m_shaderMode = ShaderMode::DISPLACEMENT;
				}
			}
		}
        else if (btnName == "Apply Cube Map") {
            for (Obj3D* o : m_obj3DVector) {
                if (o->isSelected()) {
                    o->m_shaderMode = ShaderMode::CUBE;
                }
            }
        }

        else if(btnName == "CatmullRom Curve"){
            if(renderer3d->m_controls.size() >=4){
                buildCatmullRomCurve();
                clearControlPoints();
                renderer3d->m_controls.clear();
            }

        }
        else if(btnName == "Nurbs Curve"){
            if(renderer3d->m_controls.size() >= 4){
                buildNurbsCurve();
                clearControlPoints();
                renderer3d->m_controls.clear();
            }

        }
        else if(btnName == "Bezier Cubique Curve"){
            if(renderer3d->m_controls.size() >= 4){
                buildBezierCurve();
                clearControlPoints();
                renderer3d->m_controls.clear();
            }

        }
        else if(btnName == "Bezier Cubique Surface"){
            if(sqrt(renderer3d->m_controls.size()) - floor(sqrt(renderer3d->m_controls.size())) == 0){
                buildBezierSurface();
                clearControlPoints();
                renderer3d->m_controls.clear();
            }

        }
        else if(btnName == "Coons Surface"){
            if(renderer3d->m_controls.size() == 12){
                buildCoonsSurface();
                clearControlPoints();
                renderer3d->m_controls.clear();
            }

        }
	}	
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	if (m_mode == MODE_2D) {
		if (ofGetMousePressed(OF_MOUSE_BUTTON_LEFT) && ofGetMousePressed(OF_MOUSE_BUTTON_RIGHT)) {
			switch (m_state) {
			case AppState::ACTION_SELECT:
			case AppState::ACTION_TRANSLATE:
			case AppState::ACTION_GROUPSELECT:
				m_buffer.clear();
				m_buffer.push_back(Coord(x, y));
				m_buffer.push_back(Coord(x, y));
				m_state = AppState::ACTION_ROTATE;
				break;
			case AppState::ACTION_ROTATE:
				m_buffer[0] = m_buffer[1];
				m_buffer[1].setX(x);
				m_buffer[1].setY(y);
				rotateSelection();
				break;
			}
		}
		else if (button == OF_MOUSE_BUTTON_LEFT) {
			switch (m_state) {
			case AppState::ACTION_SELECT:
				m_buffer.clear();
				m_buffer.push_back(Coord(x, y));
				m_buffer.push_back(Coord(x, y));
				m_state = AppState::ACTION_GROUPSELECT;
				break;
			case AppState::ACTION_GROUPSELECT:
				m_buffer[1].setX(x);
				m_buffer[1].setY(y);
				updateGroupSelection();
				break;
			}
		}
		else if (button == OF_MOUSE_BUTTON_RIGHT &&
				(ofGetKeyPressed(OF_KEY_LEFT_CONTROL) || ofGetKeyPressed(OF_KEY_RIGHT_CONTROL))) {
			switch (m_state) {
			case AppState::ACTION_SELECT:
				m_buffer.clear();
				m_buffer.push_back(Coord(x, y));
				m_buffer.push_back(Coord(x, y));
				m_state = AppState::ACTION_RESIZE;
				break;
			case AppState::ACTION_RESIZE:
				m_buffer[0] = m_buffer[1];
				m_buffer[1].setX(x);
				m_buffer[1].setY(y);
				resizeSelection();
				break;
			}
		}
		else if (button == OF_MOUSE_BUTTON_RIGHT) {
			switch (m_state) {
			case AppState::ACTION_SELECT:
				m_buffer.clear();
				m_buffer.push_back(Coord(x, y));
				m_buffer.push_back(Coord(x, y));
				m_state = AppState::ACTION_TRANSLATE;
				break;
			case AppState::ACTION_TRANSLATE:
				m_buffer[0] = m_buffer[1];
				m_buffer[1].setX(x);
				m_buffer[1].setY(y);
				translateSelection(m_buffer[1].getX() - m_buffer[0].getX(), m_buffer[1].getY() - m_buffer[0].getY());
				break;
			}
		}
	}
	else {
		//3D
	}
	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	bool isMouseOnPanels = mouseIsOverPanel();
	if (m_mode == MODE_2D && !isMouseOnPanels) {
		if (button == OF_MOUSE_BUTTON_LEFT) {
			switch (m_state) {
			case AppState::BUILD_RECTANGLE:
				m_buffer.push_back(Coord(x, y));
				if (m_buffer.size() == 2) {
					buildRectangle();
					m_buffer.clear();
				}
				break;
			case AppState::BUILD_TRIANGLE:
				m_buffer.push_back(Coord(x, y));
				if (m_buffer.size() == 3) {
					buildTriangle();
					m_buffer.clear();
				}
				break;
			case AppState::BUILD_CIRCLE:
				m_buffer.push_back(Coord(x, y));
				if (m_buffer.size() == 2) {
					buildCircle();
					m_buffer.clear();
				}
				break;
			case AppState::BUILD_LINE:
				m_buffer.push_back(Coord(x, y));
				if (m_buffer.size() == 2) {
					buildLine();
					m_buffer.clear();
				}
				break;
			case AppState::ACTION_SELECT:
				m_buffer.clear();
				m_buffer.push_back(Coord(x, y));
				if (!(ofGetKeyPressed(OF_KEY_LEFT_CONTROL) || ofGetKeyPressed(OF_KEY_RIGHT_CONTROL))) {
					clearSelected();
				}
				for (Obj2D* o : m_obj2DVector) {
					if (o->checkSelected(Coord(x, y), m_clickRadius)) {
						break;
					}
				}
				break;
			}
		}
		else if (button == OF_MOUSE_BUTTON_RIGHT) {
			switch (m_state) {
			case AppState::ACTION_SELECT:
				m_buffer.clear();
				m_buffer.push_back(Coord(x, y));
			}
		}
	}
	else if (m_mode == MODE_3D && !isMouseOnPanels) {
		//3D
        if (button == OF_MOUSE_BUTTON_LEFT) {
            switch (m_state) {
            case AppState::BUILD_CUBE:
                m_buffer3D.push_back(Coord3D(x, y, 0));
                if (m_buffer3D.size() == 1) {
                    buildCube();
                    m_buffer3D.clear();
                }
                break;
            case AppState::BUILD_SPHERE:
                m_buffer3D.push_back(Coord3D(x, y, 0));
                if (m_buffer3D.size() == 1) {
                    buildSphere();
                    m_buffer3D.clear();
                }
                break;
			case AppState::BUILD_TERRAIN:
				m_buffer3D.push_back(Coord3D(x, y, 0));
				if (m_buffer3D.size() == 1) {
					buildTerrain();
					m_buffer3D.clear();
				}
				break;
            }

            if (m_state == CAMERA ){
                m_currentPositionX = x;
                m_currentPositionY = y;
                m_currentPosition2X = x;
                m_currentPosition2Y = y;
                m_changeCameraOrientation = true;
            }
        }

	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	if (m_mode == MODE_2D) {
		if (button == OF_MOUSE_BUTTON_LEFT) {
			switch (m_state) {
			case AppState::ACTION_ROTATE:
			case AppState::ACTION_GROUPSELECT:
				m_buffer.clear();
				m_state = AppState::ACTION_SELECT;
				break;
			}
		}
		else if (button == OF_MOUSE_BUTTON_RIGHT) {
			switch (m_state) {
			case AppState::ACTION_ROTATE:
			case AppState::ACTION_RESIZE:
			case AppState::ACTION_TRANSLATE:
				m_buffer.clear();
				m_state = AppState::ACTION_SELECT;
				break;
			}
		}
	}
    else if (m_mode == MODE_3D
             && m_state == CAMERA && button == OF_MOUSE_BUTTON_LEFT) {
		//3D
        m_changeCameraOrientation = false;
        m_addPan = 0;
        m_addTilt = 0;
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

void ofApp::buildRectangle() {
	double width = m_buffer[1].getX() - m_buffer[0].getX();
	double height = m_buffer[1].getY() - m_buffer[0].getY();
	if (width >= 0 && height >= 0) { //0 = top-left, 1 = bottom-right
		m_obj2DVector.push_back(new app::Rectangle(m_buffer[0], width, height, 0, renderer2d->strokeWidth.get(), renderer2d->colorStroke.get(), renderer2d->colorSelected.get(), renderer2d->colorFill.get()));
	}
	else if (width < 0 && height >= 0) { //0 = top-right, 1 = bottom-left
		m_obj2DVector.push_back(new app::Rectangle(Coord(m_buffer[1].getX(), m_buffer[0].getY()), -width, height, 0, renderer2d->strokeWidth.get(), renderer2d->colorStroke.get(), renderer2d->colorSelected.get(), renderer2d->colorFill.get()));
	}
	else if (width >= 0 && height < 0) { //0 = bottom-left, 1 = top-right
		m_obj2DVector.push_back(new app::Rectangle(Coord(m_buffer[0].getX(), m_buffer[1].getY()), width, -height, 0, renderer2d->strokeWidth.get(), renderer2d->colorStroke.get(), renderer2d->colorSelected.get(), renderer2d->colorFill.get()));
	}
	else { //0 = bottom-right, 1 = top-left
		m_obj2DVector.push_back(new app::Rectangle(m_buffer[1], -width, -height, 0, renderer2d->strokeWidth.get(), renderer2d->colorStroke.get(), renderer2d->colorSelected.get(), renderer2d->colorFill.get()));
	}
}

void ofApp::buildTriangle() {
	m_obj2DVector.push_back(new app::Triangle(m_buffer, 0, renderer2d->strokeWidth.get(), renderer2d->colorStroke.get(), renderer2d->colorSelected.get(), renderer2d->colorFill.get()));
}

void ofApp::buildCircle() {
	double radius = calculateDistance(m_buffer[0], m_buffer[1]);
	m_obj2DVector.push_back(new app::Circle(m_buffer[0], radius, 0, renderer2d->strokeWidth.get(), renderer2d->colorStroke.get(), renderer2d->colorSelected.get(), renderer2d->colorFill.get()));
}

void ofApp::buildLine() {
	m_obj2DVector.push_back(new app::Line2D(m_buffer, 0, renderer2d->strokeWidth.get(), renderer2d->colorStroke.get(), renderer2d->colorSelected.get(), renderer2d->colorStroke.get()));
}

void ofApp::buildCube() {
    m_obj3DVector.push_back(new app::Cube3D(renderer3d->dimension, m_buffer3D, renderer2d->strokeWidth.get(), renderer2d->colorStroke.get(), renderer2d->colorSelected.get(), renderer2d->colorFill.get()));
}

void ofApp::buildSphere() {
    m_obj3DVector.push_back(new app::Sphere3D(renderer3d->dimension, m_buffer3D, renderer2d->strokeWidth.get(), renderer2d->colorStroke.get(), renderer2d->colorSelected.get(), renderer2d->colorFill.get()));
}

void ofApp::buildTerrain() {
	m_obj3DVector.push_back(new app::Terrain3D(m_buffer3D, "image.jpg", renderer2d->strokeWidth.get(), renderer2d->colorStroke.get(), renderer2d->colorSelected.get(), renderer2d->colorFill.get()));
}

double ofApp::calculateDistance(Coord p_coord1, Coord p_coord2) {
	double x2 = pow((p_coord1.getX() - p_coord2.getX()), 2);
	double y2 = pow((p_coord1.getY() - p_coord2.getY()), 2);
	return sqrt(x2 + y2);
}

void ofApp::clearSelected() {
	for (Obj2D* o : m_obj2DVector) {
		o->setSelected(false);
	}
}

void ofApp::updateGroupSelection() {
	double width = m_buffer[1].getX() - m_buffer[0].getX();
	double height = m_buffer[1].getY() - m_buffer[0].getY();
	Coord topLeft = Coord(0,0);
	if (width >= 0 && height >= 0) { //0 = top-left, 1 = bottom-right
		topLeft = m_buffer[0];
		//cout << "TOP-LEFT" << endl;
	}
	else if (width < 0 && height >= 0) { //0 = top-right, 1 = bottom-left
		topLeft = Coord(m_buffer[1].getX(), m_buffer[0].getY());
		//cout << "TOP-RIGHT" << endl;
	}
	else if (width >= 0 && height < 0) { //0 = bottom-left, 1 = top-right
		topLeft = Coord(m_buffer[0].getX(), m_buffer[1].getY());
		//cout << "BOTTOM-LEFT" << endl;
	}
	else { //0 = bottom-right, 1 = top-left
		topLeft = m_buffer[1];
		//cout << "BOTTOM-RIGHT" << endl;
	}
	for (Obj2D* o : m_obj2DVector) {
		o->setSelected(o->containedInRect(topLeft, abs(width), abs(height)));
	}
}

void ofApp::translateSelection(double p_x, double p_y) {
	for (Obj2D* o : m_obj2DVector) {
		if (o->isSelected()) {
			o->translate(p_x, p_y);
		}
	}
}

void ofApp::rotateSelection() {
	cout << "rotate" << endl;
	for (Obj2D* o : m_obj2DVector) {
		if (o->isSelected()) {
			o->rotate(m_buffer[0], m_buffer[1]);
		}
	}
}

void ofApp::resizeSelection() {
	for (Obj2D* o : m_obj2DVector) {
		if (o->isSelected()) {
			o->resize(calculateDistance(m_buffer[1], o->getRotationCenter()) / calculateDistance(m_buffer[0], o->getRotationCenter()));
		}
	}
}

std::vector<Obj2D*> ofApp::getCollectionObjects(app::Obj2DCollection* p_coll) {
	return p_coll->getObjVector();
}

void ofApp::exit()
{
	ofLog() << "<ofApp::exit>";
}

void ofApp::clear2DButtons(){
    bLine.set(false);
    bTriangle.set(false);
    bRectangle.set(false);
    bCircle.set(false);
}

void ofApp::clear3DButtons(){
    bCube.set(false);
    renderer3d->bCloud.set(false);
}

void ofApp::drawGrid() {
	int x_smallDiv = 10;
	int y_smallDiv = 10;
	int x_bigDiv = 5;
	int y_bigDiv = 5;
	ofSetColor(0, 0, 0);
	ofSetLineWidth(1);
	for (int i = 0; i <= x_smallDiv*x_bigDiv; i++) {
		ofDrawLine(i*ofGetWidth() / (x_smallDiv*x_bigDiv), 0, i*ofGetWidth() / (x_smallDiv*x_bigDiv), ofGetHeight());
	}
	for (int i = 0; i <= y_smallDiv*y_bigDiv; i++) {
		ofDrawLine(0, i*ofGetHeight() / (y_smallDiv*y_bigDiv), ofGetWidth(), i*ofGetHeight() / (y_smallDiv*y_bigDiv));
	}
	ofSetLineWidth(2);
	for (int i = 0; i <= x_bigDiv; i++) {
		ofDrawLine(i*ofGetWidth() / (x_bigDiv), 0, i*ofGetWidth() / (x_bigDiv), ofGetHeight());
	}
	for (int i = 0; i <= y_bigDiv; i++) {
		ofDrawLine(0, i*ofGetHeight() / (y_bigDiv), ofGetWidth(), i*ofGetHeight() / (y_bigDiv));
	}
}

void ofApp::bControlChanged(bool & p_toggle) {
    bControlPoint.set(p_toggle);
    if(m_obj3DVector.size() > 0 && m_obj3DVector[m_selectionIndex]->isSelected()){
        m_obj3DVector[m_selectionIndex]->setControl(p_toggle);
        if(p_toggle && renderer3d->notIncluded(m_obj3DVector[m_selectionIndex])){

            if(m_obj3DVector[m_selectionIndex]->getType() == EnumVectorDrawMode::PRIMITIVE_CUBE){
                app::Cube3D* c = dynamic_cast<app::Cube3D*>(m_obj3DVector[m_selectionIndex]);
                renderer3d->m_controls.push_back(ofVec4f(c->getPosition().x, c->getPosition().y, c->getPosition().z, 1));

            }
            if(m_obj3DVector[m_selectionIndex]->getType() == EnumVectorDrawMode::PRIMITIVE_SPHERE){
                app::Sphere3D* c = dynamic_cast<app::Sphere3D*>(m_obj3DVector[m_selectionIndex]);
                renderer3d->m_controls.push_back(ofVec4f(c->getPosition().x, c->getPosition().y, c->getPosition().z, 1));

            }
        }
    }
}

void ofApp::buildCatmullRomCurve() {
    m_obj3DVector.push_back(new app::CourbeCatmullRom(renderer2d->strokeWidth.get(), renderer2d->colorStroke.get(), renderer2d->colorSelected.get(), renderer3d->tesselation, renderer3d->m_controls, renderer2d->colorFill.get()));
}

void ofApp::buildNurbsCurve() {
    m_obj3DVector.push_back(new app::CourbeNurbs(renderer2d->strokeWidth.get(), renderer2d->colorStroke.get(), renderer2d->colorSelected.get(), renderer3d->tesselation, renderer3d->m_controls, renderer2d->colorFill.get()));
}

void ofApp::buildBezierCurve() {
    m_obj3DVector.push_back(new app::CourbeBezierCubique(renderer2d->strokeWidth.get(), renderer2d->colorStroke.get(), renderer2d->colorSelected.get(), renderer3d->tesselation, renderer3d->m_controls, renderer2d->colorFill.get()));
}

void ofApp::buildBezierSurface() {
    m_obj3DVector.push_back(new app::SurfaceBezierCubique(renderer2d->strokeWidth.get(), renderer2d->colorStroke.get(), renderer2d->colorSelected.get(), renderer3d->tesselation, renderer3d->m_controls, renderer2d->colorFill.get()));
}

void ofApp::buildCoonsSurface() {
    m_obj3DVector.push_back(new app::SurfaceCoons(renderer2d->strokeWidth.get(), renderer2d->colorStroke.get(), renderer2d->colorSelected.get(), renderer3d->tesselation, renderer3d->m_controls, renderer2d->colorFill.get()));
}

void ofApp::clearControlPoints(){
    for(Obj3D *o: m_obj3DVector){
        o->setControl(false);
    }
}

ofApp::~ofApp()
{
	if (nullptr != renderer2d)
		delete renderer2d;
	if (nullptr != rendererModel) //*
		delete rendererModel; //*
	
}

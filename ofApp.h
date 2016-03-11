#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Renderer2D.h"
#include "Coord.h"
#include "state.h"
#include "Obj2D.h"
#include "Rectangle.h"
#include "Triangle.h"
#include "Circle.h"
#include "Line2D.h"
#include "Image2D.h"
#include "Obj2DCollection.h"
#include <vector>

class Renderer2D;

class ofApp : public ofBaseApp{
	public:

		std::vector<Coord> m_buffer;
		std::vector<Obj2D*> m_obj2DVector;
		AppState m_state;
		AppMode m_mode;
		int m_clickRadius;
		bool isTakingScreenshot;
		bool isClearingButtonsShapes, isClearingButtonsModes;

		ofApp();
		~ofApp();
		void exit();

		void setup();
		void update();
		void draw();
		void drawCursor();
		bool mouseIsOverPanel();

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
        void buttonPressed(const void * sender);
		
		void vSyncChanged(bool & vSync);
		void b2DChanged(bool & p_2D);
		void b3DChanged(bool & p_3D);
		void bLineChanged(bool & p_Line);
		void bTriangleChanged(bool & p_Triangle);
		void bRectangleChanged(bool & p_Rectangle);
		void bCircleChanged(bool & p_Circle);
		void bSelectChanged(bool & p_Select);
		void clearButtons();

		void buildRectangle();
		void buildTriangle();
		void buildCircle();
		void buildLine();
		double calculateDistance(Coord p_coord1, Coord p_coord2);
		void clearSelected();
		void updateGroupSelection();
		void translateSelection(double p_x, double p_y);
		void rotateSelection();
		void deleteSelection();
		
		Renderer2D *renderer2d;

		std::vector<Obj2D*> getCollectionObjects(app::Obj2DCollection* p_coll);

		ofxPanel shapesPanel, shapesParamsPanel, menuPanel;
		ofParameter<bool> vSync;
		ofParameter<bool> b2D, b3D;
        ofParameter<bool> bLine, bTriangle, bRectangle, bCircle, bSelect;
        ofParameterGroup shapesSettingsParams, shapesParams, menuBarParams;
		ofXml settings;
        ofTrueTypeFont font;
		ofxButton *importButton, *exportButton, *mergeButton, *unmergeButton;
		//ofxButton *lineButton, *triangleButton, *rectangleButton, *circleButton;
};

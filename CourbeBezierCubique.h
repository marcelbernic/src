#pragma once
#include "Shape3D.h"

namespace app {
    class CourbeBezierCubique : public Shape3D
    {
    private:
        std::vector<ofVec4f> m_controls;
        ofMesh m_mesh;
        ofColor m_colorCourbe;
    public:
        CourbeBezierCubique(int p_lineStroke, ofColor p_lineColor, ofColor p_lineColorSelected, double p_tesselation, vector<ofVec4f> p_controls, ofColor p_colorFill);
        ~CourbeBezierCubique();
        void setColorCurve(ofColor p_color);
        void setTesselation(double p_tesselation);
        vector<ofVec4f> getControlsPoints();
        void buildMesh();
        void draw() override;
        int fact(int a);
    };
}

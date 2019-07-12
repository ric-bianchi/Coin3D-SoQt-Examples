
// local includes
#include "MyTorus.h"

// Coin includes
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoBaseColor.h>
#include <Inventor/nodes/SoText3.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoTriangleStripSet.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoNormalBinding.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/nodes/SoMaterial.h>

// SoQt includes
#include <Inventor/Qt/SoQt.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>

// Qt5 includes
#include <QApplication>
#include <QWidget>

// C++ includes
#include <random>
#include <stdexcept>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cmath>



// build a default torus
SoSeparator* makeTorus()
{
  SoSeparator *sep = new SoSeparator();
  sep->ref();
  MyTorus* torus = new MyTorus();
  sep->addChild(torus->getShape());
  return sep;
}
//build a toroidal segment with start and end angles
SoSeparator* makeTorus(double Rtor, double Rmax, double SPhi, double DPhi)
{
  SoSeparator *sep = new SoSeparator();
  sep->ref();
  MyTorus* torus = new MyTorus(Rtor, Rmax, SPhi, DPhi);
  sep->addChild(torus->getShape());
  return sep;
}
// build a toroidal segment with starting and end angles and with major and minor subdivisions
SoSeparator* makeTorus(double Rtor, double Rmax, double SPhi, double DPhi, int divsMajor, int divsMinor)
{
  SoSeparator *sep = new SoSeparator();
  sep->ref();
  MyTorus* torus = new MyTorus(Rtor, Rmax, SPhi, DPhi, divsMajor, divsMinor);
  sep->addChild(torus->getShape());
  return sep;
}

// SoSeparator* makeToroidalSegment(){
//
// }



int main(int argc, char **argv)
{

  // Initialize the Qt system:
  QApplication app(argc, argv);

  // Make a main window:
  QWidget mainwin;
  mainwin.resize(400,400);

  // Initialize SoQt
  SoQt::init(&mainwin);


  //--- Define the scenegraph

  // The root of a scene graph
  SoSeparator *root = new SoSeparator;
  root->ref();

  //--- Add 3D objects to the scene

  // root->addChild( makeSimpleStripSet() );
  // root->addChild( makeSimpleStripSetWithNorms() );
  // root->addChild( makePennant() );
  // root->addChild( makeObeliskFaceSet() );

  // root->addChild( makeTorus() ); // add a default torus

  // add a toroidal segment starting at 0 degrees and stopping at 270 degrees
  root->addChild( makeTorus(50, 30, 0, 270) );

  // add a toroidal segment starting at 0 degrees and stopping at 270 degrees
  // and render it using 5 subdivisions for the major radius
  // and 4 for the cross section
  // root->addChild( makeTorus(50, 30, 0, 270, 5, 4) );

  //--- Init the viewer

  // Initialize an examiner viewer:
  SoQtExaminerViewer * eviewer = new SoQtExaminerViewer(&mainwin);
  eviewer->setSceneGraph(root);
  eviewer->show();

  // Pop up the main window.
  SoQt::show(&mainwin);

  // Loop until exit.
  SoQt::mainLoop();

  // Clean up resources.
  delete eviewer;
  root->unref();

  return app.exec();
}

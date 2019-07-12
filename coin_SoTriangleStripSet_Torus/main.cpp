
// local includes
#include "MyTorus.h"

// Coin includes
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoBaseColor.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoTriangleStripSet.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoNormalBinding.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoShapeHints.h>

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

  // A shape hints tells the ordering of polygons.
  // This ensures double-sided lighting.
  SoShapeHints *myHints = new SoShapeHints;
  myHints->vertexOrdering = SoShapeHints::COUNTERCLOCKWISE;
  root->addChild(myHints);


  // add a default torus
  // MyTorus* torus = new MyTorus();

  // add a toroidal segment starting at 0 degrees and stopping at 270 degrees
  // MyTorus* torus = new MyTorus(50, 30, -1, 0, 270);

  // add a toroidal segment starting at 0 degrees and stopping at 270 degrees
  // and render it using 5 subdivisions for the major radius
  // and 4 for the cross section
  // MyTorus* torus = new MyTorus(50, 30, -1, 0, 270, 5, 4);

  MyTorus* torus = new MyTorus(50, 30, 10, 0, 270, 5, 4);


  root->addChild(torus->getShape());

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

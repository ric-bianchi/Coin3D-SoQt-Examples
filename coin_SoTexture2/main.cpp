#include <Inventor/Qt/SoQt.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoTexture2Transform.h>
#include <QApplication>
#include <QWidget>
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

  // The root of a scene graph
  SoSeparator *root = new SoSeparator;
  root->ref();

  /*
  // Here the other 3D objects can go...
  */
  // Choose a texture
  SoTexture2 *texture = new SoTexture2;
  root->addChild(texture);
  texture->filename.setValue("grass.jpg");
  texture->wrapS = SoTexture2::Wrap::REPEAT;

  SoTexture2Transform* tr = new SoTexture2Transform;
  // tr->scaleFactor.setValue(0.01, 0.01); // it "enlarges" the texture
  // tr->scaleFactor.setValue(100., 100.); // it "reduces" the texture: more texture's tiles in the same area
  root->addChild(tr);

  // Make a cube
  SoCube* cube = new SoCube;
  cube->width = 400.0;
  cube->height = 400.0;
  cube->depth = 100.0;
  root->addChild(cube);

  /*
  // end of the 3D objects definition
  */


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

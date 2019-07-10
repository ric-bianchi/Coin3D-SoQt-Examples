#include <Inventor/Qt/SoQt.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/nodes/SoCylinder.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoCone.h>
#include <QApplication>
#include <QWidget>
#include <random>
#include <stdexcept>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cmath>


SoSeparator *
readFile(const char *filename)
{
   // Open the input file
   SoInput mySceneInput;
   if (!mySceneInput.openFile(filename)) {
      fprintf(stderr, "Cannot open file %s\n", filename);
      exit(1);
   }

   // Read the whole file into the database
   SoSeparator *myGraph = SoDB::readAll(&mySceneInput);
   if (myGraph == NULL) {
      fprintf(stderr, "Problem reading file\n");
      exit(1);
   }
   mySceneInput.closeFile();
   return myGraph;
}

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

  root->addChild(readFile("data/test.iv"));

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
}

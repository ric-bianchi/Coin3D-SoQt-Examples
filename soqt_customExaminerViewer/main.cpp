// HelloWorldSoQt.cpp

#include "customViewer.h"

// SoQt includes
#include <Inventor/Qt/SoQt.h>
//#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>

// Coin includes
#include <Inventor/nodes/SoBaseColor.h>
#include <Inventor/nodes/SoText3.h>
#include <Inventor/nodes/SoSeparator.h>

int main(int argc, char ** argv)
{
  // Init the Qt windowing system
  // and get a pointer to the window
  QWidget *window = SoQt::init("test");

  // Set the main node for the "scene graph"
  SoSeparator *root = new SoSeparator;
  root->ref();

  // Set the color for the text (in RGB mode)
  SoBaseColor *color = new SoBaseColor;
  color->rgb = SbColor(1, 1, 0); // Yellow
  root->addChild(color);

  // Set the node for the 3D text, set the visible surfaces
  // and add actual text to it
  SoText3 * text3D = new SoText3();
  text3D->string.setValue("Hello World!");
  text3D->parts.setValue("ALL");
  root->addChild(text3D);

  // Init the viewer and get a pointer to it
  CustomExaminerViewer *b = new CustomExaminerViewer(window);

  // Set the main node as content of the window and show it
  b->setSceneGraph(root);
  b->show();

  // Start the windowing system and show our window
  SoQt::show(window);
  // Loop until exit.
  SoQt::mainLoop();

  // When exiting the main loop,
  // delete the viewer and leave the handle to the main node
  delete b;
  root->unref();

  // exit.
  return 0;
}

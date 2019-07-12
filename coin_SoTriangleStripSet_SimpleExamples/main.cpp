
// Coin includes
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoBaseColor.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoTriangleStripSet.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoNormalBinding.h>
#include <Inventor/nodes/SoFaceSet.h>
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


// Make two simple polygons out of strips of triangles
SoSeparator *makeSimpleStripSet() {

  // Two polygons:
  // - The first polygon is a bent rectangle built with 4 triangles defined by 6 vertices:
  //   the first two triangles live in the x/y plane, the second two ones live in the z/y plane
  // - The second polygon is a rotated square built with 2 triangles defined by 4 vertices; it lives in the x/y plane
  static float vertexPositions[10][3] =
  { {   0,  0, 0  },{   0, 10, 0  },
    {  10,  0, 0  },{  10, 10, 0  },
    {  10,  0, 10  },{  10, 10, 10  },
    {   5, 15, 0  },
    {  10, 20, 0  },
    {  10, 10, 0  },
    {  15, 15, 0  } };

  // We declare the number of vertices in each polygon:
  // There are 2 polygons, we use one entry of the array per polygon:
  // - the first polygon is a strip of 4 triangles using 6 vertices
  // - the second polygon is composed by 2 triangles using 4 vertices
  const int numVertices[2] = { 6,4 };

  SoSeparator *result = new SoSeparator;
  result->ref();

  // A shape hints tells the ordering of polygons.
  // This ensures double-sided lighting.
  SoShapeHints *myHints = new SoShapeHints;
  myHints->vertexOrdering = SoShapeHints::COUNTERCLOCKWISE;
  result->addChild(myHints);

  // define the coordinates of the vertices
  SoCoordinate3 *myCoords = new SoCoordinate3;
  myCoords->point.setValues(0, 10, vertexPositions);
  result->addChild(myCoords);

  // define the SoTriangleStripSet, which will use the coordinates
  // stored in the preceding SoCoordinate3 node,
  // by using them accordingly to the information
  // stored in the numVertices array.
  SoTriangleStripSet *s = new SoTriangleStripSet;
  s->numVertices.setValues(0, 2, numVertices); // 2 is the number of polygons defined in numVertices

  result->addChild(s);
  result->unrefNoDelete();
  return result;
}

// TODO: to be fixed! Normals are not calculated properly, visualization is wrong!
// Make two simple polygons out of strips of triangles, with explicit normals
SoSeparator *makeSimpleStripSetWithNorms() {

  // Two polygons:
  // - The first polygon is a bent rectangle built with 4 triangles defined by 6 vertices:
  //   the first two triangles live in the x/y plane, the second two ones live in the z/y plane
  // - The second polygon is a rotated square built with 2 triangles defined by 4 vertices; it lives in the x/y plane
  static float vertexPositions[10][3] =
  { {   0,  0, 0  },{   0, 10, 0  },
    {  10,  0, 0  },{  10, 10, 0  },
    {  10,  0, 10  },{  10, 10, 10  },
    {   5, 15, 0  },
    {  10, 20, 0  },
    {  10, 10, 0  },
    {  15, 15, 0  } };

  // We declare the number of vertices in each polygon:
  // There are 2 polygons, we use one entry of the array per polygon:
  // - the first polygon is a strip of 4 triangles using 6 vertices
  // - the second polygon is composed by 2 triangles using 4 vertices
  const int numVertices[2] = { 6,4 };

  // Normals for each polygon:
  // Each entry defines a vector, which is the normal to the polygon's surface
  static float norms[6][3] =
  {
    { 0, 0, 1}, {  0, 0, 1}, //the first two triangles living in the x/y plane (normals are // z)
    {1, 0, 0}, { 1, 0, 0}, //the second two triangles living in the z/y plane (normals are // -x)
    { 0, 0, -1}, {  0, 0, 1}//the 4 triangles of the rotated square, all living in the x/y plane (normals are // z)
  };


  SoSeparator *result = new SoSeparator;
  result->ref();

  // A shape hints tells the ordering of polygons.
  // This ensures double-sided lighting.
  SoShapeHints *myHints = new SoShapeHints;
  myHints->vertexOrdering = SoShapeHints::CLOCKWISE;
  result->addChild(myHints);

  // Define the normals used:
  SoNormal *myNormals = new SoNormal;
  myNormals->vector.setValues(0, 6, norms);
  result->addChild(myNormals);
  SoNormalBinding *myNormalBinding = new SoNormalBinding;
  myNormalBinding->value = SoNormalBinding::PER_PART;
  result->addChild(myNormalBinding);

  // define the coordinates of the vertices
  SoCoordinate3 *myCoords = new SoCoordinate3;
  myCoords->point.setValues(0, 10, vertexPositions);
  result->addChild(myCoords);

  // define the SoTriangleStripSet, which will use the coordinates
  // stored in the preceding SoCoordinate3 node,
  // by using them accordingly to the information
  // stored in the numVertices array.
  SoTriangleStripSet *s = new SoTriangleStripSet;
  s->numVertices.setValues(0, 2, numVertices); // 2 is the number of polygons defined in numVertices

  result->addChild(s);
  result->unrefNoDelete();
  return result;
}


// Make a flag and a pole
SoSeparator *makePennant() {

  // Two polygons.
  // The first is a flag made of a strip of many triangles defined by 32 vertices
  // The second on is a pole defined by a strip if triangles defined by 8 vertices
  static float vertexPositions[40][3] =
  { {  0, 12  ,    0},{  0,   15,    0},
  {2.1, 12.1,  -.2},{2.1, 14.6,  -.2},
  { 4,  12.5,  -.7},{  4, 14.5,  -.7},
  {4.5, 12.6,  -.8},{4.5, 14.4,  -.8},
  {  5, 12.7,   -1},{  5, 14.4,   -1},
  {4.5, 12.8, -1.4},{4.5, 14.6, -1.4},
  {  4, 12.9, -1.6},{  4, 14.8, -1.6},
  {3.3, 12.9, -1.8},{3.3, 14.9, -1.8},
  {  3, 13  , -2.0},{  3, 14.9, -2.0},
  {3.3, 13.1, -2.2},{3.3, 15.0, -2.2},
  {  4, 13.2, -2.5},{  4, 15.0, -2.5},
  {  6, 13.5, -2.2},{  6, 14.8, -2.2},
  {  8, 13.4,   -2},{  8, 14.6,   -2},
  { 10, 13.7, -1.8},{ 10, 14.4, -1.8},
  { 12, 14  , -1.3},{ 12, 14.5, -1.3},
  { 15, 14.9, -1.2},{ 15, 15  , -1.2},
  {-.5, 15,  0},{-.5,0,  0},
  {  0,15,.5},{  0,0,.5},
  {  0, 15,-.5},{  0,0,-.5},
  {-.5,15, 0},{-.5,0, 0} };

  // We declare the number of vertices in each polygon:
  // There are 2 polygons, we use one entry of the array per polygon:
  // - the first polygon is a complex flag and it uses the first 32 vertices
  // - the second polygon is a pole and it uses the last 8 vertices
  const int numVertices[2] = { 32,8 };

  SoSeparator *result = new SoSeparator;
  result->ref();

  // A shape hints tells the ordering of polygons.
  // This ensures double-sided lighting.
  SoShapeHints *myHints = new SoShapeHints;
  myHints->vertexOrdering = SoShapeHints::COUNTERCLOCKWISE;
  result->addChild(myHints);

  // Define colors for the strips
  // Colors for the 12 faces
  const float colors[2][3] ={
    { .5, .5,  1 }, // purple flag
    { .4, .4, .4 }, // grey flagpole
  };
  SoMaterial *myMaterials = new SoMaterial;
  myMaterials->diffuseColor.setValues(0, 2, colors);
  result->addChild(myMaterials);
  SoMaterialBinding *myMaterialBinding = new SoMaterialBinding;
  myMaterialBinding->value = SoMaterialBinding::PER_PART;
  result->addChild(myMaterialBinding);   // D

  SoCoordinate3 *myCoords = new SoCoordinate3;
  myCoords->point.setValues(0, 40, vertexPositions);
  result->addChild(myCoords);

  SoTriangleStripSet *s = new SoTriangleStripSet;
  s->numVertices.setValues(0, 2, numVertices);
  result->addChild(s);
  result->unrefNoDelete();
  return result;
}



// creates an obelisk using a face set composed of eight faces.
SoSeparator* makeObeliskFaceSet()
{
    //  Eight polygons. The first four are triangles
    //  The second four are quadrilaterals for the sides.
    static float vertices[28][3] =
    {
      { 0, 30, 0}, {-2,27, 2}, { 2,27, 2},            //front tri
      { 0, 30, 0}, {-2,27,-2}, {-2,27, 2},            //left  tri
      { 0, 30, 0}, { 2,27,-2}, {-2,27,-2},            //rear  tri
      { 0, 30, 0}, { 2,27, 2}, { 2,27,-2},            //right tri
      {-2, 27, 2}, {-4,0, 4}, { 4,0, 4}, { 2,27, 2},  //front quad
      {-2, 27,-2}, {-4,0,-4}, {-4,0, 4}, {-2,27, 2},  //left  quad
      { 2, 27,-2}, { 4,0,-4}, {-4,0,-4}, {-2,27,-2},  //rear  quad
      { 2, 27, 2}, { 4,0, 4}, { 4,0,-4}, { 2,27,-2}   //right quad
    };

    // We have to declare the number of vertices in each polygon:
    // There are eight polygons, we use one entry of the array per polygon:
    // - the first, second, third, and fourth polygons have 3 vertices (triangle)
    // - the fifth, sixth, seventh, and eight polygons have 4 vertices (rectangles)
    const int numvertices[8] = {3, 3, 3, 3, 4, 4, 4, 4};

    // Normals for each polygon:
    // Each entry defines a vector, which is the normal to the polygon's surface
    static float norms[8][3] =
    {
      {0, .555,  .832}, {-.832, .555, 0}, //front, left tris
      {0, .555, -.832}, { .832, .555, 0}, //rear, right tris
      {0, .0739,  .9973}, {-.9972, .0739, 0},//front, left quads
      {0, .0739, -.9973}, { .9972, .0739, 0},//rear, right quads
    };

   SoSeparator *obelisk = new SoSeparator();
   obelisk->ref();

   // Define the normals used:
   SoNormal *myNormals = new SoNormal;
   myNormals->vector.setValues(0, 8, norms);
   obelisk->addChild(myNormals);
   SoNormalBinding *myNormalBinding = new SoNormalBinding;
   myNormalBinding->value = SoNormalBinding::PER_FACE;
   obelisk->addChild(myNormalBinding);

   // Define material for obelisk
   SoMaterial *myMaterial = new SoMaterial;
   myMaterial->diffuseColor.setValue(.4, .4, .4);
   obelisk->addChild(myMaterial);

   // Define coordinates for vertices
   SoCoordinate3 *myCoords = new SoCoordinate3;
   myCoords->point.setValues(0, 28, vertices);
   obelisk->addChild(myCoords);

   // Define the FaceSet
   // it will use the coordinates stored in the preceding SoCoordinate3 node
   // to build the faces, using them accordingly to the definitions
   // stored in the numvertices array
   SoFaceSet *myFaceSet = new SoFaceSet;
   myFaceSet->numVertices.setValues(0, 8, numvertices);
   obelisk->addChild(myFaceSet);

   obelisk->unrefNoDelete();
   return obelisk;
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


  //--- Define the scenegraph

  // The root of a scene graph
  SoSeparator *root = new SoSeparator;
  root->ref();

  //--- Add 3D objects to the scene

  // root->addChild( makeSimpleStripSet() );
  root->addChild( makeSimpleStripSetWithNorms() );
  // root->addChild( makePennant() );
  // root->addChild( makeObeliskFaceSet() );

  //--- Init the viewer and launch the app

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


/*
  Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
*/

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Node:             MyTorus                                                */
/* Description:      Represents the G4Torus Geant Geometry entity           */
/* Author:           Riccardo Maria BIANCHI, Jul 8 2019                     */
/*                                                                          */
/*--------------------------------------------------------------------------*/

// local includes
#include "MyTorus.h"
// #include "RevolutionSurfaceUtil.h" // TODO: do we need that?

#include <cassert>
#include <cmath>
#include <Inventor/actions/SoAction.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoNormalBinding.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/elements/SoTextureCoordinateElement.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/nodes/SoShapeHints.h>

#include <iostream>
#include <iomanip>



// TODO: add this to SbMath.h ??
#ifndef M_PI
  #define M_PI 3.14159265358979323846
  #define M_PI_2 	1.57079632679489661923
#endif
#define TWOPI (2.0 * M_PI)

// #define SPHERE_NUM_SLICES 30.0f
// #define SPHERE_NUM_STACKS 30.0f



// TODO: add this to SbMath.h
template <typename T> inline T Max(T a, T b) { return (a>b)?a:b; }
template <typename T> inline T Min(T a, T b) { return (a<b)?a:b; }
template <typename T> inline T Clamp(T a, T minV, T maxV) { return Min(Max(minV, a), maxV); }



//____________________________________________________________________
// Default Constructor
MyTorus::MyTorus()
  // : m_internalShape(nullptr)
{
  // sample values
  fRtor = 50;
  fRmax = 30;
  fRmin = 10;
  fSPhi = 0;
  fDPhi = TWOPI;

  // Set the number of polygons to use
  m_info.numt = 50; // number of divisions from top view
  m_info.numc = 20; // number of divisions around the torus cross section

  m_myfile.open ("vertices.csv");
}

//____________________________________________________________________
// Constructor with arguments
MyTorus::MyTorus(double Rtor, double Rmax, double Rmin, double SPhi /*degrees*/, double DPhi/*degrees*/, int divsMajor, int divsMinor)
  // : m_internalShape(nullptr)
{
  // set values
  fRtor = Rtor;
  fRmax = Rmax;

  if (Rmin==-1) {
    fRmin = Rmax;
  } else {
    fRmin = Rmin;
  }

  fSPhi = (SPhi * M_PI ) / 180;
  fDPhi = (DPhi * M_PI ) / 180;

  // Set the number of polygons to use
  m_info.numt = divsMajor; // number of divisions from top view
  m_info.numc = divsMinor; // number of divisions around the torus cross section

  m_myfile.open ("vertices.csv");
}


//____________________________________________________________________
// Destructor
MyTorus::~MyTorus()
{
  m_myfile.close();
}



//____________________________________________________________________
// Build the toroidal shape
void
MyTorus::updateInternalShape(SoTriangleStripSet* shape, SoVertexProperty* vertexProperty, double Rxs)
{
  // Each triangle strip goes around the top view
  // Number of vertices per strip = twice the number of (major subdivisions + 1)
  // because we want to loop back to the beginning for the full toroid
  const int verticesPerStrip = 2 * (m_info.numt + 1);

  // Number of triangle strips = number of minor subdivisions
  const int numStrips = m_info.numc;

  // Set the numVertices field of the single TriangleStripSet accordingly
  shape->numVertices.setNum( numStrips );
  int32_t* numVertices = shape->numVertices.startEditing();
  for ( int strip = 0; strip < numStrips; strip++ )
    numVertices[strip] = verticesPerStrip; // set the number of vertices of each strip
  shape->numVertices.finishEditing();

  // Set the size of the VertexProperty buffers
  const int numVerticesTotal = verticesPerStrip * numStrips;
  vertexProperty->vertex.setNum( numVerticesTotal );
  vertexProperty->normal.setNum( numVerticesTotal );
  vertexProperty->texCoord.setNum( numVerticesTotal );

  SbVec3f* vertices  = vertexProperty->vertex.startEditing();
  SbVec3f* normals   = vertexProperty->normal.startEditing();
  SbVec2f* texCoords = vertexProperty->texCoord.startEditing();

  // Now fill the buffers
  int vertexIndex = 0;

  // go around cross section
  for ( int strip = 0; strip < numStrips; strip++ )
  {
    // go around top view
    for ( int stripVertex = 0; stripVertex <= m_info.numt; stripVertex++ )
    {
      // each strip have two vertices per stripVertex: one on the top line and one on the bottom line
      // i.e. strip 0 has the stripVertex 0 vertices on the line 0 and 1,
      //      strip 1 has the stripVertex 0 vertices on the line 1 and 2,
      //      and so forth...
      for ( int offset = 1; offset >= 0; offset-- )
      {
        const int crossSection = strip + offset;

        std::cout << "\nstrip: " << strip << ", stripVertex: " << stripVertex << ", offset: " << offset << ", crossSection: " << crossSection << std::endl;

        vertices[vertexIndex]  = getVertex( Rxs, crossSection, m_info.numc, stripVertex, m_info.numt );
        normals[vertexIndex]   = getNormal( vertices[vertexIndex], stripVertex, m_info.numt );
        texCoords[vertexIndex] = getTexCoord( crossSection, m_info.numc, stripVertex, m_info.numt );

        std::cout << std::fixed << std::setprecision( 3 );

        std::cout << "getVertex: {" << vertices[vertexIndex][0] << ","
                                    << vertices[vertexIndex][1] << ","
                                    << vertices[vertexIndex][2] << "}";

        std::cout << ", getNormal: {" << normals[vertexIndex][0] << ","
                                      << normals[vertexIndex][1] << ","
                                      << normals[vertexIndex][2] << "}";

        std::cout << ", getTexCoord: {" << texCoords[vertexIndex][0] << ","
                                        << texCoords[vertexIndex][1] << ","
                                        << texCoords[vertexIndex][2] << "}" << std::endl;

        m_myfile << vertices[vertexIndex][0] << ","
                                        << vertices[vertexIndex][1] << ","
                                        << vertices[vertexIndex][2] << std::endl;

        // go back to the default values
        std::cout.unsetf(std::ios::fixed);
        std::setprecision( 6 );

        vertexIndex++;
      }
    }
  }


  vertexProperty->vertex.finishEditing();
  vertexProperty->normal.finishEditing();
  vertexProperty->texCoord.finishEditing();

}



//____________________________________________________________________
// Build the endcap's shape
void
MyTorus::buildEndcaps(SoFaceSet* shape, SoVertexProperty* vertexProperty, double Rxs, int slice)
{
  // Each endcap is a disk, made as a FaceSet

  // Number of vertices per strip
  const int verticesPerFace = m_info.numc;

  // Number of minor subdivisions
  const int numStrips = m_info.numc;

  //Set the numVertices field of the single FaceSet accordingly
  shape->numVertices.setNum( 1 );
  int32_t* numVertices = shape->numVertices.startEditing();
  numVertices[0] = verticesPerFace; // set the number of vertices of each strip
  shape->numVertices.finishEditing();
  // shape->numVertices.setValues(0, 1, &verticesPerFace);

  // Set the size of the VertexProperty buffers
  const int numVerticesTotal = verticesPerFace * 1;
  vertexProperty->vertex.setNum( numVerticesTotal );
  vertexProperty->normal.setNum( numVerticesTotal );
  vertexProperty->texCoord.setNum( numVerticesTotal );

  SbVec3f* vertices  = vertexProperty->vertex.startEditing();
  SbVec3f* normals   = vertexProperty->normal.startEditing();
  SbVec2f* texCoords = vertexProperty->texCoord.startEditing();

  // Now fill the buffers
  int vertexIndex = 0;

  // go around cross section
  for ( int strip = 0; strip < numStrips; strip++ )
  {
    // go around top view
    for ( int stripVertex = 0; stripVertex <= m_info.numt; stripVertex++ )
    {
      // we only want to  build the required face
      if (stripVertex != slice)
        continue;

        vertices[vertexIndex]  = getVertex( Rxs, strip, m_info.numc, stripVertex, m_info.numt );
        normals[vertexIndex]   = getNormal( vertices[vertexIndex], stripVertex, m_info.numt );
        texCoords[vertexIndex] = getTexCoord( strip, m_info.numc, stripVertex, m_info.numt );

        vertexIndex++;
    }
  } // end go around cross section

  vertexProperty->vertex.finishEditing();
  vertexProperty->normal.finishEditing();
  vertexProperty->texCoord.finishEditing();

}

//____________________________________________________________________
// Build the endcap's shape
void
MyTorus::buildEndcaps(SoTriangleStripSet* shape, SoVertexProperty* vertexProperty, double Rxs, double Rinner, int slice)
{
  // Each endcap is a cirular segment, made of one triangleStrip

  // Number of vertices of the strip
  const int verticesPerFace = 2 * m_info.numc + 2;

  // Number of minor subdivisions
  const int numStrips = m_info.numc;

  //Set the numVertices field of the single FaceSet accordingly
  shape->numVertices.setNum( 1 );
  int32_t* numVertices = shape->numVertices.startEditing();
  numVertices[0] = verticesPerFace; // set the number of vertices of each strip
  shape->numVertices.finishEditing();
  // shape->numVertices.setValues(0, 1, &verticesPerFace);

  // Set the size of the VertexProperty buffers
  const int numVerticesTotal = verticesPerFace * 1;
  vertexProperty->vertex.setNum( numVerticesTotal );
  vertexProperty->normal.setNum( numVerticesTotal );
  vertexProperty->texCoord.setNum( numVerticesTotal );

  SbVec3f* vertices  = vertexProperty->vertex.startEditing();
  SbVec3f* normals   = vertexProperty->normal.startEditing();
  SbVec2f* texCoords = vertexProperty->texCoord.startEditing();

  // Now fill the buffers
  int vertexIndex = 0;

  // go around cross section
  for ( int strip = 0; strip < numStrips; strip++ )
  {
    // go around top view
    for ( int stripVertex = 0; stripVertex <= m_info.numt; stripVertex++ )
    {
      // we only want to  build the required face
      if (stripVertex != slice)
        continue;

        vertices[vertexIndex]  = getVertex( Rxs, strip, m_info.numc, stripVertex, m_info.numt );
        normals[vertexIndex]   = getNormal( vertices[vertexIndex], stripVertex, m_info.numt );
        texCoords[vertexIndex] = getTexCoord( strip, m_info.numc, stripVertex, m_info.numt );

        vertexIndex++;

        vertices[vertexIndex]  = getVertex( Rinner, strip, m_info.numc, stripVertex, m_info.numt );
        normals[vertexIndex]   = getNormal( vertices[vertexIndex], stripVertex, m_info.numt );
        texCoords[vertexIndex] = getTexCoord( strip, m_info.numc, stripVertex, m_info.numt );

        vertexIndex++;
    }
  } // end go around cross section

  // last two vertices, to close the strip
  vertices[vertexIndex]  = getVertex( Rxs, 0, m_info.numc, slice, m_info.numt );
  normals[vertexIndex]   = getNormal( vertices[vertexIndex], slice, m_info.numt );
  texCoords[vertexIndex] = getTexCoord( 0, m_info.numc, slice, m_info.numt );

  vertexIndex++;

  vertices[vertexIndex]  = getVertex( Rinner, 0, m_info.numc, slice, m_info.numt );
  normals[vertexIndex]   = getNormal( vertices[vertexIndex], slice, m_info.numt );
  texCoords[vertexIndex] = getTexCoord( 0, m_info.numc, slice, m_info.numt );

  vertexProperty->vertex.finishEditing();
  vertexProperty->normal.finishEditing();
  vertexProperty->texCoord.finishEditing();

  return;
}



// Computes vertex position given the current torus subdivision we are working on.
// - "minorSubdiv" is the index of line: for example, strip 0 has vertices on line 0 and 1, strip 1 has them on line 1 and 2, ...
// - "numMinorSubdivs" is the total number of strips around the cross section
// - "subdiv" is the index of the current substrip: e.g., subdiv 0 out of 5 if the strip is divided in 5 substrips from the top view
// - "numSubdivs" is the total number of substrips from the top view
SbVec3f
MyTorus::getVertex( double Rcross, int minorSubdiv, int numMinorSubdivs, int subdiv, int numSubdivs )
{
  // used for the full torus
  // subdiv %= numSubdivs; // when subdiv == numSubdivs it gets back to 0
  // minorSubdiv %= numMinorSubdivs; // TODO: is that needed at all???

  // debug
  // std::cout << "subdiv(stripVertex): " << subdiv << ", minorSubdiv(crossSection): " << minorSubdiv << std::endl;
  // std::cout << "fSPhi: " << fSPhi.getValue() << " - fDPhi: " << fDPhi.getValue() << std::endl;

  const double angle = fSPhi.getValue() + fDPhi.getValue() * static_cast<double>( subdiv ) / static_cast<double>( numSubdivs );
  const double minorAngle = TWOPI * static_cast<double>( minorSubdiv ) / static_cast<double>( numMinorSubdivs );

  const double minorAngleCos = fRtor.getValue() + Rcross * cos(minorAngle); // this is the coordinate along the radius of the torus

  // return the coordinates of the vertex in spherical coordinates
  return SbVec3f( static_cast<float>(minorAngleCos * cos(angle)), // x/y plane
                  static_cast<float>(minorAngleCos * sin(angle)), // x/y plane
                  static_cast<float>(Rcross * sin(minorAngle)) ); // elevation // z
}


// Computes vertex texture coordinates given the current torus subdivision we are working on.
SbVec2f
MyTorus::getTexCoord( int minorSubdiv, int numMinorSubdivs, int subdiv, int numSubdivs )
{
  return SbVec2f( static_cast<float>(minorSubdiv) / static_cast<float>(numMinorSubdivs),
                  1.0f - static_cast<float>(subdiv) / static_cast<float>(numSubdivs) );
}


// Computes vertex normal given the current torus subdivision we are working on.
SbVec3f
MyTorus::getNormal( const SbVec3f& vert, int subdiv, int numSubdivs )
{
  subdiv %= numSubdivs;

  const double angle = fSPhi.getValue() + fDPhi.getValue() * static_cast<double>( subdiv ) / static_cast<double>( numSubdivs );

  SbVec3f norm( vert[0] - fRtor.getValue() * static_cast<float>(cos(angle)),
                vert[1] - fRtor.getValue() * static_cast<float>(sin(angle)),
                vert[2] );
  norm.normalize();

  return norm;
}

SoSeparator*
MyTorus::getSeparator( )
{

  SoSeparator *sep = new SoSeparator;
  sep->ref();

  // // A shape hints tells the ordering of polygons.
  // // This ensures double-sided lighting.
  SoShapeHints *myHints = new SoShapeHints;
  myHints->vertexOrdering = SoShapeHints::COUNTERCLOCKWISE;
  sep->addChild(myHints);

  SoVertexProperty* vertexProperty = new SoVertexProperty;
  vertexProperty->normalBinding.setValue( SoVertexProperty::PER_VERTEX );
  vertexProperty->materialBinding.setValue( SoVertexProperty::OVERALL );

  SoTriangleStripSet* shape = new SoTriangleStripSet;
  shape->vertexProperty.setValue( vertexProperty );

  updateInternalShape( shape, vertexProperty, fRmax.getValue() );
  sep->addChild(shape);

  // if Rmin is set, we build a second, inner torus
  if (fRmin.getValue() != -1) {
    SoVertexProperty* vertexProperty_inner = new SoVertexProperty;
    vertexProperty_inner->normalBinding.setValue( SoVertexProperty::PER_VERTEX );
    vertexProperty_inner->materialBinding.setValue( SoVertexProperty::OVERALL );

    SoTriangleStripSet* shape_inner = new SoTriangleStripSet;
    shape_inner->vertexProperty.setValue( vertexProperty_inner );

    updateInternalShape( shape_inner, vertexProperty_inner, fRmin.getValue() );
    sep->addChild(shape_inner);

    // add endcaps
    SoVertexProperty* vertexProperty_endcaps_a = new SoVertexProperty;
    vertexProperty_endcaps_a->normalBinding.setValue( SoVertexProperty::PER_VERTEX );
    vertexProperty_endcaps_a->materialBinding.setValue( SoVertexProperty::OVERALL );
    SoTriangleStripSet* shape_endcaps_a = new SoTriangleStripSet;
    shape_endcaps_a->vertexProperty.setValue( vertexProperty_endcaps_a );
    buildEndcaps( shape_endcaps_a, vertexProperty_endcaps_a, fRmax.getValue(), fRmin.getValue(), 0 );
    sep->addChild(shape_endcaps_a);

    SoVertexProperty* vertexProperty_endcaps_b = new SoVertexProperty;
    vertexProperty_endcaps_b->normalBinding.setValue( SoVertexProperty::PER_VERTEX );
    vertexProperty_endcaps_b->materialBinding.setValue( SoVertexProperty::OVERALL );
    SoTriangleStripSet* shape_endcaps_b = new SoTriangleStripSet;
    shape_endcaps_b->vertexProperty.setValue( vertexProperty_endcaps_b );
    buildEndcaps( shape_endcaps_b, vertexProperty_endcaps_b, fRmax.getValue(), fRmin.getValue(), m_info.numt );
    sep->addChild(shape_endcaps_b);


  }

  // if Rmin == 0, then we add endcaps
  if (fRmin.getValue() == 0) {

    // first endcap
    SoVertexProperty* vertexProperty_endcaps_a = new SoVertexProperty;
    vertexProperty_endcaps_a->normalBinding.setValue( SoVertexProperty::PER_VERTEX );
    vertexProperty_endcaps_a->materialBinding.setValue( SoVertexProperty::OVERALL );
    SoFaceSet* shape_endcaps_a = new SoFaceSet;
    shape_endcaps_a->vertexProperty.setValue( vertexProperty_endcaps_a );
    buildEndcaps( shape_endcaps_a, vertexProperty_endcaps_a, fRmax.getValue(), 0 );

    // second endcap
    SoVertexProperty* vertexProperty_endcaps_b = new SoVertexProperty;
    vertexProperty_endcaps_b->normalBinding.setValue( SoVertexProperty::PER_VERTEX );
    vertexProperty_endcaps_b->materialBinding.setValue( SoVertexProperty::OVERALL );
    SoFaceSet* shape_endcaps_b = new SoFaceSet;
    shape_endcaps_b->vertexProperty.setValue( vertexProperty_endcaps_b );
    buildEndcaps( shape_endcaps_b, vertexProperty_endcaps_b, fRmax.getValue(), m_info.numt);

    // A shape hints tells the ordering of polygons.
    // This ensures double-sided lighting.
    SoShapeHints *myHints = new SoShapeHints;
    myHints->vertexOrdering = SoShapeHints::COUNTERCLOCKWISE;
    myHints->shapeType = SoShapeHints::SOLID;
    myHints->creaseAngle.setValue(0.0);
    sep->addChild(myHints);
    sep->addChild(shape_endcaps_a);

    SoShapeHints *myHints_b = new SoShapeHints;
    myHints_b->vertexOrdering = SoShapeHints::CLOCKWISE;
    myHints_b->shapeType = SoShapeHints::SOLID;
    myHints_b->creaseAngle.setValue(0.0);
    sep->addChild(myHints_b);
    sep->addChild(shape_endcaps_b);
  }

  return sep;
}

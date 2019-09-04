
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
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoShapeHints.h>
// #include <Inventor/nodes/SoDirectionalLight.h>

#include <iostream>


// TODO: add this to SbMath.h ??
#ifndef M_PI
  #define M_PI 3.14159265358979323846
  #define M_PI_2 	1.57079632679489661923
#endif
#define TWOPI (2.0 * M_PI)



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
  fRMajor = 50;
  fRMinor = 30;
  fRInner = 10;
  fSPhi = 0;
  fDPhi = TWOPI;

  // Set the number of polygons to use
  m_info.numt = 50; // number of divisions from top view
  m_info.numc = 20; // number of divisions around the torus cross section
}

//____________________________________________________________________
// Constructor with arguments
MyTorus::MyTorus(double rMajor, double rMinor, double rInner, double SPhi /*degrees*/, double DPhi/*degrees*/, int divsMajor, int divsMinor)
  // : m_internalShape(nullptr)
{
  // set values
  fRMajor = rMajor;
  fRMinor = rMinor;

  if (rInner==-1) {
    fRInner = rMinor;
  } else {
    fRInner = rInner;
  }

  fSPhi = (SPhi * M_PI ) / 180;
  fDPhi = (DPhi * M_PI ) / 180;

  // Set the number of polygons to use
  m_info.numt = divsMajor; // number of divisions from top view
  m_info.numc = divsMinor; // number of divisions around the torus cross section
}


//____________________________________________________________________
// Destructor
MyTorus::~MyTorus()
{
  // nothing to do
}



//____________________________________________________________________
// Build the toroidal shape
void
MyTorus::updateInternalShape(SoTriangleStripSet* shape, SoVertexProperty* vertexProperty, double Rxs, bool inner)
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

        vertices[vertexIndex]  = getVertex( Rxs, crossSection, m_info.numc, stripVertex, m_info.numt );
        normals[vertexIndex]   = getNormal( vertices[vertexIndex], stripVertex, m_info.numt, inner );
        texCoords[vertexIndex] = getTexCoord( crossSection, m_info.numc, stripVertex, m_info.numt );

        vertexIndex++;
      }
    }
  }
  vertexProperty->vertex.finishEditing();
  vertexProperty->normal.finishEditing();
  vertexProperty->texCoord.finishEditing();
}



//_______________________________________________________
// Build the filled endcap
void
MyTorus::buildEndcaps(SoFaceSet* shape, SoVertexProperty* vertexProperty, double Rxs, int slice, bool invert)
{
  // Each endcap is a disk, made as a FaceSet

  // Number of vertices per strip
  const int verticesPerFace = m_info.numc;

  // Number of minor subdivisions
  const int numStrips = m_info.numc;

  //Set the numVertices field of the single FaceSet accordingly
  shape->numVertices.setValues(0, 1, &verticesPerFace);

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
      vertices[vertexIndex]  = getVertex( Rxs, strip, m_info.numc, slice, m_info.numt );
      normals[vertexIndex]   = getNormalEndCap( vertices[vertexIndex], slice, m_info.numt, invert );
      texCoords[vertexIndex] = getTexCoord( strip, m_info.numc, slice, m_info.numt );
      vertexIndex++;
  } // end go around cross section

  vertexProperty->vertex.finishEditing();
  vertexProperty->normal.finishEditing();
  vertexProperty->texCoord.finishEditing();

}

//____________________________________________________
// Build the pierced endcap
void
MyTorus::buildEndcaps(SoTriangleStripSet* shape, SoVertexProperty* vertexProperty, double Rxs, double Rinner, int slice, bool invert)
{
  // Each endcap is a cirular segment, made of one triangleStrip

  // Number of vertices of the strip
  // twice the number of the divisions,
  // plus two additional vertices to close the strip
  const int verticesPerFace = (2 * m_info.numc) + 2;

  // Number of minor subdivisions
  const int numStrips = m_info.numc;

  //Set the numVertices field of the single FaceSet accordingly
  shape->numVertices.setValues(0, 1, &verticesPerFace);

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
      vertices[vertexIndex]  = getVertex( Rxs, strip, m_info.numc, slice, m_info.numt );
      normals[vertexIndex]   = getNormalEndCap( vertices[vertexIndex], slice, m_info.numt, invert );
      texCoords[vertexIndex] = getTexCoord( strip, m_info.numc, slice, m_info.numt );

      vertexIndex++;

      vertices[vertexIndex]  = getVertex( Rinner, strip, m_info.numc, slice, m_info.numt );
      normals[vertexIndex]   = getNormalEndCap( vertices[vertexIndex], slice, m_info.numt, invert  );
      texCoords[vertexIndex] = getTexCoord( strip, m_info.numc, slice, m_info.numt );

      vertexIndex++;
  } // end go around cross section

  // last two vertices, to close the strip
  vertices[vertexIndex]  = getVertex( Rxs, 0, m_info.numc, slice, m_info.numt );
  normals[vertexIndex]   = getNormalEndCap( vertices[vertexIndex], slice, m_info.numt, invert  );
  texCoords[vertexIndex] = getTexCoord( 0, m_info.numc, slice, m_info.numt );

  vertexIndex++;

  vertices[vertexIndex]  = getVertex( Rinner, 0, m_info.numc, slice, m_info.numt );
  normals[vertexIndex]   = getNormalEndCap( vertices[vertexIndex], slice, m_info.numt, invert  );
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

  const double minorAngleCos = fRMajor.getValue() + Rcross * cos(minorAngle); // this is the coordinate along the radius of the torus

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
MyTorus::getNormal( const SbVec3f& vert, int subdiv, int numSubdivs, bool invert )
{
  subdiv %= numSubdivs;

  const double angle = fSPhi.getValue() + fDPhi.getValue() * static_cast<double>( subdiv ) / static_cast<double>( numSubdivs );

  SbVec3f norm( vert[0] - fRMajor.getValue() * static_cast<float>(cos(angle)),
                vert[1] - fRMajor.getValue() * static_cast<float>(sin(angle)),
                vert[2] );
  norm.normalize();
  // if an inner torus, we invert the normals
  if (invert) {
    norm.negate();
  }
  return norm;
}

// Computes vertex normal for the endcap
SbVec3f
MyTorus::getNormalEndCap( const SbVec3f& vert, int subdiv, int numSubdivs, bool invert )
{
  const double angle = fSPhi.getValue() + fDPhi.getValue() * static_cast<double>( subdiv ) / static_cast<double>( numSubdivs );

  // debug
  // std::cout << "SPhi,DPhi: " << fSPhi.getValue() << "," << fDPhi.getValue() << ", subdiv: " << subdiv << ", numSubdivs: " << numSubdivs << ", angle: " << angle << ", RcosA: " << fRMajor.getValue() * static_cast<float>(cos(angle)) << ", RsinA: " <<  fRMajor.getValue() * static_cast<float>(sin(angle)) << std::endl;
  // std::cout << "vert: " << vert.toString().getString() << std::endl;

  SbVec3f norm;
  if ((angle > M_PI_2) && (angle <= M_PI)) {
    norm.setValue( fRMajor.getValue() * (-1 * static_cast<float>(sin(angle))),
                   fRMajor.getValue() * static_cast<float>(cos(angle)),
                   0);
  } else if ((angle > M_PI) && (angle <= 3 * M_PI_2)) {
    norm.setValue( fRMajor.getValue() * abs(static_cast<float>(sin(angle))),
                   fRMajor.getValue() * static_cast<float>(cos(angle)),
                   0);
  } else if ((angle > 3 * M_PI_2) && (angle < 2 * M_PI)) {
    norm.setValue( fRMajor.getValue() * abs(static_cast<float>(sin(angle))),
                   fRMajor.getValue() * static_cast<float>(cos(angle)),
                   0);
  } else {
    norm.setValue( fRMajor.getValue() * static_cast<float>(sin(angle)),
                   fRMajor.getValue() * static_cast<float>(cos(angle)),
                   0);
  }
  norm.normalize();
  // if we chose to invert the normals
  if (invert) {
    norm.negate();
  }
  return norm;
}

SoSeparator*
MyTorus::getSeparator( )
{

  SoSeparator *sep = new SoSeparator;
  sep->ref();

  // // Add a directional light - TEST
  //  SoDirectionalLight *myDirLight = new SoDirectionalLight;
  //  myDirLight->direction.setValue(0, -1, -1);
  //  myDirLight->color.setValue(1, 0, 0);
  //  sep->addChild(myDirLight);

  // // A shape hints tells the ordering of polygons.
  // // This ensures double-sided lighting.
  // SoShapeHints *myHints = new SoShapeHints;
  // myHints->vertexOrdering = SoShapeHints::COUNTERCLOCKWISE;
  // sep->addChild(myHints);

  SoVertexProperty* vertexProperty = new SoVertexProperty;
  vertexProperty->normalBinding.setValue( SoVertexProperty::PER_VERTEX );
  vertexProperty->materialBinding.setValue( SoVertexProperty::OVERALL );

  SoTriangleStripSet* shape = new SoTriangleStripSet;
  shape->vertexProperty.setValue( vertexProperty );

  updateInternalShape( shape, vertexProperty, fRMinor.getValue() );
  sep->addChild(shape);

  // if rInner is set to 0, then we add filled endcaps
  if (fRInner.getValue() == 0) {

    std::cout << "\nBuild starting endcap..." <<std::endl;
    // first endcap, at the beginning of the toroidal segment
    SoVertexProperty* vertexProperty_endcaps_a = new SoVertexProperty;
    vertexProperty_endcaps_a->normalBinding.setValue( SoVertexProperty::PER_VERTEX );
    vertexProperty_endcaps_a->materialBinding.setValue( SoVertexProperty::OVERALL );
    SoFaceSet* shape_endcaps_a = new SoFaceSet;
    shape_endcaps_a->vertexProperty.setValue( vertexProperty_endcaps_a );
    buildEndcaps( shape_endcaps_a, vertexProperty_endcaps_a, fRMinor.getValue(), 0, true );
    sep->addChild(shape_endcaps_a);

    std::cout << "\nBuild ending endcap..." <<std::endl;
    // second endcap, at the end of the toroidal segment
    SoVertexProperty* vertexProperty_endcaps_b = new SoVertexProperty;
    vertexProperty_endcaps_b->normalBinding.setValue( SoVertexProperty::PER_VERTEX );
    vertexProperty_endcaps_b->materialBinding.setValue( SoVertexProperty::OVERALL );
    SoFaceSet* shape_endcaps_b = new SoFaceSet;
    shape_endcaps_b->vertexProperty.setValue( vertexProperty_endcaps_b );
    buildEndcaps( shape_endcaps_b, vertexProperty_endcaps_b, fRMinor.getValue(), m_info.numt);
    sep->addChild(shape_endcaps_b);
  }
  // if rInner is set, we build a second, inner torus and pierced endcaps
  else if (fRInner.getValue() != -1) {

    // add a second, inner torus
    SoVertexProperty* vertexProperty_inner = new SoVertexProperty;
    vertexProperty_inner->normalBinding.setValue( SoVertexProperty::PER_VERTEX );
    vertexProperty_inner->materialBinding.setValue( SoVertexProperty::OVERALL );
    SoTriangleStripSet* shape_inner = new SoTriangleStripSet;
    shape_inner->vertexProperty.setValue( vertexProperty_inner );
    updateInternalShape( shape_inner, vertexProperty_inner, fRInner.getValue(), true );
    sep->addChild(shape_inner);

    // add endcaps
    // first endcap, at the beginning of the toroidal segment
    std::cout << "\nBuild starting endcap..." <<std::endl;
    SoVertexProperty* vertexProperty_endcaps_a = new SoVertexProperty;
    vertexProperty_endcaps_a->normalBinding.setValue( SoVertexProperty::PER_VERTEX );
    vertexProperty_endcaps_a->materialBinding.setValue( SoVertexProperty::OVERALL );
    SoTriangleStripSet* shape_endcaps_a = new SoTriangleStripSet;
    shape_endcaps_a->vertexProperty.setValue( vertexProperty_endcaps_a );
    buildEndcaps( shape_endcaps_a, vertexProperty_endcaps_a, fRMinor.getValue(), fRInner.getValue(), 0, true);
    sep->addChild(shape_endcaps_a);

    // second endcap, at the end of the toroidal segment
    std::cout << "\nBuild ending endcap..." <<std::endl;
    SoVertexProperty* vertexProperty_endcaps_b = new SoVertexProperty;
    vertexProperty_endcaps_b->normalBinding.setValue( SoVertexProperty::PER_VERTEX );
    vertexProperty_endcaps_b->materialBinding.setValue( SoVertexProperty::OVERALL );
    SoTriangleStripSet* shape_endcaps_b = new SoTriangleStripSet;
    shape_endcaps_b->vertexProperty.setValue( vertexProperty_endcaps_b );
    buildEndcaps( shape_endcaps_b, vertexProperty_endcaps_b, fRMinor.getValue(), fRInner.getValue(), m_info.numt );
    sep->addChild(shape_endcaps_b);
  }

  return sep;
}

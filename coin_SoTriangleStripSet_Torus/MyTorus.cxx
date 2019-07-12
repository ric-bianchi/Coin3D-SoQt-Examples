
/*
  Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
*/

/*-----------------------------HEPVis---------------------------------------*/
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

#include <Inventor/elements/SoComplexityElement.h>
#include <Inventor/misc/SoState.h>

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
  : m_internalShape(nullptr)
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
  : m_internalShape(nullptr)
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
// Build the internal shape depending on the elements from the current state
void
MyTorus::updateInternalShape()
{
  // Each triangle strip goes around the top view
  // Number of vertices per strip = twice the number of (major subdivisions + 1)
  // because we want to loop back to the beginning for the full toroid
  const int verticesPerStrip = 2 * (m_info.numt + 1); // full torus
  // const int verticesPerStrip = 2 * info.numt; // toroidal segment

  // Number of triangle strips = number of minor subdivisions
  const int numStrips = m_info.numc;

  // Set the numVertices field of the single TriangleStripSet accordingly
  m_internalShape->numVertices.setNum( numStrips );
  int32_t* numVertices = m_internalShape->numVertices.startEditing();
  for ( int strip = 0; strip < numStrips; strip++ )
    numVertices[strip] = verticesPerStrip; // set the number of vertices of each strip
  m_internalShape->numVertices.finishEditing();

  // Set the size of the VertexProperty buffers
  const int numVerticesTotal = verticesPerStrip * numStrips;
  m_vertexProperty->vertex.setNum( numVerticesTotal );
  m_vertexProperty->normal.setNum( numVerticesTotal );
  m_vertexProperty->texCoord.setNum( numVerticesTotal );

  SbVec3f* vertices  = m_vertexProperty->vertex.startEditing();
  SbVec3f* normals   = m_vertexProperty->normal.startEditing();
  SbVec2f* texCoords = m_vertexProperty->texCoord.startEditing();

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

        vertices[vertexIndex]  = getVertex( crossSection, m_info.numc, stripVertex, m_info.numt );
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

  m_vertexProperty->vertex.finishEditing();
  m_vertexProperty->normal.finishEditing();
  m_vertexProperty->texCoord.finishEditing();
}



// Computes vertex position given the current torus subdivision we are working on.
// - "minorSubdiv" is the index of line: for example, strip 0 has vertices on line 0 and 1, strip 1 has them on line 1 and 2, ...
// - "numMinorSubdivs" is the total number of strips around the cross section
// - "subdiv" is the index of the current substrip: e.g., subdiv 0 out of 5 if the strip is divided in 5 substrips from the top view
// - "numSubdivs" is the total number of substrips from the top view
SbVec3f
MyTorus::getVertex( int minorSubdiv, int numMinorSubdivs, int subdiv, int numSubdivs )
{
  // used for the full torus
  // subdiv %= numSubdivs; // when subdiv == numSubdivs it gets back to 0
  // minorSubdiv %= numMinorSubdivs; // TODO: is that needed at all???

  std::cout << "subdiv(stripVertex): " << subdiv << ", minorSubdiv(crossSection): " << minorSubdiv << std::endl;

  // full torus
  // const double angle = M_PI_2 + TWOPI * static_cast<double>( subdiv ) / static_cast<double>( numSubdivs );
  // const double minorAngle = TWOPI * static_cast<double>( minorSubdiv ) / static_cast<double>( numMinorSubdivs );

  // test half torus
  // const double angle = M_PI_2 + M_PI * static_cast<double>( subdiv ) / static_cast<double>( numSubdivs );
  // const double minorAngle = TWOPI * static_cast<double>( minorSubdiv ) / static_cast<double>( numMinorSubdivs );

  // test
  std::cout << "fSPhi: " << fSPhi.getValue() << " - fDPhi: " << fDPhi.getValue() << std::endl;
  const double angle = fSPhi.getValue() + fDPhi.getValue() * static_cast<double>( subdiv ) / static_cast<double>( numSubdivs );
  const double minorAngle = TWOPI * static_cast<double>( minorSubdiv ) / static_cast<double>( numMinorSubdivs );

  const double minorAngleCos = fRtor.getValue() + fRmax.getValue() * cos(minorAngle); // this is the coordinate along the radius of the torus

  // return the coordinates of the vertex in spherical coordinates
  return SbVec3f( static_cast<float>(minorAngleCos * cos(angle)), // x/y plane
                  static_cast<float>(minorAngleCos * sin(angle)), // x/y plane
                  static_cast<float>(fRmax.getValue() * sin(minorAngle)) ); // elevation // z
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

  // full torus
  const double angle = M_PI_2 + TWOPI * static_cast<double>( subdiv ) / static_cast<double>( numSubdivs );

  // test
  // const double angle = fSPhi.getValue() + fDPhi.getValue() * static_cast<double>( subdiv ) / static_cast<double>( numSubdivs );

  SbVec3f norm( vert[0] - fRtor.getValue() * static_cast<float>(cos(angle)),
                vert[1] - fRtor.getValue() * static_cast<float>(sin(angle)),
                vert[2] );
  norm.normalize();

  return norm;
}

SoNode*
MyTorus::getShape( )
{
  // Initialize internal shape
  if ( m_internalShape == nullptr )
  {
    m_vertexProperty = new SoVertexProperty;
    m_vertexProperty->normalBinding.setValue( SoVertexProperty::PER_VERTEX );
    m_vertexProperty->materialBinding.setValue( SoVertexProperty::OVERALL );

    m_internalShape = new SoTriangleStripSet;
    m_internalShape->vertexProperty.setValue( m_vertexProperty );
  }

  // TODO:
  // If the cache handler is invalid, we need an update
  // if ( !m_internalShapeCache->isValid(state) )
  // {
    // updateInternalShape( state );
    updateInternalShape();
    // m_internalShapeCache->updateCache( state ); // make the cache valid again after the update
  // }

  return m_internalShape;
}

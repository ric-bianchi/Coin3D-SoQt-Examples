/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

/*---------------------------------------------------------------------------*/
/*                                                                           */
/* Node:             MyTorus                                                 */
/* Description:      Represents a Torus geometry entity                      */
/* Author:           Riccardo Maria BIANCHI, Jul 8 2019                      */
/*                                                                           */
/*---------------------------------------------------------------------------*/
#ifndef MyTorus_h
#define MyTorus_h

#include <Inventor/C/errors/debugerror.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/nodes/SoShape.h>

#include <Inventor/nodes/SoTriangleStripSet.h>
#include <Inventor/nodes/SoFaceSet.h>

class SoSFNode;
/*!
 * Class:             MyTorus
 *
 * Description: The Inventor version of the G4Cons Geant Geometry entity
 *
 * Author: Riccardo Maria BIANCHI <riccardo.maria.bianchi@cern.ch> Jul 7, 2019
 *
 *
 * Shape class representing a torus.
 * The fRMajor radius field controls the major radius of the torus (from the center).
 * The fRMinor radius field controls the outer radius of the cross section of the torus.
 * The fRInner radius field controls the inner radius of the cross section of the torus.
 *
 * The documentation from Geant4 says:
 *
 * A torus or torus segment with curved sides parallel to the z-axis.
 * The torus has a specified swept radius about which it is centered,
 * and a given minimum and maximum radius. A minimum radius of 0
 * signifies a filled torus.
 * The torus segment is specified by starting and delta angles for phi,
 * with 0 being the +x axis, PI/2 the +y axis. A delta angle of 2PI
 * signifies a complete, unsegmented torus/cylindr.
 *
 * Member Data:
 *
 *      fRMajor  radius of the torus itself, from the center
 *      fRMinor  outside radius of the torus cross-section (the torus "tube")
 *      fRInner  inside radius  of the torus cross-section (Rmin=0 if not hollow)
 *
 *      fSPhi   starting angle of the segment in radians
 *      fDPhi   delta angle of the segment in radians
 *
 * Note: partially implemented from OpenInventor tutorial at:
 * - https://developer100.openinventor.com/content/26-creating-shape-node
 *
*/

class MyTorus {

public:

  // Retrieve internal shape representing the torus
  SoSeparator* getSeparator();


  //
  //! Torus' radius
  //
  SoSFFloat fRMajor;
  //
  //! Outside radius
  //
  SoSFFloat fRMinor;
  //
  //! Inside radius
  //
  SoSFFloat fRInner;
  //
  //! Starting angle, in radians
  //
  SoSFFloat fSPhi;
  //
  //! Delta-angle, in radians
  //
  SoSFFloat fDPhi;
  //
  //! An Inventor option - slightly better render, worse performance
  //
  SoSFBool  smoothDraw;
  //
  //! Override number of phi subdivision used for rendering shape (i.e. ignore e.g. complexity value).
  //! Put field to 0 (the default) to ignore it.
  //
  SoSFInt32 pOverrideNPhi;
  //

  //
  //! Constructors
  //
  MyTorus();
  MyTorus(double rMajor, double rMinor, double rInner=-1, double SPhi=0/*degrees*/, double DPhi=360/*degrees*/, int divsMajor=70, int divsMinor=40);


protected:

  //
  //! compute bounding Box, required
  //
  // virtual void computeBBox(SoAction *action, SbBox3f &box, SbVec3f &center );

protected:
  //
  //! Destructor, required
  //
  virtual ~MyTorus();

private:

  // Structure used to pass around information about how to draw the torus
  struct TorusInfo
  {
    int numc; // number of vertices around cross-section
    int numt; // number of vertices around torus
  };

  // These methods are used to compute the different vertex properties given
  // the current torus subdivision we are working on during shape construction.
  SbVec3f getVertex( double Rcross, int minorSubdiv, int numMinorSubdivs, int subdiv, int numSubdivs );
  SbVec2f getTexCoord( int minorSubdiv, int numMinorSubdivs, int subdiv, int numSubdivs );
  SbVec3f getNormal( const SbVec3f& vert, int subdiv, int numSubdivs, bool invert=false );
  SbVec3f getNormalEndCap( const SbVec3f& vert, int subdiv, int numSubdivs, bool invert=false );

  // Update internal shape geometry depending on the Torus field values.
  void updateInternalShape( SoTriangleStripSet* shape, SoVertexProperty* vertexProperty, double Rxsection, bool inner=false );

  // build an endcap, in case of building a toroidal segment
  void buildEndcaps(SoFaceSet* shape, SoVertexProperty* vertexProperty, double Rxs, int slice, bool invert=false);
  void buildEndcaps(SoTriangleStripSet* shape, SoVertexProperty* vertexProperty, double Rxs, double Rinner, int slice, bool invert=false);

  // Use this structure to hold info about how to draw the torus
  TorusInfo m_info;
};

#endif

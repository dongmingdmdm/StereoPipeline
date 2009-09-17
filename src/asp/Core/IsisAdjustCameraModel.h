// __BEGIN_LICENSE__
//
// Copyright (C) 2006 United States Government as represented by the
// Administrator of the National Aeronautics and Space Administration
// (NASA).  All Rights Reserved.
//
// Copyright 2006 Carnegie Mellon University. All rights reserved.
//
// This software is distributed under the NASA Open Source Agreement
// (NOSA), version 1.3.  The NOSA has been approved by the Open Source
// Initiative.  See the file COPYING at the top of the distribution
// directory tree for the complete NOSA document.
//
// THE SUBJECT SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY OF ANY
// KIND, EITHER EXPRESSED, IMPLIED, OR STATUTORY, INCLUDING, BUT NOT
// LIMITED TO, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL CONFORM TO
// SPECIFICATIONS, ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR
// A PARTICULAR PURPOSE, OR FREEDOM FROM INFRINGEMENT, ANY WARRANTY THAT
// THE SUBJECT SOFTWARE WILL BE ERROR FREE, OR ANY WARRANTY THAT
// DOCUMENTATION, IF PROVIDED, WILL CONFORM TO THE SUBJECT SOFTWARE.
//
// __END_LICENSE__

#ifndef __VW_CAMERAMODEL_ISISADJUST_H__
#define __VW_CAMERAMODEL_ISISADJUST_H__

// VW
#include <vw/Math/Vector.h>
#include <vw/Math/Matrix.h>
#include <vw/Camera/CameraModel.h>
#include <vw/Camera/PinholeModel.h>
#include <vw/Cartography/PointImageManipulation.h>

// STD
#include <iomanip>

// ASP
#include <asp/Core/IsisCameraModel.h>
#include <asp/Core/Equation.h>

// Isis Header
#include <Cube.h>
#include <Camera.h>
#include <CameraDetectorMap.h>
#include <CameraDistortionMap.h>
#include <CameraFocalPlaneMap.h>

namespace vw {
namespace camera {

  class IsisAdjustCameraModel : public IsisCameraModel {
  protected:
    mutable double m_current_time;
    void* m_isis_alpha_cube;

    void set_image( double const& sample, double const& line) const;
    void set_time( double const& time ) const;

    boost::shared_ptr<BaseEquation> m_position_func;
    boost::shared_ptr<BaseEquation> m_pose_func;


  public:
    //-------------------------------------------------------------------
    //  Constructors / Deconstructor
    //-------------------------------------------------------------------
    IsisAdjustCameraModel( std::string cube_filename,
                           boost::shared_ptr<BaseEquation> position_func,
                           boost::shared_ptr<BaseEquation> pose_func );

    virtual ~IsisAdjustCameraModel();

    virtual std::string type() const { return "IsisAdjust"; }

    //-------------------------------------------------------------------
    //  Traditional Camera Routines
    //-------------------------------------------------------------------

    virtual Vector2 point_to_pixel( Vector3 const& point) const;

    virtual Vector3 pixel_to_vector( Vector2 const& pix ) const;

    virtual Vector3 camera_center( Vector2 const& pix = Vector2() ) const;

    virtual Quaternion<double> camera_pose( Vector2 const& pix = Vector2() ) const;

    virtual int getLines( void ) const;

    virtual int getSamples( void ) const;

    virtual std::string serial_number(void) const;

    //-------------------------------------------------------------------
    //  Non-Traditional Camera Routines
    //-------------------------------------------------------------------

    Vector3 pixel_to_mm_time( Vector2 const& pix ) const;

    Vector2 mm_time_to_pixel( Vector3 const& mm_time ) const;

    Vector3 point_to_mm_time( Vector3 const& mm_time, Vector3 const& point ) const;

    Vector3 mm_time_to_vector( Vector3 const& mm_time ) const;

    Vector3 camera_center( Vector3 const& mm_time ) const;

    Quaternion<double> camera_pose( Vector3 const& mm_time ) const;

    double undistorted_focal( Vector3 const& mm_time ) const;


    //-------------------------------------------------------------------
    // Interfacing with equations
    //-------------------------------------------------------------------

    boost::shared_ptr<BaseEquation> getPositionFuncPoint( void ){
      return m_position_func;
    }

    boost::shared_ptr<BaseEquation> getPoseFuncPoint( void ) {
      return m_pose_func;
    }


  };

}
}

#endif//__VW_CAMERAMODEL_ISISADJUST_H__
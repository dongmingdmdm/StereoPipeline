// __BEGIN_LICENSE__
// Copyright (C) 2006-2011 United States Government as represented by
// the Administrator of the National Aeronautics and Space Administration.
// All Rights Reserved.
// __END_LICENSE__

#include <asp/Core/InterestPointMatching.h>

using namespace vw;

namespace asp {

  Vector3 datum_intersection( cartography::Datum const& datum,
                              camera::CameraModel* model,
                              Vector2 const& pix ) {
    using namespace vw;

    // Create XYZ point against moon
    Vector3 ccenter = model->camera_center( pix );
    Vector3 cpoint  = model->pixel_to_vector( pix );
    double radius_2 = datum.semi_major_axis() *
      datum.semi_major_axis();
    double alpha = -dot_prod(ccenter, cpoint );
    Vector3 projection = ccenter + alpha*cpoint;
    if ( norm_2_sqr(projection) > radius_2 ) {
      // did not intersect
      return Vector3();
    }

    alpha -= sqrt( radius_2 -
                   norm_2_sqr(projection) );
    return ccenter + alpha * cpoint;
  }

  // Find a rough homography that maps right to left using the camera
  // and datum information.
  Matrix<double>
  rough_homography_fit( camera::CameraModel* cam1,
                        camera::CameraModel* cam2,
                        BBox2i const& box1, BBox2i const& box2,
                        cartography::Datum const& datum ) {

    // Bounce several points off the datum and fit an affine.
    std::vector<Vector3> left_points, right_points;
    left_points.reserve(10000);
    right_points.reserve(10000);
    for ( size_t i = 0; i < 100; i++ ) {
      for ( size_t j = 0; j < 100; j++ ) {
        try {
          Vector2 l( double(box1.width() - 1) * i / 99.0,
                     double(box1.height() - 1 ) * j / 99.0 );

          Vector3 intersection =
            datum_intersection( datum, cam1, l );
          if ( intersection == Vector3() )
            continue;

          Vector2 r = cam2->point_to_pixel( intersection );

          if ( box2.contains( r ) ) {
            left_points.push_back( Vector3(l[0],l[1],1) );
            right_points.push_back( Vector3(r[0],r[1],1) );
          }
        } catch (camera::PixelToRayErr const& e ) {}
      }
    }

    typedef math::HomographyFittingFunctor hfit_func;
    math::RandomSampleConsensus<hfit_func, math::InterestPointErrorMetric> ransac( hfit_func(), math::InterestPointErrorMetric(), norm_2(Vector2(box1.height(),box1.width())) / 10 );
    return ransac( right_points, left_points );
  }

  vw::Matrix<double>
  homography_fit( std::vector<vw::ip::InterestPoint> const& ip1,
                  std::vector<vw::ip::InterestPoint> const& ip2,
                  vw::BBox2i const& image_size ) {
    using namespace vw;

    typedef math::HomographyFittingFunctor hfit_func;
    math::RandomSampleConsensus<hfit_func, math::InterestPointErrorMetric> ransac( hfit_func(), math::InterestPointErrorMetric(), norm_2(Vector2(image_size.width(),image_size.height())) / 10 );
    std::vector<Vector3>  copy1, copy2;
    copy1.reserve( ip1.size() );
    copy2.reserve( ip1.size() );
    for ( size_t i = 0; i < ip1.size(); i++ ) {
      copy1.push_back( Vector3(ip1[i].x, ip1[i].y, 1) );
      copy2.push_back( Vector3(ip2[i].x, ip2[i].y, 1) );
    }
    Matrix<double> ransac_result = ransac(copy1,copy2);
    return hfit_func()(copy1,copy2,ransac_result);
  }

}
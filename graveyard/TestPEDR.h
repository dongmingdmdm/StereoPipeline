// __BEGIN_LICENSE__
// 
// Copyright (C) 2008 United States Government as represented by the
// Administrator of the National Aeronautics and Space Administration
// (NASA).  All Rights Reserved.
// 
// Copyright 2008 Carnegie Mellon University. All rights reserved.
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

/// \file TestPEDR.h
///

#include <cxxtest/TestSuite.h>
#include "stereo.h"
#include "MOLA.h"
#include "vw/vw.h"

using namespace std;
using namespace vw;

class TestPEDR : public CxxTest::TestSuite {
 public:

  void testReadPEDRByTimeRange() {

    printf("\n\n");
    string database_dir = "/irg/data/mgs/mgs-m-mola-3-pedr-l1a-v1/mgsl_21xx/data";
    double query_time = -20723097.688171;   // Epoch for M01-00115
    double dt = 7.034798;                   // Scan duration for M01-00115
    unsigned orbit = 717;                  // Orbit number for M01-00115

    try {
      MOLA_PEDR_Reader reader(database_dir, orbit);

      list<PEDR_Shot> mola_shots = reader.get_pedr_by_time_range(query_time, 
								 query_time + dt);
      
      list<PEDR_Shot>::iterator iter;
      for (iter = mola_shots.begin(); iter != mola_shots.end(); iter++) {
	PEDR_Shot shot = *iter;
	printf("Orbit: %d  Time: %10.10f  Coord: [%f, %f, %f]  Areoid: %f  Alt: %f\n",
	       shot.orbit_reference_nmuber(),
	       shot.ephemeris_time(),
	       shot.areo_latitude(),
	       shot.areo_longitude(),
	       shot.shot_planetary_radius(),
	       shot.areoid_radius(),
	       shot.shot_planetary_radius() -
	       shot.areoid_radius());
      }
      
    } catch (MOLA_PEDR_Err &e) {
      printf("Could not find MOLA database file... Exiting.\n");
      exit(1);
    }   
    
  }


  void testReadPEDRByTime() {

    printf("\n\n");
    string database_dir = "/irg/data/mgs/mgs-m-mola-3-pedr-l1a-v1/mgsl_21xx/data";
    double query_time = -20723097.688171;   // Epoch for M01-00115
    double dt = 7.034798;                   // Scan duration for M01-00115
    unsigned orbit = 717;                  // Orbit number for M01-00115

    try {
      MOLA_PEDR_Reader reader(database_dir, orbit);

      PEDR_Shot shot = reader.get_pedr_by_time(query_time, dt);
     
      printf("Orbit: %d  Time: %10.10f  Coord: [%f, %f, %f]  Areoid: %f  Alt: %f\n",
	     shot.orbit_reference_nmuber(),
	     shot.ephemeris_time(),
	     shot.areo_latitude(),
	     shot.areo_longitude(),
	     shot.shot_planetary_radius(),
	     shot.areoid_radius(),
	     shot.shot_planetary_radius() -
	     shot.areoid_radius());

    } catch (MOLA_PEDR_NotFound_Err &e) {
      printf("No matching entry found in the MOLA database... Exiting.\n");
    } catch (MOLA_PEDR_Err &e) {
      printf("\n\nAn error occured:\n");
      e.print();
      exit(1);
    }  
  }

  
  void testReadPEDRByLatLon() {

    printf("\n\n");
    string database_dir = "/irg/data/mgs/mgs-m-mola-3-pedr-l1a-v1/mgsl_21xx/data";
    unsigned orbit = 717;                  // Orbit number for M01-00115

    float north = 34.466537;
    float south = 34.114227;
    float east = 141.575714;
    float west = 141.521225;

    try {
      MOLA_PEDR_Reader reader(database_dir, orbit);

      list<PEDR_Shot> mola_shots = reader.get_pedr_by_areo_latlon(north, 
								  south, 
								  east, 
								  west);
      
      list<PEDR_Shot>::iterator iter;
      for (iter = mola_shots.begin(); iter != mola_shots.end(); iter++) {
	PEDR_Shot shot = *iter;
	printf("Orbit: %d  Time: %10.10f  Coord: [%f, %f, %f]  Areoid: %f  Alt: %f\n",
	       shot.orbit_reference_nmuber(),
	       shot.ephemeris_time(),
	       shot.areo_latitude(),
	       shot.areo_longitude(),
	       shot.shot_planetary_radius(),
	       shot.areoid_radius(),
	       shot.shot_planetary_radius() -
	       shot.areoid_radius());
	

      }

    } catch (MOLA_PEDR_NotFound_Err &e) {
      printf("No matching entry found in the MOLA database... Exiting.\n");
    } catch (MOLA_PEDR_Err &e) {
      printf("\n\nAn error occured:\n");
      e.print();
      exit(1);
    }  
  }

};

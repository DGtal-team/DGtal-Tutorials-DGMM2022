#include <iostream>
#include <vector>
#include <array>
#include <utility>

#include <DGtal/base/Common.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/helpers/Shortcuts.h>
#include <DGtal/helpers/ShortcutsGeometry.h>

#include <DGtal/images/SimpleThresholdForegroundPredicate.h>
#include <DGtal/geometry/volumes/distance/DistanceTransformation.h>
#include <DGtal/geometry/volumes/distance/VoronoiMap.h>
#include <DGtal/geometry/volumes/distance/ReducedMedialAxis.h>

#include "polyscope/polyscope.h"
#include "polyscope/point_cloud.h"
#include "polyscope/surface_mesh.h"

#include "CLI11.hpp"


using namespace DGtal;
using namespace Z3i;

// Using standard 3D digital space.
typedef Shortcuts<Z3i::KSpace>         SH3;
typedef ShortcutsGeometry<Z3i::KSpace> SHG3;



void myCallback()
{
  
  
  
}

int main(int argc, char **argv)
{
  polyscope::init();
  
  CLI::App app{"DT demo"};
  std::string filename;
  app.add_option("-i,--input,1", filename, "Input VOL file")->required()->check(CLI::ExistingFile);
  CLI11_PARSE(app,argc,argv);

  auto params = SH3::defaultParameters() | SHG3::defaultParameters() |  SHG3::parametersGeometryEstimation();
  params("surfaceComponents", "All");
  
  binary_image = SH3::makeBinaryImage(filename, params );
  auto K            = SH3::getKSpace( binary_image );
  auto surface      = SH3::makeDigitalSurface( binary_image, K, params );
 
  // Read voxel object and hands surfaces to polyscope
  auto params = SH3::defaultParameters()| SHG3::defaultParameters() | SHG3::parametersGeometryEstimation();
  binary_image = SH3::makeBinaryImage(filename, params );
  

  //Preparing predicates
  Predicate binaryshape(*binary_image, 0);
  functors::NotPointPredicate<Predicate> negpred(binaryshape);
  voronoiMap = new VoroMap(binary_image->domain(), binaryshape, Z3i::l2Metric);
  voronoiMapOutside = new VoroMapOutside(binary_image->domain(), negpred, Z3i::l2Metric);
  
  polyscope::state::userCallback = myCallback;
  polyscope::show();
  return 0;
}

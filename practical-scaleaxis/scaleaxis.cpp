#include <iostream>
#include <vector>
#include <array>
#include <utility>

#include <DGtal/base/Common.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/helpers/Shortcuts.h>
#include <DGtal/helpers/ShortcutsGeometry.h>
#include <DGtal/shapes/SurfaceMesh.h>

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
typedef SurfaceMesh< Z3i::RealPoint, Z3i::RealVector >         SurfMesh;

//
CountedPtr< SH3::BinaryImage > binary_image;

float scaleAxis=2.0;

void computeRDMA()
{
  
}

void computeScaleAxis()
{
  
}

void myCallback()
{
  if (ImGui::Button("Compute reduced medial axis"))
    computeRDMA();
  
  ImGui::SliderFloat("Scale axis parameter", &scaleAxis, 1.0, 10.0);
  
  if (ImGui::Button("COmpute scale axis"))
    computeScaleAxis();
  
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
  auto primalSurface = SH3::makePrimalSurfaceMesh(surface);
  
  //For the visualization of the digital surface.
  std::vector<std::vector<size_t>> faces;
  std::vector<RealPoint> positions;
  for(size_t face= 0 ; face < primalSurface->nbFaces(); ++face)
    faces.push_back(primalSurface->incidentVertices( face ));
  positions = primalSurface->positions();
  auto surfmesh = SurfMesh(positions.begin(),
                           positions.end(),
                           faces.begin(),
                           faces.end());
  
  
  
  polyscope::state::userCallback = myCallback;
  polyscope::show();
  return 0;
}

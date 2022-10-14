#include <iostream>
#include <vector>
#include <array>
#include <utility>

#include "CLI11.hpp"

#include <DGtal/base/Common.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/helpers/Shortcuts.h>
#include <DGtal/helpers/ShortcutsGeometry.h>
#include <DGtal/shapes/SurfaceMesh.h>
#include <DGtal/topology/NeighborhoodConfigurations.h>
#include <DGtal/topology/tables/NeighborhoodTables.h>


#include "polyscope/polyscope.h"
#include "polyscope/point_cloud.h"
#include "polyscope/surface_mesh.h"


using namespace DGtal;
using namespace Z3i;

// Using standard 3D digital space.
typedef Shortcuts<Z3i::KSpace>         SH3;
typedef ShortcutsGeometry<Z3i::KSpace> SHG3;


CountedPtr< SH3::BinaryImage > binary_image;
CountedPtr< Z3i::Object26_6 >  the_object;

/// Register to polyscope the boundary surfels of a given binary image
/// \a bimage.
void registerDigitalSurface( CountedPtr< SH3::BinaryImage > bimage,
                             std::string name )
{
  typedef SurfaceMesh< Z3i::RealPoint, Z3i::RealVector >         SurfMesh;
  auto params = SH3::defaultParameters() | SHG3::defaultParameters() |  SHG3::parametersGeometryEstimation();
  auto h=1.; //gridstep
  params( "closed", 1)("surfaceComponents", "AnyBig");
  auto K            = SH3::getKSpace( bimage );
  auto surface      = SH3::makeDigitalSurface( bimage, K, params );
  auto primalSurface   = SH3::makePrimalSurfaceMesh(surface);

  std::vector<std::vector<size_t>> faces;
  std::vector<RealPoint> positions;
  //Need to convert the faces
  for(size_t face= 0 ; face < primalSurface->nbFaces(); ++face)
    faces.push_back(primalSurface->incidentVertices( face ));
  //Recasting to vector of vertices
  positions = primalSurface->positions();
  auto surfmesh = SurfMesh(positions.begin(),
                           positions.end(),
                           faces.begin(),
                           faces.end());
  
  auto primalSurf = polyscope::registerSurfaceMesh( name, positions, faces);
}

// Removes a peel of simple points onto voxel object.
bool oneStep( CountedPtr< Z3i::Object26_6 > object )
{
  int nb_simple ; //Number of simple points of hte object

  /**
   *
   *
   *  Remove all simple points of \a object
   *  Updat the corresponding \a binary_image
  *
   *
   *
   *
   */
  
  registerDigitalSurface( binary_image, "Thinned object" );
  return nb_simple == 0;
}

// Polyscope GUI Callback
void mycallback()
{
  if (ImGui::Button("Run"))
    {
      oneStep( the_object );
    }
  if (ImGui::Button("Screenshots"))
    {
      bool finished = false;
      while ( ! finished )
        {
          finished = oneStep( the_object );
          polyscope::screenshot();
          polyscope::refresh();
        }
    }
}

// main program
int main( int argc, char* argv[] )
{
  polyscope::init();

  CLI::App app{"Homotopic Thinning demo"};
  std::string filename;
  app.add_option("-i,--input,1", filename, "Input VOL file")->required()->check(CLI::ExistingFile);
  CLI11_PARSE(app,argc,argv);

  // Read voxel object and hands surfaces to polyscope
  auto params = SH3::defaultParameters()| SHG3::defaultParameters() | SHG3::parametersGeometryEstimation();
  binary_image = SH3::makeBinaryImage(filename, params );
  
  //Visualization
  registerDigitalSurface( binary_image, "Primal surface" );  
  registerDigitalSurface( binary_image, "Thinned object" );

  
  // Build object with digital topology
  const auto K = SH3::getKSpace( binary_image );
  Domain domain( K.lowerBound(), K.upperBound() );
  Z3i::DigitalSet voxel_set( domain );
  for ( auto p : domain )
    if ( (*binary_image)( p ) ) voxel_set.insertNew( p );
  the_object = CountedPtr< Z3i::Object26_6 >( new Z3i::Object26_6( dt26_6, voxel_set ) );
  the_object->setTable(functions::loadTable<3>(simplicity::tableSimple26_6));

  // Give the hand to polyscope
  polyscope::state::userCallback = mycallback;
  polyscope::show();
  return 0;
}

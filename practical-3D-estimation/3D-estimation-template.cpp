#include <iostream>
#include <DGtal/base/Common.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/helpers/Shortcuts.h>
#include <DGtal/helpers/ShortcutsGeometry.h>

#include <polyscope/polyscope.h>
#include <polyscope/surface_mesh.h>


using namespace DGtal;
using namespace Z3i;

// Using standard 3D digital space.
typedef Shortcuts<KSpace>              SH3;
typedef ShortcutsGeometry<KSpace>      SHG3;
typedef SurfaceMesh< RealPoint, RealVector >  SurfMesh;
typedef SurfMesh::Face                 Face;
typedef SurfMesh::Vertex               Vertex;
typedef SurfMesh::Index                Index;

// Global variables to make easier GUI stuff.
polyscope::SurfaceMesh *psMesh;
SurfMesh surfmesh;
float    GridStep = 0.5;

/// Create an implicit shape \a polynomial digitized at gridstep \a h
/// @param polynomial the implicit function as a  multivariate polynomial string.
/// @param h the chosen digitization gridstep
void createShape( std::string polynomial, double h )
{
  auto params = SH3::defaultParameters() | SHG3::defaultParameters() |  SHG3::parametersGeometryEstimation();
  params("surfaceComponents", "All");
  params("polynomial", polynomial )
    ("minAABB",-10.0)("maxAABB",10.0)("offset",1.0)
    ("gridstep", h );
  auto shape        = SH3::makeImplicitShape3D( params );
  auto dshape       = SH3::makeDigitizedImplicitShape3D( shape, params );
  auto K            = SH3::getKSpace( params );
  auto binary_image = SH3::makeBinaryImage( dshape, params );
  auto surface      = SH3::makeDigitalSurface( binary_image, K, params );
  auto primalSurface= SH3::makePrimalSurfaceMesh(surface);
  auto surfels      = SH3::getSurfelRange( surface, params );
  auto true_normals = SHG3::getNormalVectors( shape, K, surfels, params );
  
  // Need to convert the faces
  std::vector<std::vector<SH3::SurfaceMesh::Vertex>> faces;
  std::vector<RealPoint> positions;
  std::vector<RealPoint> smooth_positions;
  
  for(auto face= 0 ; face < primalSurface->nbFaces(); ++face)
    faces.push_back(primalSurface->incidentVertices( face ));
  
  // Embed lattice points according to gridstep.
  positions = primalSurface->positions();
  for ( auto& x : positions ) x *= h;

  // Create DGtal surface mesh object.
  surfmesh = SurfMesh(positions.begin(), positions.end(),
                      faces.begin(), faces.end());
  std::cout << surfmesh << std::endl;
  std::cout << "number of non-manifold Edges = "
            << surfmesh.computeNonManifoldEdges().size() << std::endl;
  // Create rendered polyscope surface.
  psMesh = polyscope::registerSurfaceMesh("digital surface", positions, faces);
  psMesh->addFaceVectorQuantity( "True normal vector field", true_normals );
}

/// Defines the GUI buttons and reactions.
void myCallback()
{
  if(ImGui::Button("Sphere")) createShape( "sphere9", GridStep );
  ImGui::SameLine();
  if(ImGui::Button("Torus")) createShape( "torus", GridStep );
  ImGui::SameLine();
  if(ImGui::Button("Cylinder")) createShape( "x^2-2*x*y+y^2+z^2-25", GridStep );
  ImGui::SliderFloat("Gridstep h parameter", &GridStep, 0.025, 2.0);
}

int main()
{
  // Gives you the list of predefined shapes
  auto L = SH3::getPolynomialList();
  for ( const auto& e : L )
    std::cout << e.first << " : " << e.second << std::endl;

  // Initialize polyscope
  polyscope::init();

  // Create shape
  createShape( "sphere9", GridStep );

  // Set the callback function
  polyscope::state::userCallback = myCallback;
  polyscope::show();
  return EXIT_SUCCESS;
}

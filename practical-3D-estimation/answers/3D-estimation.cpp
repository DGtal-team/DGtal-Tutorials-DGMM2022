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
polyscope::SurfaceMesh *psSmoothMesh;
SurfMesh surfmesh;
float    GridStep = 0.5;
float    ErrorLoo = 0.0;
float    ErrorL2  = 0.0;
int      Estimator= 0;
double   Area0    = 0.0;
double   Area1    = 0.0;
double   EstArea0 = 0.0;
double   EstArea1 = 0.0;
double   Reach    = 9.0;

/// Create an implicit shape \a polynomial digitized at gridstep \a h
/// @param polynomial the implicit function as a  multivariate polynomial string.
/// @param h the chosen digitization gridstep
void createShape( std::string polynomial, double h, double reach )
{
  auto params = SH3::defaultParameters() | SHG3::defaultParameters() |  SHG3::parametersGeometryEstimation();
  params("surfaceComponents", "All");
  params("polynomial", polynomial )
    ("minAABB",-10.0)("maxAABB",10.0)("offset",1.0)
    ("gridstep", h );
  Reach = reach;
  auto shape        = SH3::makeImplicitShape3D( params );
  auto dshape       = SH3::makeDigitizedImplicitShape3D( shape, params );
  auto K            = SH3::getKSpace( params );
  auto binary_image = SH3::makeBinaryImage( dshape, params );
  auto surface      = SH3::makeDigitalSurface( binary_image, K, params );
  auto primalSurface= SH3::makePrimalSurfaceMesh(surface);
  auto surfels      = SH3::getSurfelRange( surface, params );
  auto true_normals    = SHG3::getNormalVectors( shape, K, surfels, params );
  auto trivial_normals = SHG3::getTrivialNormalVectors( K, surfels );
  auto normals         =
    Estimator == 0 ? trivial_normals :
    Estimator == 1 ? SHG3::getCTrivialNormalVectors( surface, surfels, params )
    : SHG3::getIINormalVectors( binary_image, surfels, params );
  
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
  psMesh->addFaceVectorQuantity( "Estimated normal vector field", normals );
  psMesh->addFaceVectorQuantity( "True normal vector field", true_normals );

  // Compute errors
  auto angle_diff  = SHG3::getVectorsAngleDeviation( true_normals, normals );
  auto stat_angles = SHG3::getStatistic( angle_diff );
  ErrorLoo = stat_angles.max();
  ErrorL2  = sqrt( stat_angles.variance() );

  // View errors
  psMesh->addFaceScalarQuantity( "Angle error", angle_diff )
    ->setMapRange( { 0.0, M_PI / 20.0 } ) // 10° is bad !
    ->setColorMap( "coolwarm" );

  // Compute area (method Euler integration)
  Area0    = 0.0;
  Area1    = 0.0;
  EstArea0 = 0.0;
  EstArea1 = 0.0;
  auto L1  = normals[ 0 ].L_1;
  for ( Face i = 0; i < surfmesh.nbFaces(); i++ )
    {
      Area0    += trivial_normals[ i ].dot( true_normals[ i ] );
      EstArea0 += trivial_normals[ i ].dot( normals[ i ] );
      Area1    += 1.0 / true_normals[ i ].norm( L1 );
      EstArea1 += 1.0 / normals[ i ].norm( L1 );
    }
  Area0    *= GridStep * GridStep;
  Area1    *= GridStep * GridStep;
  EstArea0 *= GridStep * GridStep;
  EstArea1 *= GridStep * GridStep;


  // Create smooth surface
  auto ppositions = SHG3::getPositions( shape, positions, params );
  psSmoothMesh    = polyscope::registerSurfaceMesh("smooth surface", ppositions, faces);
  
  // Compute curvatures and estimate reach.
  auto curvatures = SHG3::getPrincipalCurvaturesAndDirections( shape, K, surfels, params );
  auto all_K      = angle_diff; // will store the maximal curvatures
  double max_K    = 0.0; 
  for ( Face i = 0; i < surfmesh.nbFaces(); i++ )  
    {
      const double k1 = std::get<0>( curvatures[ i ] );
      const double k2 = std::get<1>( curvatures[ i ] );
      all_K[ i ] = std::max( fabs( k1 ), fabs( k2 ) );
      max_K      = std::max( max_K, all_K[ i ] );
    }
  psSmoothMesh->addFaceScalarQuantity( "Max curvatures", all_K )
    ->setMapRange( { 0.0, max_K } ) 
    ->setColorMap( "coolwarm" );
  // Estimate reach
  Reach = 1.0 / max_K;
  
  // Compute manifoldness
  auto M = angle_diff;
  auto Loo = true_normals[ 0 ].L_infty;
  for ( Face i = 0; i < surfmesh.nbFaces(); i++ )
    {
      double a = acos( std::min( 1.0, true_normals[ i ].norm( Loo ) ) );
      M[ i ] = a > ( 1.260 * GridStep / Reach ) ? 0.0 : 2.0;
    }
  // Compute bijectiveness
  for ( Face i = 0; i < surfmesh.nbFaces(); i++ )
    {
      auto   n = true_normals[ i ];
      auto   c = std::min( fabs( n[ 0 ] ), std::min( fabs( n[ 1 ] ), fabs( n[ 2 ] ) ) );
      bool   b = c > ( 2.0 * sqrt( 3.0 ) * GridStep / Reach );
      if ( ( ! b ) && ( M[ i ] < 0.5 ) ) M[ i ] = 1.0;
    }
  psMesh->addFaceScalarQuantity( "Manifoldness / Bijectivity", M );
  psSmoothMesh->addFaceScalarQuantity( "Manifoldness / Bijectivity", M );
}

/// Defines the GUI buttons and reactions.
void myCallback()
{
  if(ImGui::Button("Sphere")) createShape( "sphere9", GridStep, 9.0 );
  ImGui::SameLine();
  if(ImGui::Button("Torus")) createShape( "torus", GridStep, 2.0 );
  ImGui::SameLine();
  if(ImGui::Button("Goursat")) createShape( "goursat", GridStep, 2.0 );
  if(ImGui::Button("Leopold")) createShape( "leopold", GridStep, 0.5 );
  ImGui::SameLine();
  if(ImGui::Button("Goursat-H")) createShape( "goursat-hole", GridStep, 1.0 );
  ImGui::SameLine();
  if(ImGui::Button("Cylinder")) createShape( "x^2-2*x*y+y^2+z^2-25", GridStep, 5.0 );
  ImGui::Text( "Reach is at most %f", Reach );
  ImGui::SliderFloat("Gridstep h parameter", &GridStep, 0.025, 2.0);
  ImGui::Text( "Normal estimator: " );          ImGui::SameLine();
  ImGui::RadioButton("Trivial",  &Estimator, 0); ImGui::SameLine();
  ImGui::RadioButton("CTrivial", &Estimator, 1); ImGui::SameLine();
  ImGui::RadioButton("II",       &Estimator, 2);
  ImGui::Text( "Normal error loo=%f   l2=%f", ErrorLoo, ErrorL2 );
  // If you wish to compare with the exact phere9 true area:
  // double target_area = 4.0 * M_PI * 9.0 * 9.0;
  ImGui::Text( "Expected area0=%f area1=%f", Area0, Area1 );
  ImGui::Text( "Computed area0=%f area1=%f", EstArea0, EstArea1 );
  // ImGui::Text( "Error Expected area0=%f%% area1=%f%%",
  //              100.0 * fabs( Area0 - target_area ) / target_area,
  //              100.0 * fabs( Area1 - target_area ) / target_area );
  double target_area = ( Area0 + Area1 ) / 2.0;
  ImGui::Text( "Error Computed area0=%f%% area1=%f%%",
               100.0 * fabs( EstArea0 - target_area ) / target_area,
               100.0 * fabs( EstArea1 - target_area ) / target_area );
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
  createShape( "sphere9", GridStep, 9.0 );

  // Set the callback function
  polyscope::state::userCallback = myCallback;
  polyscope::show();
  return EXIT_SUCCESS;
}

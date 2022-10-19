#include "DGtal/base/Common.h"
#include "DGtal/helpers/StdDefs.h"

#include "DGtal/shapes/Shapes.h"
#include "DGtal/shapes/ShapeFactory.h"
#include "DGtal/shapes/GaussDigitizer.h"
#include "DGtal/geometry/curves/GridCurve.h"
#include "DGtal/io/boards/Board2D.h"

//normal/curvature
#include "DGtal/geometry/curves/estimation/MostCenteredMaximalSegmentEstimator.h"
#include "DGtal/geometry/curves/StabbingCircleComputer.h"

using namespace DGtal;

//------------------------------------------------------------------------------
template <typename TShape, typename TKSpace>
void fillShapeBoundary( const TShape& aShape,
			double aH,
			TKSpace& aKSpace,
			std::vector<typename TKSpace::Point>& aVector) { 
  
  // Creates a digitizer
  GaussDigitizer<typename TKSpace::Space,TShape> dig;
  dig.attach( aShape ); // attaches the shape and init: 
  dig.init( aShape.getLowerBound(), aShape.getUpperBound(), aH ); 

  // Creates a Khalimsky space
  if (! aKSpace.init( dig.getLowerBound(), dig.getUpperBound(), true ) )
    throw std::runtime_error("Error in creating KSpace");
   
  // Extracts shape boundary
  SurfelAdjacency<TKSpace::dimension> sAdj( true );
  typename TKSpace::SCell bel
    = Surfaces<TKSpace>::findABel( aKSpace, dig, 10000 );
  // Getting the consecutive surfels of the 2D boundary
  Surfaces<TKSpace>::track2DBoundaryPoints( aVector, aKSpace, sAdj, dig, bel );
}

//------------------------------------------------------------------------------
template <typename InputIterator, typename Functor, typename OutputIterator> 
void getEstimates(const InputIterator& itb,
		  const InputIterator& ite,
		  const Functor& aF,
		  OutputIterator out,
		  const double& aH) {

  // create a segment computer, i.e., a way of computing a geometric primitive
  using SegmentComputer = StabbingCircleComputer<InputIterator>;
  SegmentComputer sc;

  // create an estimator from the segment computer and the functor
  using Estimator = MostCenteredMaximalSegmentEstimator<SegmentComputer,Functor>;
  Estimator estimator(sc, aF);
  estimator.init( itb, ite ); 
  estimator.eval( itb, ite, out, aH );
}

//------------------------------------------------------------------------------
int main( int argc, char** argv )
{

  using namespace Z2i;

  //----------------------------------------------------------------------------
  trace.beginBlock ( "Digitization" );
  
  double h = 0.1; 
  if (argc >= 2) h = std::atof(argv[1]);
  trace.info() << "Grid step = " << h << std::endl; 

  // shape
  Ellipse2D<Space> shape( 0.5, 0.5, 5.0, 3.0, 0.3 );
  //or Flower2D<Space> shape( 0.5, 0.5, 5.0, 3.0, 5, 0.3 );
  //or whatever shape you like...
  
  // Khalimsky space
  K2 kspace;
  
  // boundary points
  std::vector<Point> points;
  // discretize the shape and track the boundary 
  fillShapeBoundary(shape, h, kspace, points); 
   
  trace.endBlock();
  
  //----------------------------------------------------------------------------
  trace.beginBlock ( "Gridcurve and Board2D" );

  // create a grid curve from the points
  Curve gridcurve( kspace );
  gridcurve.initFromVector( points );
  if (gridcurve.isOpen()) {
    std::cerr << "GridCurve is expected to be closed" << std::endl;
    exit(EXIT_FAILURE); 
  }
  // create a board
  Board2D board;  
  // draw the grid curve onto the board
  board << SetMode( "PointVector", "Grid" )
	<< gridcurve;
  // save the drawing
  board.saveSVG("gridcurve.svg");

  trace.endBlock();
  
  //----------------------------------------------------------------------------
  trace.beginBlock ( "Gridcurve -> normal/curvature/measure" );

  // create a range of incident points
  using Range = Curve::IncidentPointsRange;
  Range range = gridcurve.getIncidentPointsRange();

  // we will use circulators because the gridcuve is closed
  using Iterator = Range::ConstCirculator;
  
  // Curvature estimates 
  std::vector<double> curvatures;
  using CFunctor = CurvatureFromDCAEstimator<StabbingCircleComputer<Iterator>,false>;
  CFunctor cf;
  getEstimates(range.c(), range.c(), cf, std::back_inserter(curvatures), h);

  // Normal estimates 
  std::vector<RealVector> normalVectors;
  using NFunctor = NormalFromDCAEstimator<StabbingCircleComputer<Iterator> >;
  NFunctor nf;
  getEstimates(range.c(), range.c(), nf, std::back_inserter(normalVectors), h);

  // Measure estimates
  std::vector<double> measures;
  unsigned int idx = 0;
  for ( auto it = range.begin(); it != range.end(); ++it, ++idx ) {
    Vector trivialNormal = it->first - it->second;
    measures.push_back( abs(trivialNormal.dot(normalVectors[idx])) ); 
  }
  
  // print to standard output
  std::cout << "# idx nx ny curv len" << std::endl;
  int n = gridcurve.size(); 
  for ( int i = 0; i < n; i++ ) {
    std::cout << i << " "
	      << normalVectors[i][0] << " "
	      << normalVectors[i][1] << " "
	      << curvatures[i] << " "
	      << measures[i]
	      << std::endl;
  }
  std::cout << std::endl;    
  // to get the curvature plot with gnuplot: 
  // - export the data: ./your-exe > data
  // - plot them with gnuplot:  plot "data" using 1:4 with lines
 
  trace.endBlock();

  //----------------------------------------------------------------------------
  trace.beginBlock ( "Integral of curvature" );

  double totalCurvature = 0.0; 
  for ( int i = 0; i < n; i++ ) {
    totalCurvature += h*measures[i]*curvatures[i]; 
  }
  double error = std::abs(totalCurvature - 2*M_PI); 
  trace.info() << "Total curvature = "
	       << totalCurvature
	       << std::endl; 
  trace.info() << "Error = "
	       << error
	       << std::endl; 
  
  trace.endBlock();  
  
  return EXIT_SUCCESS; 
}

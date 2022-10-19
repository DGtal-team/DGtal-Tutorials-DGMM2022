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


  trace.endBlock();
  
  //----------------------------------------------------------------------------
  trace.beginBlock ( "Gridcurve -> normal/curvature/measure" );

 
  trace.endBlock();

  //----------------------------------------------------------------------------
  trace.beginBlock ( "Integral of curvature" );

  trace.endBlock();
  
  return EXIT_SUCCESS; 
}

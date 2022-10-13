#include <iostream>
#include <DGtal/base/Common.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/boards/Board2D.h>
#include <DGtal/geometry/curves/ArithmeticalDSS.h>

using namespace DGtal;

int main()
{
  Z2i::Point a(0,0);
  Z2i::Point b(33,43);
  
  Z2i::Domain domain(a,b);
  
  NaiveDSS8<Z2i::Integer> dss(a,b);
  
  Board2D board;
  board << domain;
  board << dss;
  
  board.saveSVG("simple-domain.svg");
  return 0;
}

#include <iostream>
#include <DGtal/base/Common.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/boards/Board2D.h>

using namespace DGtal;

int main()
{
  Z2i::Point a(0,0);
  Z2i::Point b(10,10);
  Z2i::Point c(4,7);
  Z2i::Domain domain(a,b);
  
  Board2D board;
  board << domain;
  board << a << b << c;
  
  board.saveSVG("simple-domain.svg");
  return 0;
}

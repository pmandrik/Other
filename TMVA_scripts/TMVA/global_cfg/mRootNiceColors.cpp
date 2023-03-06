
#ifndef mRootNiceColors_
#define mRootNiceColors_ 1

#include <vector>
#include <ostream>
#include "TColor.h"

namespace mRoot {
  inline Color_t FindFreeCustomColorIndex(Color_t start = 1000){
    for (Color_t i = std::max(start, Color_t(1000)), e = 10000; i < e; ++i)
      if (!gROOT->GetColor(i)) return i;
    return -1;
  }

  class FakeTColor : public TColor{
    public : 
      FakeTColor(double r, double g, double b) : TColor(FindFreeCustomColorIndex(), r/255., g/255., b/255.){}
      int GetIndex(){
        return this->fNumber;
      }
  };

  std::vector <int> getNiceColors(int values = 25){
    std::vector <FakeTColor*> colors = { new FakeTColor( 75., 122., 192. ),
                                         new FakeTColor( 241., 72., 75. ),
                                         new FakeTColor( 31., 140., 61.  ),
                                         new FakeTColor( 233., 90.,  154.  ),
                                         new FakeTColor( 241., 121., 41. ),
                                         new FakeTColor( 170., 143., 104. ),
                                         new FakeTColor( 180., 212., 79. ),
                                         new FakeTColor( 169., 169., 169. ),
                                         new FakeTColor( 34.,  148., 232. ),
                                         new FakeTColor( 253., 205.,  74. ),
                                         new FakeTColor( 81., 179., 157. ),
                                         new FakeTColor( 107., 76., 154. ),
                                         new FakeTColor( 255., 15., 15. ),
                                         new FakeTColor( 15., 255., 15. ),
                                         new FakeTColor( 15., 15., 255. ),
                                         new FakeTColor( 255.,  15., 255. ),
                                         new FakeTColor( 255., 255., 15.  ),
                                         new FakeTColor( 15.,  255., 255. ),
                                         new FakeTColor( 242., 203., 110. ),
                                         new FakeTColor( 220., 119., 31.  ),
                                         new FakeTColor( 148., 58.,  48.  ),
                                         new FakeTColor( 174., 124., 145. ),
                                         new FakeTColor( 105., 175., 132. ),
                                         new FakeTColor( 137., 148., 242. ),
                                         new FakeTColor( 56.,  114., 221. ),
                                         new FakeTColor( 31.,  147., 138. ),
                                         new FakeTColor( 124., 32.,  148. ),
                                         new FakeTColor( 125., 105., 174. ),
                                         new FakeTColor( 222., 56.,  135. ),
                                         new FakeTColor( 174., 174., 174. ),
                                         new FakeTColor( 125., 147.,  77. ),
                                         new FakeTColor( 211., 242., 220. ),
                                         new FakeTColor( 123., 220., 127. ) };
    
    std::vector <int> colors_ids;
    for(auto color : colors){
      colors_ids.push_back( color->GetIndex() );
    }

    values -= colors_ids.size();
    for(int i = 0; i < values; i++){
      colors_ids.push_back( i+1 );
    }
    return colors_ids;
  }

  void PrintNiceColors(){
    std::vector <int> colors = mRoot::getNiceColors();
    for(auto color : colors) std::cout << color << std::endl;
  }
};

#endif



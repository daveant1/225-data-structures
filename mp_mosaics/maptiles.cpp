/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>
#include "maptiles.h"
//#include "cs225/RGB_HSL.h"

using namespace std;


Point<3> convertToXYZ(LUVAPixel pixel) {
    return Point<3>( pixel.l, pixel.u, pixel.v );
}

MosaicCanvas* mapTiles(SourceImage const& theSource,
                       vector<TileImage>& theTiles)
{
    /**
     * @todo Implement this function!
     */
     //generate tilemap for TileImage lookup, and vector of treepoints, and  and points
     map<Point<3>, TileImage*> tilemap;
     vector<Point<3>> treepoints;
     for(size_t i =0; i < theTiles.size(); ++i){
       treepoints.push_back(convertToXYZ(theTiles[i].getAverageColor()));
       tilemap[treepoints[i]] = &theTiles[i];
     }
     KDTree<3> tiletree(treepoints);

     //Generate Mosaic
     int rows = theSource.getRows();
     int cols = theSource.getColumns();
     MosaicCanvas* canvas = new MosaicCanvas(rows, cols);
     for(int i = 0; i<rows; ++i){
       for (int j = 0; j<cols; ++j){
         Point<3> region = convertToXYZ(theSource.getRegionColor(i,j));
         Point<3> neighbor = tiletree.findNearestNeighbor(region);
         TileImage* im_ptr = tilemap[neighbor];
         canvas->setTile(i, j, im_ptr);
       }
     }
     return canvas;
}

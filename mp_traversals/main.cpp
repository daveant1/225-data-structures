
#include "cs225/PNG.h"
#include "FloodFilledImage.h"
#include "Animation.h"

#include "imageTraversal/DFS.h"
#include "imageTraversal/BFS.h"

#include "colorPicker/RainbowColorPicker.h"
#include "colorPicker/GradientColorPicker.h"
#include "colorPicker/GridColorPicker.h"
#include "colorPicker/SolidColorPicker.h"
#include "colorPicker/MyColorPicker.h"

using namespace cs225;

int main() {

  // @todo [Part 3]
  // - The code below assumes you have an Animation called `animation`
  // - The code provided below produces the `myFloodFill.png` file you must
  //   submit Part 3 of this assignment -- uncomment it when you're ready.
  // HSLAPixel pix1(11, 1.0, 0.5);
  // HSLAPixel pix2(216, 1.0, 0.5);
  MyColorPicker cp1;
  RainbowColorPicker cp2(0.05);
  PNG png_;
  png_.readFromFile("GCE.png");


  BFS bfs(png_, Point(0,0), 0.6);
  DFS dfs(png_, Point(107, 143), 0.3);

  FloodFilledImage image(png_);
  image.addFloodFill(bfs, cp2);
  image.addFloodFill(dfs, cp1);

  Animation animation = image.animate(1000);

  PNG lastFrame = animation.getFrame( animation.frameCount() - 1 );
  lastFrame.writeToFile("myFloodFill.png");
  animation.write("myFloodFill.gif");



  return 0;
}

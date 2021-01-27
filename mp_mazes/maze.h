#pragma once

#include <vector>
#include <cstdlib>
#include "dsets.h"
#include "cs225/PNG.h"
using namespace std;
using namespace cs225;

class SquareMaze{
public:
  SquareMaze();
  void makeMaze(int width, int height);
  bool 	canTravel(int x, int y, int dir) const;
  void 	setWall(int x, int y, int dir, bool exists);
  vector<int> solveMaze();
  PNG* drawMaze() const;
  PNG* drawMazeWithSolution();
private:
  int w_;
  int h_;

  class cell{
  public:
    bool right;
    bool down;
    int idx;
  };
  vector<cell> maze_;
};

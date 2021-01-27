/* Your code here! */
#include "maze.h"
#include <vector>
#include <queue>
#include <unordered_map>
#include <cstdlib>
#include "cs225/HSLAPixel.h"
using cs225::HSLAPixel;
using namespace std;

SquareMaze::SquareMaze(){}


void SquareMaze::makeMaze(int width, int height){
  w_ = width;
  h_ = height;
  maze_.clear();
  cell c;
  c.right = true;
  c.down = true;
  for(int i = 0; i < (width*height); ++i){
    c.idx = i;
    maze_.push_back(c);
  }

  DisjointSets d;
  d.addelements(width*height);
  int num_cells = 1;

  while(num_cells < width*height){
    int dir = rand()%2;
    int x = rand()%w_;
    int y = rand()%h_;
    int cell = (y*w_)+x;
    if(!dir){
      if((x < w_-1) && (d.find(cell+1)!=d.find(cell))){
        setWall(x, y, 0, false);
        d.setunion(cell+1, cell);
        num_cells+=1;
      }
    }else{
      if((y < h_-1) && (d.find(cell+w_)!=d.find(cell))){
        setWall(x, y, 1, false);
        d.setunion(cell+w_, cell);
        num_cells+=1;
      }
    }
  }
}


bool SquareMaze::canTravel(int x, int y, int dir) const{
  int idx = (y*w_)+x;
  if(dir==0 && x < w_-1){
    if(maze_[idx].right){
      return false;
    }else{
      return true;
    }
  }
  if(dir==1 && y < h_-1){
    if(maze_[idx].down){
      return false;
    }else{
      return true;
    }
  }
  if(dir==2 && x > 0){
    if(maze_[idx-1].right){
      return false;
    }else{
      return true;
    }
  }
  if(dir==3 && y > 0){
    if(maze_[idx-w_].down){
      return false;
    }else{
      return true;
    }
  }
  return false;
}


void SquareMaze::setWall(int x, int y, int dir, bool exists){
  int idx = (y*w_)+x;
  if(!dir){
    maze_[idx].right = exists;
  }else{
    maze_[idx].down = exists;
  }
}


vector<int> SquareMaze::solveMaze(){

  vector<bool> visited;
  visited.resize(w_*h_);
  for(int i = 0; i < w_*h_; ++i){
    visited[i] = false;
  }

  unordered_map<int, cell*> prev_cells;
  unordered_map<int, int> prev_dir;
  queue<cell> bfs;
  bfs.push(maze_[0]);
  visited[0] = true;

  while(!bfs.empty()){
    cell temp = bfs.front();
    int x = temp.idx % w_;
    int y = temp.idx / w_;
    bfs.pop();

    if(canTravel(x, y, 0) && visited[temp.idx+1]==false){
      visited[temp.idx+1] = true;
      prev_dir[temp.idx+1] = 0;
      prev_cells[temp.idx+1] = &maze_[temp.idx];
      bfs.push(maze_[temp.idx+1]);
    }

    if(canTravel(x, y, 1) && visited[temp.idx+w_]==false){
      visited[temp.idx+w_] = true;
      prev_dir[temp.idx+w_] = 1;
      prev_cells[temp.idx+w_] = &maze_[temp.idx];
      bfs.push(maze_[temp.idx+w_]);
    }

    if(canTravel(x, y, 2) && visited[temp.idx-1]==false){
      visited[temp.idx-1] = true;
      prev_dir[temp.idx-1] = 2;
      prev_cells[temp.idx-1] = &maze_[temp.idx];
      bfs.push(maze_[temp.idx-1]);
    }

    if(canTravel(x, y, 3) && visited[temp.idx-w_]==false){
      visited[temp.idx-w_] = true;
      prev_dir[temp.idx-w_] = 3;
      prev_cells[temp.idx-w_] = &maze_[temp.idx];
      bfs.push(maze_[temp.idx-w_]);
    }
  }

  vector<int> path;
  vector<int> longest;
  for(int i = 0; i < w_; ++i){
    cell* curr = &maze_[((h_-1)*w_)+i];
    path.clear();
    while(curr->idx != 0){
      path.insert(path.begin(), prev_dir[curr->idx]);
      curr = prev_cells[curr->idx];
    }
    if(path.size() > longest.size()){
      longest = path;
    }
  }
  return longest;
}


PNG* SquareMaze::drawMaze() const{
  PNG* p = new PNG(w_*10+1,h_*10+1);
  for(int i = 0; i < h_*10+1; ++i){
    p->getPixel(0,i) = HSLAPixel(0,0,0);
  }
  for(int i = 10; i < w_*10+1; ++i){
    p->getPixel(i,0) = HSLAPixel(0,0,0);
  }
  for(const cell& curr: maze_){
    int x = curr.idx % w_;
    int y = curr.idx / w_;
    if(curr.right){
      for(int k = 0; k <= 10; ++k){
        p->getPixel((x+1)*10, y*10+k) = HSLAPixel(0,0,0);
      }
    }
    if(curr.down){
      for(int k = 0; k <= 10; ++k){
        p->getPixel(x*10+k, (y+1)*10) = HSLAPixel(0,0,0);
      }
    }
  }
  return p;
}

PNG* SquareMaze::drawMazeWithSolution(){
  PNG* p = drawMaze();
  vector<int> solution = solveMaze();
  int curr_x = 5;
  int curr_y = 5;
  for(int& i: solution){
    if(i==0){
      for(int x = 0; x <= 10; ++x){
        p->getPixel(curr_x+x, curr_y) = HSLAPixel(0,1,0.5,1);
      }
      curr_x+=10;
    }else if(i==1){
      for(int y = 0; y <= 10; ++y){
        p->getPixel(curr_x, curr_y+y) = HSLAPixel(0,1,0.5,1);
      }
      curr_y+=10;
    }else if(i==2){
      for(int x = 0; x <= 10; ++x){
        p->getPixel(curr_x-x, curr_y) = HSLAPixel(0,1,0.5,1);
      }
      curr_x-=10;
    }else if(i==3){
      for(int y = 0; y <= 10; ++y){
        p->getPixel(curr_x, curr_y-y) = HSLAPixel(0,1,0.5,1);
      }
      curr_y-=10;
    }
  }
  for(int k = 1; k<=9; ++k){
    p->getPixel((curr_x/10)*10+k, h_*10) = HSLAPixel(0,0,1);
  }
  return p;
}

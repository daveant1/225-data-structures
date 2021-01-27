#include <cmath>
#include <iterator>
#include <iostream>
#include <vector>

#include "../cs225/HSLAPixel.h"
#include "../cs225/PNG.h"
#include "../Point.h"

#include "ImageTraversal.h"


/**
 * Calculates a metric for the difference between two pixels, used to
 * calculate if a pixel is within a tolerance.
 *
 * @param p1 First pixel
 * @param p2 Second pixel
 * @return the difference between two HSLAPixels
 */
double ImageTraversal::calculateDelta(const HSLAPixel & p1, const HSLAPixel & p2) {
  double h = fabs(p1.h - p2.h);
  double s = p1.s - p2.s;
  double l = p1.l - p2.l;

  // Handle the case where we found the bigger angle between two hues:
  if (h > 180) { h = 360 - h; }
  h /= 360;

  return sqrt( (h*h) + (s*s) + (l*l) );
}

/**
 * Default iterator constructor.
 */
ImageTraversal::Iterator::Iterator() {
  /** @todo [Part 1] */
  done_ = true;
  trav_ = NULL;
}

ImageTraversal::Iterator::Iterator(PNG & png, Point start, double tolerance, ImageTraversal* traversal): png_(png), start_(start), tolerance_(tolerance), trav_(traversal) {
  done_ = false;
  unsigned arr_size = png_.width() * png_.height();
  visited = std::vector<bool> (arr_size, false);
  curr_ = start_;
  if(canvisit(start)){
    visited[start_.x + start_.y * png_.width()] = true;
  }
  else{
    done_=true;
  }
}
/**
 * Iterator increment opreator.
 *
 * Advances the traversal of the image.
 */
ImageTraversal::Iterator & ImageTraversal::Iterator::operator++() {
  /** @todo [Part 1] */
  if(canvisit(Point(curr_.x+1, curr_.y))){
    trav_->add(Point(curr_.x+1, curr_.y));
  }
  if(canvisit(Point(curr_.x, curr_.y+1))){
    trav_->add(Point(curr_.x, curr_.y+1));
  }
  if(canvisit(Point(curr_.x-1, curr_.y))){
    trav_->add(Point(curr_.x-1, curr_.y));
  }
  if(canvisit(Point(curr_.x, curr_.y-1))){
    trav_->add(Point(curr_.x, curr_.y-1));
  }

  if(trav_->empty()){
    done_ = true;
    return *this;
  }

  Point peek = trav_->pop();
  while(canvisit(peek) == false){
    if(trav_->empty()){
      done_ = true;
      return *this;
    }
    peek = trav_->pop();
  }
  curr_ = peek;
  visited[curr_.x + curr_.y * png_.width()] = true;
  return *this;
}

/**
 * Iterator accessor opreator.
 *
 * Accesses the current Point in the ImageTraversal.
 */
Point ImageTraversal::Iterator::operator*() {
  /** @todo [Part 1] */
  return curr_;
}

/**
 * Iterator inequality operator.
 *
 * Determines if two iterators are not equal.
 */
bool ImageTraversal::Iterator::operator!=(const ImageTraversal::Iterator &other) {
  /** @todo [Part 1] */
  return(other.done_ != this->done_);
}

//function to determine if the next pixel is valid to visit
bool ImageTraversal::Iterator::canvisit(Point point){
  if(point.x >= png_.width() || point.y >= png_.height()){
    return false;
  }
  if(visited[point.x + point.y*png_.width()]){
    return false;
  }

  HSLAPixel & pixel1 = png_.getPixel(start_.x, start_.y);
  HSLAPixel & pixel2 = png_.getPixel(point.x, point.y);
  double tol = calculateDelta(pixel1, pixel2);
  if(tol >= tolerance_){
    return false;
  }
  return true;
}

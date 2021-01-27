#include "StickerSheet.h"
#include "Image.h"

#include <iostream>
#include <cstdlib>

using namespace std;

//Constructor for default StickerSheet
StickerSheet::StickerSheet(const Image & picture, unsigned max){
  max_= max;
  stcount_ = 0;
  base_ = new Image(picture);
  //array of x coordinates of stickers
  xcoor_ = new int[max];
  //array of y coordinates of stickers
  ycoor_ = new int[max];
  //initalize stickers_ as array of pointers to images
  stickers_ = new Image*[max];
  for(unsigned i = 0; i < max; i++){
    stickers_[i] = NULL;
    xcoor_[i] = 0;
    ycoor_[i] = 0;
  }
}

//Destructor for StickerSheet
StickerSheet::~StickerSheet(){
  erase();
}

//Helper function for destructor of StickerSheet
void StickerSheet::erase(){
  for(unsigned i = 0; i < max_; i++){
    if (stickers_[i] != NULL){
      delete stickers_[i];
      stickers_[i] = NULL;
    }
  }
  delete[] stickers_;
  delete[] xcoor_;
  delete[] ycoor_;
  delete base_;
  stickers_ = NULL;
  xcoor_ = NULL;
  ycoor_ = NULL;
  base_ = NULL;
}

//Copy Constructor for StickerSheet
StickerSheet::StickerSheet(const StickerSheet & other){
  copy(other);
}

//Helper function for copy constructor and assignment operator
void StickerSheet::copy(const StickerSheet & other){
  //Redefine all private members like constructor
  max_ = other.max_;
  stcount_ = other.stcount_;
  base_ = new Image(*other.base_);
  xcoor_ = new int[other.max_];
  ycoor_ = new int[other.max_];
  stickers_ = new Image*[other.max_];
  //Loop to copy arrays from other to this
  for(unsigned i = 0; i < other.max_; i++){
    xcoor_[i] = other.xcoor_[i];
    ycoor_[i] = other.ycoor_[i];
    if (other.stickers_[i] != NULL){
      stickers_[i] = new Image(*other.stickers_[i]);
    }
    else
      stickers_[i] = NULL;
  }
}

const StickerSheet & StickerSheet::operator=(const StickerSheet & other){
  if(this != &other){
    erase();
    copy(other);
  }
    return *this;
}

int StickerSheet::addSticker(Image & sticker, unsigned x, unsigned y){
  if (stcount_ >= max_){
    return -1;
  }
  else{
    int index = 0;
    while(stickers_[index] != NULL){
      index++;
    }
    stickers_[index] = new Image(sticker);
    xcoor_[index] = x;
    ycoor_[index] = y;
    stcount_ += 1;
    return index;
  }
}

void StickerSheet::removeSticker(unsigned index){
  if(index < max_ && stickers_[index] != NULL){
    delete stickers_[index];
    stickers_[index] = NULL;
    xcoor_[index] = 0;
    ycoor_[index] = 0;
    return;
  }
  else
    return;
}

void StickerSheet::changeMaxStickers(unsigned max){
  if (max_ == max){
    return;
  }

  //we must recreate arrays with new max and rewrite them since they cannot be extended
  else if (max > max_){
    //Create local temporary stickersheet with old base_ and new max to assign to this
    StickerSheet temp = StickerSheet(*base_, max);
    //Reassign old array values to new arrays
    for (unsigned i = 0; i < stcount_; i++){
      if (stickers_[i] != NULL){
        temp.xcoor_[i] = xcoor_[i];
        temp.ycoor_[i] = ycoor_[i];
        temp.stickers_[i] = new Image(*stickers_[i]);
      }
    }
    //Reassign this to array with new max_
    *this = temp;
    return;
  }
  //if max is less than the current max_ we remove all stickers beyond new max
  else{
    for(unsigned i = max; i < max_; i++){
      removeSticker(i);
    }
    //Finally reassign max sticker count
    max_ = max;
    return;
  }///DEBUG::: we haven't actually resized or shortened array in this instance
}

Image * StickerSheet::getSticker(unsigned index){
  if (stickers_[index] == NULL || index >= max_)
    return NULL;
  else
    return stickers_[index];
}

Image StickerSheet::render() const{
  //First we print the base image and check if the stickers will go beyond the
  //borders of the base, adjusting accordingly
  Image rend = Image(*base_);
  unsigned xmax = rend.width();
  unsigned ymax = rend.height();
  for(unsigned i = 0; i < max_; i++){
    if (stickers_[i] != NULL){
      unsigned newxmax = xcoor_[i] + stickers_[i]->width();
      if (newxmax > xmax)
        xmax = newxmax;
      unsigned newymax = ycoor_[i] + stickers_[i]->height();
      if (newymax > ymax)
        ymax = newymax;
    }
  }
  rend.resize(xmax, ymax);


  //Now we print the image
  for(unsigned i = 0; i < max_; i++){
    if(stickers_[i] != NULL){
      unsigned stwidth = stickers_[i]->width();
      unsigned stheight = stickers_[i]->height();
      for(unsigned x = xcoor_[i]; x < (xcoor_[i] + stwidth); x++){
        for(unsigned y = ycoor_[i]; y < (ycoor_[i] + stheight); y++){
          HSLAPixel & basepixel = rend.getPixel(x, y);
          HSLAPixel & stpixel = stickers_[i]->getPixel(x - xcoor_[i], y - ycoor_[i]);
          //Only print the pixel if the alpha channel is non-zero
          if(stpixel.a != 0){
            basepixel = stpixel;
          }
        }
      }
    }
  }
  return rend;
}

bool StickerSheet::translate(unsigned index, unsigned x, unsigned y){
  if(index >= max_ || stickers_[index] == NULL)
    return false;
  else{
    xcoor_[index] = x;
    ycoor_[index] = y;
    return true;
  }
}

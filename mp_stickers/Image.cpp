#include "Image.h"
#include "cs225/PNG.h"
#include "cs225/HSLAPixel.h"

#include <iostream>
#include <cmath>
#include <cstdlib>

using cs225::HSLAPixel;
using cs225::PNG;
using namespace cs225;


  void Image::lighten(){
    for (unsigned x = 0; x < this->width(); x++) {
      for (unsigned y = 0; y < this->height(); y++) {
        HSLAPixel & pixel = this->getPixel(x,y);
        pixel.l += 0.1;
        if (pixel.l > 1)
          pixel.l = 1;
      }
    }
  }

  void Image::lighten(double amount){
    for (unsigned x = 0; x < this->width(); x++) {
      for (unsigned y = 0; y < this->height(); y++) {
        HSLAPixel & pixel = this->getPixel(x,y);
        pixel.l += amount;
        if (pixel.l > 1)
          pixel.l = 1;
      }
    }
  }

  void Image::darken(){
    for (unsigned x = 0; x < this->width(); x++) {
      for (unsigned y = 0; y < this->height(); y++) {
        HSLAPixel & pixel = this->getPixel(x,y);
        pixel.l -= 0.1;
        if (pixel.l < 0)
          pixel.l = 0;
      }
    }
  }

  void Image::darken(double amount){
    for (unsigned x = 0; x < this->width(); x++) {
      for (unsigned y = 0; y < this->height(); y++) {
        HSLAPixel & pixel = this->getPixel(x,y);
        pixel.l -= amount;
        if (pixel.l < 0)
          pixel.l = 0;
      }
    }
  }

  void Image::desaturate(){
    for (unsigned x = 0; x < this->width(); x++) {
      for (unsigned y = 0; y < this->height(); y++) {
        HSLAPixel & pixel = this->getPixel(x,y);
        pixel.s -= 0.1;
        if (pixel.s < 0)
          pixel.s = 0;
      }
    }
  }

  void Image::desaturate(double amount){
    for (unsigned x = 0; x < this->width(); x++) {
      for (unsigned y = 0; y < this->height(); y++) {
        HSLAPixel & pixel = this->getPixel(x,y);
        pixel.s += 0.1;
        if (pixel.s > 1)
          pixel.s = 1;
      }
    }
  }

  void Image::saturate(){
    for (unsigned x = 0; x < this->width(); x++) {
      for (unsigned y = 0; y < this->height(); y++) {
        HSLAPixel & pixel = this->getPixel(x,y);
        pixel.s += 0.1;
        if (pixel.s > 1)
          pixel.s = 1;
      }
    }
  }

  void Image::saturate(double amount){
    for (unsigned x = 0; x < this->width(); x++) {
      for (unsigned y = 0; y < this->height(); y++) {
        HSLAPixel & pixel = this->getPixel(x,y);
        pixel.s += amount;
        if (pixel.s > 1)
          pixel.s = 1;
      }
    }
  }

  void Image::grayscale(){
    for (unsigned x = 0; x < this->width(); x++) {
      for (unsigned y = 0; y < height(); y++) {
        HSLAPixel & pixel = this->getPixel(x,y);
        pixel.s = 0;
      }
    }
  }

  void Image::illinify(){
    for (unsigned x = 0; x < this->width(); x++) {
      for (unsigned y = 0; y < this->height(); y++) {
        HSLAPixel & pixel = this->getPixel(x,y);
        if (pixel.h < 114 || pixel.h > 293){ //if pixel hue is closer to orange than blue
          pixel.h = 11;
        }
        else{
          pixel.h = 216;
        }
      }
    }
  }

  void Image::rotateColor(double degrees){
    for (unsigned x = 0; x < this->width(); x++) {
      for (unsigned y = 0; y < this->height(); y++) {
        HSLAPixel & pixel = this->getPixel(x,y);
        pixel.h += degrees;
        while(pixel.h > 360)
          pixel.h = pixel.h - 360;
        while (pixel.h < 0)
          pixel.h = pixel.h + 360;
      }
    }
  }

  void Image::scale(double factor){
    unsigned int oldwidth = this->width();
    unsigned int oldheight = this->height();
    unsigned int newwidth = this->width() * factor;
    unsigned int newheight = this->height() * factor;

    PNG *resized = new PNG(newwidth, newheight);
    for (unsigned x = 0; x < newwidth; x++) {
      for (unsigned y = 0; y < newheight; y++) {
        HSLAPixel & newpixel = resized->getPixel(x, y);
        HSLAPixel & oldpixel = this->getPixel(x/factor, y/factor);
        newpixel = oldpixel;
      }
    }
    this->resize(newwidth, newheight);

    for (unsigned x = 0; x < newwidth; x++) {
      for (unsigned y = 0; y < newheight; y++) {
        HSLAPixel & oldpixel = resized->getPixel(x,y);
        HSLAPixel & newpixel = this->getPixel(x,y);
        newpixel = oldpixel;
      }
    }
    delete resized;
  }

  void Image::scale(unsigned w, unsigned h){
    double width = w;
    double height = h;
    double w_factor = (double)(width/this->width());
    double h_factor = (double)(height/this->height());
    double factor = std::min(w_factor, h_factor);
    scale(factor);
  }

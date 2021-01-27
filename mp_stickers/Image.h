/**
 * @file Image.h
 * Contains your declaration of the interface for the Image class.
 */

#pragma once

#include <string>
using std::string;

#include "cs225/PNG.h"
using namespace cs225;

class Image: public PNG{
  public:
    //Lighten an Image by increasing the luminance of every pixel by 0.1.
    //This function ensures that the luminance remains in the range [0, 1].
    void lighten();

    //Lighten an Image by increasing the luminance of every pixel by amount.
    void lighten(double amount);

    //Darken an Image by decreasing the luminance of every pixel by 0.1.
    void darken();

    //Darkens an Image by decreasing the luminance of every pixel by amount.
    void darken(double amount);

    //Desaturates an Image by decreasing the saturation of every pixel by 0.1.
    void desaturate();

    //Desaturates an Image by decreasing the saturation of every pixel by amount
    void desaturate(double amount);

    //Turns the image grayscale.
    void grayscale();

    //Illinify the image.
    void illinify();

    //Rotates the color wheel by degrees.
    //Rotating in a positive direction increases the degree of the hue. This function ensures that the hue remains in the range [0, 360].
    void rotateColor(double degrees);

    //Saturates an Image by increasing the saturation of every pixel by 0.1.
    void saturate();

    //Saturates an Image by increasing the saturation of every pixel by amount.
    void saturate(double amount);

    //This function both resizes the Image and scales the contents.
    void scale(double factor);

    //This function both resizes the Image and scales the contents.
    //Scales the image to fit within the size (w x h).
    //This function preserves the aspect ratio of the image, so the result will always be an image of width w or of height h (not necessarily both).
    void scale(unsigned w, unsigned h);

  };

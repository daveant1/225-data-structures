/**
 * @file StickerSheet.h
 * Contains your declaration of the interface for the StickerSheet class.
 */
#pragma once
#include "Image.h"

class StickerSheet{
public:
    //Initializes this StickerSheet with a deep copy of the base picture and the ability to hold a max number of stickers (Images) with indices 0 through max - 1.
    StickerSheet(const Image & picture, unsigned max);

    //Frees all space allocated by StickerSheet
    ~StickerSheet();

    //The copy constructor makes this StickerSheet an independent copy of the source.
    //Hint: Code is likely shared between all of the "Rule of Three" functions. Would a helper function (or two) make your life easier?
    StickerSheet(const StickerSheet & other);

    //A helper function for Destructor
    void erase();
    //A helper function for the copy constructor as well as the assignment operator
    void copy(const StickerSheet & other);

    //Adds a sticker to the StickerSheet, so that the top-left of the sticker's Image is at (x, y) on the StickerSheet.
    //The sticker must be added to the lowest possible layer available.
    int addSticker(Image & sticker, unsigned x, unsigned y);

    //Modifies the maximum number of stickers that can be stored on this StickerSheet without changing existing stickers' indices.
    //If the new maximum number of stickers is smaller than the current number number of stickers, the stickers with indices above max - 1 will be lost.
    void changeMaxStickers(unsigned max);

    //Returns a pointer to the sticker at the specified index, not a copy of it.
    //That way, the user can modify the Image.
    //If the index is invalid, return NULL.
    Image * getSticker(unsigned index);

    //The assignment operator for the StickerSheet class.
    // Hint: Code is likely shared between all of the "Rule of Three" functions. Would a helper function (or two) make your life easier?
    const StickerSheet & operator=(const StickerSheet & other);

    //Removes the sticker at the given zero-based layer index.
    //Make sure that the other stickers don't change order.
    void removeSticker(unsigned index);

    //Renders the whole StickerSheet on one Image and returns that Image.
    //The base picture is drawn first and then each sticker is drawn in order starting with layer zero (0), then layer one (1), and so on.
    //If a pixel's alpha channel in a sticker is zero (0), no pixel is drawn for that sticker at that pixel. If the alpha channel is non-zero, a pixel is drawn. (Alpha blending is awesome, but not required.)
    //The returned Image always includes the full contents of the picture and all stickers. This means that the size of the result image may be larger than the base picture if some stickers go beyond the edge of the picture.
    Image render() const;

    //Changes the x and y coordinates of the Image in the specified layer.
    //If the layer is invalid or does not contain a sticker, this function must return false. Otherwise, this function returns true.
    bool translate(unsigned index, unsigned x, unsigned y);

  private:
    //max number of stickers allowed on StickerSheet
    unsigned max_;
    //count of stickers
    unsigned stcount_;
    //array of image x coordinates
    int * xcoor_;
    //array of image y coordinates
    int * ycoor_;
    //base image of StickerSheet
    Image * base_;
    //array of stickers on sheet
    Image ** stickers_;
};

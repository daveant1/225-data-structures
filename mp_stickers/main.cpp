#include "Image.h"
#include "StickerSheet.h"

int main() {

  //
  // Reminder:
  //   Before exiting main, save your creation to disk as myImage.png
  //
Image grumpy, nic, brian, baby;
grumpy.readFromFile("grumpy_cat.png");
nic.readFromFile("nic_cage.png");
brian.readFromFile("brian.png");
baby.readFromFile("baby.png");
StickerSheet sheet = StickerSheet(grumpy, 5);
sheet.addSticker(nic, 0, 0);
sheet.addSticker(baby, 750, 500);
sheet.addSticker(brian, 250, 700);
Image result = sheet.render();
result.writeToFile("myImage.png");

  return 0;
}

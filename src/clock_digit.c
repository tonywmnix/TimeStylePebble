#include "clock_digit.h"
  
  
/*
 * Array mapping numbers to resource ids
 */
uint32_t ClockDigit_imageIds[10] = {RESOURCE_ID_CLOCK_DIGIT_0,
                                    RESOURCE_ID_CLOCK_DIGIT_1,
                                    RESOURCE_ID_CLOCK_DIGIT_2,
                                    RESOURCE_ID_CLOCK_DIGIT_3,
                                    RESOURCE_ID_CLOCK_DIGIT_4,
                                    RESOURCE_ID_CLOCK_DIGIT_5,
                                    RESOURCE_ID_CLOCK_DIGIT_6,
                                    RESOURCE_ID_CLOCK_DIGIT_7,
                                    RESOURCE_ID_CLOCK_DIGIT_8,
                                    RESOURCE_ID_CLOCK_DIGIT_9};

void ClockDigit_setNumber(ClockDigit* this, int number) {
  if(this->currentNum != number) {
    
    //save the old digit image so we can dellocate it
    GBitmap* oldImage = this->currentImage;
    
    //change over to the new digit image
    this->currentImageId = ClockDigit_imageIds[number];
    this->currentImage = gbitmap_create_with_resource(this->currentImageId);
    this->currentNum = number;
    
    //set the layer to the new image
    bitmap_layer_set_bitmap(this->imageLayer, this->currentImage);
    
    //deallocate the old bg image
    gbitmap_destroy(oldImage);
  }
}

void ClockDigit_setColor(ClockDigit* this, GColor fg, GColor bg) {
  // set the new colors
  this->fgColor = fg;
  this->bgColor = bg;
  
  // now, determine what the intermediate colors will be (for AA)
  int colorIncrementR = (fg.r * 85 - bg.r * 85) / 3;
  int colorIncrementG = (fg.g * 85 - bg.g * 85) / 3;
  int colorIncrementB = (fg.b * 85 - bg.b * 85) / 3;
  
//   printf("IncR: %d, IncG: %d, IncB: %d", colorIncrementR, colorIncrementG, colorIncrementB);
  
  GColor midColor1 = GColorFromRGB(fg.r * 85 - colorIncrementR,
                                   fg.g * 85 - colorIncrementG,
                                   fg.b * 85 - colorIncrementB);
  
//   printf("R: %d, G: %d, B: %d", fg.r * 85 - colorIncrementR, fg.g * 85 - colorIncrementG, fg.b * 85 - colorIncrementB);
 
  
//   printf("Generated Midcolor1 R: %d, G: %d, B: %d", midColor1.r, midColor1.g, midColor1.b);
  
  GColor midColor2 = GColorFromRGB(bg.r * 85 + colorIncrementR,
                                   bg.g * 85 + colorIncrementG,
                                   bg.b * 85 + colorIncrementB);
  
//   printf("R: %d, G: %d, B: %d", bg.r * 85 + colorIncrementR, bg.g * 85 + colorIncrementG, bg.b * 85 + colorIncrementB);
  
//   printf("Generated Midcolor2 R: %d, G: %d, B: %d", midColor2.r, midColor2.g, midColor2.b);
  
  GColor* pal = gbitmap_get_palette(this->currentImage);

  pal[0] = fg;
  pal[1] = midColor1;
  pal[2] = midColor2;
  pal[3] = bg;
}

void ClockDigit_construct(ClockDigit* this, GPoint pos) {
  this->currentNum = -1;
  this->bgColor = GColorWhite;
  this->fgColor= GColorBlack;
  
  this->imageLayer = bitmap_layer_create(GRect(pos.x, pos.y, 48, 71));
}

void ClockDigit_destruct(ClockDigit* this) {
  // destroy the background layer
  bitmap_layer_destroy(this->imageLayer);
  
  // deallocate the background image
  gbitmap_destroy(this->currentImage);
}
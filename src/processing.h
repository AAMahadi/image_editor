#ifndef PROCESSING_H

#define PROCESSING_H

#include <raylib.h>
#include <stdlib.h>
#include <string.h>

void saveBMP(const Image *myImage, const char *filenameBuffer);

//features
void applyColorInversion(Image *myImage, Color *pixels);
void applyGrayscale(Image *myImage, Color *pixels);
void applyHflip(Image *myImage, Color *pixels);
void applyVflip(Image *myImage, Color *pixels);
void applyRotate(Image *myImage, Color *pixels);
void applyBrightness(Image *myImage, Color *pixels, int offset);
void applyBlur(Image *myImage, Color *pixels);
void applySharp(Image *myImage, Color *pixels);
void applyCrop(Image *myImage, int startX, int startY, int cropWidth, int cropHeight, Color *pixels);

#endif
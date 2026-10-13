#include "processing.h"

void saveBMP(const Image *myImage, const char *filenameBuffer){
    char filename[512] = "./output-images/";
    strcat(filename, filenameBuffer);
    if(ExportImage(*myImage, filename)) {
        TraceLog(LOG_INFO, "sucessfully saved image as %s", filenameBuffer);
    }
    else {
        TraceLog(LOG_INFO, "Failed to save image as %s", filenameBuffer);
    }
}

//features
void applyColorInversion(Image *myImage, Color *pixels){
    for(int y = 0; y < myImage->height; y++) {
        for(int x = 0; x < myImage->width; x++) {
            int idx = x + y*myImage->width;

            pixels[idx].r = 255 - pixels[idx].r;
            pixels[idx].g = 255 - pixels[idx].g;
            pixels[idx].b = 255 - pixels[idx].b;
        }
    }
}

void applyGrayscale(Image *myImage, Color *pixels){
    for(int y = 0; y < myImage->height; y++) {
        for(int x = 0; x < myImage->width; x++) {
            int idx = x + y * myImage->width;
            
            unsigned int red = pixels[idx].r;
            unsigned int green = pixels[idx].g;
            unsigned int blue = pixels[idx].b;
            
            unsigned int gray = red * 0.299 + green * 0.587 + blue * 0.114;

            pixels[idx].r = gray;
            pixels[idx].g = gray;
            pixels[idx].b = gray;
        }
    }
}

void applyHflip(Image *myImage, Color *pixels){
    for(int y = 0; y < myImage->height; y++) {
        for(int x = 0; x <=myImage->width / 2; x++) {
            int src_idx = x + y * myImage->width;
            int dst_idx = (myImage->width - 1 - x) + y * myImage->width;

            unsigned int temp = pixels[src_idx].r;
            pixels[src_idx].r = pixels[dst_idx].r;
            pixels[dst_idx].r = temp;

            temp = pixels[src_idx].g;
            pixels[src_idx].g = pixels[dst_idx].g;
            pixels[dst_idx].g = temp;

            temp = pixels[src_idx].b;
            pixels[src_idx].b = pixels[dst_idx].b;
            pixels[dst_idx].b = temp;
        }
    }
}
void applyVflip(Image *myImage, Color *pixels){
    for(int y = 0; y <= myImage->height / 2; y++) {
        for(int x = 0; x < myImage->width; x++) {
            int src_idx = x + y * myImage->width;
            int dst_idx = x + (myImage->height - 1 -y) * myImage->width;

            unsigned int temp = pixels[src_idx].r;
            pixels[src_idx].r = pixels[dst_idx].r;
            pixels[dst_idx].r = temp;

            temp = pixels[src_idx].g;
            pixels[src_idx].g = pixels[dst_idx].g;
            pixels[dst_idx].g = temp;

            temp = pixels[src_idx].b;
            pixels[src_idx].b = pixels[dst_idx].b;
            pixels[dst_idx].b = temp;
        }
    }
}

void applyRotate(Image *myImage, Color *pixels) {
    Color *newPixels = (Color *)malloc(myImage->width * myImage->height * sizeof(Color));

    for(int y = 0; y < myImage->height; y++) {
        for(int x = 0; x < myImage->width; x++) {
            int src_idx = x + y * myImage->width;
            int dst_idx = (myImage->height - 1 - y) + x * myImage->height;

            newPixels[dst_idx] = pixels[src_idx];
        }
    }

    int temp = myImage->width;
    myImage->width = myImage->height;
    myImage->height = temp;

    free(myImage->data);
    myImage->data = newPixels;
}

void applyBrightness(Image *myImage, Color *pixels, int offset) {
    for(int y = 0; y < myImage->height; y++) {
        for(int x = 0; x < myImage->width; x++) {
            int idx = x + y * myImage->width;

            pixels[idx].r = ((pixels[idx].r + offset) > 255)? 255 : (((pixels[idx].r + offset) < 0)? 0 : (pixels[idx].r + offset));
            pixels[idx].g = ((pixels[idx].g + offset) > 255)? 255 : (((pixels[idx].g + offset) < 0)? 0 : (pixels[idx].g + offset));
            pixels[idx].b = ((pixels[idx].b + offset) > 255)? 255 : (((pixels[idx].b + offset) < 0)? 0 : (pixels[idx].b + offset));
        }
    }
}

void applyBlur(Image *myImage, Color *pixels) {
    Color *newPixels = (Color *)malloc(myImage->width * myImage->height * sizeof(Color));

    for(int y = 0; y < myImage->height; y++) {
        for(int x = 0; x < myImage->width; x++) {
            int idx1 = x + y * myImage->width;
            unsigned int sum_red = 0;
            unsigned int sum_green = 0;
            unsigned int sum_blue = 0;

            int count = 0;
            for(int dx = -1; dx <= 1; dx++) {
                for(int dy = -1; dy <= 1; dy++) {
                    if((dx+x) >= 0 && (dy+y) >= 0) {
                        if((dx+x) < myImage->width && (dy+y) < myImage->height) {
                            count++;

                            int idx2 = (dx+x) + (dy+y) * myImage->width;

                            sum_red += pixels[idx2].r;
                            sum_green += pixels[idx2].g;
                            sum_blue += pixels[idx2].b;
                        }
                    }
                }
            }
            newPixels[idx1].r = sum_red / count; 
            newPixels[idx1].g = sum_green / count; 
            newPixels[idx1].b = sum_blue / count; 
            newPixels[idx1].a = pixels[idx1].a;
        }
    }

    free(myImage->data);
    myImage->data = newPixels;
}

void applySharp(Image *myImage, Color *pixels) {
    Color *newPixels = (Color *)malloc(myImage->width * myImage->height * sizeof(Color));

    int kernel[] = {0, -1, 0,
                   -1, 5, -1,
                    0, -1, 0};

    for(int y = 0; y < myImage->height; y++) {
        for(int x = 0; x < myImage->width; x++) {
            int idx1 = x + y * myImage->width;

            int red = 0;
            int green = 0;
            int blue = 0;

            for(int dx = -1, X = 0; dx <= 1; dx++, X++) {
                for(int dy = -1, Y = 0; dy <= 1; dy++, Y++) {
                    if((dx+x) >= 0 && (dy+y) >= 0) {
                        if((dx+x) < myImage->width && (dy+y) < myImage->height) {
                            int idx2 = (dx+x) + (dy+y) * myImage->width;
                            int idx3 = X + Y * 3;

                            red += pixels[idx2].r * kernel[idx3];
                            green += pixels[idx2].g * kernel[idx3];
                            blue += pixels[idx2].b * kernel[idx3];
                        }
                    }
                }
            }
            newPixels[idx1].r = (red > 255)? 255:((red < 0)? 0:red);
            newPixels[idx1].g = (green > 255)? 255:((green < 0)? 0:green);
            newPixels[idx1].b = (blue > 255)? 255:((blue < 0)? 0:blue);
            newPixels[idx1].a = pixels[idx1].a;
        }
    }

    free(myImage->data);
    myImage->data = newPixels;
}

void applyCrop(Image *myImage, int startX, int startY, int cropWidth, int cropHeight, Color *pixels) {
    if(startX >= myImage->width || startY >= myImage->height) return;

    if(startX+cropWidth > myImage->width) cropWidth = myImage->width - startX;
    if(startY+cropHeight > myImage->height) cropHeight = myImage->height - startY;

    Color *newPixels = (Color *)malloc(cropWidth * cropHeight * sizeof(Color));

    for(int y = 0; y < cropHeight; y++) {
        for(int x = 0; x < cropWidth; x++) {
            int idx1 = (x+startX) + (y+startY)*myImage->width;
            int idx2 = x + y * cropWidth;

            newPixels[idx2] = pixels[idx1];
        }
    }
    
    myImage->width = cropWidth;
    myImage->height = cropHeight;

    free(myImage->data);
    myImage->data = newPixels;
}
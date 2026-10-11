#include <raylib.h>
#include <string.h>
#include <stdlib.h>

#include "processing.h"

int main() {
    int const screenWidth = 1280;
    int const screenHeight = 800;

    //initializing the window
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Image Editor");

    //creating image variables
    Image myImage = {0};
    Texture2D myTexture = {0};
    Color *pixels = NULL;

    //boolean declarations
    bool isImageLoaded = false;
    bool isFileMenuOpen = false;
    bool showSaveDialog = false;
    bool isBrightnessInputBoxSelected = false;
    bool isStartXinputSelected = false;
    bool isStartYinputSelected = false;
    bool isCropWidthInputSelected = false;
    bool isCropHeightInputSelected = false;

    //saved file name
    char filenameBuffer[64] = "";
    int nameSize = strlen(filenameBuffer);

    char brightnessInput[20] = "";
    int inputSize = strlen(brightnessInput);

    char startX[1080] = "";
    int startXsize = strlen(startX);

    char startY[1080] = "";
    int startYsize = strlen(startY);
    
    char cropWidth[1080] = "";
    int cropWidthSize = strlen(cropWidth);

    char cropHeight[1080] = "";
    int cropHeightSize = strlen(cropHeight);

    SetTargetFPS(60);


    while(!WindowShouldClose()) {
        //image loading
        if(IsFileDropped()) {
            FilePathList droppedfiles = LoadDroppedFiles();
            if(droppedfiles.count > 0) {
                if(isImageLoaded) {
                    UnloadTexture(myTexture);
                    UnloadImage(myImage);
                }

                myImage = LoadImage(droppedfiles.paths[0]);
                
                if(myImage.data != NULL) {
                    ImageFormat(&myImage, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
                    myTexture = LoadTextureFromImage(myImage);
                    pixels = (Color *)myImage.data;

                    isImageLoaded = true;
                }
            }
            UnloadDroppedFiles(droppedfiles);
        }

        //layout maths
        int topPanelHeight = 30;
        int sidePanelWidth = 320;
        int sidePanelX = GetScreenWidth() - sidePanelWidth;

        int initTextX = (GetScreenWidth() - sidePanelWidth - 370) / 2;
        int initTextY = (GetScreenHeight() - topPanelHeight - 20) / 2;

        int imageX = 0;
        int imageY = 0;

        if(isImageLoaded) {
            imageX = (GetScreenWidth() - sidePanelWidth - myImage.width) / 2;
            imageY = (GetScreenHeight() - topPanelHeight - myImage.height) / 2;
        }

        if(imageX < 0) imageX = 0;
        if(imageY < 0) imageY = 0;

        //mousePosition
        Vector2 mousePos = GetMousePosition();

        //menu bar
        Rectangle btnFile = {10, 2, 40, topPanelHeight-2};
        //dropdown
        Rectangle dropdown = {10, topPanelHeight, 150, btnFile.height+20};
        Rectangle btnSaveAs = {10, dropdown.y, 150, btnFile.height+20};
        //save dialog
        Rectangle saveDialog = {(GetScreenWidth() - 500)/2, (GetScreenHeight() - 300)/2, 500, 300};
        Rectangle inputBox = {saveDialog.x+150, saveDialog.y+50, 300, 45};
        Rectangle btnCancel = {saveDialog.x+(saveDialog.width-330)/2, inputBox.y+inputBox.height+70, 150, 40};
        Rectangle btnSave = {btnCancel.x+btnCancel.width+30, inputBox.y+inputBox.height+70, 150, 40};
        //features
        //inversion
        Rectangle btnInv = {sidePanelX+(sidePanelWidth-200)/2, topPanelHeight+30, 200, 40};
        //grayscale
        Rectangle btnGray = {sidePanelX+(sidePanelWidth-200)/2, btnInv.y+btnInv.height+10, 200, 40};
        //horizontal flip
        Rectangle btnHflip = {sidePanelX+(sidePanelWidth-200)/2, btnGray.y+btnGray.height+10, 200, 40};
        //vertical flip
        Rectangle btnVflip = {sidePanelX+(sidePanelWidth-200)/2, btnHflip.y+btnHflip.height+10, 200, 40};
        //rotate 90 degree
        Rectangle btnRotate = {sidePanelX+(sidePanelWidth-200)/2, btnVflip.y+btnVflip.height+10, 200, 40};
        //brightness part
        Rectangle brightBox = {sidePanelX+(sidePanelWidth-250)/2, btnRotate.y+btnRotate.height+10, 250, 70};
        Rectangle offsetInput = {brightBox.x+18, brightBox.y+21, 150, 30};
        Rectangle btnApplyBright = {offsetInput.x+160, offsetInput.y+2.5, 55, 25};
        //blur
        Rectangle btnBlur = {sidePanelX+(sidePanelWidth-200)/2, brightBox.y+brightBox.height+10, 200, 40};
        //sharpen
        Rectangle btnSharp = {sidePanelX+(sidePanelWidth-200)/2, btnBlur.y+btnBlur.height+10, 200, 40};
        //crop part
        Rectangle cropBox = {sidePanelX+(sidePanelWidth-250)/2, btnSharp.y+btnSharp.height+10, 250, 150};
        Rectangle startXinput = {cropBox.x+110, cropBox.y+21, 130, 30};
        Rectangle startYinput = {cropBox.x+110, startXinput.y+startXinput.height+10, 130, 30};
        Rectangle cropWidthInput = {cropBox.x+110, startYinput.y+startYinput.height+10, 130, 30};
        Rectangle cropHeightInput = {cropBox.x+110, cropWidthInput.y+cropWidthInput.height+10, 130, 30};
        Rectangle btnApplyCrop = {cropBox.x+20, cropHeightInput.y+cropHeightInput.height+10, 60, 25};

        //files and saves
        if(showSaveDialog) {
            int key = GetCharPressed();

            while(key > 0) {
                if((key >= 32) && (key <= 125) && (nameSize < 63)) {
                    filenameBuffer[nameSize] = (char)key;
                    filenameBuffer[nameSize+1] = '\0';
                    nameSize++;
                }
                key = GetCharPressed();
            }

            if(IsKeyPressed(KEY_BACKSPACE)) {
                if(nameSize > 0) {
                    nameSize--;
                    filenameBuffer[nameSize] = '\0'; 
                }
            }

            if(IsKeyPressed(KEY_ENTER) || (CheckCollisionPointRec(mousePos, btnSave) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))) {
                if(nameSize > 0) {
                    saveBMP(&myImage, filenameBuffer);
                    TraceLog(LOG_INFO, "saved as: %s", filenameBuffer);
                }
                showSaveDialog = false;
            }

            else if(IsKeyPressed(KEY_ESCAPE) || (CheckCollisionPointRec(mousePos, btnCancel) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))) {
                showSaveDialog = false;
            } 
        }
        else {
            //bright
            if(isBrightnessInputBoxSelected) {
                int key = GetCharPressed();

                while(key > 0) {
                    if((key >= '0' && key <= '9') || key == '+' || key == '-') {
                        brightnessInput[inputSize] = (char)key;
                        brightnessInput[inputSize+1] = '\0';
                        inputSize++;
                    }
                    key = GetCharPressed();
                }
                if(IsKeyPressed(KEY_BACKSPACE)) {
                    if(inputSize > 0) {
                        inputSize--;
                        brightnessInput[inputSize] = '\0';
                    }
                }
                if(IsKeyPressed(KEY_ENTER) && inputSize > 0) {
                    if(isImageLoaded) {
                        applyBrightness(&myImage, pixels, atoi(brightnessInput));
                        UpdateTexture(myTexture, pixels);
                    }
                    isBrightnessInputBoxSelected = false;
                }
            }

            //crop
            if(isStartXinputSelected) {
                int key = GetCharPressed();

                while(key > 0) {
                    if(key >= '0' && key <= '9') {
                        startX[startXsize] = (char)key;
                        startX[startXsize+1] = '\0';
                        startXsize++;
                    }
                    key = GetCharPressed();
                }
                if(IsKeyPressed(KEY_BACKSPACE)) {
                    if(startXsize > 0) {
                        startXsize--;
                        startX[startXsize] = '\0';
                    }
                }
                if(IsKeyPressed(KEY_ENTER) && startXsize > 0) {
                    isStartXinputSelected = false;
                    isStartYinputSelected = true;
                }
            }
            
            if(isStartYinputSelected) {
                int key = GetCharPressed();

                while(key > 0) {
                    if(key >= '0' && key <= '9') {
                        startY[startYsize] = (char)key;
                        startY[startYsize+1] = '\0';
                        startYsize++;
                    }
                    key = GetCharPressed();
                }
                if(IsKeyPressed(KEY_BACKSPACE)) {
                    if(startYsize > 0) {
                        startYsize--;
                        startY[startYsize] = '\0';
                    }
                }
                if(IsKeyPressed(KEY_ENTER) && startYsize > 0) {
                    isStartYinputSelected = false;
                    isCropWidthInputSelected = true;
                }
            }

            if(isCropWidthInputSelected) {
                int key = GetCharPressed();

                while(key > 0) {
                    if(key >= '0' && key <= '9') {
                        cropWidth[cropWidthSize] = (char)key;
                        cropWidth[cropWidthSize+1] = '\0';
                        cropWidthSize++;
                    }
                    key = GetCharPressed();
                }
                if(IsKeyPressed(KEY_BACKSPACE)) {
                    if(cropWidthSize > 0) {
                        cropWidthSize--;
                        cropWidth[cropWidthSize] = '\0';
                    }
                }
                if(IsKeyPressed(KEY_ENTER) && cropWidthSize > 0) {
                    isCropWidthInputSelected = false;
                    isCropHeightInputSelected = true;
                }
            }

            if(isCropHeightInputSelected) {
                int key = GetCharPressed();

                while(key > 0) {
                    if(key >= '0' && key <= '9') {
                        cropHeight[cropHeightSize] = (char)key;
                        cropHeight[cropHeightSize+1] = '\0';
                        cropHeightSize++;
                    }
                    key = GetCharPressed();
                }
                if(IsKeyPressed(KEY_BACKSPACE)) {
                    if(cropHeightSize > 0) {
                        cropHeightSize--;
                        cropHeight[cropHeightSize] = '\0';
                    }
                }
                if(IsKeyPressed(KEY_ENTER) && cropHeightSize > 0) {
                    isCropHeightInputSelected = false;
                }
            }

            if(isImageLoaded && CheckCollisionPointRec(mousePos, btnGray) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                applyGrayscale(&myImage, pixels);
                UpdateTexture(myTexture, myImage.data);
            }
            else if(isImageLoaded && CheckCollisionPointRec(mousePos, btnInv) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                applyColorInversion(&myImage, pixels);       
                UpdateTexture(myTexture, myImage.data);
            }
            else if(isImageLoaded && CheckCollisionPointRec(mousePos, btnHflip) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                applyHflip(&myImage, pixels);       
                UpdateTexture(myTexture, myImage.data);
            }
            else if(isImageLoaded && CheckCollisionPointRec(mousePos, btnVflip) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                applyVflip(&myImage, pixels);       
                UpdateTexture(myTexture, myImage.data);
            }
            else if(isImageLoaded && CheckCollisionPointRec(mousePos, btnRotate) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                applyRotate(&myImage, pixels);       
                
                pixels = (Color *)myImage.data;
                
                UnloadTexture(myTexture);
                myTexture = LoadTextureFromImage(myImage);
            }
            else if(isImageLoaded && CheckCollisionPointRec(mousePos, btnApplyBright) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                applyBrightness(&myImage, pixels, atoi(brightnessInput));
                UpdateTexture(myTexture, pixels);
            }
            else if(isImageLoaded && CheckCollisionPointRec(mousePos, btnBlur) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                for(int i = 0; i < 3; i++) {
                    applyBlur(&myImage, pixels);
                    pixels = (Color *)myImage.data;
                }
                UpdateTexture(myTexture, pixels);
            }
            else if(isImageLoaded && CheckCollisionPointRec(mousePos, btnSharp) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                applySharp(&myImage, pixels);
                pixels = (Color *)myImage.data;

                UpdateTexture(myTexture, pixels);
            }
            else if(isImageLoaded && CheckCollisionPointRec(mousePos, btnApplyCrop) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && startXsize > 0 && startYsize > 0 && cropWidthSize > 0 && cropHeightSize > 0) {
                applyCrop(&myImage, atoi(startX), atoi(startY), atoi(cropWidth), atoi(cropHeight), pixels);
                pixels = (Color *)myImage.data;

                UnloadTexture(myTexture);
                myTexture = LoadTextureFromImage(myImage);
            }

            if(CheckCollisionPointRec(mousePos, offsetInput) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                isBrightnessInputBoxSelected = true;
            }
            
            else if(!CheckCollisionPointRec(mousePos, offsetInput) && isBrightnessInputBoxSelected && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                isBrightnessInputBoxSelected = false;
            }
            
            if(CheckCollisionPointRec(mousePos, startXinput) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                isStartXinputSelected = true;
            }
            else if(!CheckCollisionPointRec(mousePos, startXinput) && isStartXinputSelected && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                isStartXinputSelected = false;
            }
            if(CheckCollisionPointRec(mousePos, startYinput) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                isStartYinputSelected = true;
            }
            else if(!CheckCollisionPointRec(mousePos, startYinput) && isStartYinputSelected && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                isStartYinputSelected = false;
            }
            if(CheckCollisionPointRec(mousePos, cropWidthInput) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                isCropWidthInputSelected = true;
            }
            else if(!CheckCollisionPointRec(mousePos, cropWidthInput) && isCropWidthInputSelected && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                isCropWidthInputSelected = false;
            }
            if(CheckCollisionPointRec(mousePos, cropHeightInput) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                isCropHeightInputSelected = true;
            }
            else if(!CheckCollisionPointRec(mousePos, cropHeightInput) && isCropHeightInputSelected && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                isCropHeightInputSelected = false;
            }
            

            if(CheckCollisionPointRec(mousePos, btnFile) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                isFileMenuOpen = !isFileMenuOpen;
            }
            else if(isFileMenuOpen && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !CheckCollisionPointRec(mousePos, dropdown)) {
                isFileMenuOpen = false;
            }

            if(isImageLoaded && isFileMenuOpen && CheckCollisionPointRec(mousePos, btnSaveAs) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                showSaveDialog = true;
                isFileMenuOpen = false;
            }

        }


        BeginDrawing();
            ClearBackground(RAYWHITE);

            //image drawing
            if(isImageLoaded) {
                DrawTexture(myTexture, imageX, imageY, WHITE);
            }
            else {
                DrawText("Drag an image file to open", initTextX, initTextY, 30, LIGHTGRAY);
            }
            //draw panels
            DrawRectangle(sidePanelX, topPanelHeight, sidePanelWidth, GetScreenHeight() - topPanelHeight, LIGHTGRAY);
            DrawRectangle(0, 0, GetScreenWidth(), topPanelHeight, GRAY);

            //draw menu
            DrawRectangleRec(btnFile, GRAY);
            DrawText("File", btnFile.x+10, btnFile.y + 10, 15, WHITE);

            //inverse button
            DrawRectangleRec(btnInv, GRAY);
            if(CheckCollisionPointRec(mousePos, btnInv)) {
                DrawRectangleRec(btnInv, DARKGRAY);
                DrawRectangleLinesEx(btnInv, 1, BLACK); 
            }
            DrawText("Invert Colors", btnInv.x+30, btnInv.y+10, 17, BLACK);

            //grayscale button
            DrawRectangleRec(btnGray, GRAY);
            if(CheckCollisionPointRec(mousePos, btnGray)) {
                DrawRectangleRec(btnGray, DARKGRAY);
                DrawRectangleLinesEx(btnGray, 1, BLACK); 
            }
            DrawText("Gray Scale", btnGray.x+40, btnGray.y+10, 17, BLACK);

            //horizontal flip
            DrawRectangleRec(btnHflip, GRAY);
            if(CheckCollisionPointRec(mousePos, btnHflip)) {
                DrawRectangleRec(btnHflip, DARKGRAY);
                DrawRectangleLinesEx(btnHflip, 1, BLACK); 
            }
            DrawText("Horizontal Flip", btnHflip.x+28, btnHflip.y+10, 17, BLACK);

            //vertical flip
            DrawRectangleRec(btnVflip, GRAY);
            if(CheckCollisionPointRec(mousePos, btnVflip)) {
                DrawRectangleRec(btnVflip, DARKGRAY);
                DrawRectangleLinesEx(btnVflip, 1, BLACK); 
            }
            DrawText("Vertical Flip", btnVflip.x+35, btnVflip.y+10, 17, BLACK);

            //90 degree rotation
            DrawRectangleRec(btnRotate, GRAY);
            if(CheckCollisionPointRec(mousePos, btnRotate)) {
                DrawRectangleRec(btnRotate, DARKGRAY);
                DrawRectangleLinesEx(btnRotate, 1, BLACK); 
            }
            DrawText("Rotate Image", btnRotate.x+35, btnRotate.y+ 10, 17, BLACK);

            //brightness part
            DrawRectangleRec(brightBox, LIGHTGRAY);
            DrawText("Brightness:", brightBox.x+20, brightBox.y+5, 14, BLACK);
            
            DrawRectangleRec(offsetInput, RAYWHITE);
            DrawText(brightnessInput, offsetInput.x+10, offsetInput.y+8, 15, BLACK);
            DrawRectangleLinesEx(offsetInput, 0.5, BLACK);
            if(isBrightnessInputBoxSelected) DrawRectangleLinesEx(offsetInput, 1, BLACK);
            
            DrawRectangleRec(btnApplyBright, GRAY);
            if(CheckCollisionPointRec(mousePos, btnApplyBright)) {
                DrawRectangleRec(btnApplyBright, DARKGRAY);
                DrawRectangleLinesEx(btnApplyBright, 1, BLACK); 
            }
            DrawText("Apply", btnApplyBright.x+8, btnApplyBright.y+5, 14, BLACK);

            //blur
            DrawRectangleRec(btnBlur, GRAY);
            if(CheckCollisionPointRec(mousePos, btnBlur)) {
                DrawRectangleRec(btnBlur, DARKGRAY);
                DrawRectangleLinesEx(btnBlur, 1, BLACK); 
            }
            DrawText("Blur Image", btnBlur.x+45, btnBlur.y+10, 17, BLACK);
            
            //sharpen
            DrawRectangleRec(btnSharp, GRAY);
            if(CheckCollisionPointRec(mousePos, btnSharp)) {
                DrawRectangleRec(btnSharp, DARKGRAY);
                DrawRectangleLinesEx(btnSharp, 1, BLACK); 
            }
            DrawText("Sharpen Image", btnSharp.x+40, btnSharp.y+10, 17, BLACK);

            //crop part
            DrawRectangleRec(cropBox, LIGHTGRAY);
            DrawText("Crop:", cropBox.x+20, cropBox.y+10, 14, BLACK);
            
            DrawText("Start X:", cropBox.x+20, cropBox.y+30, 14, BLACK);
            DrawRectangleRec(startXinput, RAYWHITE);
            DrawRectangleLinesEx(startXinput, 0.5, BLACK);
            if(isStartXinputSelected) DrawRectangleLinesEx(startXinput, 1, BLACK);
            DrawText(startX, startXinput.x+10, startXinput.y+8, 15, BLACK);

            DrawText("Start Y:", cropBox.x+20, startXinput.y+startXinput.height+20, 14, BLACK);
            DrawRectangleRec(startYinput, RAYWHITE);
            DrawRectangleLinesEx(startYinput, 0.5, BLACK);
            if(isStartYinputSelected) DrawRectangleLinesEx(startYinput, 1, BLACK);
            DrawText(startY, startYinput.x+10, startYinput.y+8, 15, BLACK);

            DrawText("Crop Width:", cropBox.x+20, startYinput.y+startYinput.height+20, 14, BLACK);
            DrawRectangleRec(cropWidthInput, RAYWHITE);
            DrawRectangleLinesEx(cropWidthInput, 0.5, BLACK);
            if(isCropWidthInputSelected) DrawRectangleLinesEx(cropWidthInput, 1, BLACK);
            DrawText(cropWidth, cropWidthInput.x+10, cropWidthInput.y+8, 15, BLACK);

            DrawText("Crop Height:", cropBox.x+20, cropWidthInput.y+cropWidthInput.height+20, 14, BLACK);
            DrawRectangleRec(cropHeightInput, RAYWHITE);
            DrawRectangleLinesEx(cropHeightInput, 0.5, BLACK);
            if(isCropHeightInputSelected) DrawRectangleLinesEx(cropHeightInput, 1, BLACK);
            DrawText(cropHeight, cropHeightInput.x+10, cropHeightInput.y+8, 15, BLACK);

            DrawRectangleRec(btnApplyCrop, GRAY);
            if(CheckCollisionPointRec(mousePos, btnApplyCrop)) {
                DrawRectangleRec(btnApplyCrop, DARKGRAY);
                DrawRectangleLinesEx(btnApplyCrop, 1, BLACK); 
            }
            DrawText("Apply", btnApplyCrop.x+10, btnApplyCrop.y+6, 14, BLACK);

            //drawing filemenu
            if(isFileMenuOpen) {
                DrawRectangleRec(dropdown, LIGHTGRAY);
                
                DrawRectangleRec(btnSaveAs, LIGHTGRAY);
                DrawText("Save as", btnSaveAs.x+10, btnSaveAs.y+15, 15, BLACK);
                DrawRectangleLinesEx(btnSaveAs, 1, DARKGRAY);

            }

            //save dialog
            if(showSaveDialog) {
                DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.4));
                DrawRectangleRec(saveDialog, RAYWHITE);

                DrawText("Save As:", saveDialog.x+40, saveDialog.y+60, 20, BLACK);
                DrawRectangleRec(inputBox, RAYWHITE);
                DrawRectangleLinesEx(inputBox, 1, BLACK);
                DrawText(filenameBuffer, inputBox.x+10, inputBox.y+10, 20, BLACK);

                DrawRectangleRec(btnCancel, LIGHTGRAY);
                if(CheckCollisionPointRec(mousePos, btnCancel)) {
                    DrawRectangleRec(btnCancel, GRAY);
                    DrawRectangleLinesEx(btnCancel, 1, BLACK);
                }
                DrawText("Cancle", btnCancel.x+45, btnCancel.y+12, 15, BLACK);
                
                DrawRectangleRec(btnSave, LIGHTGRAY);
                if(CheckCollisionPointRec(mousePos, btnSave)) {
                    DrawRectangleRec(btnSave, GRAY);
                    DrawRectangleLinesEx(btnSave, 1, BLACK);
                }
                DrawText("Save", btnSave.x+50, btnSave.y+12, 15, BLACK);
            }

        EndDrawing();
    }

    if(isImageLoaded) {
        UnloadTexture(myTexture);
        UnloadImage(myImage);
    }

    CloseWindow();

    return 0;
}
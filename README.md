# 🎨 Raylib C Image Editor

A lightweight, desktop-based image editing application written entirely in C using the **Raylib** library. This project features a custom graphical user interface (GUI) and performs image processing through direct manual pixel manipulation, avoiding reliance on heavy external image processing libraries.

![Editor UI](assets/ui.png) 
*Screenshot of the main editor interface*

---

## ✨ Features

The editor supports a variety of image processing tools and transformations, all handled via custom algorithms built from scratch:

*   🌗 **Color Adjustments:** Apply color inversion or convert images to grayscale.
*   ☀️ **Brightness Control:** Adjust pixel brightness based on custom numerical offset inputs.
*   🔄 **Transformations:** Rotate the image 90 degrees or flip it horizontally and vertically.
*   ✂️ **Custom Cropping:** Crop the image precisely by defining Start X, Start Y, Width, and Height parameters.
*   💧 **Filters:** Apply custom Blur or Sharpen algorithms to your images.
*   💾 **File Export:** Save your edited output to disk via a custom save interface.

### Feature Previews
| Blur | Crop | Save Interface |
|:---:|:---:|:---:|
| ![Blur](assets/blur.png) | ![Crop](assets/crop.png) | ![Save](assets/saveinterface.png) |

*(Visual assets sourced from the `assets/` directory)*

---

## 📁 Supported File Formats

This project relies on Raylib's default internal `stb_image` loader. 
*   ✅ **PNG** (`.png`) - Recommended for full transparency support.
*   ✅ **BMP** (`.bmp`) - Recommended for uncompressed pixel data.

*Note: Highly compressed modern web formats disguised as standard extensions (e.g., WEBP masquerading as a `.jpg`) will fail to load.*

---

## 💻 System Requirements

*   **Operating System:** Linux (Tested on Ubuntu/Debian-based distributions).
*   **Compiler:** GCC (or any standard C11 compiler).
*   **Build System:** GNU Make.
*   **Version Control:** Git.
*   **Dependencies:** Standard Linux X11/OpenGL graphics headers (typically pre-installed, or available via `sudo apt install libgl1-mesa-dev libxi-dev libxcursor-dev libxrandr-dev libxinerama-dev libx11-dev`).

---

## 🚀 How to Clone and Run

This project uses a Git submodule to handle the Raylib library locally. **You do not need to install Raylib globally on your system.**

**1. Clone the repository (Important: use the recursive flag)**

    git clone --recursive https://github.com/AAMahadi/image_editor.git
    cd image_editor

*(If you already cloned without `--recursive`, run `git submodule update --init --recursive` inside the project folder).*

**2. Compile and Launch**
The provided `Makefile` will automatically build the Raylib submodule and the main application.

    make run

---

## 🛠️ Usage Guide

1. **Load:** Drag and drop a supported image file (`.png` or `.bmp`) directly into the application window.
2. **Edit:** Use the right-side control panel to apply filters, rotations, and adjustments.
3. **Parameters:** For parameterized tools like **Brightness** or **Crop**, click the input text box, type your desired numerical values, and click **Apply**.
4. **Save:** Click **File** in the top-left menu bar to access the save dialog and export your modified image.

---

## 🔜 Upcoming Features
*   ↩️ **Undo/Redo System:** Navigate backwards through your edit history.

---

## 👨‍💻 Author

**Abdullah Al Mahadi**
* GitHub: [@AAMahadi](https://github.com/AAMahadi)

---

## 📄 License

This project is licensed under the MIT License - see the LICENSE file for details.
# Image Processing Operations in C++

This project contains implementations of basic image processing operations using **Dr. Cem’s simple C++ image library (`image.h`)**. Each file corresponds to an individual image operation we covered in class and is meant to help reinforce the concepts we practiced on paper — now in code.

---

## 📂 Files Overview

Each `.cpp` file corresponds to a different image processing technique:

| Filename                    | Description                                  |
| --------------------------- | -------------------------------------------- |
| `nearestNeighbor.cpp`       | Nearest neighbor interpolation               |
| `bilinearInterpolation.cpp` | Bilinear interpolation                       |
| `brighten.cpp`              | Brightening the image                        |
| `contrast.cpp`              | Adjusting contrast                           |
| `gamma.cpp`                 | Gamma correction                             |
| `contrastStretching.cpp`    | Contrast stretching                          |
| `inverse.cpp`               | Image inversion                              |
| `histogramEqualization.cpp` | Histogram equalization                       |
| `convolution.cpp`           | Convolution with a blur kernel               |
| `image.h`                   | Dr. Cem's simple image library (from Moodle) |

Also included:

- `lena.png`: Original image
- `lena_grayscale.png`: Grayscale version for comparison (helps verify your results)

---

## How to Compile and Run

Each file is **independent**. To compile and run any of them, use the terminal commands below. Just replace the filename with the one you want.

### 🐧 On Linux:

Install the PNG development library:

- **Ubuntu/Debian**:

  ```bash
  sudo apt install libpng-dev
  ```

- **Fedora**:
  ```bash
  sudo dnf install libpng-devel
  ```

Then compile and run:

```bash
g++ -lpng yourfile.cpp -o yourprogram
./yourprogram
```

### On Windows:

You’ll need to install a C++ compiler that supports PNG, such as **MinGW** or **Visual Studio**.  
A video tutorial will be added soon to help with setup on Windows.

---

## ✅ How It Works

Each `.cpp` file:

1. Loads `lena.png` or `lena_grayscale.png`.
2. Applies a specific image processing operation.
3. Saves the result to a new output file (like `output_convolution.png`, etc).

All implementations mirror the same math we did manually in class — now automated with C++.

---

## Tips

- The grayscale version is great for comparing visual effects after applying operations.
- The code is commented to help you understand what's going on step-by-step.
- Feel free to tweak the code and try different images (just update the filename in the code).

---

## Good luck with your exams!

I hope this helps, and feel free to clone or fork and experiment as much as you want!

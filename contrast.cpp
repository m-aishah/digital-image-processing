// This program adjusts the CONTRAST of a grayscale image (lena.png).
// We're using the **exact formula** from the notes:
//
//     New = c * (I(x, y) - 2^(B - 1)) + 2^(B - 1)
//
// where:
//   - I(x, y) is the original pixel intensity
//   - B is the bit depth (8 for grayscale, so midpoint is 2^(8 - 1) = 128)
//   - c is the contrast factor — here, we chose c = 2 to double the contrast
//
// All we're doing is going through every pixel and applying this formula directly.
// Pixels brighter than 128 get brighter, pixels darker than 128 get darker.
//
// NOTE: This method can cause pixel values to go outside [0, 255], 
// so in a real system, you'd want to clamp them — but we're keeping it simple here.

#include "image.h"
#include <cmath> // for pow()

int main()
{
    GrayscaleImage im;
    im.Load("lena.png");

    // Define contrast factor (c) and midpoint (128 for 8-bit)
    float c = 2.0;
    int midpoint = pow(2, 8 - 1); // 128

    // Loop through each pixel and apply the formula
    for (int x = 0; x < im.GetWidth(); x++)
    {
        for (int y = 0; y < im.GetHeight(); y++)
        {
            int original = im(x, y);
            im(x,y) = c * (im(x,y) - midpoint) + midpoint;

        }
    }

    im.Save("output_contrast2.png");

    return 0;
}

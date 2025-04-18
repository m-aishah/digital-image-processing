// This program performs **gamma correction** on a grayscale image.
// Gamma correction is a non-linear operation that adjusts brightness in a more natural way,
// compared to simply adding or subtracting a value.
//
// In class, we saw this formula:
//     O(x, y) = (2^B - 1) * (I(x, y) / (2^B - 1))^γ
//
// Where:
//   - γ (gamma) < 1 brightens the image
//   - γ > 1 darkens the image
//   - B is the bit depth (usually 8 for grayscale), so max intensity is 255
// In this code,I'll literally just go through every pixel and apply the formula.
//
// This does not cause overflow/underflow and gives a more visually pleasing result than linear methods.

#include "image.h"
#include <cmath> // for pow()

int main()
{
    GrayscaleImage im;
    im.Load("lena.png");

    float gamma = 0.4;         // try values like 0.4 (brighten), 1.0 (no change), 2.0 (darken)
    int max_intensity = 255;   // 2^8 - 1

    for (int y = 0; y < im.GetHeight(); y++) {
        for (int x = 0; x < im.GetWidth(); x++) {
            float normalized = float(im(x, y)) / max_intensity; // devide by max intensity
            float corrected = pow(normalized, gamma); // make it to the power of gamma
            im(x, y) = corrected * max_intensity; // multiply by max intensity
        }
    }

    im.Save("output_gamma.png");

    return 0;
}

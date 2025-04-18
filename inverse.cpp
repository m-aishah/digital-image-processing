// This program inverts a grayscale image, creating a "negative" version.
// We are using the exact formula from the notes:
//
//     O(x, y) = (2^B - 1) - I(x, y)
//
// For 8-bit grayscale, this becomes:
//     O(x, y) = 255 - I(x, y)

#include "image.h"

int main()
{
    GrayscaleImage im;
    im.Load("lena.png");

    int max_val = 255;

    for (int y = 0; y < im.GetHeight(); y++) {
        for (int x = 0; x < im.GetWidth(); x++) {
            im(x, y) = max_val - im(x, y);
        }
    }

    im.Save("output_inverse.png");

    return 0;
}

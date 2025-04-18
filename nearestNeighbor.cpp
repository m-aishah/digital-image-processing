// This program performs a basic image resampling operation using NEAREST NEIGHBOR INTERPOLATION.
// We are zooming a grayscale image to a new fixed size (1200x1200 pixels).
// Nearest neighbor means for each pixel in the new image, we find the closest corresponding pixel
// in the original image (by scaling the coordinates), and copy that value.
// We use 'rounding' to pick the nearest pixel instead of calculating an average (as in linear interpolation).

#include "image.h"     // Dr. Cem’s simple image processing library (from Moodle)
#include "math.h"      // For the round() function, which we use in the scaling

int main ()
{
    // Step 1: Create an empty grayscale image object 
    GrayscaleImage im;
    // Load "lena.png" into the empty "im" object we just created
    im.Load("lena.png"); // Make sure "lena.png" is in the same folder as this file

    // Step 2: Create a new blank image with the desired output size (1200x1200)
    GrayscaleImage out(1200, 1200);

    // Step 3: Calculate the scaling factors (Sx and Sy) for x and y directions
    // These values tell us how much smaller the original image is compared to the new one.
    float Sx = float(im.GetWidth()) / out.GetWidth(); // input image width / output image width
    float Sy = float(im.GetHeight()) / out.GetHeight(); // input image height / output image height

    // Step 4: Loop through every pixel in the new (larger) image
    for (int y = 0; y < out.GetHeight(); y++) // For each row
    {
        for (int x = 0; x < out.GetWidth(); x++) // For each column
        {
            // Step 5: Map output pixel back to original image coordinates
            // The formula from class: O(x, y) = I(x * Sx, y * Sy)
            // But remember x * Sx and y * Sy usually give decimals (like 52.4, 92.8)
            // So we round to get the nearest neighbor pixel in the original image
            int orig_x = round(x * Sx);
            int orig_y = round(y * Sy);

            // Step 6: Copy pixel value from the original to the output image
            out(x, y) = im(orig_x, orig_y);
        }
    }

    // Step 7: Save the resized image to a new file
    out.Save("output_nn.png"); // You should see a bigger version of lena.png in the same folder

    return 0;
}

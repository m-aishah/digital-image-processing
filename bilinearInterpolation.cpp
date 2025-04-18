// This program performs a smooth image resampling using BILINEAR INTERPOLATION.
// Bilinear interpolation provides a better quality result than nearest neighbor by considering the values of the 4 nearest pixels
// and computing a weighted average based on how close the new pixel is to each of them.
// The result is a smoother and less pixelated image when zoomed.

// We're resizing a grayscale image (lena.png) to a new fixed size (1400x1400 pixels).
// You should see a clearer result than with nearest neighbor, especially around edges.

#include "image.h"     // Dr. Cem’s image library from Moodle
#include "math.h"      // For floor() function

int main()
{
    // Step 1: Load the original grayscale image
    GrayscaleImage im;
    im.Load("lena.png"); // Make sure this image is in the same folder

    // Step 2: Create a new blank image with the desired size
    GrayscaleImage out(1400, 1400); // We're scaling up to 1400x1400

    // Step 3: Calculate the scaling ratios
    // Sx and Sy determine how input image coordinates are mapped to the output
    float Sx = float(im.GetWidth()) / out.GetWidth();
    float Sy = float(im.GetHeight()) / out.GetHeight();

    // Step 4: Loop through each pixel in the output image
    for (int y = 0; y < out.GetHeight(); y++)
    {
        for (int x = 0; x < out.GetWidth(); x++)
        {
            // So this is the part where we do the bilinear interpolation
            // You know on paper we do some steps that involve finding c, d, and F right and figuring out
            // which values to use as c and d
            // We do the same here in code

            // Step 5: Map output pixel (x, y) to floating point coordinates (x', y') in input image
            // meaning we multiply x with Sx and y with Sy just like we do on paper
            float x_prime = x * Sx;
            float y_prime = y * Sy;

            // Step 6: Get the top-left corner of the 2x2 neighborhood (a,b)
            // then we usually try to find c and d right? like we locate where the point we are looking for is
            // We do the same here in code but in a more structured way
            int ax = floor(x_prime); // we find the top-left corner of the 2x2 neighborhood
            int ay = floor(y_prime); // we find the top-left corner of the 2x2 neighborhood
            int bx = ax + 1; // we find the top-right corner of the 2x2 neighborhood
            int by = ay + 1; // we find the bottom-left corner of the 2x2 neighborhood

            // Step 7: Get the fractional part (used as weights for interpolation)
            // I mean we get that Factor (F) which we usually pick everything after the decimal point
            float fx = x_prime - ax; // we get for the x coordinate
            float fy = y_prime - ay; // and for the y coordinate
            // Now we have the 4 neighboring pixels and their weights

            // Step 8: Perform bilinear interpolation using the 4 neighboring pixels
            // Then usually in the notes we use the bilinear interpolation formula right?
            // Again We do the same here in code but in a more structured way
            // Rather than doing it 3 times with different c and d values
            // we just do it once with the 4 neighboring pixels in one formula.
            // This is the formula: 
            // (1 - fx)(1 - fy) * Top-Left +
            // fx(1 - fy)       * Top-Right +
            // (1 - fx)fy       * Bottom-Left +
            // fx * fy          * Bottom-Right
            // 
            // In short: closer neighbors contribute more to the result
            out(x, y) = 
                        (1 - fx) * (1 - fy) * im(ax, ay) +
                        (    fx) * (1 - fy) * im(bx, ay) +
                        (1 - fx) * (    fy) * im(ax, by) +
                        (    fx) * (    fy) * im(bx, by);
        }
    }

    // Step 9: Save the interpolated (resized) image
    out.Save("output_bl.png"); // You should see a smooth, scaled-up version of lena.png

    return 0;
}

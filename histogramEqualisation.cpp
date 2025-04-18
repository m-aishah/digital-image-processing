// This program performs HISTOGRAM EQUALIZATION on a grayscale image.
//
// This operation redistributes the pixel intensity values so that they’re more evenly spread out,
// which usually gives us better contrast and detail — especially if the image is a bit washed out or too dark.
//
// We’re following the **exact steps from the notes**, so this should feel very familiar:
//
//     1. Count how many times each intensity (0–255) appears in the image → Histogram
//     2. Convert those counts into probabilities → PDF
//     3. Turn that into a cumulative distribution → CDF
//     4. Multiply each CDF value by the maximum intensity (255) → Replacement values
//     5. For every pixel in the image, replace it using this mapping
//
// We use 4 arrays: histogram[], pdf[], cdf[], and a replacement[] for the new pixel values.
// Let’s get started.

#include "image.h"
#include <vector>

int main()
{
    // Step 1: Load the image
    GrayscaleImage im;
    im.Load("lena.png");

    int width = im.GetWidth();
    int height = im.GetHeight();
    int total_pixels = width * height;

    // Step 2: Create the arrays we'll need
    std::vector<int> histogram(256, 0);     // For counting occurrences of each intensity (0–255)
    std::vector<float> pdf(256, 0.0);       // Probability distribution function (PDF)
    std::vector<float> cdf(256, 0.0);       // Cumulative distribution function (CDF)
    std::vector<Byte> replacement(256);     // Final replacement values after scaling CDF

    // Step 3: Build the histogram
    // For every pixel, just count how many times each intensity value appears
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Byte value = im(x, y);
            histogram[value]++;
        }
    }

    // Step 4: Compute the PDF by dividing counts by total number of pixels
    for (int i = 0; i < 256; i++) {
        pdf[i] = float(histogram[i]) / total_pixels;
    }

    // Step 5: Build the CDF by adding up the PDF values cumulatively
    // CDF[i] = PDF[0] + PDF[1] + ... + PDF[i]
    cdf[0] = pdf[0];
    for (int i = 1; i < 256; i++) {
        cdf[i] = cdf[i - 1] + pdf[i];
    }

    // Step 6: Multiply each CDF value by the max intensity (255) to get the replacement
    // This maps the original pixel values to new, spread-out ones
    for (int i = 0; i < 256; i++) {
        replacement[i] = cdf[i] * 255; // No need to round here since Byte truncates automatically
    }

    // Step 7: Apply the replacement to each pixel in the image
    // For each pixel, just look up its new value using the replacement array
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Byte original = im(x, y);
            im(x, y) = replacement[original];
        }
    }

    // Step 8: Save the result
    // Now the output image should have better contrast and a more uniform histogram
    im.Save("output_histeq.png");

    return 0;
}

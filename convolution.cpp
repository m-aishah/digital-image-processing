// This program shows how to perform CONVOLUTION operation on a grayscale image.
// Convolution is basically about taking a small matrix (kernel/filter) and sliding it over the image,
// multiplying the kernel/filter values with the corresponding pixel values, and summing them up.
//
// This is useful for various image processing tasks like blurring, sharpening, edge detection, etc.
// By using different kernel/filters/filters, we can achieve different effects.
//
// In this code, we’ll use a simple 3x3 kernel/filter for blurring the image.
// The kernel/filter is a small matrix that we slide over the image.
//
// The blurring kernel/filter we’ll use in this example is:
//     1  1  1
//     1  1  1 
//     1  1  1
//
// and the actual input image is the lena.png image.
//
// We'll be doing it exactly how we do it on paper which is:
// First we handle the filter/kernel by first normalising and then mirroring it.
// Then we loop through every pixel in the image,
// and for each pixel if it at the edge of the image we just copy the value as it is
// and if it is not at the edge we do the convolution by "centering the kernel/filter" on that pixel
// then multiplying the kernel/filter with the pixel values and summing them up.

#include "image.h" // Dr. Cem’s simple image processing library (from Moodle)


int main()
{
    // Step 1: Load the original grayscale image
    GrayscaleImage im;
    im.Load("lena.png"); // Make sure this image is in the same folder

    // Step 2: Create a new blank image with the same size
    GrayscaleImage out(im.GetWidth(), im.GetHeight());

    // Step 3: Define the convolution kernel/filter (3x3 blurring kernel/filter)
    float kernel[3][3] = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };

    // Step 4: Normalize the kernel/filter
    float sum = 0;
    // First we calculate the total sum (1+1+...+1 = 9 for this kernel)
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            sum += kernel[i][j];
        }
    }
    // Then divide each value by the sum (so each becomes 1/9)
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            kernel[i][j] /= sum;
        }
    }

    // Step 5: Perform convolution
    // Loop through each pixel in the image
    for (int y = 0; y < im.GetHeight(); y++) { // For each row
        for (int x = 0; x < im.GetWidth(); x++) { // For each column
            // If the pixel is at the edge, just copy the value as it is
            if (x == 0 || x == im.GetWidth() - 1 || y == 0 || y == im.GetHeight() - 1) {
                out(x, y) = im(x, y);
            }
            else {
                // Otherwise, perform convolution:
                // 1. Center the kernel over (x,y)
                // 2. Multiply kernel values with image pixels
                // 3. Sum all the products
                float new_val = 0;
                for (int ky = -1; ky <= 1; ky++) {       // Kernel row offset
                    for (int kx = -1; kx <= 1; kx++) {   // Kernel column offset
                        // kernel[ky+1][kx+1] because kernel indices go from 0 to 2
                        // im(x+kx, y+ky) gets the neighbor pixel
                        new_val += kernel[ky+1][kx+1] * im(x+kx, y+ky);
                    }
                }
                out(x, y) = new_val; // Store the result
            }
        }
    }

    // Step 6: Save the new image to a file
    out.Save("output_convolution.png"); // You should see a blurred version of lena.png in the same folder
    return 0;

}
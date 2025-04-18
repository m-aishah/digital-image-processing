// This program performs a simple image processing task: BRIGHTENING A GRAYSCALE IMAGE.
// What we’re doing here is increasing the brightness of every pixel in the image, 
// by adding a constant value. This makes the whole image look lighter.
//
// One important thing: we have to “cap” the values at 255.
// That’s because in an 8-bit grayscale image, pixel values can only go from 0 to 255.
// If we try to go beyond that, weird things happen — so we make sure nothing goes over 255. :)

#include "image.h" // We’re using Dr. Cem’s simple image library for this. You can find it on Moodle.

int main()
{
    // Step 1: Let’s create an empty grayscale image object.
    // This will hold our image data.
    GrayscaleImage im;

    // Step 2: Define how much we want to brighten the image.
    // This number will be added to every pixel.
    Byte b = 10; // You can change this number depending on how much you want to brighten the image.

    // Step 3: Load the image.
    // We’re using “lena.png” as our example image.
    // Just make sure the image is in the same folder as this code file.
    // (You can also use your own image if you want — just update the filename.)
    im.Load("lena.png");

    // Step 4: Loop through every pixel in the image.
    // We go row by row (top to bottom), and inside each row, we go column by column (left to right).
    for (int y = 0; y < im.GetHeight(); y++)
    {
        for (int x = 0; x < im.GetWidth(); x++)
        {
            // Step 5: Brighten each pixel
            // im(x, y) accesses the pixel at (x, y) in the image.
            // We add 'b' to the current pixel value.
            // Use 'car' to cap the result at 255, so it doesn’t overflow.
            // "im(x, y) + b" this part does the brightening
            // "car(im(x, y) + b, 255)" this part ensures the resullt after adding does not exceed 255
            im(x, y) = car(im(x, y) + b, 255); 
        }
    }

    /// Step 6: Save the new, brightened image to a file.
    // We’re calling it “output.png” — you’ll see this appear in the same folder as your code.
    // Try comparing it to the original to see the difference.
    im.Save("output.png");

    return 0;
}

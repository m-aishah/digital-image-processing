
#pragma once

#include <stdio.h>
#include <vector>
#include "png.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <math.h>

typedef unsigned char Byte;

struct RGBA {
	RGBA() : r(0), g(0), b(0), a(255) { }
	RGBA(Byte r, Byte g, Byte b, Byte a = 255) : r(r), g(g), b(b), a(a) { }
	RGBA(Byte lum) : r(lum), g(lum), b(lum), a(255) { }

	Byte luminance() const {
		return 0.299*r + 0.587*g + 0.114*b;
	}

	Byte r, g, b, a;
};

class GrayscaleImage;

class ColorImage {
public:

	ColorImage() :
		width(0), height(0) { }

	ColorImage(int width, int height) :
		width(width), height(height), data(width*height) { }

	ColorImage(const GrayscaleImage &);

	RGBA &operator()(int x, int y) {
		return data[x + y * width];
	}

	RGBA operator()(int x, int y) const {
		return data[x + y * width];
	}

	RGBA Get(int x, int y) const {
		if (x < 0 || x >= width || y < 0 || y >= height) {
			return RGBA(0, 0, 0, 0);
		}
		else {
			return data[x + y * width];
		}
	}

	void Clear() {
		for (int i = 0; i < width*height; i++) {
			data[i] = 0;
		}
	}

	int GetWidth() const { return width; }

	int GetHeight() const { return height; }

	void Save(std::string filename) {
		FILE *fp = NULL;
		png_structp png_ptr = NULL;
		png_infop info_ptr = NULL;

		// Open file for writing (binary mode)
		fp = fopen(filename.c_str(), "wb");
		if (fp == NULL) {
			fprintf(stderr, "Could not open file %s for writing\n", filename.c_str());
			goto finalise;
		}

		// Initialize write structure
		png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (png_ptr == NULL) {
			fprintf(stderr, "Could not allocate write struct\n");
			goto finalise;
		}

		// Initialize info structure
		info_ptr = png_create_info_struct(png_ptr);
		if (info_ptr == NULL) {
			fprintf(stderr, "Could not allocate info struct\n");
			goto finalise;
		}

		// Setup Exception handling
		if (setjmp(png_jmpbuf(png_ptr))) {
			fprintf(stderr, "Error during png creation\n");
			goto finalise;
		}

		png_init_io(png_ptr, fp);

		// Write header (8 bit colour depth)
		png_set_IHDR(png_ptr, info_ptr, width, height,
			8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

		png_write_info(png_ptr, info_ptr);


		for (int y = 0; y < height; y++) {
			png_write_row(png_ptr, (unsigned char*)&data[y*width]);
		}


		// End write
		png_write_end(png_ptr, NULL);

	finalise:
		if (fp != NULL) fclose(fp);
		if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
		if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
	}

	void Load(std::string filename) {
		FILE *fp = fopen(filename.c_str(), "rb");

		png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (!png) return;

		png_infop info = png_create_info_struct(png);
		if (!info) return;

		if (setjmp(png_jmpbuf(png))) return;

		png_init_io(png, fp);

		png_read_info(png, info);

		width = png_get_image_width(png, info);
		height = png_get_image_height(png, info);
		int color_type = png_get_color_type(png, info);
		int bit_depth = png_get_bit_depth(png, info);

		data.resize(width*height);

		// Read any color_type into 8bit depth, RGBA format.
		// See http://www.libpng.org/pub/png/libpng-manual.txt

		if (bit_depth == 16)
			png_set_strip_16(png);

		if (color_type == PNG_COLOR_TYPE_PALETTE)
			png_set_palette_to_rgb(png);

		// PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
		if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
			png_set_expand_gray_1_2_4_to_8(png);

		if (png_get_valid(png, info, PNG_INFO_tRNS))
			png_set_tRNS_to_alpha(png);

		// These color_type don't have an alpha channel then fill it with 0xff.
		if (color_type == PNG_COLOR_TYPE_RGB ||
			color_type == PNG_COLOR_TYPE_GRAY ||
			color_type == PNG_COLOR_TYPE_PALETTE)
			png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

		if (color_type == PNG_COLOR_TYPE_GRAY ||
			color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
			png_set_gray_to_rgb(png);

		png_read_update_info(png, info);

		if (png_get_rowbytes(png, info) != width * 4) {
			data.resize(data.size() + png_get_rowbytes(png, info) - width * 4);
		}

		unsigned char **row_pointers = new unsigned char*[height];
		for (int y = 0; y < height; y++) {
			row_pointers[y] = (unsigned char*)&data[y*width];
		}

		png_read_image(png, row_pointers);

		fclose(fp);
	}

private:
	std::vector<RGBA> data;
	int width, height;
};


class GrayscaleImage {
public:

	GrayscaleImage() :
		width(0), height(0) { }

	GrayscaleImage(int width, int height) :
		width(width), height(height), data(width*height) { }

	GrayscaleImage(const ColorImage &im) {
		width = im.GetWidth();
		height = im.GetHeight();
		data.resize(width*height);

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				data[x + y * width] = im(x, y).luminance();
			}
		}
	}
	int GetWidth() const { return width; }

	int GetHeight() const { return height; }

	Byte &operator()(int x, int y) {
		return data[x + y * width];
	}    

	Byte operator()(int x, int y) const {
		return data[x + y * width];
	}

	Byte Get(int x, int y) const {
		if (x < 0 || x >= width || y < 0 || y >= height) {
			return 0;
		}
		else {
			return data[x + y * width];
		}
	}
	
	void Clear() {
		for (int i = 0; i < width*height; i++) {
			data[i] = 0;
		}
	}

	void Save(std::string filename) {
		FILE *fp = NULL;
		png_structp png_ptr = NULL;
		png_infop info_ptr = NULL;

		// Open file for writing (binary mode)
		fp = fopen(filename.c_str(), "wb");
		if (fp == NULL) {
			fprintf(stderr, "Could not open file %s for writing\n", filename.c_str());
			goto finalise;
		}

		// Initialize write structure
		png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (png_ptr == NULL) {
			fprintf(stderr, "Could not allocate write struct\n");
			goto finalise;
		}

		// Initialize info structure
		info_ptr = png_create_info_struct(png_ptr);
		if (info_ptr == NULL) {
			fprintf(stderr, "Could not allocate info struct\n");
			goto finalise;
		}

		// Setup Exception handling
		if (setjmp(png_jmpbuf(png_ptr))) {
			fprintf(stderr, "Error during png creation\n");
			goto finalise;
		}

		png_init_io(png_ptr, fp);

		// Write header (8 bit colour depth)
		png_set_IHDR(png_ptr, info_ptr, width, height,
			8, PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

		png_write_info(png_ptr, info_ptr);


		for (int y = 0; y < height; y++) {
			png_write_row(png_ptr, (unsigned char*)&data[y*width]);
		}


		// End write
		png_write_end(png_ptr, NULL);

	finalise:
		if (fp != NULL) fclose(fp);
		if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
		if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
	}

	void Load(std::string filename) {
		FILE *fp = fopen(filename.c_str(), "rb");

		png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (!png) return;

		png_infop info = png_create_info_struct(png);
		if (!info) return;

		if (setjmp(png_jmpbuf(png))) return;

		png_init_io(png, fp);

		png_read_info(png, info);

		width = png_get_image_width(png, info);
		height = png_get_image_height(png, info);
		int color_type = png_get_color_type(png, info);
		int bit_depth = png_get_bit_depth(png, info);

		data.resize(width*height);



		// Read any color_type into 8bit depth, RGBA format.
		// See http://www.libpng.org/pub/png/libpng-manual.txt

		if (bit_depth == 16)
			png_set_strip_16(png);

		if (color_type == PNG_COLOR_TYPE_PALETTE)
			png_set_palette_to_rgb(png);

		// PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
		if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
			png_set_expand_gray_1_2_4_to_8(png);

		if (png_get_valid(png, info, PNG_INFO_tRNS))
			png_set_tRNS_to_alpha(png);

		if (color_type == PNG_COLOR_TYPE_RGB)
			png_set_rgb_to_gray_fixed(png, 1, -1, -1);
		else if (color_type == PNG_COLOR_TYPE_RGB_ALPHA) {
			png_set_rgb_to_gray_fixed(png, 1, -1, -1);
			png_set_strip_alpha(png);
		}

		if (color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
			png_set_strip_alpha(png);
		}

		png_read_update_info(png, info);

		if (png_get_rowbytes(png, info) != width) {
			data.resize(data.size() + png_get_rowbytes(png, info) - width);
		}

		unsigned char **row_pointers = new unsigned char*[height];
		for (int y = 0; y < height; y++) {
			row_pointers[y] = (unsigned char*)&data[y*width];
		}

		png_read_image(png, row_pointers);

		fclose(fp);
	}

private:
	std::vector<Byte> data;
	int width, height;
};

void SaveHist(const GrayscaleImage &im, std::string filename, float scale = 0.05) {
	GrayscaleImage hist(256, 512);
	std::vector<int> counts(256);

	for (int y = 0; y < im.GetHeight(); y++) {
		for (int x = 0; x < im.GetWidth(); x++) {
			counts[im(x, y)]++;
		}
	}

	for (int x = 0; x < 256; x++) {
		for (int y = 0; y < std::min<int>(512, counts[x] * scale); y++) {
			hist(x, 511 - y) = 255;
		}
	}

	hist.Save(filename);
}

void SaveHist(const ColorImage &im, std::string filename, float scale = 0.05) {
	ColorImage hist(768, 512);
	std::vector<int> counts(768);

	for (int y = 0; y < im.GetHeight(); y++) {
		for (int x = 0; x < im.GetWidth(); x++) {
			counts[im(x, y).r]++;
			counts[im(x, y).g + 256]++;
			counts[im(x, y).b + 512]++;
		}
	}

	for (int x = 0; x < 768; x++) {
		for (int y = 0; y < std::min<int>(512, counts[x] * scale); y++) {
			if (x < 256)
				hist(x, 511 - y).r = 255;
			else if (x < 512)
				hist(x, 511 - y).g = 255;
			else
				hist(x, 511 - y).b = 255;
		}
	}

	hist.Save(filename);
}

ColorImage::ColorImage(const GrayscaleImage &im) {
	width = im.GetWidth();
	height = im.GetHeight();
	data.resize(width*height);

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			data[x + y * width] = im(x, y);
		}
	}
}

int car(float val, int limit) {
	return std::clamp((int)std::round(val), 0, limit);
}
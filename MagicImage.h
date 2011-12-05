#ifndef _MAGICIMAGE_H__
#define _MAGICIMAGE_H__

#include <memory>
#include <limits>
#include <cmath>
#include "exceptions.h"
#include "types.h"


struct MagicImage
{
	typedef uint32_t dimension_t;
	dimension_t width;
	dimension_t height;
	/// stride in bytes
	ssize_t stride;
	PixelFormat pixfmt;
	const uint8_t *imageData;

	MagicImage() :
		width(0u), height(0u), stride(0),
		pixfmt(PixelFormat::NONE),
		imageData(nullptr)
	{
	}

	MagicImage(dimension_t width, dimension_t height,
		ssize_t stride, PixelFormat pixfmt, const uint8_t *data) :
		width(width), height(height), stride(stride),
		pixfmt(pixfmt), imageData(data)
	{
	}


	dimension_t GetMagicOffset()
	{
		if(this->imageData == nullptr)
			throw invalid_pointer("imageData is nullptr");
		if(this->pixfmt != PixelFormat::X8R8G8B8)
			throw not_supported("not supported pixel format");

		dimension_t offsetCand = 0;
		auto candDiffSum = std::numeric_limits<double>::max();

		for (dimension_t offsetX = 10; offsetX != this->width; offsetX ++)
		{
			long long diffSum = 0;

			for (dimension_t x = offsetX; x != this->width; x ++)
			{
				for (dimension_t y = 0; y != this->height; y ++)
				{
					diffSum += llabs((long long)this->imageData[y*this->stride + x*4 + 0]
						- (long long)this->imageData[y*this->stride + (x - offsetX)*4 + 0]);
					diffSum += llabs((long long)this->imageData[y*this->stride + x*4 + 1]
						- (long long)this->imageData[y*this->stride + (x - offsetX)*4 + 1]);
					diffSum += llabs((long long)this->imageData[y*this->stride + x*4 + 2]
						- (long long)this->imageData[y*this->stride + (x - offsetX)*4 + 2]);
				}
			}

			double curScore = (double)diffSum / (double)(this->width - offsetX);

			if(candDiffSum > curScore) {
				candDiffSum = curScore;
				offsetCand = offsetX;
			}
			if(curScore < 0.05 * this->height * 256) {
				return offsetX;
			}
		}

		return offsetCand;
	}
};

#endif

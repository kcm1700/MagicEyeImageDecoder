/*
 * Copyright (c) 2011, ChanMin Kim (kcm1700@gmail.com)
 *
 * This files is licensed under a Creative Commons license:
 * http://creativecommons.org/licenses/by-nc-sa/3.0/
 */

#ifndef _MAGICIMAGE_H__
#define _MAGICIMAGE_H__

#include <new>
#include <memory>
#include <limits>
#include <cmath>
#include "exceptions.h"
#include "types.h"


namespace MagicDecoder
{
	const int k_minimumMagicOffset = 10;
	const double k_magicOffsetTolerateRatio = 0.06;

	dimension_t GetMagicOffset(const Image *originalImage)
	{
		if(originalImage->imageData == nullptr)
			throw invalid_pointer("imageData is nullptr");
		if(originalImage->pixfmt != PixelFormat::X8R8G8B8)
			throw not_supported("not supported pixel format");

		dimension_t offsetCand = 0;
		auto candDiffSum = std::numeric_limits<double>::max();

		for (dimension_t offsetX = k_minimumMagicOffset; offsetX != originalImage->width; offsetX ++)
		{
			long long diffSum = 0;

			for (dimension_t x = offsetX; x != originalImage->width; x ++)
			{
				for (dimension_t y = 0; y != originalImage->height; y ++)
				{
					diffSum += llabs((long long)originalImage->imageData[y*originalImage->stride + x*4 + 0]
						- (long long)originalImage->imageData[y*originalImage->stride + (x - offsetX)*4 + 0]);
					diffSum += llabs((long long)originalImage->imageData[y*originalImage->stride + x*4 + 1]
						- (long long)originalImage->imageData[y*originalImage->stride + (x - offsetX)*4 + 1]);
					diffSum += llabs((long long)originalImage->imageData[y*originalImage->stride + x*4 + 2]
						- (long long)originalImage->imageData[y*originalImage->stride + (x - offsetX)*4 + 2]);
				}
			}

			double curScore = (double)diffSum / (double)(originalImage->width - offsetX);

			if(candDiffSum > curScore) {
				candDiffSum = curScore;
				offsetCand = offsetX;
			}

			if(curScore < k_magicOffsetTolerateRatio * originalImage->height * 256) {
				return offsetX;
			}
		}

		return offsetCand;
	}

	static void AllocMagicBoard(Image *dest,
		int magicOffset, int width, int height, PixelFormat pixfmt = PixelFormat::X8R8G8B8)
	{
		new (dest) Image(width - magicOffset, height, (width - magicOffset)*4, pixfmt, new BYTE[4 * (width - magicOffset) * height]);
	}

	static void FreeMagicBoard(Image *dest)
	{
		delete[] dest->imageData;
		dest->~Image();
	}
	
	void DrawMagic_LowQuality(int magicOffset, Image *originalImage, Image *drawBoard) {
		int minRelX = std::numeric_limits<int>::max(), maxRelX = std::numeric_limits<int>::min();

		if(originalImage->imageData == nullptr)
			throw invalid_pointer("imageData is nullptr");
		if(originalImage->pixfmt != PixelFormat::X8R8G8B8)
			throw not_supported("not supported pixel format");
		if(drawBoard->imageData == nullptr)
			throw invalid_pointer("imageData is nullptr");
		if(drawBoard->pixfmt != PixelFormat::X8R8G8B8)
			throw not_supported("not supported pixel format");
		if(drawBoard->width != originalImage->width - magicOffset ||
			drawBoard->height != originalImage->height)
			throw not_supported("not supported dimension");

		int height = drawBoard->height;
		int width = drawBoard->width;

		int *result = new int[width * height];

		for(int i = 0; i < height ;i ++) {
			const BYTE *curLine = &originalImage->imageData[i*originalImage->stride];
			for(int j = 0; j < width; j ++) {
				for(int C = 6 ; C >= 1; C --) {
					double minDiff = std::numeric_limits<double>::max();
					int relX = 0;
					for(int l = j + (magicOffset)*0.5; l < j + magicOffset * 1.1; l ++) {
						if(l + C >= (int)originalImage->width) break;
						int curDiff = 0;
						for(int k = 0;k < C; k ++) {
							curDiff += llabs((long long)curLine[(j+k)*4 + 0] - (long long)curLine[(l+k)*4 + 0]);
							curDiff += llabs((long long)curLine[(j+k)*4 + 1] - (long long)curLine[(l+k)*4 + 1]);
							curDiff += llabs((long long)curLine[(j+k)*4 + 2] - (long long)curLine[(l+k)*4 + 2]);
						}
						if(minDiff > curDiff + C*abs(l-j-magicOffset) / magicOffset) {
							minDiff = curDiff + C*abs(l-j-magicOffset) / magicOffset;
							relX = l-j-magicOffset;
						}
					}
					if(minDiff < C*4 || C == 4) {
						result[i*width+j] = relX;
						minRelX = std::min(minRelX, relX);
						maxRelX = std::max(maxRelX, relX);
						break;
					}
				}
			}
		}
		if(maxRelX == minRelX) maxRelX ++;
		for(int i = 0;i < height;i ++) {
			for(int j = 0; j < width; j++) {
				drawBoard->imageData[(i*drawBoard->stride+j*4)+0] = 
				drawBoard->imageData[(i*drawBoard->stride+j*4)+1] = 
				drawBoard->imageData[(i*drawBoard->stride+j*4)+2] =
					(result[i*width+j] - minRelX) * 255 / (maxRelX - minRelX);
				drawBoard->imageData[(i*drawBoard->stride+j*4)+3] = 0xFF; 
			}
		}

		delete[] result;
	}
};

#endif

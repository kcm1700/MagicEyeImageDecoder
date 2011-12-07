#ifndef _TYPES_H__
#define _TYPES_H__
#include <cstdint>
#include <bits/types.h>

typedef uint8_t BYTE;
typedef uint32_t dimension_t;

enum class PixelFormat : uint32_t
{
	NONE = 0,
	X8R8G8B8 = 1
};

struct Image
{
	dimension_t width;
	dimension_t height;
	/// stride in bytes
	ssize_t stride;
	PixelFormat pixfmt;
	BYTE *imageData;

	Image()
		: width(0), height(0), stride(0), pixfmt(PixelFormat::NONE), imageData(NULL)
	{
	}

	Image(dimension_t width, dimension_t height,
		ssize_t stride, PixelFormat pixfmt, BYTE *data)
		: width(width), height(height), stride(stride),
		pixfmt(pixfmt), imageData(data)
	{
	}
};


#endif

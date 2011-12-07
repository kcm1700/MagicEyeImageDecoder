#include <stdio.h>
#include <functional>
#include "MagicImage.h"
#include <memory>
#include <memory.h>
#include <algorithm>

int dt[1024][1024][2];

int main()
{
//#	include "testimage.h"
//#	include "magiceye3.h"
#	include "magiceyeshark.h"
	std::unique_ptr<BYTE[]> data(new BYTE[width * height * 4]);
	auto *pdata = header_data;
	for(unsigned int i = 0; i < width * height;i ++) {
		BYTE *ptr = data.get() + i*4;
		HEADER_PIXEL(pdata, ptr);
	}

	Image a(width,height,width*4,PixelFormat::X8R8G8B8,data.get());

	int magicOffset = MagicDecoder::GetMagicOffset(&a);
	fprintf(stderr, "%d\n", magicOffset);
	fprintf(stderr, "%dx%d\n",width-magicOffset, height);

	Image tmp;
	MagicDecoder::AllocMagicBoard(&tmp, magicOffset, a.width, a.height, a.pixfmt); 
	MagicDecoder::DrawMagic_LowQuality(magicOffset, &a, &tmp);
	fwrite(tmp.imageData, tmp.height, tmp.stride, stdout);
	MagicDecoder::FreeMagicBoard(&tmp); 
	return 0;
}

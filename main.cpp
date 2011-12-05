#include <stdio.h>
#include <functional>
#include "MagicImage.h"
#include <memory>
#include <memory.h>
#include <algorithm>

int main()
{
#	include "testimage.h"
	std::unique_ptr<uint8_t[]> data(new uint8_t[width * height * 4]);
	for(unsigned int i = 0; i < width * height;i ++) {
		uint8_t *ptr = data.get() + i*4;
		HEADER_PIXEL(header_data, ptr);
	}
	MagicImage a(width,height,width*4,PixelFormat::X8R8G8B8,data.get());

	int magicOffset = a.GetMagicOffset();

	fprintf(stderr, "%d\n", magicOffset);

	fprintf(stderr, "%dx%d\n",width-magicOffset, height);

	std::unique_ptr<int[]> result(new int[(width-magicOffset) * height]);
	std::unique_ptr<uint8_t[]> output(new uint8_t[(width-magicOffset) * height * 4]);

	memset(result.get(), 0, sizeof(int) * (width-magicOffset) * height);
	memset(output.get(), 0, sizeof(uint8_t) * (width-magicOffset) * height * 4);

	int minRelX = std::numeric_limits<int>::max(), maxRelX = std::numeric_limits<int>::min();

	for(unsigned int i = 0; i < height ;i ++) {
		const uint8_t *curLine = &a.imageData[i*a.stride];
		for(int j = 0; j < (int)(width) - magicOffset - 4 + 1; j ++) {
			int minDiff = std::numeric_limits<int>::max();
			int relX = 0;
			for(int l = 0; l < magicOffset && j/magicOffset*magicOffset+magicOffset+l+3 < width; l ++) {
				int curDiff = 0;
				for(int k = 0;k < 4; k ++) {
					curDiff += llabs((long long)curLine[(j+k)*4 + 0] - (long long)curLine[(j/magicOffset*magicOffset+magicOffset+l+k)*4 + 0]);
					curDiff += llabs((long long)curLine[(j+k)*4 + 1] - (long long)curLine[(j/magicOffset*magicOffset+magicOffset+l+k)*4 + 1]);
					curDiff += llabs((long long)curLine[(j+k)*4 + 2] - (long long)curLine[(j/magicOffset*magicOffset+magicOffset+l+k)*4 + 2]);
				}
				if(minDiff > curDiff) {
					minDiff = curDiff;
					relX = j % magicOffset - l;
				}
			}
			result[i*(width-magicOffset)+j] = relX;
			minRelX = std::min(minRelX, relX);
			maxRelX = std::max(maxRelX, relX);
		}
	}
	if(maxRelX == minRelX) maxRelX ++;
	for(unsigned int i = 0;i < height;i ++) {
		for(int j = 0; j < (int)(width) - magicOffset - 4 + 1; j++) {
			output[(i*(width-magicOffset)+j)*4+0] = 
			output[(i*(width-magicOffset)+j)*4+1] = 
			output[(i*(width-magicOffset)+j)*4+2] = (result[i*(width-magicOffset)+j] - minRelX) * 255 / (maxRelX - minRelX);
			output[(i*(width-magicOffset)+j)*4+3] = 0xFF; 
		}
	}

	fwrite(output.get(), height*(width-magicOffset)*4, 1, stdout);

	return 0;
}

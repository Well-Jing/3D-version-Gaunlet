#include "ffImage.h"

//#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"

ffImage* ffImage::readFromFile(const char* _fileName)
{
	int			_picType = 0;
	int			_width = 0;
	int			_height = 0;

	//stbimage读入的图片是反过来的
	//stbi_set_flip_vertically_on_load(true);

	unsigned char* bits = stbi_load(_fileName, &_width, &_height, &_picType, STBI_rgb_alpha);
	ffImage* _image = new ffImage(_width, _height, _picType, (ffRGBA*)bits);

	stbi_image_free(bits);
	return _image;
}
ffImage* ffImage::readFromMemory(unsigned char* _data, unsigned int _width, unsigned int _height)
{
	int			_picType = 0;
	int			_readWidth = 0;
	int			_readHeight = 0;
	int			_compPerPixel = 0;

	//stbimage读入的图片是反过来的
	//stbi_set_flip_vertically_on_load(true);

	int _len = 0;
	if (!_height)
	{
		_len = _width;
	}
	else
	{
		_len = _width * _height;
	}

	unsigned char* bits = stbi_load_from_memory(_data, _len, &_readWidth, &_readHeight, &_compPerPixel, STBI_rgb_alpha);
	ffImage* _image = new ffImage(_readWidth, _readHeight, _picType, (ffRGBA*)bits);

	stbi_image_free(bits);
	return _image;
}
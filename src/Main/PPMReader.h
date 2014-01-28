#pragma once

struct Texture
{
	BYTE*	data;
	int		width;
	int		height;

	Texture()
		: data(NULL), width(0), height(0)
	{};

	virtual ~Texture()
	{
		if (data != NULL)
		{
			delete[] data;
		}
	};
};

class PPMReader
{
public:
	PPMReader(void);
	~PPMReader(void);

	static int ReadFile(const char *path, Texture& texture);
//	BYTE* image;
//	int WIDTH;
//	int HEIGHT;
};

#include "stdafx.h"
#include "Thinker.h"
#include "Displayer.h"

namespace Daniel
{
	Thinker::~Thinker()
	{

	}

	Thinker::Thinker()
	{
		m_direction = eDir_Left;
		m_times = 0;
	}

	void Thinker::Think()
	{
		for (int i=0;i<REPEATS;i++)
		{
			Transition();
			Displayer::GetInstance()->DetermineCenter(m_next.x, m_next.y);	
		}
	}

	void Thinker::Think( DWORD times )
	{
		for (int i=0;i<times;i++)
		{
			Transition();
			Displayer::GetInstance()->DetermineCenter(m_next.x, m_next.y);	
		}
	}

	void Thinker::Transition()
	{
		std::vector<POINT>::iterator it = 
			std::find<std::vector<POINT>::iterator, POINT>(m_points.begin(), m_points.end(), m_next);

		if (it == m_points.end()) // turn left, and forward one step
		{
			m_points.push_back(m_next);
			switch(m_direction)
			{
			case eDir_Up:
				{
					m_direction = eDir_Left;
					m_next.x = m_next.x - 1;					
				}
				break;
			case eDir_Left:
				{
					m_direction = eDir_Down;
					m_next.y = m_next.y + 1;
				}
				break;
			case eDir_Down:
				{
					m_direction = eDir_Right;
					m_next.x = m_next.x + 1;
				}
				break;
			case eDir_Right:
				{
					m_direction = eDir_Up;
					m_next.y = m_next.y - 1;
				}
				break;
			}			
		}
		else  // turn right, and forward one step
		{
			m_points.erase(it);
			switch(m_direction)
			{
			case eDir_Up:
				{
					m_direction = eDir_Right;
					m_next.x = m_next.x + 1;
				}
				break;
			case eDir_Left:
				{
					m_direction = eDir_Up;
					m_next.y = m_next.y - 1;
				}
				break;
			case eDir_Down:
				{
					m_direction = eDir_Left;
					m_next.x = m_next.x - 1 ;
				}
				break;
			case eDir_Right:
				{
					m_direction = eDir_Down;
					m_next.y = m_next.y + 1;
				}
				break;
			}			
			
		}

		m_times++;
		printf("Transition Times: %d\t\t Black Grids: %d\n", m_times, m_points.size());
	}

	void Thinker::ThinkOnce()
	{
		Transition();
		Displayer::GetInstance()->DetermineCenter(m_next.x, m_next.y);		
	}

	void Thinker::SaveAsBitmap()
	{
		char filename[1024] = {0};
		sprintf(filename, "Langton's Ant__%d.bmp", m_times);

		int max_x = 0;
		int max_y = 0;
		int min_x = 0;
		int min_y = 0;

		for (std::vector<POINT>::iterator it = m_points.begin();
			it != m_points.end();
			it++)
		{
			max_x = max(max_x, it->x);
			min_x = min(min_x, it->x);

			max_y = max(max_y, it->y);
			min_y = min(min_y, it->y);
		}

		int gridSz = Displayer::GetInstance()->GetGridSize();

		int width = (max_x - min_x) * gridSz;
		int height = (max_y - min_y) * gridSz;

		unsigned char* pRGBBuffer = new unsigned char[width * height * 3];
		memset(pRGBBuffer, 255, width * height * 3);

		for (int i=0;i<width;i += gridSz)
		{
			for (int j=0;j<height;j += gridSz)
			{
				pRGBBuffer[i*3*width + 3*j] = 0;
				pRGBBuffer[i*3*width + 3*j + 1] = 0;
				pRGBBuffer[i*3*width + 3*j + 2] = 0;
			}
		}

		write_bmp(filename, pRGBBuffer, width, height, 24);

	}

	bool Thinker::write_bmp(const char* filename,unsigned char *pRGBBuffer,int width,int height,int ibitCount)
	{
		BITMAPFILEHEADER bmpheader;
		BITMAPINFO bmpinfo;
		FILE *fp;

		fp = fopen(filename,"wb");
		if(!fp)return false;

		bmpheader.bfType = ('M'<<8)|'B';
		bmpheader.bfReserved1 = 0;
		bmpheader.bfReserved2 = 0;
		bmpheader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		bmpheader.bfSize = bmpheader.bfOffBits + width*height*ibitCount/8;

		bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmpinfo.bmiHeader.biWidth = width;
		bmpinfo.bmiHeader.biHeight = height;
		bmpinfo.bmiHeader.biPlanes = 1;
		bmpinfo.bmiHeader.biBitCount = ibitCount;
		bmpinfo.bmiHeader.biCompression = BI_RGB;
		bmpinfo.bmiHeader.biSizeImage = 0;
		bmpinfo.bmiHeader.biXPelsPerMeter = 100;
		bmpinfo.bmiHeader.biYPelsPerMeter = 100;
		bmpinfo.bmiHeader.biClrUsed = 0;
		bmpinfo.bmiHeader.biClrImportant = 0;

		fwrite(&bmpheader,sizeof(BITMAPFILEHEADER),1,fp);
		fwrite(&bmpinfo.bmiHeader,sizeof(BITMAPINFOHEADER),1,fp);
		fwrite(pRGBBuffer,width*height*ibitCount/8,1,fp);
		fclose(fp);

		return true;
	}

}



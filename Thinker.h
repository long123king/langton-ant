#pragma once

#include <vector>
#include <algorithm>
#include <string>

#define REPEATS 20

namespace Daniel
{
	typedef struct _POINT
	{
		int x;
		int y;
		_POINT()
			:x(0)
			,y(0)
		{

		}		
	}POINT;

	inline bool operator== (const _POINT& op1, const _POINT& op2)
	{
		return (op1.x == op2.x && op1.y == op2.y);
	}

	enum DIRECTION
	{
		eDir_Up = 1,
		eDir_Left,
		eDir_Down,		
		eDir_Right
	};

	class Thinker
	{
	public:
		static Thinker* GetInstance()
		{
			static Thinker _instance;
			return &_instance;
		}

		~Thinker();

		std::vector<POINT>::iterator Begin()
		{
			return m_points.begin();
		}

		std::vector<POINT>::iterator End()
		{
			return m_points.end();
		}

		int GetNextX()
		{
			return m_next.x;
		}

		int GetNextY()
		{
			return m_next.y;
		}

		DIRECTION GetDirection()
		{
			return m_direction;
		}

		void Think();

		void Think(DWORD times);

		void ThinkOnce();

		void SaveAsBitmap();

	private:
		Thinker();

		void Transition();

		bool write_bmp(const char* filename,unsigned char *pRGBBuffer,int width,int height,int ibitCount);

	private:
		std::vector<POINT> m_points;
		DIRECTION m_direction;
		POINT m_next;
		DWORD m_times;
	};
}



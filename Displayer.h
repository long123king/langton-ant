#pragma once
#include "Thinker.h"

#define ORG_X	400
#define ORG_Y	400

namespace Daniel
{
	class Displayer
	{
	public:
		static Displayer* GetInstance()
		{
			static Displayer _instance;
			return &_instance;
		}
		~Displayer(void);

		void SetHDC(HDC hdc)
		{
			m_hdc = hdc;
		}

		void SetGridSize(DWORD dwGridSize)
		{
			m_GridSize = dwGridSize;
		}

		void SetCenter(int org_x, int org_y)
		{
			m_Org_x = org_x;
			m_Org_y = org_y;
		}

		void DetermineCenterAsAWhole(std::vector<POINT>::iterator iBegin, std::vector<POINT>::iterator iEnd);

		void DetermineCenter(int cur_x, int cur_y);

		void DrawGrid(int x, int y);

		void DrawSmallGrid(int x, int y);

		void DrawCenterGrid();

		void DrawNextGrid();

		void GetRefreshRect(RECT* lpRect);

		DWORD GetGridSize()
		{
			return m_GridSize;
		}

	private:
		Displayer(void);

	private:
		HDC m_hdc;
		int m_Org_x;
		int m_Org_y;
		DWORD m_GridSize;
		int m_lastDetermine;
	};

}



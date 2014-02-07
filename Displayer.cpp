#include "StdAfx.h"
#include "Displayer.h"
#include <math.h>

#define MAX_CENTER_DISP 10
#define MAX_CENTER_DISP_INTERVAL MAX_CENTER_DISP

#define DISPLAY_RANGE	40 * 40 * 2

namespace Daniel
{
	Displayer::Displayer(void)
	{
		m_hdc = NULL;
		m_Org_x = 0;
		m_Org_y = 0;
		m_GridSize = 0;
		m_lastDetermine = MAX_CENTER_DISP_INTERVAL;
	}


	Displayer::~Displayer(void)
	{
	}

	void Displayer::DrawGrid( int x, int y )
	{
		int disp_sum = 
			abs(x - m_Org_x) * abs(x - m_Org_x) + 
			abs(y - m_Org_y) * abs(y - m_Org_y);


		if (disp_sum > DISPLAY_RANGE)
		{
			return;
		}

		x -= m_Org_x;
		y -= m_Org_y;

		Rectangle(m_hdc, 
			ORG_X+ m_GridSize*x, 
			ORG_Y - m_GridSize*(y+1), 
			ORG_X + m_GridSize*(x+1),
			ORG_Y - m_GridSize*y);
	}

	void Displayer::DetermineCenterAsAWhole( std::vector<POINT>::iterator iBegin, std::vector<POINT>::iterator iEnd )
	{
		int max_disp_x = m_Org_x;
		int max_disp_y = m_Org_y;

		int max_disp_sum = 0;

		for(std::vector<Daniel::POINT>::iterator it = iBegin;
			it != iEnd;
			it++)
		{
			int disp_sum = 
				abs(it->x - m_Org_x) * abs(it->x - m_Org_x) + 
				abs(it->y - m_Org_y) * abs(it->x - m_Org_y);

			if (disp_sum > max_disp_sum)
			{
				max_disp_sum = disp_sum;
				max_disp_x = it->x;
				max_disp_y = it->y;
			}
		}

		if (max_disp_sum > MAX_CENTER_DISP * MAX_CENTER_DISP)
		{
			m_Org_x = (max_disp_x + m_Org_x) / 2;
			m_Org_y = (max_disp_y + m_Org_y) / 2;
		}
	}

	void Displayer::DetermineCenter( int cur_x, int cur_y )
	{
		if (--m_lastDetermine > 0)
			return;
		else
			m_lastDetermine = MAX_CENTER_DISP_INTERVAL;

		int disp_sum = 
			abs(cur_x - m_Org_x) * abs(cur_x - m_Org_x) + 
			abs(cur_y - m_Org_y) * abs(cur_y - m_Org_y);


		if (disp_sum > MAX_CENTER_DISP * MAX_CENTER_DISP)
		{
			m_Org_x = /*cur_x;//*/(cur_x + m_Org_x) / 2;
			m_Org_y = /*cur_y;//*/(cur_y + m_Org_y) / 2;
		}
	}

	void Displayer::DrawCenterGrid()
	{
		DrawGrid(m_Org_x, m_Org_y);
	}

	void Displayer::DrawNextGrid()
	{
		DrawSmallGrid(Thinker::GetInstance()->GetNextX(), Thinker::GetInstance()->GetNextY());
	}

	void Displayer::DrawSmallGrid( int x, int y )
	{
		x -= m_Org_x;
		y -= m_Org_y;

		::POINT pts[3];

		int base_x = ORG_X + m_GridSize*x;
		int base_y = ORG_Y - m_GridSize*y;

		switch (Thinker::GetInstance()->GetDirection())
		{
		case eDir_Up:
			pts[0].x = base_x;
			pts[0].y = base_y - m_GridSize;

	 		pts[1].x = base_x + m_GridSize;
			pts[1].y = base_y - m_GridSize;

			pts[2].x = base_x + m_GridSize / 2;
			pts[2].y = base_y;

			Polygon(m_hdc, pts, 3);
			break;
		case eDir_Left:
			pts[0].x = base_x + m_GridSize;
			pts[0].y = base_y;

	 		pts[1].x = base_x + m_GridSize;
			pts[1].y = base_y - m_GridSize;

			pts[2].x = base_x;
			pts[2].y = base_y - m_GridSize / 2;

			Polygon(m_hdc, pts, 3);
			break;
		case eDir_Down:
			pts[0].x = base_x;
			pts[0].y = base_y;

	 		pts[1].x = base_x + m_GridSize;
			pts[1].y = base_y;

			pts[2].x = base_x + m_GridSize / 2;
			pts[2].y = base_y - m_GridSize;

			Polygon(m_hdc, pts, 3);

			break;
		case eDir_Right:
			pts[0].x = base_x;
			pts[0].y = base_y;

	 		pts[1].x = base_x;
			pts[1].y = base_y - m_GridSize;

			pts[2].x = base_x + m_GridSize;
			pts[2].y = base_y - m_GridSize / 2;

			Polygon(m_hdc, pts, 3);
			break;
		}

	}

	void Displayer::GetRefreshRect( RECT* lpRect )
	{
		if (lpRect == NULL)
			return;

		if (m_lastDetermine == MAX_CENTER_DISP_INTERVAL)
		{
			lpRect->top = 0;
			lpRect->bottom = 800;
			lpRect->left = 0;
			lpRect->right = 800;
			return;
		}

		int disp_x = Thinker::GetInstance()->GetNextX() - m_Org_x;
		int disp_y = Thinker::GetInstance()->GetNextY() - m_Org_y;

		if (disp_x > 0 && disp_y > 0)
		{
			lpRect->top = ORG_Y - disp_y * m_GridSize;
			lpRect->left = ORG_X;
			lpRect->right = ORG_X + disp_x * m_GridSize;
			lpRect->bottom = ORG_Y;
		}
		else if (disp_x <= 0 && disp_y <= 0)
		{
			lpRect->bottom = ORG_Y - disp_y * m_GridSize;
			lpRect->right = ORG_X;
			lpRect->left = ORG_X + disp_x * m_GridSize;
			lpRect->top = ORG_Y;
		}
		else if (disp_x > 0 && disp_y <= 0)
		{
			lpRect->bottom = ORG_Y - disp_y * m_GridSize;
			lpRect->left = ORG_X;
			lpRect->right = ORG_X + disp_x * m_GridSize;
			lpRect->top = ORG_Y;
		}
		else if (disp_x <= 0 && disp_y > 0)
		{
			lpRect->top = ORG_Y - disp_y * m_GridSize;
			lpRect->right = ORG_X;
			lpRect->left = ORG_X + disp_x * m_GridSize;
			lpRect->bottom = ORG_Y;
		}

		lpRect->top -= 100;
		lpRect->left -= 100;
		lpRect->right += 100;
		lpRect->bottom += 100;
	}

}


/*!
* \file ExtendThinker.cpp
* \author Daniel King (long123king@163.com)
*/
#include "stdafx.h"
#include "ExtendThinker.h"

namespace Daniel
{
	ExtendThinker::ExtendThinker(void)
		:m_hFile(INVALID_HANDLE_VALUE)
		,m_hFileMapping(INVALID_HANDLE_VALUE)
		,m_pBase(NULL)
		,m_currentOffset(0)
		,m_curr_inner_x(50)
		,m_curr_inner_y(50)
		,m_curr_outer_x(0)
		,m_curr_outer_y(0)
		,m_delta_inner_x(0)
		,m_delta_inner_y(1)
	{

	}

	 
	ExtendThinker::~ExtendThinker(void)
	{

	}

	void ExtendThinker::initialize( const char* filename )
	{
		m_hFile = CreateFileA(filename,
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_ALWAYS,
			0,
			NULL);
		if (INVALID_HANDLE_VALUE == m_hFile)
		{
			return;
		}

		m_hFileMapping = CreateFileMapping(m_hFile,
			NULL,
			PAGE_READWRITE,
			0,
			MAX_FILE_SIZE,
			NULL);

		if (INVALID_HANDLE_VALUE == m_hFileMapping)
		{
			CloseHandle(m_hFile);
			m_hFile = INVALID_HANDLE_VALUE;
			return;
		}

		m_pBase = MapViewOfFile(m_hFileMapping,
			FILE_MAP_READ | FILE_MAP_WRITE,
			0,
			0,
			0);

		if (m_pBase == NULL)
		{
			CloseHandle(m_hFile);
			m_hFile = INVALID_HANDLE_VALUE;
			CloseHandle(m_hFileMapping);
			m_hFileMapping = INVALID_HANDLE_VALUE;
		}
	}

	void ExtendThinker::finalize()
	{
		if (m_pBase != NULL)
		{
			UnmapViewOfFile(m_pBase);
		}

		if (m_hFileMapping != INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hFileMapping);
		}

		if (m_hFile != INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hFile);
		}
	}

	Block* ExtendThinker::getBlock( long pos_x, long pos_y )
	{
		if (m_mapBlocks.find((pos_x << 16 + pos_y)) != m_mapBlocks.end())
		{
			return m_mapBlocks.at((pos_x << 16 + pos_y));
		}

		// allocate a block from file
		if ((m_currentOffset += BLOCK_SIZE) > MAX_FILE_SIZE)
		{
			printf("Severe error occurred, file size is not enough!\n");
			return NULL;
		}
		
		Block* block = new Block((unsigned char*)m_pBase + m_currentOffset);
		m_mapBlocks[(pos_x << 16 + pos_y)] = block;
		return block;
	}

	void ExtendThinker::transition()
	{
		Block* block = getBlock(m_curr_outer_x, m_curr_outer_y);
		if (block == NULL)
		{
			return;
		}

		if (block->isBitSet(m_curr_inner_x, m_curr_inner_y))
		{
			block->ClearBit(m_curr_inner_x, m_curr_inner_y);

			// rotate clockwise
			if (m_delta_inner_x == 0)
			{
				swap_negtive(m_delta_inner_x, m_delta_inner_y);
			}
			else
			{
				swap(m_delta_inner_x, m_delta_inner_y);
			}
		}
		else
		{
			block->SetBit(m_curr_inner_x, m_curr_inner_y);

			// rotate counter-clockwise
			if (m_delta_inner_x == 0)
			{
				swap(m_delta_inner_x, m_delta_inner_y);
			}
			else
			{
				swap_negtive(m_delta_inner_x, m_delta_inner_y);
			}						
		}

		m_curr_inner_x += m_delta_inner_x;
		m_curr_inner_y += m_delta_inner_y;

		// Actually, this is not going to happen at the same time
		if (!in_range(m_curr_inner_x, 0, BLOCK_LEN))
		{
			if (m_curr_inner_x < 0)
			{
				m_curr_outer_x --;
				m_curr_inner_x += BLOCK_LEN;
			}
			else
			{
				m_curr_outer_x ++;
				m_curr_inner_x -= BLOCK_LEN;
			}
		}
		else if(!in_range(m_curr_inner_y, 0, BLOCK_LEN))
		{
			if (m_curr_inner_y < 0)
			{
				m_curr_outer_y --;
				m_curr_inner_y += BLOCK_LEN;
			}
			else
			{
				m_curr_outer_y ++;
				m_curr_inner_y -= BLOCK_LEN;
			}
		}
	}

}



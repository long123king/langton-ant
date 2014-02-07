/*!
* \file ExtendThinker.h
* \brief Implement another enhanced "Thinker" class.
* \author Daniel King (long123king@163.com)
*/
#pragma once
#include <map>
#include "Windows.h"

#define MAX_DIMENSION_LEN	100
#define BLOCK_LEN			100
#define BLOCK_SIZE			(BLOCK_LEN * BLOCK_LEN / 8)
#define MAX_FILE_SIZE	(MAX_DIMENSION_LEN * MAX_DIMENSION_LEN * BLOCK_LEN * BLOCK_LEN / 8)

namespace Daniel
{
	class Block
	{
	public:
		Block(unsigned char* base)
			:m_base(base)
		{

		}
		~Block()
		{

		}

		bool isBitSet(int x, int y)
		{
			if (x < 0 ||
				x >= 100 ||
				y < 0 ||
				y >= 100)
			{
				return false;
			}

			unsigned char byte = m_base[(100 * x + y) / 8];
			
			return ( (byte >> ((100 * x + y) % 8)) == 1);
		}

		void SetBit(int x, int y)
		{
			if (x < 0 ||
				x >= 100 ||
				y < 0 ||
				y >= 100)
			{
				return;
			}

			m_base[(100 * x + y) / 8] |= ( 1 << ((100 * x + y) % 8));
		}

		void ClearBit(int x, int y)
		{
			if (x < 0 ||
				x >= 100 ||
				y < 0 ||
				y >= 100)
			{
				return;
			}

			m_base[(100 * x + y) / 8] ^= ~( 1 << ((100 * x + y) % 8));
		}
	private:
		unsigned char* m_base;
	};

	class ExtendThinker
	{
	public:
		static ExtendThinker* GetInstance()
		{
			static ExtendThinker _instance;
			return &_instance;
		}
		~ExtendThinker(void);

		void initialize(const char* filename);

		void finalize();

		void transition();

	private:
		ExtendThinker(void);

		Block* getBlock(long pos_x, long pos_y);

		void swap(long p1, long p2)
		{
			long tmp = p1;
			p1 = p2;
			p2 = tmp;
		}

		void swap_negtive(long p1, long p2)
		{
			long tmp = p1;
			p1 = -p2;
			p2 = -tmp;
		}

		/// \brief test if value in [min, max)
		/// 
		///
		bool in_range(long value, long min, long max)
		{
			return (value >= min && value < max);
		}

	private:
		HANDLE	m_hFile;
		HANDLE	m_hFileMapping;
		PVOID	m_pBase;
		std::map<long, Block*>	m_mapBlocks;
		unsigned long m_currentOffset;

		// Coordinates 
		long m_curr_inner_x;
		long m_curr_inner_y;
		long m_curr_outer_x;
		long m_curr_outer_y;

		// satisfy: (delta_x^2 + delta_y^2 == 1)
		long m_delta_inner_x;
		long m_delta_inner_y;
	};
}



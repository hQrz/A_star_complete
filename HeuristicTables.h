#pragma once
#ifndef HEURISTICTABLES_H
#define HEURISTICTABLES_H

#include<map>
#include<vector>
#include"public.h"
#define ABS(a) a=a<0?-a:a
class RMTable {
public:
	RMTable();
	~RMTable();
	void PuzzleDislocal() {
		unsigned char lower[] = { 0x00,0x01,0x02,0x03,
			0x04,0x05,0x06,0x07,
			0x08,0x09,0x0a,0x0b,
			0x0c,0x0d,0x0e,0x0f },
			upper[] = { 0x00, 0x10, 0x20, 0x30 ,
			0x40 ,0x50 ,0x60 ,0x70 ,
			0x80 ,0x90 ,0xa0 ,0xb0 ,
			0xc0 ,0xd0 ,0xe0 ,0xf0 };

		//int d[4] = { 0,0,0,0 };
		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 16; j++) {
				if (i != j) {
					for (int k = 0; k < 8; k++) {
						int count = 0;
						if (i != k * 2)
							count++;
						if (j != k * 2 + 1)
							count++;
						this->dislocal[upper[i] | lower[j]].push_back(count);
					}
				}
			}
		}
	}
	void PuzzlueManhattan() {
		unsigned char lower[] = { 0x00,0x01,0x02,0x03,
			0x04,0x05,0x06,0x07,
			0x08,0x09,0x0a,0x0b,
			0x0c,0x0d,0x0e,0x0f },
			upper[] = { 0x00, 0x10, 0x20, 0x30 ,
			0x40 ,0x50 ,0x60 ,0x70 ,
			0x80 ,0x90 ,0xa0 ,0xb0 ,
			0xc0 ,0xd0 ,0xe0 ,0xf0 };

		//int d[4] = { 0,0,0,0 };
		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 16; j++) {
				if (i != j) {
					for (int k = 0; k < 8; k++) {
						short a, b, c, d;
						a = (k % 2) * 2 - i % 4;
						b = k / 2 - i / 4;
						c = (k % 2) * 2 + 1 - j % 4;
						d = k / 2 - j / 4;
						ABS(a);
						ABS(b);
						ABS(c);
						ABS(d);
						if (i==0) {
							a = 0; b = 0;
						}
						if (j==0) {
							c = 0; d = 0;
						}
						this->dislocal[upper[i] | lower[j]].push_back((int)(a + b + c + d));
					}
				}
			}
		}
	}
	void GenTable() {
		unsigned char lower[] = { 0x00,0x01,0x02,0x03,
								  0x04,0x05,0x06,0x07,
								  0x08,0x09,0x0a,0x0b,
								  0x0c,0x0d,0x0e,0x0f },
			upper[] = { 0x00, 0x10, 0x20, 0x30 ,
						0x40 ,0x50 ,0x60 ,0x70 ,
						0x80 ,0x90 ,0xa0 ,0xb0 ,
						0xc0 ,0xd0 ,0xe0 ,0xf0 };

		//int d[4] = { 0,0,0,0 };
		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 16; j++) {
				if (i != j) {
					for (int k = 0; k < 8; k++) {
						short a, b, c, d;
						a = (k % 2) * 2 - i % 4;
						b = k / 2 - i / 4;
						c = (k % 2) * 2 + 1 - j % 4;
						d = k / 2 - j / 4;
						ABS(a);
						ABS(b);
						ABS(c);
						ABS(d);
						this->table[upper[i] | lower[j]].push_back((int)(a + b + c + d));
					}
				}
			}
		}
#if IS_PUZZLE_HEURISITC_MANHATTAN == true
		PuzzlueManhattan();
#else
		PuzzleDislocal();
#endif // IS_PUZZLE_HEURISTIC_MANHATTAN == true

		
		
	}

	uchar* GetNormalizeMapping(uchar *m) {
		uchar key = 0, val = 0;
		uchar *mapping = new uchar[16];
		for (short i = 0; i < 8; i++) {
			key = m[i] >> 4;
			mapping[key] = val;
			val++;
			key = m[i] & 0x0f;
			mapping[key] = val;
			val++;
		}
		return mapping;
	}
	void Normalize(uchar *m, uchar *mapping) {
		uchar temp = 0, val = 0;
		for (short i = 0; i < 8; i++) {
			val = 0; temp = 0;
			val = m[i] >> 4;
			val = mapping[val];
			val <<= 4;
			temp = m[i] & 0x0f;
			val = val | mapping[temp];
			m[i] = val;
		}
	}
	void BackwardNormalize(uchar *m, uchar* mapping) {
		uchar *inverse = new uchar[16];
		for (short i = 0; i < 16; i++) {
			inverse[mapping[i]] = i;
		}
		Normalize(m, inverse);
	}
	static std::map<unsigned char, std::vector<int>> table;//<val,position> -> manhattan distance
	static std::map<unsigned char, std::vector<int>> dislocal;
	//static std::map<>
};
std::map<unsigned char, std::vector<int>> RMTable::table;
std::map<unsigned char, std::vector<int>> RMTable::dislocal;
RMTable::RMTable(){
}

RMTable::~RMTable() {
}

#endif // !HEURISTICTABLES_H

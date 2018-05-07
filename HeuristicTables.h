#pragma once
#ifndef HEURISTICTABLES_H
#define HEURISTICTABLES_H

#include<map>
#include<vector>
#include "GenProblem.h"
class RMTable {
public:
	RMTable();
	~RMTable();
	void GenTable() {
		unsigned char lower[] = { 0x00,0x01,0x02,0x03,
								  0x04,0x05,0x06,0x07,
								  0x08,0x09,0x0a,0x0b,
								  0x0c,0x0d,0x0e,0x0f },
			upper[] = { 0x00, 0x10, 0x20, 0x30 ,
						0x40 ,0x50 ,0x60 ,0x70 ,
						0x80 ,0x90 ,0xa0 ,0xb0 ,
						0xc0 ,0xd0 ,0xe0 ,0xf0 };
		
		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 16; j++) {
				if (i != j) {
					for (int k = 0; k < 8; k++) {
						int p1 = k * 2 - i, p2 = k * 2 + 1 - j;
						p1 = p1 >= 0 ? p1 : -p1;
						p2 = p2 >= 0 ? p2 : -p2;
						this->table[upper[i] | lower[j]].push_back(p1 / 4 + p1 % 4 + p2 / 4 + p2 % 4);
					}
				}
			}
		}
	}
	static std::map<unsigned char, std::vector<int>> table;//<val,position> -> manhattan distance
};
std::map<unsigned char, std::vector<int>> RMTable::table;
RMTable::RMTable(){
}

RMTable::~RMTable() {
}

#endif // !HEURISTICTABLES_H

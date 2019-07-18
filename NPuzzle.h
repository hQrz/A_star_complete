#pragma once
#ifndef NPUZZLE_H
#define NPUZZLE_H

#include<iostream>
#include<vector>
#include<string>
#include"HeuristicTables.h"
#include"public.h"
#include<sstream>
#include<bitset>
#include<queue>
using namespace std;


enum PuzzleDir {
	left = 0, up = 1, right = 2, down = 3
};
class BasePuzzle {
public:
	BasePuzzle() {
		maze[0] = 0x01;	maze[1] = 0x23;
		maze[2] = 0x45;	maze[3] = 0x67;
		maze[4] = 0x89;	maze[5] = 0xab;
		maze[6] = 0xcd;	maze[7] = 0xef;
		x = 0;
		y = 0;
	}
	void initBy(BasePuzzle *r) {
		memcpy(this->maze, r->maze, sizeof(unsigned char) * 8);
		this->x = r->x;
		this->y = r->y;
	}
	virtual ~BasePuzzle() = default;
	bool Move(PuzzleDir dir) {
		switch (dir) {
		case PuzzleDir::left:
			if (this->x > 0) {
				if (x==2) {
					maze[1 + y * 2] |= (maze[y * 2] << 4);
					maze[y * 2] &= 0xf0;
				} else {
					maze[x / 2 + y * 2] >>= 4;
				}
				this->x--;
			} else
				return false; 
			break;
		case PuzzleDir::up:
			if (this->y > 0 ) {
				if (this->x == 1 || this->x == 3) {
					maze[x / 2 + y * 2] |= (maze[x / 2 + (y - 1) * 2] & 0x0f);
					maze[x / 2 + (y - 1) * 2] &= 0xf0;

				} else {
					maze[x / 2 + y * 2] |= (maze[x / 2 + (y - 1) * 2] & 0xf0);
					maze[x / 2 + (y - 1) * 2] &= 0x0f;
				}
				this->y--;
			} else
				return false; 
			break;
		case PuzzleDir::right:
			if (this->x < PUZZLE_WIDTH - 1) {
				if (x == 1) {
					maze[y * 2] |= (maze[1 + y * 2] >> 4);
					maze[1 + y * 2] &= 0x0f;
				} else {
					maze[x / 2 + y * 2] <<= 4;
				}
				this->x++;
			} else
				return false;
			break;
		case PuzzleDir::down:
			if (this->y < PUZZLE_WIDTH - 1) {
				if (this->x == 1 || this->x == 3) {
					maze[x / 2 + y * 2] |= (maze[x / 2 + (y + 1) * 2] & 0x0f);
					maze[x / 2 + (y + 1) * 2] &= 0xf0;
				} else {
					maze[x / 2 + y * 2] |= (maze[x / 2 + (y + 1) * 2] & 0xf0);
					maze[x / 2 + (y + 1) * 2] &= 0x0f;
				}
				this->y++;
			} else
				return false;
			break;
		default:
			break;
		}
		return true;
	}
	vector<std::pair<short, short>> Disturb(int steps = 10) {
		if (steps < 10) {
			steps = 5;
		} 
		vector<std::pair<short, short>> path;
		for (size_t i = 0; i < steps; ) {
			short dir = rand() % 4;
			if (this->Move((PuzzleDir)dir)) {
				path.push_back(make_pair(100, dir));
				i++;
			}
		}
		return path;
	}
	int SumOfManhattanDistance() {
		int count = 0;
		RMTable t;
		for (int i = 0; i < 8; i++) {
			count += t.dislocal[maze[i]][i];
		}
		return count;
	}
	int DisLocalNum() {
		
	}
	void initByInstance(uchar *num) {
		for (short i = 0; i < 8; i++) {
			this->maze[i] = num[i];
			if ((num[i] & 0xf0) == 0) {
				this->x = (i % 2) * 2;
				this->y = i / 2;
			} else if ((num[i] & 0x0f) == 0) {
				this->x = (i % 2) * 2 + 1;
				this->y = i / 2;
			}
		}
	}
	uchar maze[8];
	uchar x;
	uchar y;
};

class BestPuzzle :public BasePuzzle {
public:
	BestPuzzle() {
		g_val = 0;
		h_val = 0;
		father = nullptr;
	}
	~BestPuzzle() = default;

	ostringstream PrintMaze() {
		std::ostringstream sout;
		sout << "g=" << this->g_val << " h=" << this->h_val << " f=g+h=" << this->g_val + this->h_val << std::endl;
		for (int i = 0; i < 8; i += 2) {
			sout << std::hex << (short)(maze[i] >> 4) << " " << std::hex << (short)(maze[i] & 0x0f) << " " << std::hex << (short)(maze[i + 1] >> 4) << " " << std::hex << (short)(maze[i + 1] & 0x0f) << std::endl;
		}
		sout << std::endl;
		return sout;
	}
	void HeuristicActive() {
		/*int count = this->SumOfManhattanDistance();
		this->h_val = count / 2;
		if (count % 2 != 0) {
			this->h_val++;
		}*/
		this->h_val= this->SumOfManhattanDistance();
	}
	void GenChildren(queue<BestPuzzle> &q) {
		for (size_t i = 0; i < 4; i++) {
			BestPuzzle rm;
			rm.father = this;
			rm.g_val = this->g_val + 1;
			memcpy(rm.maze, this->maze, sizeof(unsigned char) * 8);
			rm.x = this->x;
			rm.y = this->y;
			if (rm.Move((PuzzleDir)i)) {
				rm.HeuristicActive();
				q.push(rm);
			}			
		}
	}
	void GenChildren(vector<BestPuzzle> &q) {
		for (size_t i = 0; i < 4; i++) {
			BestPuzzle rm;
			rm.father = this;
			rm.g_val = this->g_val + 1;
			memcpy(rm.maze, this->maze, sizeof(unsigned char) * 8);
			rm.x = this->x;
			rm.y = this->y;
			if (rm.Move((PuzzleDir)i)) {
				rm.HeuristicActive();
				q.push_back(rm);
			}
		}
	}

	unsigned short g_val;
	unsigned short h_val;
	BestPuzzle *father;
};

class FrontierPuzzle :public BasePuzzle{
public:
	FrontierPuzzle(){
		opr = 0x0f;
		g_val = 0;
		h_val = 0;
		father = nullptr;
	}
	~FrontierPuzzle() = default;
	ostringstream PrintMaze() {
		std::ostringstream sout;
		sout << "g=" << this->g_val << " h=" << this->h_val << " f=g+h=" << this->g_val + this->h_val << std::endl;
		for (int i = 0; i < 8; i += 2) {
			sout << std::hex << (short)(maze[i] >> 4) << " " << std::hex << (short)(maze[i] & 0x0f) << " " << std::hex << (short)(maze[i + 1] >> 4) << " " << std::hex << (short)(maze[i + 1] & 0x0f) << std::endl;
		}
		sout << std::endl;
		return sout;
	}
	void SetOpr(PuzzleDir dir) {
		switch (dir) {
		case PuzzleDir::left:
			this->opr &= 0xfe;
			break;
		case PuzzleDir::up:
			this->opr &= 0xfd;
			break;
		case PuzzleDir::right:
			this->opr &= 0xfb;
			break;
		case PuzzleDir::down:
			this->opr &= 0xf7;
			break;
		default:
			break;
		}
	}
	void Ancestor() {
		while (this->father->isRelayNode() == false) {
			this->father = this->father->father;
		}
	}
	bool checkOpr(short dir) {
		uchar temp = 0x01;
		temp <<= dir;
		return (bool)(this->opr & temp);
	}
	void foo() {
		if (this->x == 0)
			this->SetOpr(PuzzleDir::left);
		if (this->x == PUZZLE_WIDTH - 1)
			this->SetOpr(PuzzleDir::right);
		if (this->y == 0)
			this->SetOpr(PuzzleDir::up);
		if (this->y == PUZZLE_WIDTH - 1)
			this->SetOpr(PuzzleDir::down);
	}

	void HeuristicActive() {
		this->h_val = this->SumOfManhattanDistance();
	}
	void GenChildren(queue<FrontierPuzzle> &q) {
		this->foo();
		for (short i = 0; i < 4; i++) {
			if (checkOpr(i)) {
				FrontierPuzzle rm;
				rm.father = this;
				rm.Ancestor();
				rm.g_val = this->g_val + 1;
				memcpy(rm.maze, this->maze, sizeof(unsigned char) * 8);
				rm.x = this->x;
				rm.y = this->y;
				rm.Move((PuzzleDir)i);
				rm.HeuristicActive();
				rm.SetOpr((PuzzleDir)((i + 2) % 4));
				q.push(rm);
				this->SetOpr((PuzzleDir)i);
			}
		}
	}
	bool isRelayNode() {
		return (bool)(opr & 0x10);
	}
	bool isNone() {
		return opr == 0x00;
	}
	void SetRelayNode() {
		opr |= 0x10;
	}
	uchar opr;//0000 1111
	unsigned short g_val;
	unsigned short h_val;
	FrontierPuzzle *father;
};

class AlphaPuzzle:public BasePuzzle {
public:
	AlphaPuzzle() {
		opr = 0x0f;
		g_val = 0;
		h_val = 0;
		alpha = -1;
		father = nullptr;
	}
	~AlphaPuzzle() = default;
	void init() {
		for (short i = 0; i < 8; i++) {
			if ((this->maze[i] & 0xf0) == 0) {
				this->x = (i % 2) * 2;
				this->y = i / 2;
				return;
			} else if ((this->maze[i] & 0x0f) == 0) {
				this->x = (i % 2) * 2 + 1;
				this->y = i / 2;
				return;
			}
		}
	}
	ostringstream PrintMaze() {
		std::ostringstream sout;
		sout << "g=" << this->g_val << " h=" << this->h_val << " f=g+h=" << this->g_val + this->h_val << " alpha:" << this->alpha << std::endl;
		for (int i = 0; i < 8; i += 2) {
			sout << std::hex << (short)(maze[i] >> 4) << " " << std::hex << (short)(maze[i] & 0x0f) << " " << std::hex << (short)(maze[i + 1] >> 4) << " " << std::hex << (short)(maze[i + 1] & 0x0f) << std::endl;
		}
		sout << std::endl;
		return sout;
	}
	void SetOpr(PuzzleDir dir) {
		switch (dir) {
		case PuzzleDir::left:
			this->opr &= 0x0e;
			break;
		case PuzzleDir::up:
			this->opr &= 0x0d;
			break;
		case PuzzleDir::right:
			this->opr &= 0x0b;
			break;
		case PuzzleDir::down:
			this->opr &= 0x07;
			break;
		default:
			break;
		}
	}
	void Ancestor() {
		while (this->father->opr == false) {
			this->father = this->father->father;
		}
	}
	bool checkOpr(short dir) {
		uchar temp = 0x01;
		temp <<= dir;
		return (bool)(this->opr & temp);
	}
	void foo() {
		if (this->x == 0)
			this->SetOpr(PuzzleDir::left);
		if (this->x == PUZZLE_WIDTH - 1)
			this->SetOpr(PuzzleDir::right);
		if (this->y == 0)
			this->SetOpr(PuzzleDir::up);
		if (this->y == PUZZLE_WIDTH - 1)
			this->SetOpr(PuzzleDir::down);
	}
	void HeuristicActive() {
		this->h_val = this->SumOfManhattanDistance();
	}
	void GenChildren(std::queue<AlphaPuzzle> &q, short mid, short u) {
		this->alpha = -1;
		this->foo();
		for (short i = 0; i < 4; i++) {
			if (checkOpr(i)) {
				AlphaPuzzle rm;
				rm.g_val = this->g_val + 1;
				rm.x = this->x;
				rm.y = this->y;
				//rm->opr = this->opr;
				memcpy(rm.maze, this->maze, sizeof(unsigned char) * 8);
				rm.Move((PuzzleDir)i);
				rm.HeuristicActive();
				if (rm.g_val + rm.h_val > u) {
					if (rm.g_val + rm.h_val < alpha || alpha == -1) {
						alpha = rm.g_val + rm.h_val;
					}
					//delete rm;
					continue;
				}
				if (this->g_val == mid || this->g_val == 0) {
					rm.father = this;
				} else {
					rm.father = this->father;
				}
				this->SetOpr((PuzzleDir)i);
				rm.SetOpr((PuzzleDir)((i + 2) % 4));
				q.push(rm);
			}
		}
	}

	bool isNone() {
		return opr == 0x00;
	}
	int getShrinkCount() {
		int count = 0;
		uchar temp = this->opr;
		for (short i = 0; i < 4; i++) {
			count += (temp % 2);
			temp >>= 1;
		}
		return count;
	}
	uchar opr;
	unsigned short g_val;
	unsigned short h_val;
	short alpha;
	AlphaPuzzle *father;
};
#endif // !NPUZZLE_H

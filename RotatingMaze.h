#pragma once
#ifndef ROTATINGMAZE_H
#define ROTATINGMAZE_H

#include<iostream>
#include<vector>
#include<string>
#include"HeuristicTables.h"
#include"public.h"
#include<sstream>
#include<bitset>
enum POSITION {
	lu = 0,//left up
	u = 1, //up
	ru = 2,//right up
	l = 3, //left
	c = 4, //center
	r = 5, //right
	ld = 6,//left down
	d = 7, //down
	rd = 8 //right down
};
enum ROTA {
	CLOCK = 0, CCLOCK = 1
};
#define CORSS_TURN(a,b,c,d)\
uchar temp1 = a,temp2=d;\
a &= 0xf0; d &= 0x0f;\
a |= (c & 0x0f);\
d |= (b & 0xf0);\
c &= 0xf0; b &= 0x0f;\
c |= (temp2 >> 4);\
b |= (temp1 << 4);

class BaseMaze {
public:
	BaseMaze() {
		maze[0] = 0x01;	maze[1] = 0x23;
		maze[2] = 0x45;	maze[3] = 0x67;
		maze[4] = 0x89;	maze[5] = 0xab;
		maze[6] = 0xcd;	maze[7] = 0xef;
	}
	virtual ~BaseMaze() = default;
	void rotating(POSITION p, ROTA d) {
#if MAZE_WIDTH == 3
		if (p==POSITION::ru) {
			p = POSITION::l;
	} else if (p == POSITION::l) {
		p = POSITION::c;
	}
#endif
		switch (p) {
		case POSITION::lu:turn(d, maze[0], maze[2]); break;
		case POSITION::u:turn(d, maze[0], maze[1], maze[2], maze[3]); break;
#if MAZE_WIDTH == 4
		case POSITION::ru:turn(d, maze[1], maze[3]); break;
#endif
		case POSITION::l:turn(d, maze[2], maze[4]); break;
		case POSITION::c:turn(d, maze[2], maze[3], maze[4], maze[5]); break;
#if MAZE_WIDTH == 4
		case POSITION::r:turn(d, maze[3], maze[5]); break;
		case POSITION::ld:turn(d, maze[4], maze[6]); break;
		case POSITION::d:turn(d, maze[4], maze[5], maze[6], maze[7]); break;
		case POSITION::rd:turn(d, maze[5], maze[7]); break;
#endif
		default:break;
		}
	}
	void clockwise(uchar &a, uchar &b) {// 01 xx  _\  40 xx
		uchar temp = a;                 // 45 xx  -/  51 xx
		a >>= 4; a |= (b & 0xf0);
		b <<= 4; b |= (temp & 0x0f);
	}
	void counterclockwise(uchar &a, uchar &b) {
		uchar temp = a;
		a <<= 4; a |= (b & 0x0f);
		b >>= 4; b |= (temp & 0xf0);
	}
	void clockwise(uchar &a, uchar &b, uchar &c, uchar &d) {//a<- 01 23 ->b    _ \  05 13
		CORSS_TURN(a, b, c, d);                             //c<- 45 67 ->d    - /  46 27
	}
	void counterclockwise(uchar &a, uchar &b, uchar &c, uchar &d) {//a<- 01 23 ->b    _ \  02 63
		CORSS_TURN(c, d, a, b);                                    //c<- 45 67 ->d    - /  41 57
	}
	void turn(ROTA &dg, uchar &a, uchar &b) {
		switch (dg) {
		case CLOCK:clockwise(a, b); break;
		case CCLOCK:counterclockwise(a, b); break;
		default:break;
		}
	}
	void turn(ROTA &dg, uchar &a, uchar &b, uchar &c, uchar &d) {
		switch (dg) {
		case CLOCK:clockwise(a, b, c, d); break;
		case CCLOCK:counterclockwise(a, b, c, d); break;
		default:break;
		}
	}
	void left_up(ROTA d) {
		turn(d, maze[0], maze[2]);
	}
	void up(ROTA d) {
		turn(d, maze[0], maze[1], maze[2], maze[3]);
	}
	void right_up(ROTA d) {
		turn(d, maze[1], maze[3]);
	}
	void left(ROTA d) {
		turn(d, maze[2], maze[4]);
	}
	void center(ROTA d) {
		turn(d, maze[2], maze[3], maze[4], maze[5]);
	}
	void right(ROTA d) {
		turn(d, maze[3], maze[5]);
	}
	void left_down(ROTA d) {
		turn(d, maze[4], maze[6]);
	}
	void down(ROTA d) {
		turn(d, maze[4], maze[5], maze[6], maze[7]);
	}
	void right_down(ROTA d) {
		turn(d, maze[5], maze[7]);
	}
	int DislocationNumber(uchar *m) {
		int count = 0;
		uchar temp = 0;
		for (int i = 0; i < 8; i++) {
			temp = maze[i] ^ m[i];
			if (temp > 0) {
				if ((temp & 0xf0) > 0) {
					count += 2;
				} else {
					++count;
				}
			}
		}
		return count;
	}
	int SumOfManhattanDistance() {
		int count = 0;
		RMTable t;
		for (int i = 0; i < 8; i++) {
			count += t.table[maze[i]][i];
		}
		return count;
	}
	int MaxManhattanDistance() {
		int max = 0;
		RMTable t;
		for (int i = 0; i < 8; i++) {
			if (t.table[maze[i]][i] > max) {
				max = t.table[maze[i]][i];
			}
		}
		return max;
	}
	std::vector<std::pair<short, short>> Disturb(short steps = 0) {
#if SINGLE_TEST == 1
		std::string pst[] = { "lu","u","ru","l","c","r","ld","d","rd" }, rst[] = { "c","cc","h" };
#endif // SINGLE_TEST == 1
		std::vector<std::pair<short, short>> opr;
		while (steps <= 0) {
			steps = rand() % 10000;
		}
		for (short i = 0; i < steps; i++) {
#if MAZE_WIDTH==4
			int d = rand() % 9, r = rand() % 2;
#elif MAZE_WIDTH == 3
			int d = rand() % 4, r = rand() % 2;
#endif // MAZE_WIDTH==4
			rotating((POSITION)d, (ROTA)r);
#if SINGLE_TEST == 1
			std::cout << pst[d] << "  " << rst[r] << std::endl;
#else
			opr.push_back(std::make_pair(d, r));
#endif // SINGLE_TEST==1	
		}
		return opr;
	}
	uchar maze[8];
};

class RotatingMaze :public BaseMaze {
public:
	RotatingMaze() {
		this->father = nullptr;
		this->g_val = 0;
		this->h_val = 0;
	}
	RotatingMaze(const RotatingMaze &rm) {
		this->father = rm.father;
		this->g_val = rm.g_val;
		this->h_val = rm.h_val;
		memcpy(this->maze, rm.maze, sizeof(unsigned char) * 8);
	}
	virtual ~RotatingMaze() = default;

	std::ostringstream PrintMaze() {
		std::ostringstream sout;
		sout << "g=" << this->g_val << " h=" << this->h_val << " f=g+h=" << this->g_val + this->h_val << std::endl;
		for (int i = 0; i < 8; i += 2) {
			sout << std::hex << (short)(maze[i] >> 4) << " " << std::hex << (short)(maze[i] & 0x0f) << " " << std::hex << (short)(maze[i + 1] >> 4) << " " << std::hex << (short)(maze[i + 1] & 0x0f) << std::endl;
		}
		sout << std::endl;
		return sout;
	}
	void initBy(BaseMaze *r) {
		memcpy(this->maze, r->maze, sizeof(unsigned char) * 8);
	}
	void HeuristicActive() {
		short count = this->SumOfManhattanDistance();
		this->h_val = count / 4;
		if (count % 4 != 0) {
			this->h_val++;
		}
	}
	void GenChildren(std::queue<RotatingMaze> &q) {
#if MAZE_WIDTH==4
		for (int i = 0; i < 9; i++) {
#elif MAZE_WIDTH==3
		for (int i = 0; i < 4; i++) {
#endif // MAZE_WIDTH==4
			for (int j = 0; j < 2; j++) {
				RotatingMaze rm;
				rm.father = this;
				rm.g_val = this->g_val + 1;
				memcpy(rm.maze, this->maze, sizeof(unsigned char) * 8);
				rm.rotating((POSITION)i, (ROTA)j);
				//rm.h_val = (float)(rm.SumOfManhattanDistance() / 4.0);
				rm.HeuristicActive();
				q.push(rm);
			}
		}
	}
	void GenChildren(std::vector<RotatingMaze> &q) {
#if MAZE_WIDTH==4
		for (int i = 0; i < 9; i++) {
#elif MAZE_WIDTH==3
		for (int i = 0; i < 4; i++) {
#endif // MAZE_WIDTH==4
			for (int j = 0; j < 2; j++) {
				RotatingMaze rm;
				rm.father = this;
				rm.g_val = this->g_val + 1;
				memcpy(rm.maze, this->maze, sizeof(unsigned char) * 8);
				rm.rotating((POSITION)i, (ROTA)j);
				//rm.h_val = (float)(rm.SumOfManhattanDistance() / 4.0);
				rm.HeuristicActive();
				q.push_back(rm);
			}
		}
	}
	virtual void SetRelayNode() {
	}
//property:
	RotatingMaze *father;
	unsigned short g_val;
	unsigned short h_val;
};

class FrontierRMaze :public BaseMaze {
public:
	FrontierRMaze() {
		father = nullptr;
#if MAZE_WIDTH == 4
		opr = 0x03ffff;
#elif MAZE_WIDTH==3
		opr = 0x0000ff;
#endif // MAZE_WIDTH == 4	
		this->g_val = 0;
		this->h_val = 0;
	}
	~FrontierRMaze() {}
	virtual void resetBit(int index) {
		opr.reset(index);
	}
	bool isFrontier() {
		return opr.none();
	}
	std::ostringstream PrintMaze() {
		std::ostringstream sout;
		sout << "g=" << this->g_val << " h=" << this->h_val << " f=g+h=" << this->g_val + this->h_val << std::endl;
		for (int i = 0; i < 8; i += 2) {
			sout << std::hex << (short)(maze[i] >> 4) << " " << std::hex << (short)(maze[i] & 0x0f) << " " << std::hex << (short)(maze[i + 1] >> 4) << " " << std::hex << (short)(maze[i + 1] & 0x0f) << std::endl;
		}
		sout << std::endl;
		return sout;
	}
	void Ancestor() {
		while (this->father->opr.test(18) == false) {
			this->father = this->father->father;
		}
	}
	void initBy(BaseMaze *r) {
		memcpy(this->maze, r->maze, sizeof(unsigned char) * 8);
	}
	void HeuristicActive() {
		int count = this->SumOfManhattanDistance();
		this->h_val = count / 4;
		if (count % 4 != 0) {
			this->h_val++;
		}
	}
	void GenChildren(std::queue<FrontierRMaze> &q) {
#if MAZE_WIDTH==4
		for (int i = 0; i < 9; i++) {
#elif MAZE_WIDTH==3
		for (int i = 0; i < 4; i++) {
#endif // MAZE_WIDTH==4
			for (int j = 0; j < 2; j++) {
				if (opr.test(i * 2 + j)) {
					FrontierRMaze rm;
					rm.father = this;
					rm.Ancestor();
					rm.g_val = this->g_val + 1;
					memcpy(rm.maze, this->maze, sizeof(unsigned char) * 8);
					this->opr.reset(i * 2 + j);
					rm.rotating((POSITION)i, (ROTA)j);
					rm.opr.reset(i * 2 + (j + 1) % 2);
					rm.HeuristicActive();
					q.push(rm);
				}
			}
		}
	}
	bool isRelayNode() {
		return opr.test(18);
	}
	void SetRelayNode() {
		opr.set(18);
	}
//property:
	std::bitset<24> opr;
	FrontierRMaze *father;
	unsigned short g_val;
	unsigned short h_val;
	//0000 0000 0000 0000 0000 0000
	//____ _R r  d l  r c  l r  u l
	//____ _e d    d         u    u
};
template<typename T>
class SparseMaze:public BaseMaze {
public:
	SparseMaze() {
		this->father = nullptr;
		this->Rho = 18;
		this->g_val = 0;
		this->h_val = 0;
	}
	~SparseMaze() = default;
	std::ostringstream PrintMaze() {
		std::ostringstream sout;
		sout << "g=" << this->g_val << " h=" << this->h_val << " f=g+h=" << this->g_val + this->h_val << std::endl;
		for (int i = 0; i < 8; i += 2) {
			sout << std::hex << (short)(maze[i] >> 4) << " " << std::hex << (short)(maze[i] & 0x0f) << " " << std::hex << (short)(maze[i + 1] >> 4) << " " << std::hex << (short)(maze[i + 1] & 0x0f) << std::endl;
		}
		sout << std::endl;
		return sout;
	}
	void Ancestor() {
		while (this->father->opr.test(18) == false) {
			this->father = this->father->father;
		}
	}
	void initBy(BaseMaze *r) {
		memcpy(this->maze, r->maze, sizeof(unsigned char) * 8);
	}
	void HeuristicActive() {
		int count = this->SumOfManhattanDistance();
		this->h_val = count / 4;
		if (count % 4 != 0) {
			this->h_val++;
		}
	}
	void GenChildren(std::queue<SparseMaze> &q) {
#if MAZE_WIDTH==4
		for (int i = 0; i < 9; i++) {
#elif MAZE_WIDTH==3
		for (int i = 0; i < 4; i++) {
#endif // MAZE_WIDTH==4
			for (int j = 0; j < 2; j++) {
				SparseMaze rm;
				rm.father = this;
				rm.Ancestor();
				rm.g_val = this->g_val + 1;
				memcpy(rm.maze, this->maze, sizeof(unsigned char) * 8);
				rm.rotating((POSITION)i, (ROTA)j);
				rm.HeuristicActive();
				q.push(rm);
			}
		}
	}
	
	void SetRelayNode() {
		this->Rho = 0xff;
	}
	SparseMaze *father;
	uchar Rho;
	T g_val;
	T h_val;
};
class AlphaMaze :public BaseMaze {
public:
	AlphaMaze() {
		this->father = nullptr;
#if MAZE_WIDTH==4
		this->opr = 0x03ffff;
#elif MAZE_WIDTH==3
		this->opr = 0x0000ff;
#endif // MAZE_WIDTH==4
		this->g_val = 0;
		this->h_val = 0;
		this->alpha = -1;
	}
	AlphaMaze(const AlphaMaze& am) {
		memcpy(this->maze, am.maze, sizeof(unsigned char) * 8);
		this->father = am.father;
		this->g_val = am.g_val;
		this->h_val = am.h_val;
		this->alpha = am.alpha;
		this->opr = am.opr;
	}
	~AlphaMaze() = default;
	std::ostringstream PrintMaze() {
		std::ostringstream sout;
		sout << "g=" << this->g_val << " h=" << this->h_val << " f=g+h=" << this->g_val + this->h_val << " alpha:" << this->alpha << std::endl;
		for (int i = 0; i < 8; i += 2) {
			sout << std::hex << (short)(maze[i] >> 4) << " " << std::hex << (short)(maze[i] & 0x0f) << " " << std::hex << (short)(maze[i + 1] >> 4) << " " << std::hex << (short)(maze[i + 1] & 0x0f) << std::endl;
		}
		sout << std::endl;
		return sout;
	}
	void init() {
	
	}
	void initBy(BaseMaze *r) {
		memcpy(this->maze, r->maze, sizeof(unsigned char) * 8);
	}
	void HeuristicActive() {
		int count = this->SumOfManhattanDistance();
		this->h_val = count / 4;
		if (count % 4 != 0) {
			this->h_val++;
		}
	}
	void GenChildren(std::queue<AlphaMaze> &q, short mid, short u) {
		this->alpha = -1;
#if MAZE_WIDTH==4
		for (int i = 0; i < 9; i++) {
#elif MAZE_WIDTH==3
		for (int i = 0; i < 4; i++) {
#endif // MAZE_WIDTH==4
			for (int j = 0; j < 2; j++) {
				if (opr.test(i * 2 + j)) {
					AlphaMaze rm;
					rm.g_val = this->g_val + 1;
					memcpy(rm.maze, this->maze, sizeof(unsigned char) * 8);
					rm.rotating((POSITION)i, (ROTA)j);
					rm.HeuristicActive();
					if (rm.g_val + rm.h_val > u) {
						if (rm.g_val + rm.h_val < alpha || alpha == -1) {
							alpha = rm.g_val + rm.h_val;
						}
						continue;
					}
					if (this->g_val == mid || this->g_val == 0) {
						rm.father = this;
					} else {
						rm.father = this->father;
					}
					this->opr.reset(i * 2 + j);
					rm.opr.reset(i * 2 + (j + 1) % 2);
					q.push(rm);
				}
			}
		}
	}
	int getShrinkCount() {
		if (opr.test(18)) {
			return opr.count() - 1;
		}
		return opr.count();
	}
	bool isNone() {
		return opr.none();
	}
//property:
	std::bitset<24> opr;
	AlphaMaze *father;
	short alpha;
	unsigned short g_val;
	unsigned short h_val;
};
class Maze3 :public BaseMaze{
public:
	Maze3() {
	}
	~Maze3() = default;
	void Disturb(int times) {

	}
	virtual void GenChildren(std::queue<Maze3> &q) {
		int index[4] = { 0,1,3,4 };
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 2; j++) {
				Maze3 rm;
				memcpy(rm.maze, this->maze, sizeof(unsigned char) * 8);
				rm.rotating((POSITION)index[i], (ROTA)j);
				q.push(rm);
			}
		}
	}
};

#endif // !ROTATINGMAZE_H

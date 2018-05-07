#pragma once
#ifndef GENPROBLEM_H
#define GENPROBLEM_H

#include<random>
#include<string>

class NPuzzle {
public:
	NPuzzle();
	NPuzzle(int N);
	NPuzzle(const NPuzzle &np);
	void Init(int N);
	~NPuzzle();
	void MoveLeft() {
		p[x][y] = p[x][y - 1];
		--y;
	}
	void MoveRight() {
		p[x][y] = p[x][y + 1];
		++y;
	}
	void MoveUp() {
		p[x][y] = p[x-1][y];
		--x;
	}
	void MoveDown() {
		p[x][y] = p[x+1][y];
		++x;
	}
	unsigned char **p;
	char N;
	char x;//0(空位)的坐标
	char y;
};

NPuzzle::NPuzzle() {
	try {
		Init(3);
	} catch (std::string &e) {
		throw e;
	}
}

inline NPuzzle::NPuzzle(int N) {
	try {	
		Init(N);
	} catch (std::string &e) {
		throw e;
	}
}

inline NPuzzle::NPuzzle(const NPuzzle & np) {
	Init(np.N);
	this->x = np.x;
	this->y = np.y;
	for (int i = 0; i < this->N; ++i) {
		for (int j = 0; j < this->N; ++j) {
			this->p[i][j] = np.p[i][j];
		}
	}
}

inline void NPuzzle::Init(int N) {
	N = N <= 0 ? 3 : N;
	p = new unsigned char *[N];
	if (p == nullptr) {
		throw "内存不够";
	}
	for (int i = 0; i < N; i++) {
		p[i] = new unsigned char[N];
		if (p[i] == nullptr) {
			throw "内存不够";
		}
	}
	this->N = N;
	int index = 0;
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			p[i][j] = index++;
		}
	}
	this->x = N >= 0 ? 0 : -1;
	this->y = this->x;
}

NPuzzle::~NPuzzle() {
	for (int i = 0; i < this->N; ++i) {
		delete[] p[i];
	}
	delete[] p;
	this->x = -1;
	this->y = -1;
	this->N = -1;
}
enum DIR {
	up=0,down=1,left=2,right=3
};
NPuzzle GenRandNPuzzle(int N) {
	N = N > 0 ? N : 3;
	NPuzzle *p = new NPuzzle(N);
	unsigned int s;
	std::srand((unsigned int)&s);
	unsigned steps = rand()&0x3ff, dir = 0;
	while (steps > 0) {
		dir = rand() & 0x3;
		switch (dir) {
		case DIR::left:
			if (p->y != 0) {
				p->MoveLeft();
				--steps;
			}break;
		case DIR::right:
			if (p->y != N - 1) {
				p->MoveRight();
				--steps;
			}break;
		case DIR::up:
			if (p->x != 0) {
				p->MoveUp();
				--steps;
			}break;
		case DIR::down:
			if (p->x != N - 1) {
				p->MoveDown();
				--steps;
			}break;
		default:
			break;
		}
	}
	return *p;
}

#endif // !GENPROBLEM_H

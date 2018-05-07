#pragma once
#ifndef RUBIK_H
#define RUBIK_H
#include<memory>
#include<iostream>
#include<time.h>
#include<string>
#define RUBIK_WIDTH 3
#define uchar unsigned char
enum color {
	red,
	orange,
	green,
	blue,
	yellow,
	white
};
class flat {
public:
	static const int up = 0;
	static const int down = 1;
	static const int left = 2;
	static const int right = 3;
	static const int front = 4;
	static const int back = 5;
};
class Rubik {
public:
	Rubik();
	Rubik(const Rubik &);
	~Rubik();
	void U() {//�� ˳ʱ��
		TurnClock(flats[flat::up]);
		for (short i = 0; i < 3; ++i) {
			uchar temp = flats[flat::front][i];
			flats[flat::front][i] = flats[flat::right][i];
			flats[flat::right][i] = flats[flat::back][i];
			flats[flat::back][i] = flats[flat::left][i];
			flats[flat::left][i] = temp;
		}
	}
	void u(){//�� ��ʱ��
		TurnRevClock(flats[flat::up]);
		for (short i = 0; i < 3; ++i) {
			uchar temp = flats[flat::front][i];
			flats[flat::front][i] = flats[flat::left][i];
			flats[flat::left][i] = flats[flat::back][i];
			flats[flat::back][i] = flats[flat::right][i];
			flats[flat::right][i] = temp;
		}
	}
	void D() {//�� ˳ʱ��(�������´���ħ����)
		TurnRevClock(flats[flat::down]);
		for (short i = 6; i < 9; ++i) {
			uchar temp = flats[flat::front][i];
			flats[flat::front][i] = flats[flat::right][i];
			flats[flat::right][i] = flats[flat::back][i];
			flats[flat::back][i] = flats[flat::left][i];
			flats[flat::left][i] = temp;
		}
	}
	void d() {//�� ��ʱ��(�������´���ħ����)
		TurnClock(flats[flat::down]);
		for (short i = 6; i < 9; ++i) {
			uchar temp = flats[flat::front][i];
			flats[flat::front][i] = flats[flat::left][i];
			flats[flat::left][i] = flats[flat::back][i];
			flats[flat::back][i] = flats[flat::right][i];
			flats[flat::right][i] = temp;
		}
	}
	void L() {//�� ˳ʱ��(�������󴩹�ħ����)
		TurnRevClock(flats[flat::left]);
		uchar temp = flats[flat::front][0];
		flats[flat::front][0] = flats[flat::down][0];
		flats[flat::down][0] = flats[flat::back][8];
		flats[flat::back][8] = flats[flat::up][0];
		flats[flat::up][0] = temp;
		temp = flats[flat::front][3];
		flats[flat::front][3] = flats[flat::down][3];
		flats[flat::down][3] = flats[flat::back][5];
		flats[flat::back][5] = flats[flat::up][3];
		flats[flat::up][3] = temp;
		temp = flats[flat::front][6];
		flats[flat::front][6] = flats[flat::down][6];
		flats[flat::down][6] = flats[flat::back][2];
		flats[flat::back][2] = flats[flat::up][6];
		flats[flat::up][6] = temp;
	}
	void l() {//�� ��ʱ��(�������󴩹�ħ����)
		TurnClock(flats[flat::left]);
		uchar temp = flats[flat::front][0];
		flats[flat::front][0] = flats[flat::up][0];
		flats[flat::up][0] = flats[flat::back][8];
		flats[flat::back][8] = flats[flat::down][0];
		flats[flat::down][0] = temp;
		temp = flats[flat::front][3];
		flats[flat::front][3] = flats[flat::up][3];
		flats[flat::up][3] = flats[flat::back][5];
		flats[flat::back][5] = flats[flat::down][3];
		flats[flat::down][3] = temp;
		temp = flats[flat::front][6];
		flats[flat::front][6] = flats[flat::up][6];
		flats[flat::up][6] = flats[flat::back][2];
		flats[flat::back][2] = flats[flat::down][6];
		flats[flat::down][6] = temp;
	}
	void R() {//�� ˳ʱ��(���ҿ�)
		TurnClock(flats[flat::right]);
		uchar temp = flats[flat::front][2];
		flats[flat::front][2] = flats[flat::down][2];
		flats[flat::down][2] = flats[flat::back][6];
		flats[flat::back][6] = flats[flat::up][2];
		flats[flat::up][2] = temp;
		temp = flats[flat::front][5];
		flats[flat::front][5] = flats[flat::down][5];
		flats[flat::down][5] = flats[flat::back][3];
		flats[flat::back][3] = flats[flat::up][5];
		flats[flat::up][5] = temp;
		temp = flats[flat::front][8];
		flats[flat::front][8] = flats[flat::down][8];
		flats[flat::down][8] = flats[flat::back][0];
		flats[flat::back][0] = flats[flat::up][8];
		flats[flat::up][8] = temp;
	}
	void r() {//�� ��ʱ��(���ҿ�)
		TurnRevClock(flats[flat::right]);
		uchar temp = flats[flat::front][2];
		flats[flat::front][2] = flats[flat::up][2];
		flats[flat::up][2] = flats[flat::back][6];
		flats[flat::back][6] = flats[flat::down][2];
		flats[flat::down][2] = temp;
		temp = flats[flat::front][5];
		flats[flat::front][5] = flats[flat::up][5];
		flats[flat::up][5] = flats[flat::back][3];
		flats[flat::back][3] = flats[flat::down][5];
		flats[flat::down][5] = temp;
		temp = flats[flat::front][8];
		flats[flat::front][8] = flats[flat::up][8];
		flats[flat::up][8] = flats[flat::back][0];
		flats[flat::back][0] = flats[flat::down][8];
		flats[flat::down][8] = temp;
	}
	void F() {//ǰ ˳ʱ��
		TurnClock(flats[flat::front]);
		uchar temp = flats[flat::up][6];
		flats[flat::up][6] = flats[flat::left][8];
		flats[flat::left][8] = flats[flat::down][2];
		flats[flat::down][2] = flats[flat::right][0];
		flats[flat::right][0] = temp;
		temp = flats[flat::up][7];
		flats[flat::up][7] = flats[flat::left][5];
		flats[flat::left][5] = flats[flat::down][1];
		flats[flat::down][1] = flats[flat::right][3];
		flats[flat::right][3] = temp;
		temp = flats[flat::up][8];
		flats[flat::up][8] = flats[flat::left][2];
		flats[flat::left][2] = flats[flat::down][0];
		flats[flat::down][0] = flats[flat::right][6];
		flats[flat::right][6] = temp;
	}
	void f() {//ǰ ��ʱ��
		TurnRevClock(flats[flat::front]);
		uchar temp = flats[flat::up][6];
		flats[flat::up][6] = flats[flat::right][0];
		flats[flat::right][0] = flats[flat::down][2];
		flats[flat::down][2] = flats[flat::left][8];
		flats[flat::left][8] = temp;
		temp = flats[flat::up][7];
		flats[flat::up][7] = flats[flat::right][3];
		flats[flat::right][3] = flats[flat::down][1];
		flats[flat::down][1] = flats[flat::left][5];
		flats[flat::left][5] = temp;
		temp = flats[flat::up][8];
		flats[flat::up][8] = flats[flat::right][6];
		flats[flat::right][6] = flats[flat::down][0];
		flats[flat::down][0] = flats[flat::left][2];
		flats[flat::left][2] = temp;
	}
	void B() {//�� ˳ʱ��(��ǰ���󴩹�ħ����)
		TurnRevClock(flats[flat::back]);
		uchar temp = flats[flat::up][0];
		flats[flat::up][0] = flats[flat::left][6];
		flats[flat::left][6] = flats[flat::down][8];
		flats[flat::down][8] = flats[flat::right][2];
		flats[flat::right][2] = temp;
		temp = flats[flat::up][1];
		flats[flat::up][1] = flats[flat::left][3];
		flats[flat::left][3] = flats[flat::down][7];
		flats[flat::down][7] = flats[flat::right][5];
		flats[flat::right][5] = temp;
		temp = flats[flat::up][2];
		flats[flat::up][2] = flats[flat::left][0];
		flats[flat::left][0] = flats[flat::down][6];
		flats[flat::down][6] = flats[flat::right][8];
		flats[flat::right][8] = temp;
	}
	void b() {//�� ��ʱ��(��ǰ���󴩹�ħ����)
		TurnClock(flats[flat::back]);
		uchar temp = flats[flat::up][0];
		flats[flat::up][0] = flats[flat::right][2];
		flats[flat::right][2] = flats[flat::down][8];
		flats[flat::down][8] = flats[flat::left][6];
		flats[flat::left][6] = temp;
		temp = flats[flat::up][1];
		flats[flat::up][1] = flats[flat::right][5];
		flats[flat::right][5] = flats[flat::down][7];
		flats[flat::down][7] = flats[flat::left][3];
		flats[flat::left][3] = temp;
		temp = flats[flat::up][2];
		flats[flat::up][2] = flats[flat::right][8];
		flats[flat::right][8] = flats[flat::down][6];
		flats[flat::down][6] = flats[flat::left][0];
		flats[flat::left][0] = temp;
	}
	void TurnClock(uchar *surface) {
		uchar temp = surface[0];
		surface[0] = surface[6];
		surface[6] = surface[8];
		surface[8] = surface[2];
		surface[2] = temp;
		temp = surface[1];
		surface[1] = surface[3];
		surface[3] = surface[7];
		surface[7] = surface[5];
		surface[5] = temp;
	}
	void TurnRevClock(uchar *surface) {
		uchar temp = surface[0];
		surface[0] = surface[2];
		surface[2] = surface[8];
		surface[8] = surface[6];
		surface[6] = temp;
		temp = surface[1];
		surface[1] = surface[5];
		surface[5] = surface[7];
		surface[7] = surface[3];
		surface[3] = temp;
	}
	void PrintRubik() {
		for (short i = 0; i < 9; i += 3)
			std::cout << "     " << (short)flats[flat::up][i] << (short)flats[flat::up][i + 1] << (short)flats[flat::up][i + 2] << std::endl;
		for (short i = 0; i < 9; i += 3)
			std::cout << (short)flats[flat::left][i] << (short)flats[flat::left][i + 1] << (short)flats[flat::left][i + 2] << "  " << (short)flats[flat::front][i] << (short)flats[flat::front][i + 1] << (short)flats[flat::front][i + 2] << "  " << (short)flats[flat::right][i] << (short)flats[flat::right][i + 1] << (short)flats[flat::right][i + 2] << "  " << (short)flats[flat::back][i] << (short)flats[flat::back][i + 1] << (short)flats[flat::back][i + 2] << std::endl;
		for (short i = 0; i < 9; i += 3)
			std::cout << "     " << (short)flats[flat::down][i] << (short)flats[flat::down][i + 1] << (short)flats[flat::down][i + 2] << std::endl;
		std::cout << std::endl;
	}
	short DislocationNumber(Rubik &r) {
		short count = 0;
		for (short i = 0; i < 6; ++i) {
			for (short j = 0; j < 9; j++) {
				if (r.flats[i][j] != this->flats[i][j]) ++count;
			}
		}
		return count;
	}
	void Disturb(unsigned short steps=0) {
		srand((unsigned)time(0));
		if (steps==0) {
			unsigned short steps = rand() % 10000;
		}
		short picker = 0;
		std::string opr_str = "UuDdLlRrFfBb";
		for (unsigned short i = 0; i < steps; i++) {
			picker = rand() % 12;
			OPR(picker);
			std::cout << opr_str[picker];
		}
		std::cout << "\n";
	}
	void OPR(short &opr) {
		switch (opr) {
		case 0:this->U();break;
		case 1:this->u();break;
		case 2:this->D();break;
		case 3:this->d();break;
		case 4:this->L();break;
		case 5:this->l();break;
		case 6:this->R();break;
		case 7:this->r();break;
		case 8:this->F();break;
		case 9:this->f();break;
		case 10:this->B();break;
		case 11:this->b();break;
		default:break;
		}
	}
	bool operator <(Rubik &r)const {
		return  this->val < r.val;
	}
	bool operator >(Rubik &r)const {
		return this->val > r.val;
	}
	Rubik *father;
	uchar flats[6][9];
	short val;
	short costs;
};

Rubik::Rubik() {
	this->father = NULL;
	this->val = 0;
	this->costs = 0;
	std::memset(flats[flat::up],green,sizeof(unsigned char)*9);
	std::memset(flats[flat::down], blue, sizeof(unsigned char) * 9);
	std::memset(flats[flat::left], yellow, sizeof(unsigned char) * 9);
	std::memset(flats[flat::right], white, sizeof(unsigned char) * 9);
	std::memset(flats[flat::front], red, sizeof(unsigned char) * 9);
	std::memset(flats[flat::back], orange, sizeof(unsigned char) * 9);
}

inline Rubik::Rubik(const Rubik &r) {
	this->father = r.father;
	this->val = r.val;
	this->costs = r.costs;
	for (short i = 0; i < 6; i++) {
		std::memcpy(this->flats[i], r.flats[i], sizeof(unsigned char) * 9);
	}
}

Rubik::~Rubik() {

}

#endif // !RUBIK_H

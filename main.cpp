#include<iostream>
#include<vector>
#include<queue>
#include<list>
#include<fstream>
#include<ctime>
#include<thread>
#include<string>
#include"GenProblem.h"
#include"Rubik.h"
#include"HeuristicAlgorithms.h"
#include"RotatingMaze.h"
#include"HeuristicTables.h"
#include"public.h"
#include<functional>
using namespace std;

FrontierRMaze* toFrontier(RotatingMaze* r, int length) {
	FrontierRMaze *f = new FrontierRMaze[length];
	for (size_t i = 0; i < length; i++) {
		f[i].initBy(&r[i]);
	}
	return f;
}
template<typename T>
AlphaMaze<T>* toAlphaMaze(RotatingMaze* r, int length) {
	AlphaMaze<T> *f = new AlphaMaze<T>[length];
	for (size_t i = 0; i < length; i++) {
		f[i].initBy(&r[i]);
	}
	return f;
}
void PrintPath(queue<RotatingMaze> &path) {
	cout << "path length:" << path.size() - 1 << endl;
	while (!path.empty()) {
		RotatingMaze q = path.front();
		path.pop();
		cout << q.PrintMaze().str();
	}
}
template<typename T>
void GenSample() {
	std::string pst[] = { "lu","u","ru","l","c","r","ld","d","rd" }, rst[] = { "c","cc","h" };
	fstream fout,b_fout;
	fout.open(MAZES_FILE_NAME_TXT,ios::out);
	b_fout.open(MAZES_FILE_NAME_BIN, ios::out | ios::binary);
	T r[100];
	vector<pair<short, short>> seq;
	//int disNum[10] = { 10,20,30,50,60,80,100,120,150,200 };
	//int disNum[10] = { 4,5,6,7,8,9,10,11,12,13 };
	//int disNum[10] = { 10,10,20,20,30,30,40,50,100,200 };
	int disNum[10] = { 13,14,15,20,20,25,30,35,35,40 };
	for (size_t i = 0; i < 10; i++) {
		for (size_t k = 0; k < 10; k++) {
			seq.clear();
			seq = r[i * 10 + k].Disturb(disNum[i]);
			fout << dec << i * 10 + k << "th" << endl;
			for (size_t j = 0; j < 8; j += 2) {
				fout << hex << (short)(r[i * 10 + k].maze[j] >> 4) << " " << hex << (short)(r[i * 10 + k].maze[j] & 0x0f) << " " << (short)(r[i * 10 + k].maze[j + 1] >> 4) << " " << hex << (short)(r[i * 10 + k].maze[j + 1] & 0x0f) << endl;
			}
			for (size_t j = 0; j < seq.size(); j++) {
				fout << pst[seq[j].first] << " " << rst[seq[j].second] << "|";
			}
			fout << endl << endl;
		}
	}
	fout.close();
	b_fout.write((char*)r, sizeof(T) * 100);
	b_fout.close();
}
template<typename T>
T* ReadMazes() {
	T *buffer = new T[100];
	fstream fin(MAZES_FILE_NAME_BIN, ios::in | ios::binary);
	fin.read((char *)buffer, sizeof(T) * 100);
	fin.close();
	return buffer;
}

enum SearchType {
	IDA,BFS,FS,SMGS
};
template<typename T>
void Search(int num, T *r, function<queue<T>(T &start, T &goal, unsigned long long &total)> search_function) {
	fstream fout(experiment_file_name[num % 5], ios::out);
	if (fout.fail()) {
		throw "open file failed!";
	}
	fout.close();
	clock_t start, end;
	T g;
	unsigned long long total = 0;
	for (size_t i = num; i < 100; i += 5) {
		total = 0;
		fout.open(experiment_file_name[num % 5], ios::app);
		fout << "init "<< dec << i << ":\n" << r[i].PrintMaze().str();
		start = clock();
		queue<T> path = search_function(r[i], g, total);
		end = clock();
		fout << "time:" << (double)(end - start) / CLOCKS_PER_SEC << " s  " << "length:" << path.size() - 1 << "  search nodes:" << total << endl;
		while (!path.empty()) {
			T q = path.front();
			path.pop();
			fout << q.PrintMaze().str();
		}
		fout << endl;
		fout.close();
		cout << i << " succeed! saved in file " << num % 5 << "th" << endl;
	}
}
template<typename T>
void Experiment(T *r, function<queue<T>(T &start, T &goal, unsigned long long &total)> search_function) {
	//T *r = ReadMazes<T>();
	thread exp0(Search<T>, 0, r, search_function),
		exp1(Search<T>, 1, r, search_function),
		exp2(Search<T>, 2, r, search_function),
		exp3(Search<T>, 3, r, search_function),
		exp4(Search<T>, 4, r, search_function);
	exp0.join();
	exp1.join();
	exp2.join();
	exp3.join();
	exp4.join();
}
void IDA_Experiment() {
	function<queue<RotatingMaze>(RotatingMaze &start, RotatingMaze &goal, unsigned long long &total)> search_function = iterative_deepening_seach<RotatingMaze>;
	RotatingMaze *r = ReadMazes<RotatingMaze>();
	Experiment<RotatingMaze>(r, search_function);
}
void BFA_Experiment(){
	function<queue<RotatingMaze>(RotatingMaze &start, RotatingMaze &goal, unsigned long long &total)> search_function = best_first_search<RotatingMaze>;
	//Experiment(search_function);
	RotatingMaze *r = ReadMazes<RotatingMaze>();
	//thread exp0(Search<RotatingMaze>, 0, r, search_function);
	//exp0.join();
	Experiment<RotatingMaze>(r, search_function);
}
void FS_Experiment() {
	function<queue<FrontierRMaze>(FrontierRMaze &start, FrontierRMaze &goal, unsigned long long &total)> search_function = frontier_search<FrontierRMaze>;
	//Experiment<FrontierRMaze>(search_function);
	RotatingMaze *r = ReadMazes<RotatingMaze>();
	FrontierRMaze *f = toFrontier(r, 100);
	//thread exp0(Search<FrontierRMaze>, 0, f, search_function);
	//exp0.join();
	Experiment<FrontierRMaze>(f, search_function);
}

void FSA_Experiment() {
	function<queue<AlphaMaze<float>>(AlphaMaze<float> &start, AlphaMaze<float> &goal, unsigned long long &total)> search_function = frontier_search_with_alpha_pruning<AlphaMaze<float>>;
	RotatingMaze *r = ReadMazes<RotatingMaze>();
	AlphaMaze<float> *f = toAlphaMaze<float>(r, 100);
	thread exp0(Search<AlphaMaze<float>>, 0, f, search_function);
	exp0.join();
	//Experiment<AlphaMaze<float>>(f, search_function);
}

int main() {
	srand((unsigned)time(0));
	RMTable t; t.GenTable();
	GenSample<RotatingMaze>();
	//IDA_Experiment();
	//BFA_Experiment();
	//FS_Experiment();
	FSA_Experiment();
	//double_float_int_compare();
	//cout << sizeof(bitset<64>) << endl;
	getchar();
	return 0;
}
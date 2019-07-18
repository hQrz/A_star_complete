#include<iostream>
#include<vector>
#include<queue>
#include<list>
#include<fstream>
#include<ctime>
#include<thread>
#include<string>
#include"Rubik.h"
#include"HeuristicAlgorithms.h"
#include"RotatingMaze.h"
#include"HeuristicTables.h"
#include"public.h"
#include"NPuzzle.h"
#include<functional>
using namespace std;

template<typename T,typename Y>
T* fromBaseSamples(Y *r,int length) {
	T *f = new T[length];
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
	std::string dst[] = { "left","up","right","down" };
	fstream fout,b_fout;
	std::string filename;
#if IS_PUZZLE_SOLVER == false
#if MAZE_WIDTH==4
	filename = MAZES_4_FILE_NAME;
#elif MAZE_WIDTH==3
	filename = MAZES_3_FILE_NAME;
#endif // MAZE_WIDTH==4
#else
#if PUZZLE_WIDTH==4
	filename = PUZZLE_4_FILE_NAME;
#elif PUZZLE_WIDTH==3
	filename = PUZZLE_3_FILE_NAME;
#endif
#endif // IS_PUZZLE_SOLVER == false

	fout.open(filename+".txt",ios::out);
	b_fout.open(filename + ".bd", ios::out | ios::binary);
	T r[100];
	vector<pair<short, short>> seq;
	int disNum[10] = { 100,100,100,100,100,200,200,200,200,200 };
	//int disNum[10] = { 4,5,6,7,8,9,10,11,12,13 };
	//int disNum[10] = { 10,10,11,11,12,12,13,13,14,14 };
	//int disNum[10] = { 10,10,20,20,30,30,40,50,100,200 };
	//int disNum[10] = { 60,60,60,60,65,65,70,70,70,70 };
	for (size_t i = 0; i < 10; i++) {
		for (size_t k = 0; k < 10; k++) {
			seq.clear();
			seq = r[i * 10 + k].Disturb(disNum[i]);

			fout << dec << i * 10 + k << "th" << endl;
			for (size_t j = 0; j < 8; j += 2) {
				fout << hex << (short)(r[i * 10 + k].maze[j] >> 4) << " " << hex << (short)(r[i * 10 + k].maze[j] & 0x0f) << " " << (short)(r[i * 10 + k].maze[j + 1] >> 4) << " " << hex << (short)(r[i * 10 + k].maze[j + 1] & 0x0f) << endl;
			}
			for (size_t j = 0; j < seq.size(); j++) {
				if (seq[i].first == 100) {
					fout << dst[seq[j].second] << "|";
				} else {
					fout << pst[seq[j].first] << " " << rst[seq[j].second] << "|";
				}
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
	std::string filename;
#if IS_PUZZLE_SOLVER == false
#if MAZE_WIDTH==4
	filename = MAZES_4_FILE_NAME;
#elif MAZE_WIDTH==3
	filename = MAZES_3_FILE_NAME;
#endif // MAZE_WIDTH==4
#else
#if PUZZLE_WIDTH==4
	filename = PUZZLE_4_FILE_NAME;
#elif PUZZLE_WIDTH==3
	filename = PUZZLE_3_FILE_NAME;
#endif
#endif // IS_PUZZLE_SOLVER == false
	T *buffer = new T[100];
	fstream fin(filename+".bd", ios::in | ios::binary);
	fin.read((char *)buffer, sizeof(T) * 100);
	fin.close();
	return buffer;
}
template<typename T>
void FileSaveAs(T *r, ExpInfo *eInfo) {
	string filename;
#if IS_PUZZLE_SOLVER == false
	filename = ".\\result\\" + search_type_name[EXPERIMENT_SEARCH_TYPE] + maze_file_name[MAZE_WIDTH];
#else
	filename = ".\\result\\" + search_type_name[EXPERIMENT_SEARCH_TYPE] + puzzle_file_name[PUZZLE_WIDTH];
#endif // IS_PUZZLE_SOLVER == false

	
	fstream fout(filename+".txt", ios::out), ffout(filename + "_search_nodes.txt", ios::out),ftime(filename+"_time.txt",ios::out),flength(filename+"_length.txt",ios::out);
	if (fout.fail() || ffout.fail() || ftime.fail() || flength.fail()) {
		throw "open file failed!";
	}
	fout.close();

	fout.open(filename + ".txt", ios::app);
	for (size_t i = 0; i < 100; i++) {
		ffout << eInfo[i].total << endl;
		ftime << eInfo[i].time << endl;
		flength << eInfo[i].length << endl;
		fout << "init " << dec << i << ":\n" << r[i].PrintMaze().str();
		fout << "time:" << eInfo[i].time << " s  " << "length:" << eInfo[i].length << "  search nodes:" << eInfo[i].total << endl;
		fout << endl;
	}
	fout.close();
	ffout.close();
	ftime.close();
	flength.close();
}

template<typename T>
void Search(int num, T *r, ExpInfo *eInfo, function<queue<T>(T &start, T &goal, unsigned long long &total)> search_function) {
	string filename;
#if IS_PUZZLE_SOLVER == false
	filename = ".\\path\\" + search_type_name[EXPERIMENT_SEARCH_TYPE] + maze_file_name[MAZE_WIDTH] + experiment_file_name[num % 5];
#else
	filename = ".\\path\\" + search_type_name[EXPERIMENT_SEARCH_TYPE] + puzzle_file_name[PUZZLE_WIDTH] + experiment_file_name[num % 5];
#endif // IS_PUZZLE_SOLVER == false

	fstream fout(filename, ios::out);
	if (fout.fail()) {
		throw "open file failed!";
	}
	fout.close();
	clock_t start, end;
	T g;
	unsigned long long total = 0;
	for (size_t i = num; i < 100; i += 5) {
		total = 0;
		fout.open(filename, ios::app);
		fout << "init "<< dec << i << ":\n" << r[i].PrintMaze().str();
		cout << i << ": ";
		start = clock();
		queue<T> path = search_function(r[i], g, total);
		end = clock();
		eInfo[i].time = (float)(end - start) / CLOCKS_PER_SEC;
		eInfo[i].total = total;
		eInfo[i].length = path.size() - 1;
		fout << "time:" << eInfo[i].time << " s  " << "length:" << eInfo[i].length << "  search nodes:" << total << endl;
		
		while (!path.empty()) {
			T q = path.front();
			path.pop();
			fout << q.PrintMaze().str();
		}
		fout << endl;
		fout.close();
		cout << i << " search nodes:" << total << endl;
		cout << i << " succeed! saved in file " << num % 5 << "th" << endl;

	}
}
template<typename T>
void Experiment(T *r, ExpInfo *eInfo, function<queue<T>(T &start, T &goal, unsigned long long &total)> search_function) {
	thread exp0(Search<T>, 0, r, eInfo, search_function),
		exp1(Search<T>, 1, r, eInfo, search_function),
		exp2(Search<T>, 2, r, eInfo, search_function),
		exp3(Search<T>, 3, r, eInfo, search_function),
		exp4(Search<T>, 4, r, eInfo, search_function);
	exp0.join();
	exp1.join();
	exp2.join();
	exp3.join();
	exp4.join();
}

template<typename T>
void LinearExperiment(T *r, ExpInfo *eInfo, function<queue<T>(T &start, T &goal, unsigned long long &total)> search_function) {
	for (size_t i = 0; i < 5; i++) {
		Search<T>(i, r, eInfo, search_function);
	}
}
void IDA_Experiment() {
	ExpInfo eInfo[100];
#if IS_PUZZLE_SOLVER == false
	vector<queue<RotatingMaze>> allpath;
	function<queue<RotatingMaze>(RotatingMaze &start, RotatingMaze &goal, unsigned long long &total)> search_function = iterative_deepening_seach<RotatingMaze>;
	BaseMaze *r = ReadMazes<BaseMaze>();
	RotatingMaze *f = fromBaseSamples<RotatingMaze, BaseMaze>(r, 100);
	Experiment<RotatingMaze>(f, eInfo, search_function);
	FileSaveAs<RotatingMaze>(f, eInfo);
#else
	vector<queue<BestPuzzle>> allpath;
	function<queue<BestPuzzle>(BestPuzzle &start, BestPuzzle &goal, unsigned long long &total)> search_function = iterative_deepening_seach<BestPuzzle>;
	BasePuzzle *r = ReadMazes<BasePuzzle>();
	BestPuzzle *f = fromBaseSamples<BestPuzzle, BasePuzzle>(r, 100);
	Experiment<BestPuzzle>(f, eInfo, search_function);
	FileSaveAs<BestPuzzle>(f, eInfo);
#endif // IS_PUZZLE_SOLVER == false
}
void BFA_Experiment(){
	ExpInfo eInfo[100];
#if IS_PUZZLE_SOLVER == false
	vector<queue<RotatingMaze>> allpath;
	function<queue<RotatingMaze>(RotatingMaze &start, RotatingMaze &goal, unsigned long long &total)> search_function = best_first_search<RotatingMaze>;
	BaseMaze *r = ReadMazes<BaseMaze>();
	RotatingMaze *f = fromBaseSamples<RotatingMaze, BaseMaze>(r, 100);
	//thread exp0(Search<RotatingMaze>, 0, r, search_function);
	//exp0.join();
	LinearExperiment<RotatingMaze>(f, eInfo, search_function);
	FileSaveAs<RotatingMaze>(f, eInfo);
#else
	vector<queue<BestPuzzle>> allpath;
	function<queue<BestPuzzle>(BestPuzzle &start, BestPuzzle &goal, unsigned long long &total)> search_function = best_first_search<BestPuzzle>;
	//Experiment(search_function);
	BasePuzzle *r = ReadMazes<BasePuzzle>();
	BestPuzzle *f = fromBaseSamples<BestPuzzle, BasePuzzle>(r, 100);
	LinearExperiment<BestPuzzle>(f, eInfo, search_function);
	FileSaveAs<BestPuzzle>(f, eInfo);
#endif // IS_PUZZLE_SOLVER == false
}
void FS_Experiment() {
	ExpInfo eInfo[100];
#if IS_PUZZLE_SOLVER == false
	vector<queue<FrontierRMaze>> allpath;
	function<queue<FrontierRMaze>(FrontierRMaze &start, FrontierRMaze &goal, unsigned long long &total)> search_function = frontier_search<FrontierRMaze>;
	BaseMaze *r = ReadMazes<BaseMaze>();
	FrontierRMaze *f = fromBaseSamples<FrontierRMaze,BaseMaze>(r, 100);
	LinearExperiment<FrontierRMaze>(f, eInfo, search_function);
	FileSaveAs<FrontierRMaze>(f, eInfo);
#else
	vector<queue<FrontierPuzzle>> allpath;
	function<queue<FrontierPuzzle>(FrontierPuzzle &start, FrontierPuzzle &goal, unsigned long long &total)> search_function = frontier_search<FrontierPuzzle>;
	//Experiment<FrontierRMaze>(search_function);
	BasePuzzle *r = ReadMazes<BasePuzzle>();
	FrontierPuzzle *f = fromBaseSamples<FrontierPuzzle, BasePuzzle>(r, 100);
	LinearExperiment<FrontierPuzzle>(f, eInfo, search_function);
	FileSaveAs<FrontierPuzzle>(f, eInfo);
#endif
	
}

void FSA_Experiment() {
	ExpInfo eInfo[100];
#if IS_PUZZLE_SOLVER == false
	vector<queue<AlphaMaze>> allpath;
	function<queue<AlphaMaze>(AlphaMaze &start, AlphaMaze &goal, unsigned long long &total)> search_function = frontier_search_with_alpha_pruning<AlphaMaze>;
	RotatingMaze *r = ReadMazes<RotatingMaze>();
	AlphaMaze *f = fromBaseSamples<AlphaMaze, BaseMaze>(r, 100);
	//thread exp0(Search<AlphaMaze>, 0, f, search_function);
	//exp0.join();
	LinearExperiment<AlphaMaze>(f, eInfo, search_function);
	FileSaveAs<AlphaMaze>(f, eInfo);
#else
	vector<queue<AlphaPuzzle>> allpath;
	function<queue<AlphaPuzzle>(AlphaPuzzle &start, AlphaPuzzle &goal, unsigned long long &total)> search_function = frontier_search_with_alpha_pruning<AlphaPuzzle>;
	BasePuzzle *r = ReadMazes<BasePuzzle>();
	AlphaPuzzle *f = fromBaseSamples<AlphaPuzzle, BasePuzzle>(r, 100);
	LinearExperiment<AlphaPuzzle>(f, eInfo, search_function);
	FileSaveAs<AlphaPuzzle>(f, eInfo);
#endif
}

void BFHS_Experiment() {
	ExpInfo eInfo[100];
#if IS_PUZZLE_SOLVER == false
	vector<queue<RotatingMaze>> allpath;
	function<queue<RotatingMaze>(RotatingMaze &start, RotatingMaze &goal, unsigned long long &total)> search_function = breadth_first_heuristic_search<RotatingMaze>;
	//Experiment(search_function);
	BaseMaze *r = ReadMazes<BaseMaze>();
	RotatingMaze *f = fromBaseSamples<RotatingMaze, BaseMaze>(r, 100);
	//thread exp0(Search<RotatingMaze>, 0, r, search_function);
	//exp0.join();
	LinearExperiment<RotatingMaze>(f, eInfo, search_function);
	FileSaveAs<RotatingMaze>(f, eInfo);
#else
	vector<queue<BestPuzzle>> allpath;
	function<queue<BestPuzzle>(BestPuzzle &start, BestPuzzle &goal, unsigned long long &total)> search_function = breadth_first_heuristic_search<BestPuzzle>;
	//Experiment(search_function);
	BasePuzzle *r = ReadMazes<BasePuzzle>();
	BestPuzzle *f = fromBaseSamples<BestPuzzle, BasePuzzle>(r, 100);
	LinearExperiment<BestPuzzle>(f, eInfo, search_function);
	FileSaveAs<BestPuzzle>(f, eInfo);
#endif
}
template<typename T>
void bfs(int height, vector<size_t> layer) {
	T start;
	vector<T> open[2], close[2];
	int current = 0;
	open[current].push_back(start);
	queue<T> clist;
	layer.push_back(0);
	while (height > 0) {
		layer.push_back(open[current].size());
		while (!open[current].empty()) {
			T temp = open[current].back(); open[current].pop_back();
			temp.GenChildren(clist);
			close[current].push_back(temp);
			while (!clist.empty()) {
				T item = clist.front(); clist.pop();
				if (find<T>(open[1], item) >= 0 || find<T>(close[0], item) >= 0 || find<T>(close[1], item) >= 0 || find<T>(open[0], item) >= 0) {
					continue;
				}
				open[(current + 1) % 2].push_back(item);
			}
			
		}
		
		current = (current + 1) % 2;
		close[current].clear();
		height--;
	}
}
template<typename T>
void PrintVec(vector<T> &vec) {
	for (size_t i = 0; i < vec.size(); i++) {
		cout << i << "  :" << vec[i] << endl;
	}
}
void branch_factor() {
	vector<size_t> layer;
	bfs<Maze3>(10, layer);
	PrintVec<size_t>(layer);
}
void SaveIntances() {
	fstream fout, b_fout;
	std::string filename = "REKorf";
	BasePuzzle r[100];
	fout.open(filename + ".txt", ios::out);
	b_fout.open(filename + ".bd", ios::out | ios::binary);
	uchar num[100][8];
	memset(num, 0, sizeof(char) * 100 * 8);
	{
		fstream ins("instance.txt", ios::in);
		uchar lower[] = { 0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b, 0x0c,0x0d,0x0e,0x0f };
		short temp;
		if (ins.fail()) {
			return;
		}
		for (short i = 0; i < 100; i++) {
			for (short j = 0; j < 8; j++) {
				ins >> temp;
				if (temp > 0x0f) {
					cout << "error in line:" << i << "  " << j << endl;
				}
				num[i][j] |= (lower[temp] << 4);
				ins >> temp;
				if (temp > 0x0f) {
					cout << "error in line:" << i << "  " << j << endl;
				}
				num[i][j] |= lower[temp];
			}
		}
		ins.close();
	}
	for (short i = 0; i < 100; i++) {
		r[i].initByInstance(num[i]);
		fout << dec << i << "th" << endl;
		for (size_t j = 0; j < 8; j += 2) {
			fout << hex << (short)(r[i].maze[j] >> 4) << " " << hex << (short)(r[i].maze[j] & 0x0f) << " " << (short)(r[i].maze[j + 1] >> 4) << " " << hex << (short)(r[i].maze[j + 1] & 0x0f) << endl;
		}
		fout << "x:" << dec << (short)r[i].x << "  y:" << dec << (short)r[i].y << endl;
		fout << endl << endl;
	}
	fout.close();
	b_fout.write((char*)r, sizeof(BasePuzzle) * 100);
	b_fout.close();
}
int main() {
	srand((unsigned)time(0));
	RMTable t; t.GenTable();
#if IS_PUZZLE_SOLVER==true
	//GenSample<BasePuzzle>();
#else
	//GenSample<BaseMaze>();
#endif // IS_PUZZLE_SOLVER==true
	/*SaveIntances();*/
	cout << "RM:" << sizeof(RotatingMaze) << " FM:" << sizeof(FrontierRMaze) << " AM:" << sizeof(AlphaMaze) << endl;
	cout << "BP:" << sizeof(BestPuzzle) << " FP:" << sizeof(FrontierPuzzle) << " AP:" << sizeof(AlphaPuzzle) << endl;
	switch (EXPERIMENT_SEARCH_TYPE) {
	case 0:IDA_Experiment(); break;
	case 1:BFA_Experiment(); break;
	case 2:FS_Experiment(); break;
	case 3:BFHS_Experiment(); break;
	case 4:FSA_Experiment(); break;
	default:
		break;
	}

	//double_float_int_compare();
	//cout << sizeof(bitset<64>) << endl;
	//branch_factor();
	getchar();
	return 0;
}
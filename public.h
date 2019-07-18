#pragma once
#ifndef PUBLIC_H
#define PUBLIC_H
#include<string>
#include"RotatingMaze.h"
#include<iostream>
using namespace std;

enum SearchType {
	DFIDA = 0, BFA = 1, FA = 2, BFIDA = 3, FBPA = 4
};
const std::string search_type_name[5] = { "DFIDA_","BFA_","FA_","BFIDA_","FBPA_" };
#define uchar unsigned char
#define SINGLE_TEST 0

#define MAZES_4_FILE_NAME "mazes4"
#define MAZES_3_FILE_NAME "mazes3"
#define PUZZLE_4_FILE_NAME "puzzle4"
#define PUZZLE_3_FILE_NAME "puzzle3"
const std::string maze_file_name[5] = { "mazes0","mazes1","mazes2","mazes3","mazes4" };
const std::string puzzle_file_name[5] = { "puzzle0" ,"puzzle1" ,"puzzle2" ,"puzzle3" ,"puzzle4" };
const std::string experiment_file_name[5] = { "_experiment_0.txt" ,"_experiment_1.txt" ,"_experiment_2.txt" ,"_experiment_3.txt","_experiment_4.txt" };
#define IS_PUZZLE_HEURISITC_MANHATTAN true
#define IS_PUZZLE_SOLVER true
#define IS_REBUILT_PATH false
#define MAZE_WIDTH 4
#define PUZZLE_WIDTH 4
#define EXPERIMENT_SEARCH_TYPE 4
struct ExpInfo {
	float time;
	unsigned long long total;
	short length;
};
unsigned long long shrink_check;




#endif // !PUBLIC_H
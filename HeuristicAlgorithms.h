#pragma once
#ifndef HEURISTICALGORITHMS_H
#define HEURISTICALGORITHMS_H
#include<queue>
#include<vector>
#include"Rubik.h"
#include"RotatingMaze.h"
#include"HashTable.h"
#include<thread>
using namespace std;
#define NOT_FOUND -1
#define IS_TEST_NEW_OPEN_STRUCTURE 0
template<typename T>
class greater_class {
public:
	bool operator()(const T &a, const T &b) {
		return (a.g_val + a.h_val) > (b.g_val + b.h_val);
	}
};
template<typename T>
void path(T &goal, queue<T> &p) {
	if (goal.father == NULL) {
		p.push(goal);
		return;
	}
	path<T>(*(goal.father), p);
	p.push(goal);
}
template<typename T>
void subFind(vector<T> &l, int s, int e, T &target, int *index) {
	for (int i = s; i <= e && *index == -1; ++i) {
		if (memcmp(l[i].maze, target.maze, sizeof(unsigned char) * 8) == 0)
			*index = i; return;
	}
}
template<typename T>
int find(vector<T> &l, T &target) {//2^31*8/
	/*for (int index = l.size() - 1; index >= 0; --index) {
		if (memcmp(l[index].maze, target.maze, sizeof(unsigned char) * 8) == 0)
			return index;
	}
	return -1;*/
	//size_t count = l.size() / 4;
	int index = -1;
	subFind<T>(l, 0, (int)l.size() - 1, target, &index);
	/*if (count < 100000000) {
		subFind<T>(l, 0, l.size() - 1, target, &index);
	} else {
		thread a(subFind<T>, l, 0, count, target, &index);
		thread b(subFind<T>, l, count + 1, 2 * count, target, &index);
		thread c(subFind<T>, l, 2 * count + 1, 3 * count, target, &index);
		thread d(subFind<T>, l, 3 * count + 1, 4 * count, target, &index);
		thread e(subFind<T>, l, 4 * count + 1, l.size() - 1, target, &index);
		a.join(); b.join(); c.join(); d.join(); e.join();
	}*/
	return index;
}
template<typename T>
queue<T> best_first_search(T &start, T &goal, unsigned long long &total) {
	short temp = start.SumOfManhattanDistance();
	start.h_val = (float)(temp / 4.0);
#if IS_TEST_NEW_OPEN_STRUCTURE==1
	HashTable<T> open, close;
	open.push(start.h_val, start);
	typename list<T>::iterator foo;
#else
	vector<T> open, close;
	make_heap(open.begin(), open.end(), greater_class<T>());
	open.push_back(start);
	push_heap(open.begin(), open.end(), greater_class<T>());
#endif
	queue<T> clist;
	int index = 0;
	T *x = nullptr;
#if IS_TEST_NEW_OPEN_STRUCTURE==1
	while (open.all_size != 0) {
		//pop_heap(open.begin(), open.end(), greater_class<T>());
		//x = new T(open.back());
		//open.pop_back();
		x = new T(open.min());
		open.pop_min();
		if (x->h_val == 0) {
			queue<T> p;
			path<T>(*x, p);
			std::cout << std::dec << "OPEN.SIZE()=" << open.size() << "  CLOSE.SIZE()=" << close.size() << std::endl;
			return p;
		}
		x->GenChildren(clist);
		while (!clist.empty()) {
			T item = clist.front(); clist.pop();
			if ((index = open.find(item, foo)) != -1) {
				foo->g_val = item.g_val;
				foo->father = x;
			} else if ((index = close.close_find(item, foo)) != -1) {
				if (foo->g_val > item.g_val) {
					close.ht[index].erase(foo);
					close.all_size--;
					open.push(item.g_val + item.h_val, *new T(item));
				}
			} else {
				open.push(item.g_val + item.h_val, *new T(item));
			}
		}
		close.push(x->g_val + x->h_val, *x);
    }
#else
	while (!open.empty()) {
		pop_heap(open.begin(), open.end(), greater_class<T>());
		x = new T(open.back());
		open.pop_back();
		if (x->h_val == 0) {
			queue<T> p;
			path<T>(*x, p);
			std::cout << std::dec << "OPEN.SIZE()=" << open.size() << std::endl;
			total = open.size() + close.size();
			return p;
		}
		x->GenChildren(clist);
		while (!clist.empty()) {
			T item = clist.front(); clist.pop();
			if ((index = find<T>(open, item)) != -1) {
				if (open[index].g_val > item.g_val) {
					open[index].g_val = item.g_val;
					open[index].father = x;
					make_heap(open.begin(), open.end(), greater_class<T>());
				}
			} else if ((index = find<T>(close, item)) != -1) {
				if (close[index].g_val > item.g_val) {
					close[index] = close[close.size() - 1];
					close.pop_back();
					open.push_back(item);
					push_heap(open.begin(), open.end(), greater_class<T>());
				}
			} else {
				open.push_back(item);
				push_heap(open.begin(), open.end(), greater_class<T>());
			}
		}
		close.push_back(*x);
	}
#endif // IS_TEST_NEW_OPEN_STRUCTURE==1
	return *new queue<T>();
}
template<typename T>
unsigned long long IDAStar(queue<T> &bestpath, T &s, float &U, float &u) {
	if (s.h_val == 0) {
		bestpath.push(s); return 1;
	}
	vector<T> clist;
	s.GenChildren(clist);
	sort(clist.begin(), clist.end(), [](T &a, T &b) {return a.h_val > b.h_val; });
	unsigned long long sub = 0;
	while (!clist.empty()) {
		T c = clist.back(); clist.pop_back();
		float f = c.g_val + c.h_val;
		if (f > U) {
			if (f < u) {
				u = f;
			}
		} else {
			sub += IDAStar<T>(bestpath, c, U, u);
			if (bestpath.size() > 0) {
				bestpath.push(s); return sub + 1;
			}
		}
	}
	return sub + 18;
}

template<typename T>
queue<T> iterative_deepening_seach(T &start,T &goal,unsigned long long &total) {
	start.h_val = (float)(start.SumOfManhattanDistance() / 4.0);
	float u = start.h_val, U = 0;
	queue<T> bestpath;
	while (bestpath.empty() && u != 0) {
		U = u;
		u = 0xffff;
		total += IDAStar<T>(bestpath, start, U, u);
	}
	return bestpath;
}

/*template<typename T>
queue<T> breath_first_iterative_search(T &start, T &goal) {
	start.h_val = start.SumOfManhattanDistance() / 4;
	unsigned short u = start.h_val, U = 0;
	queue<T> fronters,clist;
	while (u != 0) {
		U = u;
		u = 0xffff;
		fronters.push(start);
		while (!fronters.empty()) {
			T node = fronters.front(); fronters.pop();
			node.GenChildren(clist);
			while (!clist.empty()) {
				T child = clist.front(); clist.pop();
				if (child.h_val == 0) {
					queue<T> p;
					path<T>(child, p);
					return p;
				}
				int f = child.g_val + child.h_val;
				if (f > U) {
					if (f < u) {
						u = f;
					}
				} else {
					fronters.push(child);
				}
			}
		}
	}
	return queue<T>();
}*/


template<typename T>
queue<T> frontier_search(T &start, T &goal, unsigned long long &total) {
	short temp = start.SumOfManhattanDistance(), mid;
	start.h_val = (float)(temp / 4.0);
	start.SetRelayNode();
	mid = (short)(start.h_val / 2);
	vector<T> open,relaynode;
	relaynode.push_back(start);
	make_heap(open.begin(), open.end(), greater_class<T>());
	open.push_back(start);
	push_heap(open.begin(), open.end(), greater_class<T>());
	queue<T> clist;
	T *x = nullptr;
	int index = 0;
	while (!open.empty()) {
		pop_heap(open.begin(), open.end(), greater_class<T>());
		x = new T(open.back());
		if (x->h_val == 0) {
			queue<T> p, p_;
			total = open.size() + relaynode.size() > total ? open.size() + relaynode.size() : total;
			if (relaynode.size() == 1) {//起始与目标为父子关系
				p.push(start);
				return p;
			}//std::cout << std::dec << "OPEN.SIZE()=" << open.size() << std::endl;
			T s1, s2, s2_, s3;
			memcpy(s1.maze, start.maze, sizeof(unsigned char) * 8);
			memcpy(s2.maze, x->father->maze, sizeof(unsigned char) * 8);
			memcpy(s2_.maze, x->father->maze, sizeof(unsigned char) * 8);
			memcpy(s3.maze, goal.maze, sizeof(unsigned char) * 8);
			RMTable rm;
			uchar *m0 = rm.GetNormalizeMapping(s2.maze), *m1 = rm.GetNormalizeMapping(s3.maze);
			rm.Normalize(s1.maze, m0); rm.Normalize(s2.maze, m0);
			p = frontier_search(s1, s2, total);
			for (size_t i = 0; i < p.size(); i++) {
				T temp = p.front(); p.pop();
				rm.BackwardNormalize(temp.maze, m0);
				p.push(temp);
			}
			rm.Normalize(s2_.maze, m1); rm.Normalize(s3.maze, m1);
			p_ = frontier_search(s2_, s3, total);
			while (!p_.empty()) {
				T temp = p_.front(); p_.pop();
				rm.BackwardNormalize(temp.maze, m1);
				p.push(temp);
			}
			return p;
		}
		if ((short)x->g_val == mid) {
			if (find<T>(relaynode, *x) == -1) {
				x->SetRelayNode();
				relaynode.push_back(*x);
			}
		}
		open.pop_back();
		x->GenChildren(clist);
		while (!clist.empty()) {
			T item = clist.front(); clist.pop();
			if ((index = find<T>(open, item)) != -1) {
				if (open[index].g_val > item.g_val) {
					open[index].g_val = item.g_val;
					open[index].father = x;
					make_heap(open.begin(), open.end(), greater_class<T>());
				}
			} else {
				open.push_back(item);
				push_heap(open.begin(), open.end(), greater_class<T>());
			}
		}
		if (x->opr.test(18))
			continue;
		delete x;
	}
	return *new queue<T>();
}
template<typename T>
queue<T> sparse_memory_graph_search(T &start, T &goal, unsigned long long &total) {

}
template<typename T>
class greater_class_alpha {
public:
	bool operator()(const T &a, const T &b) {
		return (a.g_val + a.h_val > a.alpha ? a.g_val + a.h_val : a.alpha) > (b.g_val + b.h_val > b.alpha ? b.g_val + b.h_val : b.alpha);
	}
};
template<typename T>
queue<T> frontier_search_with_alpha_pruning(T &start, T &goal, unsigned long long &total) {
	short temp = start.SumOfManhattanDistance(), mid;
	float uu = 1000;
	start.h_val = (float)(temp / 4.0);
	start.SetRelayNode();
	mid = (short)(start.h_val / 2);
	mid = mid == 0 ? 1 : mid;
	vector<T> open;
	//relaynode.push_back(start);
	make_heap(open.begin(), open.end(), greater_class_alpha<T>());
	open.push_back(start);
	push_heap(open.begin(), open.end(), greater_class_alpha<T>());
	queue<T> clist;
	T *x = nullptr;
	int index = 0;
	while (!open.empty()) {
		pop_heap(open.begin(), open.end(), greater_class_alpha<T>());
		x = new T(open.back());
		if (x->h_val == 0) {
			queue<T> p, p_;
			total = open.size() > total ? open.size() : total;
			if (x->g_val <= 1) {//起始与目标为父子关系
				p.push(start);
				return p;
			}//std::cout << std::dec << "OPEN.SIZE()=" << open.size() << std::endl;
			T s1, s2, s2_, s3;
			memcpy(s1.maze, start.maze, sizeof(unsigned char) * 8);
			memcpy(s2.maze, x->father->maze, sizeof(unsigned char) * 8);
			memcpy(s2_.maze, x->father->maze, sizeof(unsigned char) * 8);
			memcpy(s3.maze, goal.maze, sizeof(unsigned char) * 8);
			RMTable rm;
			uchar *m0 = rm.GetNormalizeMapping(s2.maze), *m1 = rm.GetNormalizeMapping(s3.maze);
			rm.Normalize(s1.maze, m0); rm.Normalize(s2.maze, m0);
			p = frontier_search_with_alpha_pruning(s1, s2, total);
			for (size_t i = 0; i < p.size(); i++) {
				T temp = p.front(); p.pop();
				rm.BackwardNormalize(temp.maze, m0);
				p.push(temp);
			}
			rm.Normalize(s2_.maze, m1); rm.Normalize(s3.maze, m1);
			p_ = frontier_search_with_alpha_pruning(s2_, s3, total);
			while (!p_.empty()) {
				T temp = p_.front(); p_.pop();
				rm.BackwardNormalize(temp.maze, m1);
				p.push(temp);
			}
			return p;
		}
		if ((short)x->g_val == mid) {
			x->SetRelayNode();
		}
		open.pop_back();
		if (open.size() > 0) {
			float temp = open.front().g_val + open.front().h_val;
			uu = temp > open.front().alpha ? temp : open.front().alpha;
		}
		x->GenChildren(clist, uu);
		while (!clist.empty()) {
			T item = clist.front(); clist.pop();
			if ((index = find<T>(open, item)) != -1) {
				if (open[index].g_val > item.g_val) {
					open[index].g_val = item.g_val;
					open[index].father = x;
					make_heap(open.begin(), open.end(), greater_class_alpha<T>());
				}
			} else {
				open.push_back(item);
				push_heap(open.begin(), open.end(), greater_class_alpha<T>());
			}
		}
		if (x->opr.none()) {
			delete x;
			continue;
		}
		open.push_back(*x);
		push_heap(open.begin(), open.end(), greater_class_alpha<T>());
	}
	return *new queue<T>();
}

#endif // !HEURISTICALGORITHMS_H

#pragma once
#ifndef HASHTABLE_H
#define HASHTABLE_H

#include<vector>
#include<queue>
#include<list>
template<typename T>
class greater_h {
public:
	bool operator()(const T a, const T b) {
		return a.h_val > b.h_val;
	}
};

template<typename T>
class HashTable {
public:
	HashTable() {
		for (size_t i = 0; i < 30; i++) {
			std::list<T> t;
			ht.push_back(t);
		}
		min_index = -1;
		all_size = 0;
	}
	~HashTable() {
		ht.clear();
	}
	void push(unsigned int index, T &val) {
		for (typename std::list<T>::iterator i = ht[index].begin(); i != ht[index].end(); i++) {
			if (i->h_val >= val.h_val) {
				ht[index].insert(i, val);
				++all_size;
				return;
			}
		}
		ht[index].push_back(val);
		++all_size;
	}
	T& min() {
		for (size_t i = 0; i < ht.size(); i++) {
			if (ht[i].size()!=0) {
				min_index = i;
				return ht[i].front();
			}
		}
	}
	void pop_min() {
		ht[min_index].pop_front();
		--all_size;
	}

	size_t size() {
		size_t s = 0;
		for (size_t i = 0; i < ht.size(); i++) {
			s += ht[i].size();
		}
		return s;
	}

	int find(T& target, typename std::list<T>::iterator &it) {
		int f = target.h_val + target.g_val;
		while (++f < ht.size()) {
			for (typename std::list<T>::iterator i = ht[f].begin(); i != ht[f].end(); ++i) {
				if (memcmp(i->maze, target.maze, sizeof(unsigned char) * 8) == 0) {
					it = i;
					return f;
				}
			}
		}
		return -1;
	}
	int close_find(T& target, typename std::list<T>::iterator &it) {
		int f = target.h_val;
		for (typename std::list<T>::iterator i = ht[f].begin(); i != ht[f].end(); ++i) {
			if (memcmp(i->maze, target.maze, sizeof(unsigned char) * 8) == 0) {
				it = i;
				return f;
			}
		}
		return -1;
	}
	std::vector<std::list<T>> ht;
	int min_index;
	size_t all_size;
};

#endif // !HASHTABLE_H

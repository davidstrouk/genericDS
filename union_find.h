#ifndef UNION_FIND_H_
#define UNION_FIND_H_

#include "team.h"
#include "group.h"

class UnionFind {
public:
	Team** _teams;
	Group** _groups;
	int _size;

	UnionFind(int n) : _size(n) {
		_teams = new Team*[_size];
		_groups = new Group*[_size];
		for(int i = 0 ; i < _size ; i++) {
			_teams[i] = new Team(i);
			_groups[i] = new Group(_teams[i]);
		}
	}

	Group* Find(int team) {
		Team* node = _teams[team];
		while(node->_father) {
			node = node->_father;
		}
		Team* root = node;
		int index = node->_number;

		node = _teams[team];
		while(node->_father) {
			Team* tmp_father = node->_father;
			node->_father = root;
			node = tmp_father;
		}

		return _groups[index];
	}

	int Union(int team1, int team2) {

		Team* root1 = _teams[team1];
		while(root1->_father) {
			root1 = root1->_father;
		}

		Team* root2 = _teams[team2];
		while(root2->_father) {
			root2 = root2->_father;
		}

		int size1 = _groups[root1->_number]->_size;
		int size2 = _groups[root2->_number]->_size;
		int trolls_number1 = _groups[root1->_number]->_trolls_number;
		int trolls_number2 = _groups[root2->_number]->_trolls_number;

		int group_name;
		if(size1 <= size2) {
			root1->_father = root2;
			_groups[root1->_number]->_root = _groups[root2->_number]->_root;
			_groups[root2->_number]->_size += size1;
			_groups[root2->_number]->_trolls_number += trolls_number1;
			root2->_group_name = root1->_group_name;
			group_name = root2->_group_name;
		} else {
			root2->_father = root1;
			_groups[root2->_number]->_root = _groups[root1->_number]->_root;
			_groups[root1->_number]->_trolls_number += trolls_number2;
			group_name = root1->_group_name;
		}

		delete _groups[root2->_group_name];
		_groups[root2->_group_name] = NULL;

		return group_name;
	}

	~UnionFind() {
		for(int i = 0 ; i < _size ; i++) {
			delete _teams[i];
			if(_groups[i]) {
				delete _groups[i];
			}
		}

		delete[] _teams;
		delete[] _groups;
	}

};



#endif /* UNION_FIND_H_ */

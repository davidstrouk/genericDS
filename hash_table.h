/*
 * hash_table.h
 *
 *  Created on: May 25, 2016
 *      Author: david_000
 */

#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#include "list.h"
#include "troll.h"

const int SIZE = 24593;

class HashTable {
	List<Troll*> table[SIZE];
public:

	HashTable() { }

	void insert(Troll* troll) {
		table[(troll->_TrollID)%SIZE].insert(troll);
	}

	Troll* search(int TrollID) {
		List<Troll*>* list = &(table[TrollID%SIZE]);
		for(List<Troll*>::iterator it = list->begin() ; it != list->end() ; ++it) {
			if((*it)->_TrollID == TrollID) {
				return *it;
			}
		}
		return NULL;
	}

	void remove(Troll* troll) {
		table[(troll->_TrollID)%SIZE].remove(troll);
	}

//	~HashTable() {
//
//		for(int i = 0 ; i < SIZE ; i++) {
//			while(table[i]._head) {
//				table[i].remove(table[i]._head->_data);
//			}
//		}
//	}

};



#endif /* HASH_TABLE_H_ */

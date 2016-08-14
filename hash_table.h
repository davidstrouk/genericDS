#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

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

};



#endif /* HASH_TABLE_H_ */

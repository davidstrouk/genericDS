/*
 * group.h
 *
 *  Created on: Jun 1, 2016
 *      Author: david_000
 */

#ifndef GROUP_H_
#define GROUP_H_

#include "troll.h"

class Group {
public:
	Team* _root;
	int _size;
	int _trolls_number;
	Troll* _strongest_troll;

	Group(Team* root) : _root(root), _size(1), _trolls_number(0),  _strongest_troll(NULL) { }

};



#endif /* GROUP_H_ */

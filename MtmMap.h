#ifndef MTMMAP_H_
#define MTMMAP_H_

#include <cassert>
#include <functional>
#include <cstdlib>
#include "Exceptions.h"

#define mapTemplate <KeyType, ValueType, CompareFunction>

namespace mtm {
/**
 * A generic map, which stores pairs formed by a combination of key value
 * and a value type, following a specific order between keys.
 * Key value is used to sort and uniquely identify the elements, while
 * the mapped value store the content associated to the key.
 */
template<class KeyType, class ValueType, class CompareFunction = std::less<
		KeyType> >
class MtmMap {
	class Node;
	Node* _head;
	ValueType _defaultValue;
	CompareFunction compare;
public:
	/**
	 * A pointer to an object in the map.
	 */
	class iterator;
	/**
	 * Couples together a pair of key and value which may be of different types.
	 */
	class Pair {
	public:
		Pair(const KeyType& key, const ValueType& value) :
				first(key), second(value) {
		}
		Pair(const Pair& pair) :
				first(pair.first), second(pair.second) {
		}
		const KeyType first;
		ValueType second;
	};
	/**
	 * Map Constructor.
	 * receives @defaultValue and creates a new map by allocating head
	 * - creating an empty linked list.
	 */
	MtmMap(const ValueType& defaultValue);
	/**
	 * Copy Constructor.
	 * creates a new allocated copy of a given map.
	 */
	MtmMap(const MtmMap& map);
	/**
	 * Destructor.
	 * de-allocates all the map's objects, and the head of the linked list.
	 */
	~MtmMap();
	/**
	 * Placement operator.
	 * de-allocates the old content of left operand, and places a new copy
	 * of the given map (right operand) to the left operand.
	 */
	MtmMap& operator=(const MtmMap& map);
	/**
	 * De-allocates all the elements that were inserted to the map.
	 * excluding head node, leaving the linked list empty but
	 * still usable.
	 */
	void clear();
	/**
	 * Allocates a new node in the linked list and inserts the given pair
	 * into the map container.
	 * uses CompareFunction criterion to locate the maximal key which is bigger
	 * than the given key, and inserts the new element before that key.
	 * IF given key already exists in the map, replaces the old value of
	 * the element with the new given value.
	 */
	void insert(Pair pair);
	/*
	 * Second version of the map that receives two parameters : a key and a value.
	 * Creates a pair from these two parameters and then inserts it according to the
	 * "original" insert function.
	 */
	void insert(const KeyType& key, const ValueType& value);
	/**
	 * Removes an element from the map.
	 * if the given key does not match any element's key in the map -
	 * throws MapElementNotFoundException.
	 * if the given key is found, the element is removed from the map.
	 */
	void remove(const KeyType& key);
	/**
	 * Returns an iterator to the element with the requested key.
	 * if no such element was found - throws MapElementNotFoundException.
	 */
	iterator find(const KeyType& key) const;
	/**
	 * Returns an iterator to the first element in the map.
	 */
	iterator begin() const;
	/**
	 * Returns an iterator to tail of the linked list (no data).
	 */
	iterator end() const;
	/**
	 * Returns a boolean value stating if the given key exists in an element
	 * in the map or not.
	 */
	bool containsKey(const KeyType& key) const;
	/**
	 * Returns the amount of elements in the map.
	 */
	unsigned int size() const;
	/**
	 * Returns a reference to the value of the element according to the key.
	 */
	ValueType& operator[](const KeyType& key);
	/**
	 * Returns a const reference to the value of the element according to the
	 * key (for const maps).
	 */
	const ValueType& operator[](const KeyType& key) const;
};

template<class KeyType, class ValueType, class CompareFunction>
class MtmMap mapTemplate::Node {
	Pair _data;
	Node* _next;
	friend class MtmMap;
public:
	Node(Pair& data, Node* next = NULL) : _data(data), _next(next) {};
};

template<class KeyType, class ValueType, class CompareFunction>
class MtmMap mapTemplate::iterator {
	const MtmMap* _map;
	Node* _current;
	friend class MtmMap;
public:
	/** Explicit constructor, disables casting of the iterator.
	 * receives a @current and a @map and creates an iterator to that node
	 * which belongs to the given map.
	 */
	explicit iterator(const MtmMap<KeyType,ValueType,CompareFunction>* map = NULL, Node* current = NULL)
	: _map(map), _current(current) {}
	/** Copy constructor,
	 * receives a @iterator and creates an iterator strictly
	 * identical to the parameter
	 */
	iterator(const iterator& it)
	: _map(it._map), _current(it._current) {}
	/**
	 * Placement operator. sets an iterator (left operand) to point to
	 * the same element as right operand iterator.
	 */
	iterator& operator=(const iterator& it) = default;
	/**
	 * First promote an iterator to point to the next element in the container
	 * and then allows placements.
	 */
	iterator& operator++() {
		if(*this == _map->end()) {
			return *this;
		}
		_current = _current->_next;
		return *this;
	}
	/**
	 * Point to the current element in the container and then promote it.
	 */
	iterator operator++(int) {
		iterator tmp_iter = *this;
		++(*this);
		return tmp_iter;
	}
	/**
	 * Dereferencing of a pointer, returning a const reference to the data
	 * saved in the node of the element pointed by the iterator.
	 * throws MapElementNotFoundException if iterator is pointing to the
	 * end of the container.
	 */
	Pair& operator*() const {
		if(*this == _map->end()) {
			throw MapElementNotFoundException();
		}
		return _current->_data;
	}
	/**
	 * Compares iterators, first by checking if they belong to the same map,
	 * then checking if both iterators point to the same element inside the map.
	 */
	bool operator==(const iterator& iterator) const {
		return (_map == iterator._map && _current == iterator._current);
	}
	/**
	 * Compares iterators, returning the opposite value returned by
	 * operator == (in other words if iterators are different).
	 */
	bool operator!=(const iterator& iterator) const {
		return !(*this == iterator);
	}
};

template<class KeyType, class ValueType, class CompareFunction>
MtmMap mapTemplate::MtmMap(const ValueType& defaultValue)
: _head(NULL), _defaultValue(defaultValue) {}

template<class KeyType, class ValueType, class CompareFunction>
MtmMap mapTemplate::MtmMap(const MtmMap& map) : _defaultValue(map._defaultValue), compare(map.compare) {
	_head = NULL;
	iterator it = map.begin();
	for(unsigned int i = 0; i < map.size(); i++) {
		insert(it._current->_data);
		it++;
	}
}

template<class KeyType, class ValueType, class CompareFunction>
MtmMap mapTemplate::~MtmMap() {
	clear();
}

template<class KeyType, class ValueType, class CompareFunction>
MtmMap mapTemplate& MtmMap<KeyType, ValueType,
CompareFunction>::operator=(const MtmMap& map) {
	if(_head == map._head) {
		return *this;
	}
	clear();
	for(iterator it = map.begin(); it != map.end(); it++) {
		insert(it._current->_data);
	}
	_defaultValue = map._defaultValue;
	compare = map.compare;
	return *this;
}

template<class KeyType, class ValueType, class CompareFunction>
typename MtmMap mapTemplate::iterator MtmMap<KeyType,
ValueType, CompareFunction>::find(const KeyType &key) const {
	for(MtmMap::iterator it = begin(); it != end(); it++) {
		if(!compare(it._current->_data.first, key)
				&& !compare(key, it._current->_data.first)) {
			return it;
		}
	}
	return end();
}

template<class KeyType, class ValueType, class CompareFunction>
void MtmMap mapTemplate::insert(Pair pair) {
	iterator it = begin();
	// edge case : if size is empty then insert first
	if(size() == 0) {
		_head = new Node(pair, NULL);
		return;
	}
	int count = 0;
	while(it != end() && compare(it._current->_data.first, pair.first)) {
		it++;
		count++;
	}
	if(it != end()
			&& !compare(it._current->_data.first, pair.first)
			&& !compare(pair.first, it._current->_data.first)) {
		it._current->_data.second = pair.second;
	} else {
		Node* toInsert = new Node(pair, _head);
		//edge case: the iterator is pointing on the head of the list
		if (it._current == _head) {
			_head = toInsert;
			return;
		}
		iterator tmp_iterator = begin();
		for(int i = 0; i < count - 1; i++) {
			tmp_iterator++;
		}
		tmp_iterator._current->_next = toInsert;
		toInsert->_next = it._current;
	}
}

template<class KeyType, class ValueType, class CompareFunction>
void MtmMap mapTemplate::insert(const KeyType& key, const ValueType& value) {
	Pair pair = Pair(key, value);
	insert(pair);
}

template<class KeyType, class ValueType, class CompareFunction>
void MtmMap mapTemplate::remove(const KeyType& key) {
	if(!containsKey(key)) {
		throw MapElementNotFoundException();
	}
	iterator it = begin();
	while(it != end() && it._current->_data.first != key) {
		it++;
	}
	if (it._current == _head) {
		_head = it._current->_next;
	} else {
		iterator tmp_iterator = begin();
		while (tmp_iterator._current->_next != it._current) {
			tmp_iterator++;
		}
		tmp_iterator._current->_next = it._current->_next;
	}
	delete it._current;
}

template<class KeyType, class ValueType, class CompareFunction>
void MtmMap mapTemplate::clear() {
	while(size() > 0) {
		iterator it = begin();
		remove(it._current->_data.first);
	}
}

template<class KeyType, class ValueType, class CompareFunction>
typename MtmMap mapTemplate::iterator MtmMap<KeyType,
ValueType, CompareFunction>::begin() const {
	return iterator(this, _head);
}

template<class KeyType, class ValueType, class CompareFunction>
typename MtmMap mapTemplate::iterator MtmMap<KeyType,
ValueType, CompareFunction>::end() const {
	return iterator(this, NULL);
}

template<class KeyType, class ValueType, class CompareFunction>
bool MtmMap mapTemplate::containsKey(const KeyType& key) const {
	iterator iterator = begin();
	while(iterator != end()) {
		if(!compare(iterator._current->_data.first, key)
				&& !compare(key, iterator._current->_data.first)) {
			return true;
		}
		iterator++;
	}
	return false;
}

template<class KeyType, class ValueType, class CompareFunction>
unsigned int MtmMap mapTemplate::size() const {
	iterator iterator = begin();
	unsigned int count = 0;
	while(iterator != end()) {
		iterator++;
		count++;
	}
	return count;
}

template<class KeyType, class ValueType, class CompareFunction>
ValueType& MtmMap mapTemplate::operator[](const KeyType& key) {
	if(!containsKey(key)) {
		insert(key, _defaultValue);
	}
	return find(key)._current->_data.second;
}

template<class KeyType, class ValueType, class CompareFunction>
const ValueType& MtmMap mapTemplate::operator[](const KeyType& key) const {
	if(containsKey(key)) {
		return find(key)._current->_data.second;
	}
	return _defaultValue;
}

}

#endif

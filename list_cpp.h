#ifndef LIST_H_
#define LIST_H_

#include <cstdlib>

#define listTemplate <DataType>

template<class DataType >

class List {
	class Node;
	Node* _head;
	DataType _defaultValue;
public:
	class iterator;
	List();
	void insert(DataType data);
	~List();

	iterator begin() const {
		return iterator(this, _head);
	}

	iterator end() const {
		return iterator(this, NULL);
	}
};

template<class DataType>
class List listTemplate::Node {
	DataType _data;
	Node* _next;
	friend class List;
public:
	Node(DataType& data, Node* next = NULL) : _data(data), _next(next) {};
};

template<class DataType>
List listTemplate::List()
: _head(NULL) {}

template<class DataType>
void List listTemplate::insert(DataType data) {

	if(!_head) {
		_head = new Node(data, NULL);
		return;
	}

	Node* temp = _head;
	Node* toInsert = new Node(data, NULL);
	while(temp->_next!=NULL) {
		temp = temp->_next;
	}

	temp->_next = toInsert;

}

template<class DataType>
List listTemplate::~List() {
	while(_head) {
		Node* temp = _head->_next;
		delete _head;
		_head = temp;
	}
}

template<class DataType>
class List listTemplate::iterator {
	const List* _list;
	Node* _current;
	friend class List;
public:
	explicit iterator(const List<DataType>* list = NULL, Node* current = NULL) : _list(list), _current(current) { }
	iterator(const iterator& it) : _list(it._list), _current(it._current) { }
	iterator& operator=(const iterator& it) = default;
	bool operator==(const iterator& iterator) const {
		return (_list == iterator._list && _current == iterator._current);
	}
	bool operator!=(const iterator& iterator) const {
		return !(*this == iterator);
	}
	iterator& operator++() {
		if(*this == _list->end()) {
			return *this;
		}
		_current = _current->_next;
		return *this;
	}
	DataType& operator*() const {
		return _current->_data;
	}
};


#endif /* LIST_H_ */

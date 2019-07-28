#pragma once

template<class T>
class List {
	template<class T>
	struct node {
		node<T>* next;
		node<T>* prev;
		T data;
	};
	node<T> *first;
	node<T> *last;
	int num;
public:
	List() {
		first = last = nullptr;
		num = 0;
	}
	~List() {
		node<T>* n1 = begin();
		while (n1 != nullptr) {
			node<T>* n2 = n1->next;
			delete n1->data;
			delete n1;
			n1 = n2;
		}
	}
	node<T>* begin() { return first; }
	node<T>* end() { return nullptr; }
	int size() { return num; }
	void erase(node<T>* n) {
		if (n == nullptr) return;
		if (n == first) {
			first = n->next;
		}
		else {
			n->prev->next = n->next;
		}
		if (n == last) {
			last = n->prev;
		}
		else {
			n->next->prev = n->prev;
		}
		delete n;
		n = nullptr;
		--num;
	}
	void push_back(T _content) {
		node<T> *n = new node<T>;
		n->data = _content;
		if (first != nullptr) {
			n->prev = last;
			last->next = n;
			last = n;
		}
		else {
			first = last = n;
			n->prev = nullptr;
		}
		n->next = nullptr;
		++num;
	}
	node<T>* find(T _content) {
		node<T>* n = begin();
		while (n != end()) {
			if (n->data == _content) return n;
			n = n->next;
		}
		return nullptr;
	}
	void operator+=(T c) { push_back(c); }
	void operator-=(T c) { erase(find(c)); }
};
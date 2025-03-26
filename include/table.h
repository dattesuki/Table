//UNN
//Litvyakov D. D.
//

#pragma once
#include <vector>


//interface for table classes
template<typename type>
class TableInterface {
protected:
public:
	virtual bool empty() = 0;
	virtual void clear() = 0;
	virtual bool insert(size_t key, type val) = 0;
	virtual bool erase(size_t key) = 0;
	virtual size_t size() = 0;
	
protected:
	TableInterface() {}
	class iterator;

};


//ancestor class for tables on a vector
template<typename type>
class TableByArray: public TableInterface<type>{
protected:
	using pair = std::pair < size_t, type>;
	using vector = std::vector<std::pair<size_t, type>>;
	TableByArray() {};
	vector mem;
public:
	bool empty() override {return mem.empty();}
	size_t size() override { return mem.size(); }
	void clear() override { mem.clear(); }
};


//unsorted tables class
template<typename type>
class UnsortedTable : public TableByArray<type> {
protected:
	using pair = std::pair < size_t, type>;
	using vector = std::vector<std::pair<size_t, type>>;
public:
	UnsortedTable() {}

	bool insert(size_t key, type val) override {
		for (size_t i = 0; i < TableByArray<type>::mem.size(); ++i) {
			if (TableByArray<type>::mem[i].first == key) return false;
		}
		TableByArray<type>::mem.push_back(pair(key,val));
		return true;
	}

	bool erase(size_t key) override {
		long num = -1;
		for (size_t i = 0; i < TableByArray<type>::mem.size(); ++i) {
			if (TableByArray<type>::mem[i].first == key) num = i;
		}
		if (num == -1) return false;
		swap(TableByArray<type>::mem[num], TableByArray<type>::mem[TableByArray<type>::mem.size() - 1]);
		TableByArray<type>::mem.pop_back();
		return true;
	}

	class iterator {
	protected:
		typename vector::iterator it;
	public:
		iterator(typename vector::iterator ptr = nullptr) {
			it = ptr;
		}
		iterator operator++(){
			typename vector::iterator temp = it;
			++it;
			return temp;
		}
		iterator operator--() {
			typename vector::iterator temp = it;
			--it;
			return temp;
		}
		pair& operator*() {	return *it;}
		type& value() { return (*it).second; }
		size_t& key() {return (*it).first;}

		iterator& operator=(const iterator& right) {
			if (this != &right) {
				it = right.it;
			}
			return *this;
		}
	};

	iterator begin(){
		iterator it = TableByArray<type>::mem.begin();
		return it;
	}

	iterator end() {
		iterator it = TableByArray<type>::mem.end();
		return it;
	}

	iterator find(size_t key) { 
		iterator it = TableByArray<type>::mem.begin();
		while (it.key() != key) ++it;
		if (it.key() != key) it = end();
		return it; 
	}
};




template<typename type>
bool le_operator(type a, type b) {
	if (a <= b) return true;
	else return false;
}


template<typename type>
bool ge_operator(type a, type b) {
	if (a >= b) return true;
	else return false;
}

//sorted tables class
template<typename type>
class SortedTable : public TableByArray<type> {
protected:
	using pair = std::pair < size_t, type>;
	using vector = std::vector<std::pair<size_t, type>>;
	bool (*my_operator)(type, type) ;
public:
	typedef bool (*func)(type, type);
	SortedTable(func f = &le_operator<type>) : my_operator(f) {}

	bool check_op(type a, type b) {
		return (*my_operator)(a, b);
	}

	class iterator {
	protected:
		typename vector::iterator it;
	public:
		iterator(typename vector::iterator ptr = nullptr) {
			it = ptr;
		}
		iterator operator++() {
			typename vector::iterator temp = it;
			++it;
			return temp;
		}
		iterator operator--() {
			typename vector::iterator temp = it;
			--it;
			return temp;
		}
		pair& operator*() { return *it; }
		type& value() { return (*it).second; }
		size_t& key() { return (*it).first; }

		iterator& operator=(const iterator& right) {
			if (this != &right) {
				it = right.it;
			}
			return *this;
		}
		bool operator==(const iterator& right) {
			if (it == right.it) return true;
			else false;
		}
		friend class SortedTable<type>;
	};

	iterator begin() {
		iterator it = TableByArray<type>::mem.begin();
		return it;
	}

	iterator end() {
		iterator it = TableByArray<type>::mem.end();
		return it;
	}
	
protected:
	iterator binary_search(size_t key) {
		size_t left, right, middle;
		bool flag = false;
		left = 0; right = TableByArray<type>::mem.size() - 1;
		while ((left <= right) && (flag != true)) {
			middle = (left + right) / 2;
			if (TableByArray<type>::mem[middle].first == key) flag = true;
			if (!(*my_operator)(TableByArray<type>::mem[middle].first, key)) right = middle - 1;
			else left = middle + 1;
		}
		iterator it = TableByArray<type>::mem.end();
		if (flag == true) {
			it = (TableByArray<type>::mem.begin() + middle);
		}
		return it;
	}

public:
	iterator find(size_t key) {
		return binary_search(key);
	}

	bool insert(size_t key, type val) override {
		//rewrite this
		for (size_t i = 0; i < TableByArray<type>::mem.size(); ++i) {
			if (TableByArray<type>::mem[i].first == key) return false;
		}
		TableByArray<type>::mem.push_back(pair(key, val));
		return true;
	}

	bool erase(size_t key) override {
		iterator iter = binary_search(key);
		if (iter == end()) return false;
		else TableByArray<type>::mem.erase(iter.it);
		return true;
	}
};



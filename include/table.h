//UNN
//Litvyakov D. D.
//

#pragma once
#include <vector>
#include <iostream>
#include <forward_list>

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
	friend std::ostream& operator<<(std::ostream& os, TableByArray<type>& table){
		for (size_t i = 0; i < table.mem.size(); ++i) {
			os << table.mem[i].first << ":" << table.mem[i].second << std::endl;
		}
		if (table.mem.empty()) os << "empty";
		return os;
	}
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
			return 	++it;;
		}
		iterator operator--() {
			return --it;
		}
		iterator operator++(int) {
			typename vector::iterator temp = it;
			it++;
			return 	temp;
		}
		iterator operator--(int) {
			typename vector::iterator temp = it;
			it--;
			return 	temp;
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
bool le_operator(size_t a, type b) {
	if (a <= b) return true;
	else return false;
}


template<typename type>
bool ge_operator(size_t a, type b) {
	if (a >= b) return true;
	else return false;
}

//sorted tables class
template<typename type>
class SortedTable : public TableByArray<type> {
protected:
	using pair = std::pair < size_t, type>;
	using vector = std::vector<std::pair<size_t, type>>;
	bool (*my_operator)(size_t, type) ;
public:
	typedef bool (*func)(size_t, type);
	SortedTable() : my_operator(&le_operator<type>) {}

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
		if (flag == false && middle != 0) it = (TableByArray<type>::mem.begin() + (middle - 1));
		return it;
	}

public:
	iterator find(size_t key) {
		if (binary_search(key).key == key) return binary_search(key).key;
		else return end();
	}

	//for le operator
	bool insert(size_t key, type val) override {
		if (TableByArray<type>::mem.empty()) {
			TableByArray<type>::mem.push_back(pair(key, val));
			return true;
		}
		size_t middle = 0;
		size_t left = 0;
		size_t right = TableByArray<type>::mem.size() - 1;
		size_t insert_pos = 0;

		while (left <= right) {
			middle = left + (right - left) / 2;

			if (TableByArray<type>::mem[middle].first == key) {
				return false; // ключ уже существует
			}

			if ((*my_operator)(TableByArray<type>::mem[middle].first, key)) {
				left = middle + 1;
				insert_pos = left;
			}
			else {
				right = middle - 1;
				insert_pos = middle;
			}
		}

		TableByArray<type>::mem.insert(TableByArray<type>::mem.begin() + insert_pos, pair(key, val));
		return true;
	}

	bool erase(size_t key) override {
		if (TableByArray<type>::mem.empty()) {
			return false;
		}
		size_t middle = 0;
		size_t left = 0;
		size_t right = TableByArray<type>::mem.size() - 1;
		bool flag = false;
		while (left <= right) {
			middle = left + (right - left) / 2;
			if (TableByArray<type>::mem[middle].first == key) {
				flag = true;
				break;
			}
			if ((*my_operator)(TableByArray<type>::mem[middle].first, key)) {
				left = middle + 1;
			}
			else {
				right = middle - 1;
			}
		}
		if (flag == true) {
			TableByArray<type>::mem.erase(TableByArray<type>::mem.begin() + middle);
		}
		return flag;
	}
};

template<typename type>
size_t hash_func(size_t key, type val) {
	size_t hash = 14695981039346656037ULL;
	hash = (hash ^ key) * 1099511628211ULL;
	return hash;
}
//hash tables class
template<typename type>
class HashTable : public TableByArray<std::forward_list<type>> {
protected:
	using pair = std::pair < size_t, std::forward_list<type>>;
	using vector = std::vector<std::pair<size_t, std::forward_list<type>>>;
	size_t (*my_hash)(size_t, type);
	size_t sz;

	bool insert(size_t key, std::forward_list<type> val) override {		return true;	}

	size_t get_hash(size_t key, type val) {
		return (((*my_hash)(key, val)) % (sz));
	}
public:
	typedef size_t (*func)(size_t, type);
	HashTable(size_t _sz, func f = &hash_func<type>) : sz(_sz), my_hash(f) {
		TableByArray<std::forward_list<type>>::mem.resize(sz);
	}

	bool insert(size_t key, type val) {
		size_t num = get_hash(key,val);
		
		return true;
	}

	bool erase(size_t key) override {

		return true;
	}

	size_t find(size_t key) {

		return key;
	}
};
//UNN
//Litvyakov D. D.
//

#pragma once
#include <vector>
#include <iostream>
#include <list>

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
	using vector = std::vector<std::pair<size_t, type> >;
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
	using vector = std::vector<std::pair<size_t, type> >;
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
	using vector = std::vector<std::pair<size_t, type> >;
	bool (*my_operator)(size_t, type);
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
	//for le operator
	/*
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
	*/
	struct Compare {
		bool operator()(const pair& a, const pair& b) const {
			return a.first < b.first;
		}
		bool operator()(const pair& a, size_t b) const {
			return a.first < b;
		}
		bool operator()(size_t a, const pair& b) const {
			return a < b.first;
		}
	};

public:
	iterator find(size_t key) {
		std::vector<std::pair<size_t,type> >::iterator it = std::lower_bound(TableByArray<type>::mem.begin(), TableByArray<type>::mem.end(), key, Compare());
		if (it == TableByArray<type>::mem.end() || (*it).first != key)
			return end();
		return iterator(it);
	}

	bool insert(size_t key, type val) override {
		std::vector<std::pair<size_t, type> >::iterator it = std::lower_bound(TableByArray<type>::mem.begin(), TableByArray<type>::mem.end(), key, Compare());
		if (it != TableByArray<type>::mem.end() && (*it).first == key)
			return false;
		TableByArray<type>::mem.insert(it, pair(key, val));
		return true;
	}

	bool erase(size_t key) override {
		std::vector<std::pair<size_t, type> >::iterator it = std::lower_bound(TableByArray<type>::mem.begin(), TableByArray<type>::mem.end(), key, Compare());
		if (it == TableByArray<type>::mem.end() || (*it).first != key)
			return false;
		TableByArray<type>::mem.erase(it);
		return true;
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
class HashTable : public TableInterface<type>  {
protected:
	using pair = std::pair < size_t, type >;
	size_t (*my_hash)(size_t, type);
	size_t sz;
    std::vector<std::list<pair> > vec;
	
	size_t get_hash(size_t key, type val) {
		return (((*my_hash)(key, val)) % (sz));
	}
public:
	typedef size_t (*func)(size_t, type);
	HashTable(size_t _sz, func f = &hash_func<type>) : sz(_sz), my_hash(f) {
        vec.resize(sz);
	}

	bool empty() override { 
		bool flag = true;
		for (size_t i = 0; i < sz; ++i) {
			if (!vec[i].empty()) flag = false;
		}
		return flag;
	}
	size_t size() override { 
		size_t sum = 0;
		for (size_t i = 0; i < sz; ++i) {
			sum += vec[i].size();
		}
		return sum;
	}
	void clear() override { 
		for (size_t i = 0; i < sz; ++i) {
			vec[i].clear();
		}
	}

	bool insert(size_t key, type val) override {
		size_t num = get_hash(key,val);
		auto it = vec[num].begin();
		for (size_t i = 0; i < vec[num].size(); ++i) {
			if ((*it).first == key) return false;
			++it;
		}
		vec[num].push_front(pair(key, val));
		return true;
	}

	bool erase(size_t key) override {
		size_t num = get_hash(key, type());
		auto it = vec[num].begin();
		bool flag = false;
		for (size_t i = 0; i < vec[num].size(); ++i) {
			if ((*(it)).first == key) {
				flag = true;
				break;
			}
			it++;
		}
		if (flag == false) return flag;
		vec[num].erase(it);
		return true;
	}

	std::pair<size_t, type>* find(size_t key) {
		size_t num = get_hash(key, type());
		auto it = vec[num].begin();
		bool flag = false;
		for (size_t i = 0; i < vec[num].size(); ++i) {
			if ((*(it)).first == key) {
				flag = true;
				break;
			}
			it++;
		}
		if (flag == false) return nullptr;
		return &(*it);
	}

	friend std::ostream& operator<<(std::ostream& os, HashTable<type>& table) {
		for (size_t i = 0; i < table.vec.size(); ++i) {
			os << "Num" << i << ":\t";
			{
				if(table.vec[i].size()!=0)
				for (auto it = table.vec[i].begin(); it != table.vec[i].end(); ++it) {
					os << (*it).first << ":" << (*it).second << " -> ";
				}
				os << std::endl;
			}
		}
		return os;
	}
};

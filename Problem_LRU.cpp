#include<iostream>
#include<list>
#include<unordered_map>

struct page_t {
	int index;
	int size;
	char* data;
};

struct cache_t {
	int max_sz;
	int sz;
	std::list<page_t> lst;

#if 0
	cache_t() {
		max_sz = 0;
		sz = 0;
		lst = {0};
	};

	~cache_t() {
		max_sz = 0
		sz = 0;
		lst = {0};
	};

	cache_t(const cache_t& copyelem){
		max_sz = copyelem.max_sz;
		sz = copyelem.sz;
		lst = copyelem.lst;
	};
#endif

};

cache_t* create_cache(int cache_size) {
	cache_t* cache = (cache_t*) calloc (1, sizeof(cache_t));
	cache -> max_sz = cache_size;
	cache -> sz = 0;
	return cache;
}



int cache_lookup(cache_t* cache, page_t * const page, std::unordered_map<int, page_t *> * hash) {
	auto res_search = hash -> find(page -> index);
	if(res_search == hash -> end()) {
		if((cache -> sz) == (cache -> max_sz)) {
			page_t page_to_delete = cache -> lst.back();
			hash -> erase(page_to_delete.index);
			cache -> lst.pop_back();
		}

		else cache -> sz++;

		cache -> lst.push_front(*page);
		hash -> insert({page -> index, page});
		return false;
	}
	
	auto page_to_transf = res_search -> second;
	if((page_to_transf -> index) != (cache -> lst.begin() -> index)) {
		cache -> lst.splice(cache -> lst.begin(), *(res_search -> second));
	}
	return true;
}


int main() {
	int size_cache = 0, count_pages = 0, i = 0, hits = 0;
	std::list<page_t> lst;
	std::unordered_map<int, page_t *> hash;
	std::cin >> size_cache;
	std::cin >> count_pages;
	std::cout << size_cache << count_pages << std::endl;
	cache_t* cache = create_cache(size_cache);
	for(i = 0; i <= count_pages - 1; i++) {
		page_t page;
		std::cin >> page.index;

		if(cache_lookup(cache, &page, &hash)) hits++;
	}
	std::cout << "Hello" << std::endl;
	return 0;
}

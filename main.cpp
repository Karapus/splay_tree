#include <iostream>
#include <list>
#include <algorithm>
#include "splay_set.hpp"

int main() {
	std::size_t n_keys;
	std::cin >> n_keys;
	Splay::SplaySet<int> set;
	for (std::size_t i = 0; i < n_keys; ++i) {
		int tmp;
		std::cin >> tmp;
		set.insert(tmp);
	}
	std::size_t n_queries;
	std::cin >> n_queries;
	std::list<int> queries;
	for (std::size_t i = 0; i < n_queries; ++i) {
		int tmp;
		std::cin >> tmp;
		queries.emplace_back(tmp);
	}
	std::size_t res = 0;
	std::for_each(queries.begin(), queries.end(), [&res, &set] (int elem) { res += set.search(elem) ? 1 : 0; });
	std::cout << res << std::endl;
}

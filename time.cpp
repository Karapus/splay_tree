#include <iostream>
#include <list>
#include <algorithm>
#include <chrono>
#include <set>
#include <unordered_set>
#include "splay_set.hpp"

namespace {
template <typename It, typename F>
double getTime(It begin, It end, F query) {
	std::size_t res = 0;
	auto start = std::chrono::high_resolution_clock::now();
	std::for_each(begin, end, [&res, query] (int elem) { res += query(elem); });
	auto finish = std::chrono::high_resolution_clock::now();
	std::cout << res << std::endl;
	return std::chrono::duration<double>(finish - start).count();
}
}

int main() {
	std::size_t n_keys;
	std::cin >> n_keys;
	Splay::SplaySet<int> splay_set;
	std::set<int> std_set;
	std::unordered_set<int> unord_set;
	for (std::size_t i = 0; i < n_keys; ++i) {
		int tmp;
		std::cin >> tmp;
		splay_set.insert(tmp);
		std_set.emplace(tmp);
		unord_set.emplace(tmp);
	}
	std::size_t n_queries;
	std::cin >> n_queries;
	std::list<int> queries;
	for (std::size_t i = 0; i < n_queries; ++i) {
		int tmp;
		std::cin >> tmp;
		queries.emplace_back(tmp);
	}
	std::clog << "Splay set time:\t"		\
		<< getTime(queries.begin(), queries.end(), [&splay_set] (int elem) { return splay_set.search(elem) ? 1 : 0; }) << std::endl \
		<< "Std::set time:\t"		\
		<< getTime(queries.begin(), queries.end(), [&std_set] (int elem) { return std_set.find(elem) != std_set.end() ? 1 : 0; }) << std::endl \
		<< "Unordered set time:\t"	\
		<< getTime(queries.begin(), queries.end(), [&unord_set] (int elem) { return unord_set.find(elem) != unord_set.end() ? 1 : 0; }) << std::endl;
}

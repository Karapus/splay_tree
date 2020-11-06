#include <iostream>
#include <fstream>
#include <random>
#include <ctime>
#include <algorithm>
#include <set>
#include <list>

namespace {
std::default_random_engine gen{static_cast<unsigned>(time(0))};

std::set<int> genData(std::size_t nkeys, int keymax, std::ofstream &of) {
	of << nkeys << std::endl;
        std::uniform_int_distribution<int> distr{0, keymax};
	std::set<int> keys;
	while (keys.size() < nkeys) {
		auto key = distr(gen);
		if (keys.emplace(key).second) {
			of << key << ' ';
		}
	}
	of << std::endl;
	return keys;
}

std::list<int> genQueries(std::size_t nqueries, int keymax, unsigned sigma, std::ofstream &of) {
	of << nqueries << std::endl;;
	std::normal_distribution<> dist(keymax / 2, sigma);
	std::list<int> res;
	for (auto i = 0lu; i < nqueries; ++i) {
		res.push_back(std::round(dist(gen)));
		of << res.back() << ' ';
	}
	of << std::endl;
	return res;
}
}

int main() {
	std::size_t nkeys, nqueries;
	int keymax;
	unsigned sigma;
	std::cout << "Input number of keys, number of queries, keymax and sigma" << std::endl;
	std::cin >> nkeys >> nqueries >> keymax >> sigma;
	std::cout << "Input filename prefix" << std::endl;
	std::string fname;
	std::cin >> fname;
	std::ofstream fdat, fans;
	fdat.open(fname + ".dat");
	fans.open(fname + ".ans");

	std::set<int> data = genData(nkeys, keymax, fdat);
	std::list<int> queries = genQueries(nqueries, keymax, sigma, fdat);
	std::size_t ans = 0;
	std::for_each(queries.begin(), queries.end(), [&data, &ans] (int elem) { ans += (data.find(elem) != data.end()) ? 1 : 0; } );
	fans << ans << std::endl;

	fdat.close();
	fans.close();		
}

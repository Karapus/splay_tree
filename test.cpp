#include <gtest/gtest.h>
#include "splay_set.hpp"
#include <vector>
#include <list>
#include <algorithm>
#include <random>

namespace {
	using T = int;
	constexpr T ksize = 100;

TEST(SearchTree, Insert) {
	Splay::SplaySet<T> set;
	std::default_random_engine e;
	std::uniform_int_distribution<int> distr{0, ksize};
	std::vector<T> v;
	for (auto i = 0; i < ksize; ++i)
	{
		auto val = distr(e);
		v.push_back(val);
		set.insert(val);
	}
	std::sort(v.begin(), v.end());
	auto el = set.min();
	for (auto i : v)
	{
		EXPECT_EQ(el->get_val(), i);
		el = el->next();
	}
}

TEST(SearchTree, CtorFromRange) {
	std::default_random_engine e;
	std::uniform_int_distribution<T> distr{0, ksize};
	std::vector<T> v;
	for (std::size_t i = 0; i < ksize; ++i)
		v.push_back(distr(e));
	Splay::SplaySet<T> set{v.begin(), v.end()};
	std::sort(v.begin(), v.end());
	auto el = set.min();
	for (auto i : v)
	{
		EXPECT_EQ(el->get_val(), i);
		el = el->next();
	}
}

TEST(SearchTree, DeleteRootNoChildren) {
	Splay::SplaySet<T> set;
	set.insert(1);
	set.erase(1);
}

TEST(SearchTree, DeleteRootLeftNode) {
	std::vector<T> v{{1, 2}};
	Splay::SplaySet<T> set{v.begin(), v.end()};
	set.erase(1);
}

TEST(SearchTree, DeleteRootRightNode) {
	std::vector<T> v{{3, 2}};
	Splay::SplaySet<T> set{v.begin(), v.end()};
	set.erase(3);
}

TEST(SearchTree, Erase) {
	std::default_random_engine e;
	std::uniform_int_distribution<T> distr{0, ksize};
	std::list<T> l;
	for (std::size_t i = 0; i < ksize; ++i)
		if (distr(e) > ksize / 2)
			l.push_back(distr(e));
		else
			l.push_front(distr(e));

	Splay::SplaySet<T> set{l.begin(), l.end()};
	for (auto i : l) {
		set.erase(i);
		auto set_el = set.min();
		auto next = (set_el) ? set_el->next() : nullptr;
		while (next) {
			EXPECT_LE(set_el->get_val(), next->get_val());
			set_el = next;
			next = set_el->next();
		}
	}
	EXPECT_TRUE(set.empty());
}
#if 0
TEST(RangeQuery, LowerBound) {
	std::vector<T> v{{1, 2, 3, 5, 6}};
	Splay::SplaySet<T> set{v.begin(), v.end()};
	auto root = set.get_root();
	EXPECT_EQ(root->lower_bound(0)->get_val(), 1);
	EXPECT_EQ(root->lower_bound(4)->get_val(), 5);
	EXPECT_EQ(root->lower_bound(5)->get_val(), 5);
	EXPECT_EQ(root->lower_bound(7), nullptr);
}

TEST(RangeQuery, UpperBound) {
	std::vector<T> v{{1, 2, 3, 5, 6}};
	Splay::SplaySet<T> set{v.begin(), v.end()};
	auto root = set.get_root();
	EXPECT_EQ(root->upper_bound(5)->get_val(), 6);
	EXPECT_EQ(root->upper_bound(4)->get_val(), 5);
	EXPECT_EQ(root->upper_bound(0)->get_val(), 1);
	EXPECT_EQ(root->upper_bound(6), nullptr);
}

TEST(RangeQuery, RangeQuery) {
	std::vector<T> v{{1, 2, 3, 5, 6}};
	Splay::SplaySet<T> set{v.begin(), v.end()};
	EXPECT_EQ(set.range_query({1, 6}), 5);
	EXPECT_EQ(set.range_query({0, 7}), 5);
	EXPECT_EQ(set.range_query({1, 4}), 3);
	EXPECT_EQ(set.range_query({4, 6}), 2);
	EXPECT_EQ(set.range_query({5, 5}), 1);
	EXPECT_EQ(set.range_query({4, 4}), 0);
	EXPECT_EQ(set.range_query({0, 0}), 0);
}
#endif
}

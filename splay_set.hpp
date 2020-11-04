#pragma once
#include "splay_tree.hpp"
#include <cassert>
#include <stack>
#include <utility>

namespace Splay
{
template <typename T>
class SplaySet final {
	SplayTree<T> *root_ = nullptr;
	void copyTree(const SplaySet &other);
	void deleteTree();
	
	public:
	SplaySet() = default;
	template <typename InputIt>
	SplaySet(InputIt first, InputIt last) {
		for (; first != last; first++)
		       insert(*first);
	}
	SplaySet(const SplaySet &other) : SplaySet(){
		copyTree(other);	
	}
	SplaySet &operator = (const SplaySet &rhs) {
		if (rhs.root_ != root_) {
			deleteTree();
			copyTree(rhs);
		}
		return *this;
	}
	SplaySet(SplaySet &&other) : SplaySet() {
		std::swap(root_, other.root_);
	}
	SplaySet &operator = (SplaySet &&other) {
		std::swap(root_, other.root_);
	}
	~SplaySet() {
		deleteTree();
	}
	const SplayTree<T> *get_root() const {
		return root_;
	}
	const SplayTree<T> *insert(const T &elem) {
		if (root_)
			return root_ = root_->insert(elem);
		else
			return root_ = new SplayTree<T>{elem};
	}
	void erase(const T &elem);
	bool empty() const {
		return !root_;
	}
	const SplayTree<T> *min() const {
		if (root_)
			return root_->min();
		return nullptr;
	}
	const SplayTree<T> *max() const {
		if (root_)
			return root_->max();
		return nullptr;
	}
	const SplayTree<T> *search(const T &elem);
	const SplayTree<T> *lowerBound(const T &val);
	const SplayTree<T> *upperBound(const T &val);

	std::size_t rangeQuery(const std::pair<T, T> &query) {
		assert(query.first <= query.second);
		auto lb = lowerBound(query.first);
		auto ub = upperBound(query.second);
		std::size_t dist = 0;
		while (lb != ub) {
			lb = lb->next();
			++dist;
		}
		return dist;
	}
};

template <typename T>
void SplaySet<T>::copyTree(const SplaySet &other) {
	if (!other.root_)
		return;
	auto max = other.max();
	for (auto node = other.min(); node != max; node = node->next())
		insert(node->get_val());
	insert(max->get_val());
}

template <typename T>
void SplaySet<T>::deleteTree() {
	std::stack<SplayTree<T> *> nodes;
	nodes.push(nullptr);
	auto node = root_;
	while (node) {
		auto left = node->get_left();
		auto right = node->get_right();
		if (!left && !right) {
			delete node;
			node = nodes.top();
			nodes.pop();
		}
		else {
			nodes.push(node);
			node = left ? left : right;
		}
	}
}

template <typename T>
const SplayTree<T> *SplaySet<T>::search(const T &elem) {
	while (true) {
		if (elem < root_->get_val()) {
			if (!root_->get_left()) {
				root_->splay();
				return nullptr;
			}
			root_ = root_->get_left();
		}
		else if (elem > root_->get_val()) {
			if (!root_->get_right()) {
				root_->splay();
				return nullptr;
			}
			root_ = root_->get_right();
		}
		else
			return root_->splay();
	}
}

template <typename T>
const SplayTree<T> *SplaySet<T>::lowerBound(const T &val) {
	SplayTree<T> *lprev = nullptr, *rprev;
	while (root_) {
		if (val < root_->get_val()) {
			lprev = root_;
			root_ = root_->get_left();
		}
		else if (val > root_->get_val()) {
			rprev = root_;
			root_ = root_->get_right();
		}
		else
			return root_->splay();
	}
	if (lprev)
		return (root_ = lprev)->splay();
	(root_ = rprev)->splay();
	return nullptr;
}

template <typename T>
const SplayTree<T> *SplaySet<T>::upperBound(const T &val) {
	SplayTree<T> *lprev = nullptr, *rprev;
	while (root_) {
		if (val < root_->get_val()) {
			lprev = root_;
			root_ = root_->get_left();
		}
		else {
			rprev = root_;
			root_ = root_->get_right();
		}
	}
	if (lprev)
		return (root_ = lprev)->splay();
	(root_ = rprev)->splay();
	return nullptr;
}

template <typename T>
void SplaySet<T>::erase(const T &elem) {
	auto node = root_;
	while (node) {
		if (elem == node->get_val()) {
			root_ = node->deleteNode();
			return;
		}
		else if (elem < node->get_val())
			node = node->get_left();
		else
			node = node->get_right();
	}
}
} //namespace Splay

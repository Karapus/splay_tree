#pragma once
#include "splay_tree.hpp"
#include <cassert>
#include <queue>
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
	void insert(const T &elem) {
		if (!root_)
			root_ = new SplayTree<T>{elem};
		else
			root_ = root_->insert(elem);
	}
	void erase(const T &elem) {
		if (!root_)
			return;
		auto node = root_->search(elem, root_);
		root_ = node->deleteNode();
	}
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
	const SplayTree<T> *lowerBound(const T &val) {
		return root_->lowerBound(val, root_);
	}
	const SplayTree<T> *upperBound(const T &val) {
		return root_->upperBound(val, root_);
	}
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
	std::queue<const SplayTree<T> *> nodes;
	nodes.push(other.root_);
	while (!nodes.empty()) {
		auto node = nodes.front();
		nodes.pop();
		root_ = root_->insert(node->get_val());
		if (node->left)
			nodes.push(node->left);
		if (node->right)
			nodes.push(node->right);
	}
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
			node->deleteLeaf();
			node = nodes.top();
			nodes.pop();
		}
		else {
			nodes.push(node);
			node = left ? left : right;
		}
	}
}
} //namespace Splay

#pragma once
#include <cassert>

namespace Splay
{
template <typename T>
class SplayTree final {
	T val_;
	SplayTree *parent_,
		   *left_ = nullptr,
		   *right_ = nullptr;

	void rotateLeft();
	void rotateRight();
	
	public:
	SplayTree(const T &elem, SplayTree *parent = nullptr) :
		val_(elem), parent_(parent)
	{}
	~SplayTree() {
		assert(!left_ && !right_);
		if (parent_) {
			if (parent_->left_ == this)
				parent_->left_ = nullptr;
			else 
				parent_->right_ = nullptr;
		}
	}

	SplayTree(const SplayTree &other) = delete;
	SplayTree &operator = (const SplayTree &other) = delete;
	SplayTree(SplayTree &&other) = delete;
	SplayTree &operator = (SplayTree &&other) = delete;

	SplayTree *insert(const T &elem);
	SplayTree *splay();

	T get_val() const {
		return val_;
	}
	SplayTree *get_left() {
		return left_;
	}
	const SplayTree *get_left() const {
		return left_;
	}
	SplayTree *get_right(){
		return right_;
	}
	const SplayTree *get_right() const {
		return right_;
	}
	const SplayTree *get_parent() const {
		return parent_;
	}
	const SplayTree *min() const {
		assert(this);
		auto node = this;
		while (node->left_)
			node = node->left_;
		return node;
	}
	SplayTree *min() {
		return const_cast<SplayTree *>(const_cast<const SplayTree *>(this)->min());
	}
	const SplayTree *max() const {
		assert(this);
		auto node = this;
		while (node->right_)
			node = node->right_;
		return node;
	}
	SplayTree *max() {
		return const_cast<SplayTree *>(const_cast<const SplayTree *>(this)->max());
	}
	const SplayTree *next() const;
	SplayTree *next() {
		return const_cast<SplayTree *>(const_cast<const SplayTree *>(this)->next());
	}
	const SplayTree *prev() const;
	SplayTree *prev() {
		return const_cast<SplayTree *>(const_cast<const SplayTree *>(this)->prev());
	}

	SplayTree *deleteNode();
};

template <typename T>
SplayTree<T> *SplayTree<T>::insert(const T &elem) {
	auto node = this;
	while (true) {
		if (elem < node->val_) {
			if (!node->left_) {
				node = node->left_ = new SplayTree{elem, node};
				break;
			}
			node = node->left_;
		}
		else if (!node->right_) {
			node = node->right_ = new SplayTree{elem, node};
			break;
		}
		else
			node = node->right_;	
	}
	return node->splay();
}

template <typename T>
SplayTree<T> *SplayTree<T>::deleteNode() {
	auto node = this;
	while (node->right_ || node->left_) {
		auto next = node->right_ ? node->right_->min() : node->left_->max();
		node->val_ = next->val_;
		node = next;
	}
	auto res = node->parent_;
	delete node;
	return res ? res->splay() : nullptr;
}

template <typename T>
const SplayTree<T> *SplayTree<T>::next() const{
	if (right_)
		return right_->min();
	auto node = this;
	auto parent = parent_;
	while (parent && (node == parent->right_)) {
		node = parent;
		parent = parent->parent_;
	}
	return parent;
}

template <typename T>
const SplayTree<T> *SplayTree<T>::prev() const{
	if (left_)
		return left_->max();
	auto node = this;
	auto parent = parent_;
	while (parent && node == parent->left_) {
		node = parent;
		parent = parent->parent_;
	}
	return parent;
}

template <typename T>
SplayTree<T> *SplayTree<T>::splay() {
	while (parent_) {
		auto gpar = parent_->parent_;
		if (!gpar) {
			if (parent_->left_ == this)
				parent_->rotateRight();
			else
				parent_->rotateLeft();
		}
		else {
			if (parent_->left_ == this) {
				if (gpar->left_ == parent_) {
					gpar->rotateRight();
					parent_->rotateRight();
				}
				else {
					parent_->rotateRight();
					parent_->rotateLeft();
				}
			}
			else {
				if (gpar->right_ == parent_) {
					gpar->rotateLeft();
					parent_->rotateLeft();
				}
				else {
					parent_->rotateLeft();
					parent_->rotateRight();
				}
			}
		}
	}
	return this;
}

template <typename T>
void SplayTree<T>::rotateLeft() {
	auto going_up = right_;
	auto trfd_subtree = going_up->left_;
	
	if (trfd_subtree)
		trfd_subtree->parent_ = this;
	right_ = trfd_subtree;

	if (parent_) {
		if (parent_->left_ == this)
			parent_->left_ = going_up;
		else
			parent_->right_ = going_up;
	}

	going_up->parent_ = parent_;
	
	parent_ = going_up;
	going_up->left_ = this;
}

template <typename T>
void SplayTree<T>::rotateRight() {
	auto going_up = left_;
	auto trfd_subtree = going_up->right_;
	
	if (trfd_subtree)
		trfd_subtree->parent_ = this;
	left_ = trfd_subtree;

	if (parent_) {
		if (parent_->left_ == this)
			parent_->left_ = going_up;
		else
			parent_->right_ = going_up;
	}

	going_up->parent_ = parent_;
	
	parent_ = going_up;
	going_up->right_ = this;
}
} //namespace Splay

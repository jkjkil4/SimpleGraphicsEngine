#include "Widget.h"

GE_NAMESPACE;

Widget::Widget(Widget* parent) : parent(parent)
{
	if (parent)
		parent->vChildren << this;
}

Widget::~Widget() {
	for (Widget* child : vChildren)
		delete child;
	take();
}


void Widget::take() {
	if (parent)
		parent->vChildren.removeOne(this);
	parent = nullptr;
}

void Widget::setParent(Widget* parent) {
	if (parent == this->parent)
		return;
	take();
	if (parent) {
		this->parent = parent;
		parent->vChildren << this;
	}
}

void Widget::addChild(Widget* child) {
	if (hasChild(child))
		return;
	child->take();
	child->parent = this;
	vChildren << child;
}

void Widget::takeChild(Widget* child) {
	auto iter = vChildren.find(child);
	if (iter != vChildren.end()) {
		(*iter)->parent = nullptr;
		vChildren.erase(iter);
	}
}

void Widget::delChild(Widget* child) {
	auto iter = vChildren.find(child);
	if (iter != vChildren.end())
		delete* iter;
}

bool Widget::hasChild(Widget* child) {
	return child->parent == this;
}
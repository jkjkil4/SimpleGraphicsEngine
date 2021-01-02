#pragma once

#include "Object.h"
#include "Vector.h"
#include "Size.h"

namespace ge {
	class Window;

	class Widget : public Object
	{
	public:
		Widget(Widget* parent = nullptr);
		~Widget() override;

		void take();	//解除该控件与父控件的绑定关系
		void setParent(Widget* parent);	//设置该控件的父控件

		void addChild(Widget* child);	//添加指定子控件
		void takeChild(Widget* child);	//解除指定子控件的绑定关系
		void delChild(Widget* child);	//移除指定子控件
		bool hasChild(Widget* child);	//得到某个控件是否为当前控件的子控件

		VAR_GET_FUNC(Parent, parent, Widget*)

	private:
		Widget* parent = nullptr;	//父控件
		Vector<Widget*> vChildren;	//子控件

	protected:
		virtual void onChildGeometryChanging(Widget* child, bool& ignore) {}
	};
}


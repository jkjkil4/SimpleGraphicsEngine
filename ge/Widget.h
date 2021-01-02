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

		void take();	//����ÿؼ��븸�ؼ��İ󶨹�ϵ
		void setParent(Widget* parent);	//���øÿؼ��ĸ��ؼ�

		void addChild(Widget* child);	//���ָ���ӿؼ�
		void takeChild(Widget* child);	//���ָ���ӿؼ��İ󶨹�ϵ
		void delChild(Widget* child);	//�Ƴ�ָ���ӿؼ�
		bool hasChild(Widget* child);	//�õ�ĳ���ؼ��Ƿ�Ϊ��ǰ�ؼ����ӿؼ�

		VAR_GET_FUNC(Parent, parent, Widget*)

	private:
		Widget* parent = nullptr;	//���ؼ�
		Vector<Widget*> vChildren;	//�ӿؼ�

	protected:
		virtual void onChildGeometryChanging(Widget* child, bool& ignore) {}
	};
}


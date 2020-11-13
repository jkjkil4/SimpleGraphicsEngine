#pragma once

#include <vector>

namespace ge {
	class Object;
	typedef void(Object::* ObjFn)();

	class Object
	{
	public:
		virtual ~Object() = default;
		
		template<typename Fn>	//T: �ۺ������ͣ������Զ������
		class Signal
		{
		public:
			struct Slot { 
				Object* obj;	//�����źŵ�Obj
				Fn fn;			//���õĲۺ���
				inline bool operator==(const Slot& other) {
					return obj == other.obj && fn == other.fn;
				}
			};
			template<typename Fn>std::vector<Slot> vSlots;

			void addSlot(Object* obj, Fn fn) {	//�󶨲�
				Slot tmpPair = { obj, fn };
				auto iter = std::find(vSlots.begin(), vSlots.end(), tmpPair);
				if (iter == vSlots.end())
					vSlots.push_back(tmpPair);
			}
			void removeSlot(Object* obj, Fn fn) {	//�Ƴ���

			}
			void removeSlots(Object* obj) {		//�Ƴ�ָ��obj�����в�
				int index = 0;
				for (auto iter = vSlots.begin(); iter != vSlots.end(); iter++) {
					if ((*iter).obj == obj) {
						vSlots.erase(iter);
						iter = vSlots.begin() + index;
					}
					else index++;
				}
			}
		};

		template<typename Fn>
		void connect(Signal<Fn> *signal, Object* sltObj, Fn sltFn) {
			signal->addSlot(sltObj, sltFn);
		}

		template<typename Fn>
		void 
	};
}

#define EMIT(signal, ...) for(Signal::Slot& slot : (signal)->vSlots) (slot.obj->*slot.fn)(__VA_ARGS__)

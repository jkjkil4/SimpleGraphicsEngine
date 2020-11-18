#pragma once

#include "header.h"
#include <vector>
#include <map>

namespace ge {
	class Object;

	class Object
	{
	public:
		typedef void(Object::*ObjFn)();

		virtual ~Object() {
			for (auto pair : vConnectedSignals) {	//�Ƴ��ź����
				pair.first->removeSlots(this);
			}
		}
		
		class SignalBase
		{
		public:
			virtual void removeSlots(Object* obj) = 0;
			virtual void removeSlots(ObjFn fn) = 0;
		};

		template<typename Fn>	//Fn: �ۺ������ͣ������Զ������
		class Signal : public SignalBase
		{
		public:
			struct Slot { 
				Object* obj;	//�����źŵ�Obj
				Fn fn;			//���õĲۺ���
				inline bool operator==(const Slot& other) {
					return obj == other.obj && fn == other.fn;
				}
			};
			std::vector<Slot> vSlots;

			void addSlot(Object* obj, Fn fn) {	//�󶨲�
				Slot tmpPair{ obj, fn };
				auto iter = std::find(vSlots.begin(), vSlots.end(), tmpPair);
				if (iter == vSlots.end()) {
					vSlots.push_back(tmpPair);
					increaseCount(obj);
				}
			}
			void removeSlot(Object* obj, Fn fn) {	//�Ƴ���
				auto iter = std::find(vSlots.begin(), vSlots.end(), Slot{ obj, fn });
				if (iter != vSlots.end()) {
					vSlots.erase(iter);
					decreaseCount(obj);
				}
			}
			void removeSlots(Object* obj) override {		//�Ƴ�ָ��obj�����в�
				for (int i = vSlots.size() - 1; i >= 0; i--) {
					Slot& slot = vSlots[i];
					if (slot.obj == obj) {
						vSlots.erase(vSlots.begin() + i);
						decreaseCount(obj);
					}
				}
			}
			void removeSlots(ObjFn fn) {	//�Ƴ�ָ���������ź����
				for (int i = vSlots.size() - 1; i >= 0; i--) {
					Slot& slot = vSlots[i];
					if ((ObjFn)slot.fn == fn) {
						vSlots.erase(vSlots.begin() + i);
						decreaseCount(slot.obj);
					}
				}
			}
			void increaseCount(Object* obj) {
				obj->vConnectedSignals[this]++;
			}
			void decreaseCount(Object* obj) {
				int& count = obj->vConnectedSignals[this];
				count--;
				if (count <= 0)
					obj->vConnectedSignals.erase(this);
			}
		};

		template<typename Fn, class Obj>
		static inline void connect(Signal<Fn> *signal, Obj* sltObj, Fn sltFn) {		//���ź����
			signal->addSlot(sltObj, sltFn);
		}

		template<typename Fn>
		static inline void disconnect(Signal<Fn> *signal, Object* sltObj, Fn sltFn) {		//����ź����
			signal->removeSlot(sltObj, sltFn);
		}

		template<typename Fn>
		static inline void disconnectAll(Signal<Fn> *signal, Object* sltObj) {	//���ָ��obj�����в�
			signal->removeSlots(sltObj);
		}

		template<typename Fn>
		static inline void disconnectAll(Signal<Fn> *signal, Fn fn) {	//���ָ����
			signal->removeSlots(fn);
		}

	private:
		std::map<SignalBase*, int> vConnectedSignals;		//�Ѱ󶨵��ź�
	};
}

//���ڷ����źŵĺ궨��
#define EMIT(signal, ...) for(auto& slot : (signal)->vSlots) (slot.obj->*slot.fn)(__VA_ARGS__)

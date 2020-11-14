#pragma once

#include "header.h"
#include <vector>
#include <map>

namespace ge {
	class Object;
	typedef void(Object::* ObjFn)();

	class Object
	{
	public:
		virtual ~Object() {
			for (auto pair : vConnectedSignals) {
				pair.first->removeSlots(this);
			}
		}
		
		class SignalBase
		{
		public:
			virtual void removeSlots(Object* obj) = 0;
		};

		template<typename Fn>	//T: 槽函数类型，用于自定义参数
		class Signal : public SignalBase
		{
		public:
			struct Slot { 
				Object* obj;	//接收信号的Obj
				Fn fn;			//调用的槽函数
				inline bool operator==(const Slot& other) {
					return obj == other.obj && fn == other.fn;
				}
			};
			std::vector<Slot> vSlots;

			void addSlot(Object* obj, Fn fn) {	//绑定槽
				Slot tmpPair{ obj, fn };
				auto iter = std::find(vSlots.begin(), vSlots.end(), tmpPair);
				if (iter == vSlots.end()) {
					vSlots.push_back(tmpPair);
					increaseCount(obj);
				}
			}
			void removeSlot(Object* obj, Fn fn) {	//移除槽
				auto iter = std::find(vSlots.begin(), vSlots.end(), Slot{ obj, fn });
				if (iter != vSlots.end()) {
					vSlots.erase(iter);
					decreaseCount(obj);
				}
			}
			void removeSlots(Object* obj) override {		//移除指定obj的所有槽
				for (int i = vSlots.size() - 1; i >= 0; i--) {
					Slot& slot = vSlots[i];
					if (slot.obj == obj) {
						vSlots.erase(vSlots.begin() + i);
						decreaseCount(obj);
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

		template<typename Fn>
		static inline void connect(Signal<Fn> *signal, Object* sltObj, Fn sltFn) {		//绑定信号与槽
			signal->addSlot(sltObj, sltFn);
		}

		template<typename Fn>
		static inline void disconnect(Signal<Fn> *signal, Object* sltObj, Fn sltFn) {		//解绑信号与槽
			signal->removeSlot(sltObj, sltFn);
		}

	private:
		std::map<SignalBase*, int> vConnectedSignals;		//已绑定的信号
	};
}

#define EMIT(signal, ...) for(auto& slot : (signal)->vSlots) (slot.obj->*slot.fn)(__VA_ARGS__)

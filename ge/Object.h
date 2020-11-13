#pragma once

namespace ge {
	class Object;
	typedef void(Object::*ObjFn)();

	class Object
	{
	public:
		virtual ~Object() = default;

		class Signal	//信号与槽的思路暂无()
		{
		public:
			void emit() {}

			
		};
	};
}
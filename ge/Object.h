#pragma once

namespace ge {
	class Object;
	typedef void(Object::*ObjFn)();

	class Object
	{
	public:
		virtual ~Object() = default;

		class Signal	//�ź���۵�˼·����()
		{
		public:
			void emit() {}

			
		};
	};
}
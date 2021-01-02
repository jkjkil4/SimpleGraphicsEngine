#pragma once

#include <vector>

namespace ge {
	template<typename T>
	class Vector : public std::vector<T>
	{
	public:
		using std::vector<T>::vector;

		iterator find(const T& value) {
			return std::find(this->begin(), this->end(), value);
		}
		bool contains(const T& value) {
			return find(value) != this->end();
		}
		void removeOne(const T& value) {
			auto iter = find(value);
			if (iter != this->end())
				this->erase(iter);
		}
		void removeAll(const T& value) {
			auto iter = find(value);
			while (iter != this->end()) {
				this->erase(iter);
				iter = find(value);
			}
		}

		inline Vector& operator<<(const T& value) { 
			this->push_back(value); 
			return *this;
		}
	};
}

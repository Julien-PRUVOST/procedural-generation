#pragma once
#include <ostream>
#include <vector>

namespace ProceduralGen
{
	using std::vector;

	template <class value_t = char>
	class Element {
		value_t val{};

	private:
		bool operator==(const Element& other) const {
			return val == other.val;
		}

		bool operator!=(const Element& other) const {
			return !(*this == other);
		}		

	public:
		static vector<value_t> defaultValues;

		Element() = default;
		~Element() = default;

		Element(const value_t&value) {
			if (isDefault(value))
				val = value_t{};
			else
				val = value_t{value};
		}

		Element(const Element& other) : val{ other.val } {}

		Element& operator=(const Element& other)
		{
			val = { other.val };
			return *this;
		}

		const value_t& value() const {
			return val;
		}

		bool compatible(const Element& other) const
		{
			return isDefault(val) || *this == other;
		}

		static void addDefaultValue(const value_t& val)
		{
			if (!isDefault(val))
				defaultValues.push_back(val);
		}

		static bool isDefault(const value_t& val) {
			return std::find(defaultValues.begin(), defaultValues.end(), val) != defaultValues.end();
		}

		bool isDefault() const
		{
			return isDefault(val);
		}
	};


	template <class T>
	bool compatible(const Element<T>& element, const Element<T>& other)
	{
		return element.compatible(other);
	}

	template <class T>
	Element<T> merge(const Element<T>& element, const Element<T>& other)
	{
		if (element.isDefault())
			return other;
		return element;
	}
}

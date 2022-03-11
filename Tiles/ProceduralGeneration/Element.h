#pragma once
#include <ostream>
#include <vector>

namespace ProceduralGen
{
	using std::vector;

	template <class value_t = char>
	class Element {

	private:
		class Config
		{
		public:
			static Config& getInstance()
			{
				static Config instance;
				return instance;
			}
		private:
			vector<value_t> defaultValues{ {} };

			Config() = default;
		public:
			Config(Config const&) = delete;
			void operator=(Config const&) = delete;

			bool isDefault(const value_t& v)
			{
				return std::find(defaultValues.begin(), defaultValues.end(), v) != defaultValues.end();
			}

			void addDefaultValue(value_t v)
			{
				defaultValues.push_back(std::move(v));
			}
		};

		value_t val{};

		bool operator==(const Element& other) const {
			return val == other.val;
		}

		bool operator!=(const Element& other) const {
			return !(*this == other);
		}		

	public:
		

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

		bool constrained(const Element& other) const
		{
			return isDefault() || *this == other;
		}

		bool compatible(const Element& other) const
		{
			return isDefault() || other.isDefault() || *this == other;
		}

		static void addDefaultValue(value_t val)
		{
			if (!isDefault(val))
				Config::getInstance().addDefaultValue(std::move(val));
		}

		static bool isDefault(const value_t& val) {
			return Config::getInstance().isDefault(val);
		}

		bool isDefault() const
		{
			return isDefault(val);
		}
	};

	template <class T>
	bool constrained(const Element<T>& element, const Element<T>& other)
	{
		return element.constrained(other);
	}

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

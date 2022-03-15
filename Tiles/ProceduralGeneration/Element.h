#pragma once
#include <vector>

#include "ElementRules.h"

namespace ProceduralGen
{
	using std::vector;

	template <class T = char>
	class Element {

	public:
		using value_t = T;

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
			vector<ElementRules<Element>> rules{ vector<Element>{ Element{ value_t{} } } };
			ElementRules<Element> ruleIncompatibleWithDefault{};

			Config() = default;

		public:
			Config(Config const&) = delete;
			void operator=(Config const&) = delete;

			void addDefaultValue(Element element)
			{
				rules[0].addElement(std::move(element));
			}

			bool isDefault(const Element& element)
			{
				return rules[0].contains(element);
			}

			void addRule(ElementRules<Element> elementRules)
			{
				rules.push_back(std::move(elementRules));
			}

			bool ruleCompatible(const Element& a, const Element& b)
			{
				for (const auto& rule : rules)
				{
					if (rule.contains(a) && rule.contains(b))
						return true;
				}
				return false;
			}

			void addIncompatibleWithDefault(Element element)
			{
				ruleIncompatibleWithDefault.addElement(std::move(element));
			}

			bool isIncompatibleWithDefault(const Element& element)
			{
				return ruleIncompatibleWithDefault.contains(element);
			}
		};

		value_t val{};

	public:
		bool operator==(const Element& other) const {
			return value() == other.value();
		}

		bool operator!=(const Element& other) const {
			return !(*this == other);
		}		

	public:
		Element() = default;
		~Element() = default;

		Element(const value_t& value) : val { value } {	}

		Element(const Element& other) : val{ other.val } {}

		Element& operator=(const Element& other)
		{
			val = { other.val };
			return *this;
		}

		const value_t& value() const {
			return val;
		}

		bool constraining(const Element& other) const
		{
			return other.isDefault() || *this == other;
		}

		bool compatible(const Element& other) const
		{
			return (isDefault() && !other.isIncompatibleWithDefault()) || (other.isDefault() && !isIncompatibleWithDefault()) || *this == other || Config::getInstance().ruleCompatible(*this, other);
		}

		static void addDefaultValue(Element element)
		{
			if (!element.isDefault())
				Config::getInstance().addDefaultValue(std::move(element));
		}

		static void addRule(ElementRules<Element> elementRules)
		{
			Config::getInstance().addRule(std::move(elementRules));
		}

		bool isDefault() const
		{
			return Config::getInstance().isDefault(*this);
		}

		static void addIncompatibleWithDefault(Element element)
		{
			Config::getInstance().addIncompatibleWithDefault(std::move(element));
		}

		bool isIncompatibleWithDefault() const
		{
			return Config::getInstance().isIncompatibleWithDefault(*this);
		}
	};

	template <class T>
	bool constraining(const Element<T>& element, const Element<T>& other)
	{
		return element.constraining(other);
	}

	template <class T>
	bool compatible(const Element<T>& element, const Element<T>& other)
	{
		return element.compatible(other);
	}

	template <class T>
	Element<T> merge(const Element<T>& element, const Element<T>& other)
	{
		if (other.isDefault())
			return element;
		return other;
	}
}

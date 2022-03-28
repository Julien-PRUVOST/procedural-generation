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
			ElementRules<Element> defaultElement{};
			ElementRules<Element> elementsCannotBePlacedOnDefault{};

			vector<ElementRules<Element>> rulesPlacing{ vector<Element>{ Element{ value_t{} } } };
			

			Config() = default;

			static bool arbitrates(const vector<ElementRules<Element>>& ruleVector, const Element& ruledElement, const Element& other)
			{
				for (const ElementRules<Element>& rule : ruleVector)
				{
					if (rule.arbitrates(ruledElement, other))
						return true;
				}
				return false;
			}

		public:
			Config(Config const&) = delete;
			void operator=(Config const&) = delete;

			void addDefaultValue(Element element)
			{
				defaultElement.addElement(std::move(element));
			}

			bool isDefault(const Element& element)
			{
				return element == Element{} || defaultElement.arbitrates(element);
			}

			void addRulePlacing(ElementRules<Element> elementRules)
			{
				rulesPlacing.push_back(std::move(elementRules));
			}

			void addCannotBePlacedOnDefault(Element element)
			{
				elementsCannotBePlacedOnDefault.addElement(std::move(element));
			}

			bool canBePlacedOnDefault(const Element& element)
			{
				return !elementsCannotBePlacedOnDefault.arbitrates(element);
			}

			bool canBePlacedOn(const Element& elementBeingPlaced, const Element& elementReceiving)
			{
				return (isDefault(elementReceiving) && canBePlacedOnDefault(elementBeingPlaced)) || arbitrates(rulesPlacing, elementBeingPlaced, elementReceiving);
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

		bool canBePlacedOn(const Element& elementReceiving) const
		{
			return isDefault() || *this == elementReceiving || Config::getInstance().canBePlacedOn(*this, elementReceiving);
		}

		bool constraintCanReceive(const Element& constraintBeingPlaced) const
		{
			return *this == constraintBeingPlaced || Config::getInstance().canBePlacedOn(constraintBeingPlaced, *this);
		}

		static void addDefaultValue(Element element)
		{
			if (!element.isDefault())
				Config::getInstance().addDefaultValue(std::move(element));
		}

		static void addRule(ElementRules<Element> elementRules)
		{
			Config::getInstance().addRulePlacing(std::move(elementRules));
		}

		bool isDefault() const
		{
			return Config::getInstance().isDefault(*this);
		}

		static void addIncompatibleWithDefault(Element element)
		{
			Config::getInstance().addCannotBePlacedOnDefault(std::move(element));
		}

	};

	template <class T>
	bool canReceive(const Element<T>& elementReceiving, const Element<T>& elementBeingPlaced)
	{
		return elementBeingPlaced.canBePlacedOn(elementReceiving);
	}

	template <class T>
	bool constraintCanReceive(const Element<T>& constraintReceiveing, const Element<T>& constraintBeingPlaced)
	{
		return constraintReceiveing.constraintCanReceive(constraintBeingPlaced);
	}

	template <class T>
	Element<T> merge(const Element<T>& element, const Element<T>& other)
	{
		if (other.isDefault())
			return element;
		return other;
	}
}

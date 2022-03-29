#pragma once
#include <vector>

namespace ProceduralGen
{
	using std::vector;

	template <class element_t>
	class ElementRules
	{
		vector<element_t> elements {};

		const element_t& getRuledElement() const
		{
			return elements[0];
		}

		bool contains(const element_t& element) const
		{
			return std::find(elements.begin(), elements.end(), element) != elements.end();
		}


	public:
		ElementRules() = default;

		ElementRules(vector<element_t> compatibleElements) : elements{std::move(compatibleElements)} {}

		void addElement(const element_t& element)
		{
			elements.push_back(element);
		}

		bool arbitrates(const element_t& ruledElement, const element_t& other) const
		{
			return getRuledElement() == ruledElement && contains(other);
		}

		bool arbitrates(const element_t& other) const
		{
			return contains(other);
		}
	};
}

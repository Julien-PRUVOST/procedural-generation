#pragma once
#include <vector>

namespace ProceduralGen
{
	using std::vector;

	template <class element_t>
	class ElementRules
	{
		vector<element_t> compatibleElements {};

	public:
		ElementRules() = default;

		ElementRules(vector<element_t> compatibleElements) : compatibleElements{std::move(compatibleElements)} {}

		void addElement(const element_t& element)
		{
			compatibleElements.push_back(element);
		}

		bool contains(const element_t& element) const
		{
			return std::find(compatibleElements.begin(), compatibleElements.end(), element) != compatibleElements.end();
		}

	};
}

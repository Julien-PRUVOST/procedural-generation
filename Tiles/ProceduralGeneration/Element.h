#pragma once


namespace ProceduralGeneration
{
	using element_t = char;

	namespace Element
	{
		inline bool compatible(const element_t& element, const element_t& other)
		{
			return element == element_t{} || element == '0' || element == other;
		}

		inline element_t merge(const element_t& element, const element_t& other)
		{
			return (element == element_t{} || element == '0') ? other : element;
		}
	}	
}

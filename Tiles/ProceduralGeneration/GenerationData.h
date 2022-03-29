#ifndef GENERATION_DATA_H
#define GENERATION_DATA_H

#include <map>
#include <string>
#include <vector>

#include "Pattern.h"

namespace ProceduralGen
{
	using namespace std;

	class GenerationData
	{
	public:
		using pattern_t = Pattern<Element<char>>;
		using tag_t = pattern_t::tag_t;

		struct WeightedPattern
		{
			using pattern_t = pattern_t;
			using weight_t = size_t;

			pattern_t pattern;
			weight_t weight = 1;

			vector<tag_t>& getTags() { return pattern.tags; }
		};

	private:
		map<tag_t, vector<WeightedPattern>> taggedWeightedPatterns{};

	public:
		void add(WeightedPattern&& tile);

		vector<WeightedPattern>& getPatterns(const tag_t& tag);
		const vector<WeightedPattern>& getPatterns(const tag_t& tag) const;

		vector<WeightedPattern> getPatterns(const vector<tag_t>& tags) const;
	};
}

#endif // GENERATION_DATA_H



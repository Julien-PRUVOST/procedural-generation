#include "GenerationData.h"

namespace ProceduralGen
{
	using WeightedPattern = GenerationData::WeightedPattern;

	void GenerationData::add(WeightedPattern&& tile)
	{
		for (tag_t& tag : tile.pattern.tags)
			taggedWeightedPatterns[tag].push_back(tile);
	}

	vector<WeightedPattern>& GenerationData::getPatterns(const tag_t& tag)
	{
		return taggedWeightedPatterns.at(tag);
	}

	const vector<WeightedPattern>& GenerationData::getPatterns(const tag_t& tag) const
	{
		return taggedWeightedPatterns.at(tag);
	}
	
	vector<WeightedPattern> GenerationData::getPatterns(const vector<tag_t>& tags) const
	{
		vector<WeightedPattern> result;

		for (const auto& tag : tags)
		{
			const vector<WeightedPattern>& patterns = getPatterns(tag);
			result.insert(result.end(), patterns.begin(), patterns.end());
		}
		return result;
	}
}


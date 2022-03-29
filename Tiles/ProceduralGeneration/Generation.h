#pragma once
#include <map>
#include <random>
#include <utility>
#include <vector>
#include <functional>

// Todo : remove those includes
#include <chrono>

#include "GenerationData.h"
#include "RandomGenerator.h"
#include "../ShinMathLib/Math.h"
#include "../ShinMathLib/VectorMath.h"
#include "../Tile.h"
#include "../Hexagonal/Grid.h"
#include "../PartialGrid/PartialGrid.h"


namespace ProceduralGen
{
	using namespace std;
	using namespace shinmathlib;	

	class GenerationProcess
	{
	private:
		using PartialGrid = shin_grid::PartialGrid<Hexagonal::Grid>;

		GenerationData generationData;
		RandomGenerator randomGenerator;
		PartialGrid& grid;

	public:
		using pattern_t = GenerationData::pattern_t;
		using pattern_ptr = pattern_t*;

		using weighted_pattern_t = GenerationData::WeightedPattern;
		using weighted_pattern_ptr = weighted_pattern_t*;

		using element_t = pattern_t::element_t;

		using RotationInfo = pattern_t::RotationInfo;

		using tag_t = GenerationData::tag_t;

		using tile_ptr = PartialGrid::tile_ptr;

		static bool trueCondition(tile_ptr) { return true; }
		static bool falseCondition(tile_ptr) { return false; }


	public:
		GenerationProcess(GenerationData generationData, RandomGenerator randomGenerator, PartialGrid& grid);

		RandomGenerator& getRandomGenerator();
		const RandomGenerator& getRandomGenerator() const;


	private:
		struct PatternInfo
		{
			weighted_pattern_t weightedPattern;
			pattern_t::RotationInfo rotationInfo;
		};

#pragma region Build
	private:
		tile_ptr getRandomPos();
		tile_ptr getRandomPosWithCondition(const std::function<bool(tile_ptr)>& condition = trueCondition);
		tile_ptr getNextTile();

		static vector<PatternInfo> getPlaceablePatterns(tile_ptr tile, const vector<weighted_pattern_t>& patternPool);
		PatternInfo choosePattern(vector<PatternInfo>& placeablePatternsInfo);
		PatternInfo choosePlaceablePattern(tile_ptr tile, const vector<weighted_pattern_t>& patternPool);

		static void merge(tile_ptr tile, const pattern_t& chosenPattern, RotationInfo& rotationInfo);
		template <class Tag>
		PatternInfo choosePlaceablePatternAndMerge(tile_ptr tile, const Tag& tag)
		{
			PatternInfo patternInfo = choosePlaceablePattern(tile, generationData.getPatterns(tag));

			merge(tile, patternInfo.weightedPattern.pattern, patternInfo.rotationInfo);

			return patternInfo;
		}

	public:
		class pattern_not_found final : public std::exception {};

		void resetGrid();
		void populate(const vector<tag_t>& patternTags);

		static void computeConstraintsFromNeighbors(tile_ptr tile, vector<tile_ptr> neighbors);
		void computeConstraintsFromNeighbors(vector<tile_ptr>& tiles) const;

		static vector<PatternInfo> getPlaceableConstrainingPatterns(tile_ptr tile, const vector<weighted_pattern_t>& patternPool);
		template <class Tag>
		PatternInfo choosePlaceableConstrainingPatternAndMerge(tile_ptr tile, Tag tag)
		{
			vector<PatternInfo> placeablePatternsInfo = getPlaceableConstrainingPatterns(tile, generationData.getPatterns(tag));

			PatternInfo patternInfo = choosePattern(placeablePatternsInfo);

			merge(tile, patternInfo.weightedPattern.pattern, patternInfo.rotationInfo);

			return patternInfo;
		}
		void populateConstrained(const vector<tag_t>& patternTags);

#pragma endregion


#pragma region Path
	public:
		struct PathData
		{
			tile_ptr start;
			tile_ptr goal;

			tag_t tagStart;
			tag_t tagGoal;
			tag_t tagPath;

			element_t linkingElement;

			std::function<bool(tile_ptr)> stopCondition = falseCondition;
		};

		struct PathBuilder
		{
			PathData data;

			tile_ptr current;

			vector<tile_ptr> path;

			void next() { path.push_back(current); }
			bool stop() const { return *current == *data.goal || data.stopCondition(current); }
			tag_t getTag() const { return *current == *data.start ? data.tagStart :
											*current == *data.goal ? data.tagGoal :
											data.tagPath; }
		};

	private:
		static vector<float> getRelativeAngles(tile_ptr referenceTile, float referenceAngle, const vector<tile_ptr>& neighbors);
		static void filterNeighbors(vector<tile_ptr>& neighbors, vector<float>& relativeAngles, float angeThreshold = 90.0f);
		static vector<float> getTileProbability(const vector<float>& relativeAngles);

		tile_ptr getNextTileInPath(PathBuilder& pathBuilder);

		class interrupted : public std::exception {};

		tile_ptr buildPathIteration(PathBuilder& pathBuilder);

	public:
		void buildPath(PathData pathData, std::function<bool(tile_ptr)> startCondition = trueCondition, std::function<bool(tile_ptr)> goalCondition = trueCondition);
#pragma endregion

#pragma region River
	public:
		struct RiverData
		{
			tag_t tagStart;
			tag_t tagEnd;
			tag_t tagRiver;

			tile_ptr start;

			element_t linkingElement;
			size_t riverMinLength;
		};

	private:
		struct RiverBuilder
		{
			RiverData data;

			tile_ptr current;
			vector<tile_ptr> river;

			size_t riverSize;

			bool tooSmall() const { return riverSize < data.riverMinLength; }
			tag_t& getTag() { return *current == *data.start ? data.tagStart :
									tooSmall() ?  data.tagRiver :
									data.tagEnd; }

			void next() { river.push_back(current); }

			void countRiverElements()
			{
				for (size_t i = 0; i != current->getPattern().data.size(); ++i)
				{
					riverSize += VectorMath::count_if(current->getPattern().data[i], [this](const pattern_t::element_t& element) {return element.canBePlacedOn(data.linkingElement); });
				}
			}
		};

		tile_ptr getNextTileInRiver(RiverBuilder& riverBuilder);

		tile_ptr buildRiverIteration(RiverBuilder& riverBuilder);
	
	public:
		void buildRiver(RiverData riverData);
#pragma endregion
	};
}

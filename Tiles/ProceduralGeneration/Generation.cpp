#include "Generation.h"

namespace ProceduralGen
{
	GenerationProcess::GenerationProcess(GenerationData generationData, RandomGenerator randomGenerator, PartialGrid& grid) :
	generationData{ generationData },
	randomGenerator{ randomGenerator },
	grid{grid}
	{
	}

	RandomGenerator& GenerationProcess::getRandomGenerator()
	{
		return randomGenerator;
	}

	const RandomGenerator& GenerationProcess::getRandomGenerator() const
	{
		return randomGenerator;
	}

#pragma region Build

	GenerationProcess::tile_ptr GenerationProcess::getRandomPos()
	{
		const size_t index = VectorMath::chooseIndex(grid.getValidTilesSize(), getRandomGenerator());
		return grid.getValidTile(index);
	}

	GenerationProcess::tile_ptr GenerationProcess::getRandomPosWithCondition(const std::function<bool(tile_ptr)>& condition)
	{
		const vector<tile_ptr> validTiles = grid.getValidTiles();

		vector<tile_ptr> pool{};
		for (tile_ptr tile : validTiles)
		{
			if (condition(tile))
				pool.push_back(tile);
		}
		const size_t index = VectorMath::chooseIndex(pool.size(), getRandomGenerator());
		return pool[index];
	}

	GenerationProcess::tile_ptr GenerationProcess::getNextTile()
	{
		// Todo : The next tile is the most constrained tile
		return nullptr;
	}

#pragma region Placeable_Patterns

	vector<GenerationProcess::PatternInfo> GenerationProcess::getPlaceablePatterns(tile_ptr tile, const vector<weighted_pattern_t>& patternPool)
	{
		vector<PatternInfo> placeablePatterns;

		for (const weighted_pattern_t& pattern : patternPool)
		{
			for (const RotationInfo& rotationInfo : tile->getPattern().canReceive(pattern.pattern))
			{
				placeablePatterns.push_back({ pattern, rotationInfo });
			}
		}

		return placeablePatterns;
	}

	GenerationProcess::PatternInfo GenerationProcess::choosePattern(vector<PatternInfo>& placeablePatternsInfo)
	{
		if (!placeablePatternsInfo.empty())
		{
			vector<weighted_pattern_t::weight_t> probability;

			for (const auto& patternInfo : placeablePatternsInfo)
			{
				probability.push_back(patternInfo.weightedPattern.weight);
			}

			const size_t index = VectorMath::chooseIndex(probability, getRandomGenerator());

			return placeablePatternsInfo[index];
		}

		throw pattern_not_found{};
	}

	GenerationProcess::PatternInfo GenerationProcess::choosePlaceablePattern(tile_ptr tile, const vector<weighted_pattern_t>& patternPool)
	{
		vector<PatternInfo> placeablePatternsInfo = getPlaceablePatterns(tile, patternPool);

		return choosePattern(placeablePatternsInfo);
	}

#pragma endregion


	void GenerationProcess::merge(tile_ptr tile, const pattern_t& chosenPattern, RotationInfo& rotationInfo)
	{
		tile->getPattern().merge(chosenPattern, rotationInfo);
	};

	void GenerationProcess::populate(const vector<tag_t>& patternTags)
	{
		const vector<tile_ptr> tiles = grid.getValidTiles();

		for (tile_ptr tile : tiles)
		{
			choosePlaceablePatternAndMerge(tile, patternTags);
		}
	}


#pragma endregion


	void GenerationProcess::resetGrid()
	{
		for (tile_ptr tile : grid.getValidTiles())
		{
			tile->getPattern().reset(vector<size_t>{6}, vector<size_t>{6, 1});
		}
	}


#pragma region Path

	vector<float> GenerationProcess::getRelativeAngles(tile_ptr referenceTile, float referenceAngle, const vector<tile_ptr>& neighbors)
	{
		vector<float> angles;

		for (const auto& neighbor : neighbors)
		{
			const float angle = referenceTile->getAngleDegreesTo(*neighbor);
			angles.push_back(Math::angularDifference(angle, referenceAngle, 360.0f));
		}

		return angles;
	}

	void GenerationProcess::filterNeighbors(vector<tile_ptr>& neighbors, vector<float>& relativeAngles, float angeThreshold)
	{
		const vector<size_t> indices = VectorMath::filteredIndex(relativeAngles, [&angeThreshold](const float& angle) { return std::abs(angle) > angeThreshold; });

		VectorMath::eraseByIndex(relativeAngles, indices);
		VectorMath::eraseByIndex(neighbors, indices);
	}

	vector<float> GenerationProcess::getTileProbability(const vector<float>& relativeAngles)
	{
		vector<float> probability;

		if (relativeAngles.empty())
		{
			return {};
		}

		const float range = std::max(0.0f, *std::max_element(relativeAngles.begin(), relativeAngles.end())) - std::min(0.0f, *std::min_element(relativeAngles.begin(), relativeAngles.end()));

		const float min = *std::min_element(relativeAngles.begin(), relativeAngles.end());

		for (const float relativeAngle : relativeAngles)
		{
			probability.push_back(range - (relativeAngle - min));
		}

		return probability;
	}


	GenerationProcess::tile_ptr GenerationProcess::getNextTileInPath(PathBuilder& pathBuilder)
	{
		if (pathBuilder.current->distance(*pathBuilder.data.goal) == 1) return pathBuilder.data.goal;

		vector<tile_ptr> candidates = grid.getValidNeighbors(pathBuilder.current);

		VectorMath::eraseOncePtr(candidates, pathBuilder.path);

		const float referenceAngle = pathBuilder.current->getAngleDegreesTo(*pathBuilder.data.goal);
		vector<float> relativeAngles = getRelativeAngles(pathBuilder.current, referenceAngle, candidates);

		filterNeighbors(candidates, relativeAngles, 90.0f);

		const vector<float> probability = getTileProbability(relativeAngles);

		return *VectorMath::choose(candidates, probability, getRandomGenerator());
	}

	GenerationProcess::tile_ptr GenerationProcess::buildPathIteration(PathBuilder& pathBuilder)
	{
		pathBuilder.next();

		// Get the next tile
		// Put the path element on the constraints of current, in direction of next. Same for next to current
		// Choose a pattern to put on current
		// Put that pattern on current
		// current = next
		tile_ptr next;
		try
		{
			next = getNextTileInPath(pathBuilder);
			pathBuilder.current->setContraintTo(*next, pathBuilder.data.linkingElement);
			next->setContraintTo(*pathBuilder.current, pathBuilder.data.linkingElement);
		}
		catch (VectorMath::cannot_choose_in_empty_range&)
		{
			pathBuilder.current = pathBuilder.data.goal;
			throw interrupted{};
		}

		const tag_t& tag = pathBuilder.getTag();
		choosePlaceablePatternAndMerge(pathBuilder.current, tag);

		if (pathBuilder.stop()) throw interrupted{};

		return next;
	}

	void GenerationProcess::buildPath(PathData pathData, std::function<bool(tile_ptr)> startCondition, std::function<bool(tile_ptr)> goalCondition)
	{
		if (!pathData.start) pathData.start = getRandomPosWithCondition(startCondition);
		if (!pathData.goal) pathData.goal = getRandomPosWithCondition(goalCondition);

		PathBuilder pathBuilder
		{
			pathData,
			pathData.start,
			{}
		};

		while (!pathBuilder.stop())
		{
			try
			{
				pathBuilder.current = buildPathIteration(pathBuilder);
			}
			catch (interrupted&)
			{
				choosePlaceablePatternAndMerge(pathBuilder.current, pathBuilder.data.tagGoal);
				return;
			}
		}

		choosePlaceablePatternAndMerge(pathBuilder.current, pathBuilder.getTag());
	}

#pragma endregion


#pragma region River

	GenerationProcess::tile_ptr GenerationProcess::getNextTileInRiver(RiverBuilder& riverBuilder)
	{
		vector<tile_ptr> neighbors = grid.getValidNeighbors(riverBuilder.current);

		VectorMath::eraseOncePtr(neighbors, riverBuilder.river);

		while (!neighbors.empty())
		{
			auto iteratorNext = VectorMath::choose(neighbors, getRandomGenerator());
			tile_ptr next = *iteratorNext;

			if (riverBuilder.tooSmall()) // Todo : Remove that quickfix that handles the end of the river
			{
				riverBuilder.current->setContraintTo(*next, riverBuilder.data.linkingElement);
				next->setContraintTo(*riverBuilder.current, riverBuilder.data.linkingElement);
			}

			vector<PatternInfo> placeablePatternsInfo = getPlaceablePatterns(riverBuilder.current, generationData.getPatterns(riverBuilder.getTag()));

			if (placeablePatternsInfo.empty())
			{
				riverBuilder.current->setContraintTo(*next, {});
				next->setContraintTo(*riverBuilder.current, {});
				neighbors.erase(iteratorNext);
			}
			else
			{
				PatternInfo patternInfo = choosePattern(placeablePatternsInfo);
				merge(riverBuilder.current, patternInfo.weightedPattern.pattern, patternInfo.rotationInfo);
				return next;
			}
		}

		return nullptr;
	}

	GenerationProcess::tile_ptr GenerationProcess::buildRiverIteration(RiverBuilder& riverBuilder)
	{
		riverBuilder.next();

		// Get the next tile
		// Put the path element on the constraints of current, in direction of next. Same for next to current
		// Choose a pattern to put on current
		// Put that pattern on current
		// current = next

		tile_ptr next = getNextTileInRiver(riverBuilder);

		if (!next)
			return next;

		riverBuilder.countRiverElements();

		return next;
	}

	void GenerationProcess::buildRiver(RiverData riverData)
	{
		if (!riverData.start)
			riverData.start = getRandomPosWithCondition([this](tile_ptr tile) {return tile->getPattern().data[1][0].isDefault(); });

		RiverBuilder riverBuilder
		{
			riverData,
			riverData.start,
			{},
			0
		};
		
		while (riverBuilder.tooSmall())
		{
			tile_ptr next = buildRiverIteration(riverBuilder);

			if (!next)
				choosePlaceablePatternAndMerge(riverBuilder.current, riverData.tagEnd);

			riverBuilder.current = next;
		}
		// Todo : Sometimes we can't place the end when the end is in a path
		buildRiverIteration(riverBuilder);
	}


#pragma endregion
}

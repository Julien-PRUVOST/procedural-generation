#include "RandomGenerator.h"

#include <chrono>

namespace ProceduralGen
{
	void RandomGenerator::setSeed(result_type newSeed)
	{
		_seed = newSeed;
		mt19937::seed(newSeed);
	}

	void RandomGenerator::setRandomSeed()
	{
		random_device rd;

		setSeed(rd());
	}

	RandomGenerator::result_type RandomGenerator::getSeed() const
	{
		return _seed;
	}

	RandomGenerator::RandomGenerator() : mt19937()
	{
		setRandomSeed();
	}

	RandomGenerator::RandomGenerator(result_type seed) : mt19937()
	{
		setSeed(seed);
	}
}


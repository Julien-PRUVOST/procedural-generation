#ifndef RANDOM_GENERATOR_H
#define RANDOM_GENERATOR_H
#include <random>

namespace ProceduralGen
{
	using namespace std;

	class RandomGenerator : public mt19937
	{
	private:
		result_type _seed{};

	public:
		void setSeed(result_type newSeed);
		void setRandomSeed();

		result_type getSeed() const;

		RandomGenerator();
		RandomGenerator(result_type seed);
	};
}



#endif // RANDOM_GENERATOR_H

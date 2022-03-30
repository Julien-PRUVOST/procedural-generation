#pragma once
#include <exception>
#include <vector>
#include <random>

namespace shinmathlib {

	namespace VectorMath
	{
		class cannot_choose_in_empty_range : public std::exception {};

		template <class IntType = size_t>
		IntType chooseIndex(const IntType& size, std::mt19937& prng)
		{
			if (size == 0) throw cannot_choose_in_empty_range{};
			std::uniform_int_distribution<IntType> chooser{ 0, size - 1 };
			return chooser(prng);
		}

		template <class ProbaType, class IntType = size_t>
		IntType chooseIndex(const std::vector<ProbaType>& probability, std::mt19937& prng)
		{
			if (probability.empty()) throw cannot_choose_in_empty_range{};
			std::discrete_distribution<IntType> chooser{ probability.begin(), probability.end() };
			return chooser(prng);
		}

		template<class T>
		auto choose(std::vector<T>& v, std::mt19937& prng)
		{
			return std::next(v.begin(), chooseIndex(v.size(), prng));
		}

		template<class T, class ProbaType>
		auto choose(std::vector<T>& v, const std::vector<ProbaType>& probability, std::mt19937& prng)
		{
			return std::next(v.begin(), chooseIndex(probability, prng));
		}

		template<class T, class Iterator>
		auto getIndex(const std::vector<T>& v, Iterator &p)
		{
			return std::distance(v.begin, p);
		}

		// Use find instead if you need the iterator index
		template<class T>
		bool contains(const std::vector<T>& v, T& element)
		{
			return std::find(v.begin(), v.end(), element) != v.end();
		}

		template<class T, class Iterator>
		bool containsAll(const std::vector<T>& v, Iterator begin, Iterator end)
		{
			if (begin == end) return true;
			return contains(v, *begin) && containsAll(v, begin + 1, end);
		}

		template<class T>
		bool containsAll(const std::vector<T>& v, std::vector<T>& elements)
		{
			return containsAll(v, elements.begin(), elements.end());
		}

		template<class T, class Iterator>
		bool containsAny(const std::vector<T>& v, Iterator begin, Iterator end)
		{
			if (begin == end) return false;
			return contains(v, *begin) || containsAll(v, begin + 1, end);
		}

		template<class T>
		bool containsAny(const std::vector<T>& v, std::vector<T>& elements)
		{
			return containsAny(v, elements.begin(), elements.end());
		}

		template<class T>
		auto find(const std::vector<T>& v, T& element)
		{
			return std::find(v.begin(), v.end(), element);
		}

		/// Assuming each element can only appear once
		template <class T>
		void eraseOnce(std::vector<T>& v, const std::vector<T>& elements)
		{
			for (auto p = elements.begin(); p != elements.end(); ++p)
			{
				auto finder = find(v, *p);
				if (finder != v.end())
				{
					v.erase(finder);
				}
			}
		}

		/// Assuming each element can only appear once
		template <class T>
		void eraseOncePtr(std::vector<T>& v, const std::vector<T>& elements)
		{
			for (auto p = elements.begin(); p != elements.end(); ++p)
			{
				auto finder = v.begin();
				for (; finder != v.end(); ++finder)
				{
					if (**finder == **p) {
						v.erase(finder);
						break;
					}
				}
			}
		}

		/// indices must be sorted in increasing order
		template <class T>
		void eraseByIndex(std::vector<T>& v, const std::vector<size_t>& indices)
		{
			for (auto p = indices.rbegin(); p != indices.rend(); ++p)
			{
				v.erase(std::next(v.begin(), *p));
			}
		}

		template <class Container, class UnaryPredicate>
		std::vector<size_t> filteredIndex(const Container& v, UnaryPredicate f)
		{
			std::vector<size_t> index;

			for (size_t i = 0; i != v.size(); ++i)
			{
				if (f(v[i]))
					index.push_back(i);
			}

			return index;
		}

		template <class It, class Init>
		Init sum(It first, It last, Init init)
		{
			for (; first != last; ++first)
			{
				init += static_cast<Init>(*first);
			}
			return init;
		}

		template <class Container, class Init>
		Init sum(Container& v, Init init)
		{
			return sum(v.begin(), v.end(), init);
		}

		template<class Container, class UnaryPredicate>
		auto count_if(const Container& v, UnaryPredicate f)
		{
			return std::count_if(v.begin(), v.end(), f);
		}

		template<class InputIt, class OutputIt>
		OutputIt rotate(const InputIt& first, const InputIt& last, OutputIt d_first, const size_t& n)
		{
			auto size = std::distance(first, last);
			for (decltype(size) i = 0; i != size; (void)++i, (void)++d_first)
			{
				*d_first = *(first + (i + n) % size);
			}
			return d_first;
		}

		template <class Container>
		Container rotate(const Container& in, const size_t& n)
		{
			Container result;
			result.reserve(in.size());

			VectorMath::rotate(in.begin(), in.end(), std::back_inserter(result), n);
			return result;
		}

		template<class Iterator, class BinaryPredicate>
		auto compare(Iterator begin, const Iterator& end, Iterator otherBegin, BinaryPredicate f)
		{
			for (; begin != end; (void)++begin, (void)++otherBegin)
			{
				if (!f(*begin, *otherBegin))
					return begin;
			}
			return begin;
		}

		template<class T, class BinaryPredicate>
		auto compare(const std::vector<T>& v, const std::vector<T>& w, BinaryPredicate f)
		{
			return compare(v.begin(), v.end(), w.begin(), f);
		}

		template <class Container>
		Container& reverse_inplace(Container& in)
		{
			std::reverse(in.begin(), in.end());
			return in;
		}

		template <class Container>
		Container reverse(Container in)
		{
			return reverse_inplace(in);
		}
	}

}
#include "string_generation/string_generator.hpp"
#include "sorting_base.hpp"
#include "common.hpp"

#include <chrono>
#include <stdexcept>

#pragma once

enum class SortProtocol
{
    Random = 0,
    Reverse = 1,
    AlmostSorted = 2
};

struct TestResult
{
    std::chrono::nanoseconds time;
    size_t comparisons;
};

class StringSortTester
{
public:
    using perm_container = std::ranges::ref_view<std::vector<std::string>>;
    using SortType = IStringSorting<perm_container,
                                    CountCharCmp>;

private:
    CountCharCmp cmp{};
    StringGenerator container;
    std::unique_ptr<SortType> sort;

public:
    StringSortTester(
        std::unique_ptr<SortType> &&sort,
        StringGenerator &&container) : container(std::move(container)), sort(std::move(sort))
    {
    }

    TestResult run_test(SortProtocol protocol, size_t length)
    {
        permutation_view<perm_container> view = (protocol == SortProtocol::Random ? container.get_random(length) : (protocol == SortProtocol::Reverse ? container.get_reversed(length) : container.get_almost_sorted(length)));

        CountCharCmp::Counter = 0;
        auto start = std::chrono::high_resolution_clock::now();
        sort->sort(view);
        auto duration = std::chrono::high_resolution_clock::now() - start;

        for (size_t i = 0; i + 1 < view.size(); i++)
        {
            if ((*view.perm_)[i] > (*view.perm_)[i + 1])
            {
                throw std::runtime_error("Incorrect sort");
            }
        }

        return {std::chrono::duration_cast<std::chrono::nanoseconds>(duration), CountCharCmp::Counter};
    }
};
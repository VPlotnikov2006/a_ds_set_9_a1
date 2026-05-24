#include "sort_tester.hpp"
#include "sortings/merge_sort.hpp"
#include "sortings/string_merge_sort.hpp"
#include "sortings/quick_sort.hpp"
#include "sortings/string_quick_sort.hpp"
#include "sortings/msb_radix_quick_sort.hpp"
#include "sortings/msb_radix_sort.hpp"

#include <iostream>
#include <fstream>
#include <memory>
#include <string>

std::string get_protocol(SortProtocol protocol)
{
    switch (protocol)
    {
    case SortProtocol::Random:
        return "Random";
    case SortProtocol::Reverse:
        return "Reverse";
    case SortProtocol::AlmostSorted:
        return "AlmostSorted";
    default:
        return "Unknown";
    }
}

SortProtocol parse_protocol(const std::string &str)
{
    if (str == "Random")
        return SortProtocol::Random;

    if (str == "Reverse")
        return SortProtocol::Reverse;

    if (str == "AlmostSorted")
        return SortProtocol::AlmostSorted;

    throw std::runtime_error("Unknown protocol: " + str);
}

std::unique_ptr<StringSortTester::SortType>
create_sort(const std::string &algorithm)
{
    if (algorithm == "merge")
    {
        return std::make_unique<
            MergeSort<StringSortTester::perm_container, CountCharCmp>>();
    }

    if (algorithm == "string_merge")
    {
        return std::make_unique<
            StringMergeSort<StringSortTester::perm_container, CountCharCmp>>();
    }

    if (algorithm == "quick")
    {
        return std::make_unique<
            QuickSort<StringSortTester::perm_container, CountCharCmp>>();
    }

    if (algorithm == "string_quick")
    {
        return std::make_unique<
            StringQuickSort<StringSortTester::perm_container, CountCharCmp>>();
    }

    if (algorithm == "msb_radix_quick")
    {
        return std::make_unique<
            MSBRadixQuickSort<StringSortTester::perm_container, CountCharCmp>>();
    }

    if (algorithm == "msb_radix")
    {
        return std::make_unique<
            MSBRadixSort<StringSortTester::perm_container, CountCharCmp>>();
    }

    throw std::runtime_error("Unknown algorithm: " + algorithm);
}

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        std::cerr
            << "Usage: "
            << argv[0]
            << " <n_repeat> <protocol> <algorithm> <output_file>\n";

        return 1;
    }

    int n_repeat = std::stoi(argv[1]);
    std::string protocol_str = argv[2];
    std::string algorithm = argv[3];
    std::string output_file = argv[4];

    SortProtocol protocol = parse_protocol(protocol_str);

    std::ofstream out(output_file, std::ios::app);

    if (!out.is_open())
    {
        std::cerr << "Cannot open output file\n";
        return 1;
    }

    for (int repeat = 0; repeat < n_repeat; ++repeat)
    {
        for (int size = 100; size <= 3000; size += 100)
        {
            auto sort = create_sort(algorithm);
            std::string sort_name = sort->name();

            StringGenerator generator(size, 10, 200);

            StringSortTester tester(
                std::move(sort),
                std::move(generator));

            auto res = tester.run_test(protocol, size);

            out << protocol_str << ","
                << sort_name << ","
                << size << ","
                << res.comparisons << ","
                << res.time.count() << "\n";

            std::cout << "Done: "
                      << "repeat=" << repeat + 1
                      << ", size=" << size
                      << "\n";
        }
    }

    return 0;
}
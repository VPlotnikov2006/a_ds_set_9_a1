#include <ranges>

#include "trie_node.hpp"
#include "alphabet.hpp"
#include "../permutation_view.hpp"

#pragma once

class StringGenerator
{
private:
    std::vector<std::string> strings;

public:
    inline static std::string_view alphabet = alphabet::get_alphabet();
    inline static std::mt19937 gen{42};
    inline static std::uniform_int_distribution<size_t> char_dst{0, alphabet.size() - 1};

    StringGenerator(size_t n_strings, size_t min_n, size_t max_n) : strings()
    {
        Node root(0);
        std::string tmp;

        build_random_trie(&root, n_strings, min_n, max_n, alphabet, gen, char_dst);
        collect(&root, strings, tmp);
    }

    auto get_random(size_t length)
    {
        permutation_view perm(std::views::all(strings));
        perm = perm.subview(0, length);
        std::uniform_int_distribution<size_t> tp_dst{0, perm.size() - 1};

        for (size_t i = 0; i < length; i++)
        {
            size_t left = tp_dst(gen), right = tp_dst(gen);
            perm.swap(left, right);
        }

        return perm;
    }

    auto get_almost_sorted(size_t length, double frac = 0.1)
    {
        permutation_view perm(std::views::all(strings));
        perm = perm.subview(0, length);
        std::uniform_int_distribution<size_t> tp_dst{0, perm.size() - 1};

        for (size_t i = 0; i < length * frac; i++)
        {
            size_t left = tp_dst(gen), right = tp_dst(gen);
            perm.swap(left, right);
        }

        return perm;
    }

    auto get_reversed(size_t length)
    {
        permutation_view perm(std::views::all(strings));
        perm = perm.subview(0, length);

        for (size_t i = 0; i * 2 < length; i++)
        {
            perm.swap(i, perm.size() - 1 - i);
        }

        return perm;
    }
};
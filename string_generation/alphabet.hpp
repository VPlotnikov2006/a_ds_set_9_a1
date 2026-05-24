#include <algorithm>
#include <string>
#include <unordered_map>

#pragma once

namespace alphabet
{
    bool is_ready = false;
    std::string alphabet;
    std::unordered_map<char, size_t> inv;

    std::string_view get_alphabet()
    {
        if (!is_ready)
        {
            for (char c = 'a'; c <= 'z'; c++)
            {
                alphabet.push_back(c);
            }

            for (char c = 'A'; c <= 'Z'; c++)
            {
                alphabet.push_back(c);
            }

            for (char c = '0'; c <= '9'; c++)
            {
                alphabet.push_back(c);
            }

            alphabet += "!@#%:;^&*()-";

            std::sort(alphabet.begin(), alphabet.end());

            for (size_t i = 0; i < alphabet.size(); i++)
            {
                inv[alphabet[i]] = i;
            }
            is_ready = true;
        }

        return alphabet;
    }
}
// Copyright 2023 Atgeirr F Rasmussen
// License: GPLv3+

#pragma once

#include <cctype>
#include <iostream>
#include <string>

#include "Word.hpp"

namespace AnsiTerminalColors {
    const std::string none = "\033[0m";
    const std::string red = "\033[31m";
    const std::string red_strong = "\033[31;1m";
    const std::string yellow = "\033[33m";
    const std::string yellow_strong = "\033[33;1m";
    const std::string blue = "\033[34m";
    const std::string blue_strong = "\033[34;1m";
    const std::string magenta = "\033[35m";
    const std::string magenta_strong = "\033[35;1m";
    const std::string default_color = "\033[39m";
    const std::string bg_white = "\033[47m";
    const std::string bg_gray = "\033[100m";
    const std::string bg_brightgreen = "\033[102m";
    const std::string bg_brightyellow = "\033[103m";
}


class WordKnowledge
{
public:
    WordKnowledge(const Word& answer, const Word& guess)
    {
        // Find exact matches.
        for (int pos = 0; pos < 5; ++pos) {
            if (guess[pos] == answer[pos]) {
                pos_match[pos] = guess[pos];
            }
        }

        // Find matches at wrong position.
        for (int answerpos = 0; answerpos < 5; ++answerpos) {
            if (pos_match[answerpos] != '0') {
                // If matched exactly, attempt no further matches.
                continue;
            }
            for (int guesspos = 0; guesspos < 5; ++guesspos) {
                if (pos_match[guesspos] != '0' || off_match[guesspos] != '0') {
                    continue;
                }
                if (answerpos != guesspos && answer[answerpos] == guess[guesspos]) {
                    off_match[guesspos] = guess[guesspos];
                    // Each answer pos will only match once, the first found in guesspos.
                    break;
                }
            }
        }

        // The rest are not matching.
        for (int pos = 0; pos < 5; ++pos) {
            if (pos_match[pos] == '0' && off_match[pos] == '0') {
                not_match[pos] = guess[pos];
            }
        }
    }

    WordKnowledge(const Word& guess, std::array<char,5> scoring)
    {
        for (int pos = 0; pos < 5; ++pos) {
            switch (scoring[pos]) {
            case 'p':
                pos_match[pos] = guess[pos];
                break;
            case 'o':
                off_match[pos] = guess[pos];
                break;
            default:
                not_match[pos] = guess[pos];
            }
        }
    }


    bool isMatching(const Word& candidate) const
    {
        // Keep track of letters used up in matching.
        bool matched_letters[5] = { false };

        // Must meet all positional matches.
        for (int pos = 0; pos < 5; ++pos) {
            if (pos_match[pos] != '0') {
                if (pos_match[pos] != candidate[pos]) {
                    return false;
                } else {
                    matched_letters[pos] = true;
                }
            }
        }

        // For each off-position match, verify that
        // there is no position match, and that there
        // is one available off-position match.
        for (int pos = 0; pos < 5; ++pos) {
            if (off_match[pos] != '0') {
                if (off_match[pos] == candidate[pos]) {
                    return false;
                }
                int candidate_pos = 0;
                for (; candidate_pos < 5; ++candidate_pos) {
                    if (matched_letters[candidate_pos]) {
                        continue;
                    }
                    if (off_match[pos] == candidate[candidate_pos]) {
                        matched_letters[candidate_pos] = true;
                        break;
                    }
                }
                if (candidate_pos == 5) {
                    // Did not find any available match.
                    return false;
                }
            }
        }

        // For each not-match, verify that there is no available match.
        for (int pos = 0; pos < 5; ++pos) {
            if (not_match[pos] != '0') {
                if (not_match[pos] == candidate[pos]) {
                    return false;
                }
                for (int candidate_pos = 0; candidate_pos < 5; ++candidate_pos) {
                    if (matched_letters[candidate_pos]) {
                        continue;
                    }
                    if (not_match[pos] == candidate[candidate_pos]) {
                        return false;
                    }
                }
            }
        }

        // We have satisfied positional-, off-position- and not-matches.
        return true;
    }

    // A number from 0..242 (3^5 - 1) compactly representing the match.
    int matchCode()
    {
        auto digit = [this](int pos) { return pos_match[pos] != '0' ? 2
                                       : off_match[pos] != '0' ? 1
                                       : 0; };
        int retval = 0;
        int digitval = 1;
        for (int pos = 0; pos < 5; ++pos) {
            retval += digitval * digit(pos);
            digitval *= 3;
        }
        return retval;
    }

    bool operator==(const WordKnowledge& wk)
    {
        for (int pos = 0; pos < 5; ++pos) {
            if (pos_match[pos] != wk.pos_match[pos]) {
                std::cout << "X" << pos << std::flush;
                return false;
            }
            if (off_match[pos] != wk.off_match[pos]) {
                std::cout << "Y" << pos << std::flush;
                std::cout << off_match[pos] << wk.off_match[pos] << std::flush;
                return false;
            }
            if (not_match[pos] != wk.not_match[pos]) {
                std::cout << "Z" << pos << std::flush;
                std::cout << not_match[pos] << wk.not_match[pos] << std::flush;
                return false;
            }
        }
        return true;
    }

    friend std::ostream& operator<<(std::ostream& os, const WordKnowledge& wk);

private:
    char pos_match[5] = { '0', '0', '0', '0', '0' };
    char off_match[5] = { '0', '0', '0', '0', '0' };
    char not_match[5] = { '0', '0', '0', '0', '0' };
};

std::ostream& operator<<(std::ostream& os, const WordKnowledge& wk)
{
    for (int ii = 0; ii < 5; ++ii) {
        if (wk.pos_match[ii] != '0') {
            os << AnsiTerminalColors::bg_brightgreen;
            os << wk.pos_match[ii]; //static_cast<char>(std::toupper(wk.pos_match[ii]));
            os << AnsiTerminalColors::none;
        } else if (wk.off_match[ii] != '0') {
            os << AnsiTerminalColors::bg_brightyellow;
            os << wk.off_match[ii];
            os << AnsiTerminalColors::none;
        } else {
            os << AnsiTerminalColors::bg_white;
            os << wk.not_match[ii];
            os << AnsiTerminalColors::none;
        }
    }
    return os;
}

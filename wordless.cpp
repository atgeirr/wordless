#include <fstream>
#include <iostream>
#include <vector>

#include "Word.hpp"
#include "WordKnowledge.hpp"

int main()
{
    // Read the words from file.
    std::vector<Word> words;
    words.reserve(15000);
    std::ifstream wordfile("words");
    Word w;
    while (wordfile >> w) {
        words.push_back(w);
    }
    std::cout << "Read " << words.size() << " words." << std::endl;

#if 0
    const size_t num = 3;
    for (size_t ii = 0; ii < num; ++ii) {
        std::cout << "\n\n=====\n" << words[ii] << "\n=====\n";
        for (size_t jj = 0; jj < num; ++jj) {
            WordKnowledge wk(words[ii], words[jj]);
            std::cout << "-----\n" << words[jj] << '\n' << wk << '\n';
            for (size_t kk = 0; kk < 3*num; ++kk) {
                std::cout << "* " << words[kk] << " | " << wk << " | " << wk.isMatching(words[kk]) << '\n';
            }
        }
    }
#else
    const size_t num = words.size()/15;
    std::vector<double> matching_fraction_avg(num, 0.0);
    constexpr int num_matchcodes = 3*3*3*3*3;
    // double progress_marked = 0.0;
    // const double progress_step = 0.01;

    // Calculate matching fraction averages.
#pragma omp parallel for
    for (size_t ii = 0; ii < num; ++ii) {
        std::vector<int> matches_by_matchcode(num_matchcodes, -1);
        // Word ii is the guess.
        double matching_fraction_sum = 0.0;
        for (size_t jj = 0; jj < num; ++jj) {
            // For each word jj, assume it is the answer.
            // Score the guess, giving a WordKnowledge.
            WordKnowledge wk(words[jj], words[ii]);
            const int matchcode = wk.matchCode();
            size_t num_matches = 0;
            if (matches_by_matchcode[matchcode] > -1) {
                // Already calculated matches for this matchcode.
                num_matches = matches_by_matchcode[matchcode];
            } else {
                // Must count matches.
                for (size_t kk = 0; kk < num; ++kk) {
                    if (wk.isMatching(words[kk])) {
                        ++num_matches;
                    }
                }
                matches_by_matchcode[matchcode] = num_matches;
            }
            // The number of possible words matching this WordKnowledge,
            // divided by the total number of words, is the matching
            // fraction.
            const double matching_fraction = double(num_matches)/double(num);
            matching_fraction_sum += matching_fraction;
        }
        matching_fraction_avg[ii] = matching_fraction_sum / double(num);
        // if (double(ii+1)/double(num) >= progress_marked + progress_step) {
        //     progress_marked += progress_step;
        //     std::cout << '.' << std::flush;
        // }
    }
    // std::cout << '\n';

    // Collect and sort words and scores.
    std::vector<std::pair<Word, double>> word_and_score(num);
    for (size_t ii = 0; ii < num; ++ii) {
        word_and_score[ii] = { words[ii], matching_fraction_avg[ii] };
    }
    std::sort(word_and_score.begin(), word_and_score.end(),
              [](const auto& x1, const auto& x2) { return x1.second < x2.second; });

    // Print the top 10.
    std::cout << "*** Top 10 words:\n";
    for (size_t ii = 0; ii < 10; ++ii) {
        std::cout << word_and_score[ii].first << ": " << word_and_score[ii].second << '\n';
    }

    // Print the bottom 10.
    std::cout << "*** Bottom 10 words:\n";
    for (size_t ii = word_and_score.size() - 10; ii < word_and_score.size(); ++ii) {
        std::cout << word_and_score[ii].first << ": " << word_and_score[ii].second << '\n';
    }
#endif
}

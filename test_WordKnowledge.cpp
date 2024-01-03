#include <cassert>

#include "WordKnowledge.hpp"

int main()
{
    std::vector<Word> words = {
        "abcde",
        "edcba",
        "aaaaa",
        "aabba",
        "bbbaa",
        "fffff"
    };
    for (const Word& answer : words) {
        for (const Word& guess : words) {
            WordKnowledge wk(answer, guess);
            std::cout << answer << " | " << guess << " | " << wk << std::endl;
        }
    }

    {
        const Word answer = "aabba";
        const Word guess = "bbbaa";
        WordKnowledge wk1(answer, guess);
        WordKnowledge wk2(guess, std::array<char, 5>{'o', 'n', 'p', 'o', 'p' });
        std::cout << answer << " | " << guess << " | " << wk1 << " | " << wk2 << std::endl;
        assert(wk1 == wk2);
    }
}

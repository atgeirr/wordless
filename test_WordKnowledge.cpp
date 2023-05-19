#include "WordKnowledge.hpp"

int main()
{
    std::vector<std::string> words = {
        "abcde",
        "edcba",
        "aaaaa",
        "aabba",
        "bbbaa",
        "fffff"
    };
    for (const std::string& answer : words) {
        for (const std::string& guess : words) {
            WordKnowledge wk(answer, guess);
            std::cout << answer << " | " << guess << " | " << wk << std::endl;
        }
    }

    {
        const std::string answer = "aabba";
        const std::string guess = "bbbaa";
        WordKnowledge wk1(answer, guess);
        WordKnowledge wk2(guess, std::array<char, 5>{'o', 'n', 'p', 'o', 'p' });
        std::cout << answer << " | " << guess << " | " << wk1 << " | " << wk2 << std::endl;
        assert(wk1 == wk2);
    }
}

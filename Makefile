all: wordless test_WordKnowledge

wordless: wordless.cpp WordKnowledge.hpp Word.hpp

test_WordKnowledge: test_WordKnowledge.cpp WordKnowledge.hpp Word.hpp

%: %.cpp
	c++ -std=c++17 -march=native -Wall -Wextra -Wshadow -Xclang -fopenmp -O0 -g $< -o $@ -lomp

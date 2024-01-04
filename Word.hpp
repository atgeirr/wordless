// Copyright 2023 Atgeirr F Rasmussen
// License: GPLv3+

#pragma once

#include <iostream>
#include <string>

template <int Length>
class GenericWord
{
public:
    GenericWord() {}
    GenericWord(const char* carray)
    {
        std::copy_n(carray, Length, w);
    }
    char& operator[](int ii)       { return w[ii]; }
    char  operator[](int ii) const { return w[ii]; }
    char*       data()       { return w; }
    const char* data() const { return w; }
private:
    char w[Length] = { '0' };
};

template <int L>
std::istream& operator>>(std::istream& is, GenericWord<L>& w)
{
    std::string fullword;
    is >> fullword;
    std::copy_n(fullword.begin(), L, w.data());
    return is;
}

template <int L>
std::ostream& operator<<(std::ostream& os, const GenericWord<L>& w)
{
    for (int ii = 0; ii < L; ++ii) {
        os << w.data()[ii];
    }
    return os;
}



using Word = GenericWord<5>;

//using Word = std::string;

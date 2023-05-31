/**************************************************************************************************
* MIT License
* 
* Copyright (c) 2023 Nick Wettstein (@Schmicki)
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
**************************************************************************************************/

#pragma once

#include <cstdint>

template <class T>
class Random;

template <>
class Random<std::uint32_t>
{
public:

    std::uint32_t _seed;

    Random() : _seed(1249375) {};
    Random(std::uint32_t seed) : _seed(seed) {};

    void seed(std::uint32_t seed)
    {
        this->_seed = seed;
    };

    std::uint32_t next()
    {
        constexpr std::uint32_t mul = 1345953181;
        constexpr std::uint32_t inc = 720491201;
        std::uint32_t val = _seed;

        _seed = _seed * mul + inc;

        return val;
    }
};

template <>
class Random<std::uint64_t>
{
public:

    std::uint64_t _seed;

    Random() : _seed(1249375) {};
    Random(std::uint64_t seed) : _seed(seed) {};

    void seed(std::uint64_t seed) { this->_seed = seed; };
    std::uint64_t next()
    {
        std::uint64_t val = _seed;
        constexpr std::uint64_t mul = 3000000000400672125;
        constexpr std::uint64_t inc = 3345903967205103329;

        _seed = _seed * mul + inc;

        return val;
    }
};
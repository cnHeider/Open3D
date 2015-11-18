// ----------------------------------------------------------------------------
// -                       Open3DV: www.open3dv.org                           -
// ----------------------------------------------------------------------------
// The MIT License (MIT)
//
// Copyright (c) 2015 Qianyi Zhou <Qianyi.Zhou@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// ----------------------------------------------------------------------------

#pragma once

#include <tuple>
#include <functional>

namespace three {

/// The namespace hash_tuple defines a general hash function for std::tuple
/// See this post for details:
///   http://stackoverflow.com/questions/7110301
/// The hash_combine code is from boost
/// Reciprocal of the golden ratio helps spread entropy and handles duplicates.
/// See Mike Seymour in magic-numbers-in-boosthash-combine:
///   http://stackoverflow.com/questions/4948780

namespace hash_tuple {

template <typename TT>
struct hash
{
	size_t operator()(TT const& tt) const
	{
		return std::hash<TT>()(tt);
	}
};

namespace {

template <class T>
inline void hash_combine(std::size_t& seed, T const& v)
{
	seed ^= hash_tuple::hash<T>()(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

template <class Tuple, size_t Index = std::tuple_size<Tuple>::value - 1>
struct HashValueImpl
{
	static void apply(size_t& seed, Tuple const& tuple)
	{
		HashValueImpl<Tuple, Index - 1>::apply(seed, tuple);
		hash_combine(seed, std::get<Index>(tuple));
	}
};

template <class Tuple>
struct HashValueImpl<Tuple, 0>
{
	static void apply(size_t& seed, Tuple const& tuple)
	{
		hash_combine(seed, std::get<0>(tuple));
	}
};

}	// unnamed namespace

template <typename ... TT>
struct hash<std::tuple<TT...>>
{
	size_t operator()(std::tuple<TT...> const& tt) const
	{
		size_t seed = 0;
		HashValueImpl<std::tuple<TT...> >::apply(seed, tt);
		return seed;
	}
};

}	// namespace hash_tuple

}	// namespace three
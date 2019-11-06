/**
 *  @file       Tuple.h
 *  @brief      Various tuple type definitions & constructs
 *
 *  Provides type definitions for: T2_TUPLE, T3_TUPLE, 
 *                                 T2_TUPLE_HASH, T3_TUPLE_HASH
 *
 *  @author     Mark L. Short
 *  @date       February 9, 2015
 */
#pragma once

#if !defined(__TUPLE_H__)
#define __TUPLE_H__

#ifndef _COMMON_DEF_H__
    #include "CommonDef.h"
#endif

#ifndef _TUPLE_
    #include <tuple>
#endif

typedef std::tuple<LEVEL_T, LEVEL_T>           T2_TUPLE; ///< 2-way tuple type
typedef std::tuple<LEVEL_T, LEVEL_T, LEVEL_T>  T3_TUPLE; ///< 3-way tuple type

#ifndef LEVEL_BITS
/// calculate the number of bits contained in the LEVEL_T type
    #define LEVEL_BITS  (sizeof(LEVEL_T) * CHAR_BIT)
#endif

/**
  @brief A primitive hash function implementation

  Performs the necessary hash calculations to allow the T2_TUPLE to be used as a 
  hash key. For compatibility with collections requiring a hash operation, it is 
  important for the resultant key to properly fit within a size_t.
*/
struct T2_TUPLE_HASH
{
    /// the underlying hash algorithm implementation
    std::size_t operator() (const T2_TUPLE& tpl) const noexcept
    { return static_cast<std::size_t>( (std::get<0>(tpl) << LEVEL_BITS) | 
                                        std::get<1>(tpl) ); };
};

/**
  @brief A primitive hash function implementation

  Performs the necessary hash calculations to allow the T3_TUPLE to be used as a 
  hash key. For compatibility with collections requiring a hash operation, it is 
  important for the resultant key to properly fit within a size_t.
*/
struct T3_TUPLE_HASH
{
    /// the underlying hash algorithm implementation
    std::size_t operator() (const T3_TUPLE& tpl) const noexcept
    {   auto tpl0 = std::get<0>(tpl);
        auto tpl1 = std::get<1>(tpl);
        auto tpl2 = std::get<2>(tpl);
        return static_cast<std::size_t>(( (std::size_t)tpl2 << (CHAR_BIT << 1) ) ^
                                        ( (std::size_t)tpl1 <<  CHAR_BIT ) ^
                                          tpl0 ); };
};

void PrintTuple(const T2_TUPLE& tpl);
void PrintTuple(const T3_TUPLE& tpl);


#endif
#pragma once

#if !defined(__MATH_UTILITY_H__)
#define __MATH_UTILITY_H__
/**
 *  @file       MathUtility.h
 *  @brief      Mathematical utility method declarations
 *  @author     Mark L. Short
 *  @date       February 9, 2014
 */

#ifndef _INC_MEMORY
    #include <memory.h>
#endif

#ifndef __COMMON_DEF_H__
    #include "CommonDef.h"
#endif

/**
  a very primitive, recursive factorial implementation
*/
struct factorial 
{
    size_t operator()(const size_t n) const 
    {
        return n < 2 ? 1 : (*this)(n - 1) * n;
    }
};

/**
   @brief Dynamic programming implementation of C(N,R)

   @sa http://csg.sph.umich.edu/abecasis/class/2006/615.05.pdf
*/
template<size_t _Sz>
struct N_Choose_R 
{
    size_t rgData[_Sz][_Sz];

public:
    N_Choose_R() noexcept
    { memset(rgData, 0, sizeof(rgData)); };

    inline size_t operator()(const size_t N, const size_t R)
    {
        if (N == R || R == 0)
        {
            return 1;
        }
        else if (rgData[N][R])
        {
            return rgData[N][R];
        }
        else
        {
            rgData[N][R] = (*this)(N - 1, R - 1) + (*this)(N - 1, R);
            return rgData[N][R];
        }
    };
};

/*
    C(n,t) = C(n-1,t) + C(n-1,t-1)
    C(n,t) = (n-t+1)C(n,t-1) / t
*/
size_t N_Choose_T(unsigned short nN /* Factor */, unsigned short nT /* T */) noexcept;

/**
  @brief Generates a sequence of numbers using a combinatorical operation

  The following function generates the next combination (lexicographically) 
  given a current sequence of numbers out of a set of nSetsize

  @param [in,out] rgSubset   address of a sequence of numbers to be used as
                             the source and destination in generating the next 
                             combinatorical sequence
  @param [in] nSubset_size   the number of elements in rgSubset
  @param [in] nSet_size      the number of elements in the overall set 

  @retval true               if there are more combinations
  @retval false              if there are no more combinations
*/
bool NextCombination(unsigned short* rgSubset, size_t nSubset_size, size_t nSet_size) noexcept;

bool NextCombination(BYTE* rgSubset, size_t nSubset_size, size_t nSet_size) noexcept;

#endif

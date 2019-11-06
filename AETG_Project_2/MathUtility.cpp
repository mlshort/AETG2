/**
 *  @file       MathUtility.cpp
 *  @brief      Implementation of MathUtility.cpp
 *  @author     Mark L. Short
 *  @date       February 9, 2014
 *
 *  <b>CITE:</b> 
 *      * The following algorithms were based off
 *        of those made publicly available, I can't
 *        exactly remember where.
 */

#include "stdafx.h"

#include <algorithm>
#include <math.h>

#include "MathUtility.h"

size_t N_Choose_T(unsigned short nN /* Factor */, unsigned short nT /* T */) noexcept
{
    size_t nResult = 0;

    if (nT > nN)
       return nResult;

    nResult = 1;

    for (int i = 1; i < nT; i++)
    {
        nResult *= nN - i + 1;
        nResult /= i;
    }

    return nResult;
}

bool NextCombination(unsigned short* rgSubset, size_t nSubset_size, size_t nSet_size) noexcept
{ 
    for (int i = nSubset_size; --i >= 0;) 
    {
        if (++rgSubset[i] <= nSet_size - (nSubset_size - i) ) 
        {
            while (++i < nSubset_size)
                rgSubset[i] = rgSubset[i-1] + 1;
            return true;
        }
    }
    return false;
}

bool NextCombination(BYTE* rgSubset, size_t nSubset_size, size_t nSet_size) noexcept
{ 
    for (int i = nSubset_size; --i >= 0;) 
    {
        if (++rgSubset[i] <= nSet_size - (nSubset_size - i) ) 
        {
            while (++i < nSubset_size)
                rgSubset[i] = rgSubset[i-1] + 1;
            return true;
        }
    }
    return false;
}

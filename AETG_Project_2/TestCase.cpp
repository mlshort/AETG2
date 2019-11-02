/**
 *  @file       TestCase.cpp
 *  @brief      CTestCase class implementation
 *  @author     Mark L. Short
 *  @date       February 9, 2015
 */

#include "stdafx.h"
#include "TestCase.h"

tostream& operator <<(tostream& os, const CTestCase& rhs)
{
    for (auto& it : rhs)
        os << it << _T(" ");

    return os;
}

size_t 
CTestCase::GetNumValidFactors(void) const noexcept
{
    size_t nResult = 0;
    // iterate over our internal data structure
    // and count test to if it contains a valid
    // level value for each factor entry.
    for (auto it : m_rgData)
    {
        if (IsValidLevel(it))
            nResult++;
    }
    return nResult;
}

TCHAR*
CTestCase::ToString(TCHAR* szDest, size_t cchLen) const
{
    size_t nLen = 0;

    for (auto& it : m_rgData)
    {
        nLen += _sntprintf(&szDest[nLen], cchLen - nLen, _T("%d "), it);
    }

    return szDest;
}

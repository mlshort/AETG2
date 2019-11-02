/**
 *  @file       ComponentSystem.cpp
 *  @brief      CComponentSystem class implementation
 *  @author     Mark L. Short
 *  @date       February 9, 2015
 */

#include "stdafx.h"

#include <random>
#include <algorithm>
#include "MathUtility.h"

#include "ComponentSystem.h"

std::random_device g_rd;       ///< used to generate a 32-bit seed
std::mt19937       g_mt;       ///< the Mersenne Twister engine

CComponentSystem::~CComponentSystem( )
{
}

bool
CComponentSystem::Init(FACTOR_T nNumFactors, LEVEL_T nNumLevels)
{ 
    bool bResult = false;

    g_mt.seed(g_rd()); // Initialize the Mersenne Twister engine

    if (( nNumFactors > 0 ) && ( nNumLevels > 0 ))
    {
        m_nNumFactors = nNumFactors;
        m_nNumLevels  = nNumLevels;
        m_rgFactors.resize(m_nNumFactors); 

        for (int nCurFactor = 0; nCurFactor < nNumFactors; nCurFactor++)
        {
            LEVEL_T nMinLevel = static_cast<LEVEL_T>(nCurFactor * nNumLevels);
            LEVEL_T nMaxLevel = nMinLevel + nNumLevels - 1;

            SetLevelRange(nCurFactor, nMinLevel, nMaxLevel);
        }
        bResult = true;
    }
    return bResult;
};

bool 
CComponentSystem::SetLevelRange(FACTOR_T nFactor, LEVEL_T nMinLevel, LEVEL_T nMaxLevel) noexcept
{
    bool bResult = false;
    if (nFactor < m_nNumFactors)
    {
        m_rgFactors[nFactor].SetLevelRange(nMinLevel, nMaxLevel);
        bResult = true;
    }
    return bResult;
}

LEVEL_T 
CComponentSystem::GetMinLevel(FACTOR_T nFactor) const noexcept
{
    LEVEL_T nResult = LEVEL_INVALID;

    if (nFactor < m_nNumFactors)
        nResult = m_rgFactors[nFactor].get_MinLevel( );

    return nResult;
};

LEVEL_T 
CComponentSystem::GetMaxLevel(FACTOR_T nFactor) const noexcept
{
    LEVEL_T nResult = LEVEL_INVALID;

    if (nFactor < m_nNumFactors)
        nResult = m_rgFactors[nFactor].get_MaxLevel( );

    return nResult;
};

LEVEL_T 
CComponentSystem::GetMaxSystemLevel(void) const noexcept
{
    LEVEL_T nResult = LEVEL_INVALID;

    if (m_nNumFactors > 0)
        nResult = m_rgFactors[m_nNumFactors - 1].get_MaxLevel( );

    return nResult;
};

LEVEL_T  
CComponentSystem::GetRandomLevel(FACTOR_T nFactor) const noexcept
{
    LEVEL_T nResult = LEVEL_INVALID;

    if (nFactor < m_nNumFactors)
    {
        if (std::_Is_UIntType<LEVEL_T>::value)
        {
            std::uniform_int_distribution<LEVEL_T>      dist(m_rgFactors[nFactor].get_MinLevel(),
                                                             m_rgFactors[nFactor].get_MaxLevel());
            nResult = dist(g_mt);
        }
        else
        {
            std::uniform_int_distribution<unsigned int> dist(m_rgFactors[nFactor].get_MinLevel(),
                                                             m_rgFactors[nFactor].get_MaxLevel());
            nResult = dist(g_mt);
        }
    }

    return nResult;
}

FACTOR_T 
CComponentSystem::GetRandomFactor(void) const noexcept
{
    FACTOR_T nResult = FACTOR_INVALID;

    if (m_nNumFactors > 1)
    {
        if (std::_Is_UIntType<LEVEL_T>::value)
        {
            std::uniform_int_distribution<LEVEL_T>          dist(0, m_nNumFactors - 1);
            nResult = dist(g_mt);
        }
        else
        {
            std::uniform_int_distribution<unsigned int>     dist(0, m_nNumFactors - 1);
            nResult = dist(g_mt);
        }
    }

    return nResult;
}

FACTOR_T
CComponentSystem::GetFactor(LEVEL_T nLevel) const noexcept
{
    FACTOR_T nResult = FACTOR_INVALID;

    if (m_nNumFactors > 0)
    {
        int iCtr = 0;
        for (auto it = m_rgFactors.begin(); (it != m_rgFactors.end()) && 
                                    (IsValidFactor(nResult) == false); ++it)
        {
            if ( it->IsInRange(nLevel) )
            {
                nResult = static_cast<FACTOR_T>(iCtr);
            }
            iCtr++;
        }
    }

    return nResult;
}

size_t
CComponentSystem::GetShuffledFactors(std::vector<FACTOR_T>& vShuffledFactors) const
{
    size_t nResult = 0;

    if (m_nNumFactors > 1) // need at least 2 factors to shuffle
    {
        // initialize the array
        vShuffledFactors.resize(m_nNumFactors);
        for (int i = 0; i < m_nNumFactors; i++)
            vShuffledFactors[i] = i;

        // in-place shuffle the array using the Mersenne Twister engine
        std::shuffle(vShuffledFactors.begin(), vShuffledFactors.end(), g_mt);

        nResult = vShuffledFactors.size();
    }

    return nResult;
}

size_t
CComponentSystem::CalcNumberOfVariableCombinations(WORD nT) const
{
    size_t nResult = 0;
 
    ///  @todo following uses 20020 bytes of stack space
    N_Choose_R<50> nChooseR;

    nResult = nChooseR(m_nNumFactors, nT);

    return nResult;
}

size_t
CComponentSystem::CalcNumberOfTWayConfigurations(WORD nT) const
{
    size_t nResult = 0;

    nResult = static_cast<size_t>(pow(m_nNumLevels, nT) * 
                                  CalcNumberOfVariableCombinations(nT));

    return nResult;
}



/**
 *  @file       TestSuite.cpp
 *  @brief      CTestSuite class implementation
 *  @author     Mark L. Short
 *  @date       February 9, 2015
 */

#include "stdafx.h"

#include <random>
#include "ComponentSystem.h"
#include "DebugUtility.h"
#include "MathUtility.h"

#include "TestSuite.h"

/// @todo refactor this
extern std::mt19937       g_mt;
/// @todo make this configurable
const  int  T_WAY = 3;

/**
  @brief  Returns the index of the largest level of a sequence
  
  @param [in] vLevels        target sequence of level values

  @retval LEVEL_T            on success containing the index value 
  @retval LEVEL_INVALID      on error
*/
LEVEL_T FindGreatestOccuringLevel(const std::vector<LEVEL_T>& vLevels);

CTestSuite::~CTestSuite( )
{
}

void
CTestSuite::Init(const CComponentSystem& system)
{
    m_nMaxSystemLevel = system.GetMaxSystemLevel( );
    m_nNumFactors     = system.get_NumFactors( );

    m_rgLevelCount.assign(m_nMaxSystemLevel + 1, 0);

    m_setUncoveredT2Tuples.clear();
    m_setUncoveredT3Tuples.clear();
    m_lstTestSuite.clear();
};

size_t 
CTestSuite::GenerateUncoveredT2Tuples(const CComponentSystem& system)
{
    m_setUncoveredT2Tuples.clear();

    m_rgLevelCount.assign(m_nMaxSystemLevel + 1, 0);

    for (FACTOR_T nCurFactor = 0; nCurFactor < m_nNumFactors - 1; nCurFactor++)
    {
        LEVEL_T nMinFactorLevel = system.GetMinLevel(nCurFactor);
        LEVEL_T nMaxFactorLevel = system.GetMaxLevel(nCurFactor);

        for (LEVEL_T j = nMinFactorLevel; j <= nMaxFactorLevel; j++)
        {
            for (LEVEL_T k = nMaxFactorLevel + 1; k <= m_nMaxSystemLevel; k++)
            {
                auto tuple = std::make_tuple(j, k);
                
                m_setUncoveredT2Tuples.insert(tuple);
                m_rgLevelCount[j] ++;
                m_rgLevelCount[k] ++;
            }
        }
    }

    return m_setUncoveredT2Tuples.size();
}


size_t 
CTestSuite::GenerateUncoveredT3Tuples(const CComponentSystem& system)
{
    m_setUncoveredT3Tuples.clear();

    m_rgLevelCount.assign(m_nMaxSystemLevel + 1, 0);

    std::vector<FACTOR_T> vColumns(T_WAY);

    FACTOR_T colFactor = 0;
    for (auto& it : vColumns)
        it = colFactor++;

    bool bMoreCombinations = true;
    while( bMoreCombinations )
    {    
        TRange<LEVEL_T> FactorRange[T_WAY];

        for (int i = 0; i < T_WAY; i++)
        {
            FactorRange[i].set_Min(system.GetMinLevel(vColumns[i]));
            FactorRange[i].set_Max(system.GetMaxLevel(vColumns[i]));
        }

        for (LEVEL_T j0 = FactorRange[0].get_Min(); j0 <= FactorRange[0].get_Max(); j0++)
        {
            for (LEVEL_T j1 = FactorRange[1].get_Min(); j1 <= FactorRange[1].get_Max(); j1++)
            {
                for (LEVEL_T j2 = FactorRange[2].get_Min(); j2 <= FactorRange[2].get_Max(); j2++)
                {
                    auto tuple = std::make_tuple(j0, j1, j2);

                    auto result = m_setUncoveredT3Tuples.insert(tuple);
#ifdef _DEBUG
                    if (result.second == false)
                        DebugTrace(_T("*** Attempt to insert duplicate tuple *** \n") );
#endif
                    m_rgLevelCount[j0] ++;
                    m_rgLevelCount[j1] ++;
                    m_rgLevelCount[j2] ++;

                }
            }
        }

        bMoreCombinations = NextCombination( vColumns.data(), vColumns.size(), m_nNumFactors);
    }

    return m_setUncoveredT3Tuples.size();
}

size_t 
CTestSuite::GenerateTestCaseT2Tuples(const CTestCase& TestCase)
{
    size_t nResult     = 0;
    size_t nNumFactors = TestCase.get_Size();

    if (nNumFactors > 1) // need at least 2 factors in the test case
    {
        m_rgTestCaseT2Tuples.clear();
        // preallocate enough memory to prevent
        // incremental memory reallocations as new items are being added
        
        m_rgTestCaseT2Tuples.reserve(nNumFactors * nNumFactors);
        for (FACTOR_T i = 0; i < nNumFactors - 1; i++)
        {
            LEVEL_T nLevel1 = TestCase[i];
            if ( IsValidLevel(nLevel1) )
            {
                for (FACTOR_T j = i + 1; j <= nNumFactors - 1; j++)
                {
                    LEVEL_T nLevel2 = TestCase[j];
                    if ( IsValidLevel(nLevel2) )
                    {
                        auto tuple = std::make_tuple(nLevel1, nLevel2);
                        m_rgTestCaseT2Tuples.push_back(tuple);
                        nResult++;
                    }
                }
            }
        }
    }

    return nResult;
}

size_t 
CTestSuite::GenerateTestCaseT3Tuples(const CTestCase& TestCase)
{
    m_rgTestCaseT3Tuples.clear();
    
    if (TestCase.GetNumValidFactors() >= T_WAY) // need at least T_WAY valid factors in the test case
    {
        std::vector<FACTOR_T> vColumns(T_WAY);
        FACTOR_T colFactor = 0;
        for (auto& it : vColumns)
            it = colFactor++;

        bool bMoreCombinations = true;
        while (bMoreCombinations)
        {
            LEVEL_T nLevel1 = TestCase[vColumns[0]];
            if (IsValidLevel(nLevel1))
            {
                LEVEL_T nLevel2 = TestCase[vColumns[1]];
                if (IsValidLevel(nLevel2))
                {
                    LEVEL_T nLevel3 = TestCase[vColumns[2]];
                    if (IsValidLevel(nLevel3))
                    {
                        auto tuple = std::make_tuple(nLevel1, nLevel2, nLevel3);
                        m_rgTestCaseT3Tuples.push_back(tuple);
                    }
                }
            }
            bMoreCombinations = NextCombination(vColumns.data(), vColumns.size(), m_nNumFactors);
//            bMoreCombinations = NextCombination(vColumns.data(), vColumns.size(), nNumFactors);
        }
    }

    return m_rgTestCaseT3Tuples.size();
}

size_t 
CTestSuite::CalculateT2TestCaseCoverage(const CTestCase& TestCase)
{
    size_t nResult = 0;

    GenerateTestCaseT2Tuples(TestCase);

    for (auto& it : m_rgTestCaseT2Tuples)
    {
        if (m_setUncoveredT2Tuples.find(it) != m_setUncoveredT2Tuples.end())
            nResult++;
    }
    
    return nResult;
}

size_t 
CTestSuite::CalculateT3TestCaseCoverage(const CTestCase& TestCase)
{
    size_t nResult = 0;

    if (GenerateTestCaseT3Tuples(TestCase))
    {
        for (auto& it : m_rgTestCaseT3Tuples)
        {
            if (m_setUncoveredT3Tuples.find(it) != m_setUncoveredT3Tuples.end( ))
                nResult++;
        }
    }
    else
    {
#ifdef _DEBUG
        TCHAR szBuffer[256] = {0};
        DebugTrace(_T("** ( %s ) - No Tuples Generated \n"),
                   TestCase.ToString(szBuffer, _countof(szBuffer) - 1) );
#endif
    }
    
    return nResult;
}


size_t 
CTestSuite::AddToT2TestSuite(const CTestCase& TestCase)
{
    GenerateTestCaseT2Tuples(TestCase);

    // iterate over the test case tuples and remove them from the 
    // uncovered tuple set
    for (auto& it : m_rgTestCaseT2Tuples)
    {
        if (m_setUncoveredT2Tuples.erase(it) > 0)
        {
            int iLvl = std::get<0>(it);
            m_rgLevelCount[iLvl] --;

            iLvl = std::get<1>(it);
            m_rgLevelCount[iLvl] --;
        }
    }

    m_lstTestSuite.push_back(TestCase);

    return m_lstTestSuite.size();
}

size_t 
CTestSuite::AddToT3TestSuite(const CTestCase& TestCase)
{
    size_t nResult = 0;

    if (GenerateTestCaseT3Tuples(TestCase))
    {
        // iterate over the test case tuples and remove them from the 
        // uncovered tuple set
        for (auto& it : m_rgTestCaseT3Tuples)
        {
            if (m_setUncoveredT3Tuples.erase(it) > 0)
            {
                int iLvl = std::get<0>(it);
                m_rgLevelCount[iLvl] --;

                iLvl = std::get<1>(it);
                m_rgLevelCount[iLvl] --;

                iLvl = std::get<2>(it);
                m_rgLevelCount[iLvl] --;
            }
        }

        m_lstTestSuite.push_back(TestCase);
        nResult = m_lstTestSuite.size();
    }

    return nResult;
}


LEVEL_T 
FindGreatestOccuringLevel(const std::vector<LEVEL_T>& vLevels)
{
    LEVEL_T nResult        = LEVEL_INVALID;

    int     iMaxLevelCount = 0;
    int     iNumLevels     = vLevels.size();
    int     iRandomMod     = 2;

    for (int i = 0; i < iNumLevels; i++)
    {
        if (vLevels[i] > iMaxLevelCount)
        {
            iMaxLevelCount = vLevels[i];
            nResult        = i;
        }
        else if ((vLevels[i] == iMaxLevelCount) && (vLevels[i] > 0))
        {   // some level of randomization to mix things up a bit
            if ((rand() % iRandomMod) == 0)
            {
                nResult = i;
                iRandomMod ++;
            }
        }
    }

    return nResult;
}

/**
    In AETG, covering arrays are constructed one row at a time. To generate a 
    row, the first t-tuple is selected based on the one involved in most uncovered
    pairs. Remaining factors are assigned levels in a random order. Levels are
    selected based on the one that covers the most new t-tuples. For each row that 
    is actually added to the covering array, there are a number, M, candidate rows 
    that are generated and only a candidate that covers the most new t-tuples is 
    added to the covering array. Once a covering is constructed, a number, R, of 
    test suites are generated and the smallest test suite generated is reported. 
    This process continues until all pairs are covered.

<b> AETG Example 1: </b>

1 begin
2   set MinArray to ∞
3   for i ← 1 to R [number of test suites] do
4       start with no tests in T
5       N ← ∞
6       while there are uncovered t-tuples in T do
7           start with an empty test C and an empty test BestCandidate
8           for j ← 1 to M [candidate rows] do
9               select the first pair that appears in the largest number of uncovered pairs
10              while free factors remain do
11                  randomly select a factor f
12                  select a level v that is in the largest number of uncovered pairs with uniform factors
13              end while
14              if C covers more t-tuples than BestCandidate then
15                  BestCandidate ← C
16              end if
17          end for
18          add test BestCandidate to T
19          N ← N + 1
20      end while
21      if T has N < MinArray tests then
22          MinArray ← N
23          BestArray ← T
24      end if
25  end for
26 end

<b> AETG Example 2: </b>

1. initialize test set TS to be an empty set
2. let UC be the set of all t-way combinations to be covered
3. while (UC is not empty) 
    {
4.    let p,v be the parameter value that appears the most number of times in UC
5.    // generate m candidates
6.    for (int i = 0; i < m; i++) 
        {
7.        let p be the 1st parameter, and reorder all other parameters randomly,
              denote the reordered parameters as p1, p2, ... pk.
8.        // select values for each candidate
9.        for (int j = 1; j <= k; j++) 
              {
10.          select a value for the j-th parameter pj such that the most uncovered
               combinations can be covered
11.           }
12.       }
13. select a test r from m candidates such that it covers the most uncovered combinations
14. add r into ts and remove from UC the set of combinations covered by r
15. }
16. return ts

*/
size_t 
CTestSuite::GenerateT2TestCaseCandidate(const CComponentSystem& system, CTestCase& Candidate)
{
    size_t   nResult = 0;

    if (m_nNumFactors > 1) // no reason to go further if we don't have at least 2 factors
    {
        // initialize the candidate
        Candidate.Init(m_nNumFactors);

        //  1. Choose a parameter f and a value l for f such that that parameter 
        //     value appears in the greatest number of uncovered pairs.

        LEVEL_T  nBestLevel  = FindGreatestOccuringLevel(m_rgLevelCount);
        FACTOR_T nCurFactor  = system.GetFactor(nBestLevel);
        //   DebugTrace (_T("Greatest Occuring (Factor,Level): (%d, %d) \n"), nFactor, nLevel);
        Candidate[nCurFactor]  = nBestLevel;

        //  2. Let f1 = f. Then choose a random order for the remaining parameters. 
        //     Then we have an order for all k parameters f1, ... fk.

        std::vector<FACTOR_T> vFactorOrder;
        //  randomize factor order
        system.GetShuffledFactors(vFactorOrder);

        for (size_t i = 0; i < vFactorOrder.size( ); i++)
        {
            nCurFactor = vFactorOrder[i];
            // verify to make sure we have not already included it in our candidate
            if (IsValidLevel(Candidate[nCurFactor]) == false)
            {
                // now we need to determine which of the levels for this factor
                // will cover the greatest number of uncovered t-tuples
                LEVEL_T nMinFactorLevel = system.GetMinLevel(nCurFactor);
                LEVEL_T nMaxFactorLevel = system.GetMaxLevel(nCurFactor);

                nBestLevel = LEVEL_INVALID;
                size_t nCoverage     = 0;
                size_t nBestCoverage = 0;
                int    iRandomMod    = 2;

                for (int j = nMinFactorLevel; j <= nMaxFactorLevel; j++)
                {
                    Candidate[nCurFactor] = j;
                    nCoverage = CalculateT2TestCaseCoverage(Candidate);

                    if (nCoverage > nBestCoverage)
                    {
                        nBestCoverage = nCoverage;
                        nBestLevel    = j;         // identify the level with the best coverage
                        nResult       = nCoverage; // return best coverage value to caller
                    }
                    else if (nCoverage == nBestCoverage)
                    {  // let's randomly determine who is going to be considered the highest
                        if (( rand( ) % iRandomMod ) == 0)
                        {
                            nBestLevel = j;
                            iRandomMod++;
                        }
                    }
                }

                // if we didn't find a level needing to be covered, go
                // ahead and assign something reasonable for the level
                if (IsValidLevel(nBestLevel) == false)
                    nBestLevel = nMinFactorLevel;

                Candidate[nCurFactor] = nBestLevel;
            }
        }
    }

    return nResult;
}

size_t 
CTestSuite::GenerateT3TestCaseCandidate(const CComponentSystem& system, CTestCase& Candidate)
{
    size_t   nResult = 0;

    if (m_nNumFactors > 2) // no reason to go further if we don't have at least 3 factors
    {
        // initialize the candidate
        Candidate.Init(m_nNumFactors);

        //  1. Choose a parameter f and a value l for f such that that parameter value appears 
        //     in the greatest number of uncovered pairs.

        LEVEL_T  nBestLevel = FindGreatestOccuringLevel(m_rgLevelCount);

#ifdef _DEBUG
        if (IsValidLevel(nBestLevel) == false)
            DebugTrace (_T("*** FindGreatestOccuringLevel - LEVEL_INVALID \n") );
#endif
        FACTOR_T nCurFactor = system.GetFactor(nBestLevel);

#ifdef _DEBUG
//      DebugTrace (_T("  Greatest Occuring (Factor,Level): (%d, %d) \n"), nCurFactor, nBestLevel);
#endif
        Candidate[nCurFactor] = nBestLevel;

        //  2. Let f1 = f. Then choose a random order for the remaining parameters. Then we have 
        //     an order for all k parameters f1, ... fk.

        std::vector<FACTOR_T> vFactorOrder;
        //  randomize factor order
        system.GetShuffledFactors(vFactorOrder);

        for (size_t i = 0; i < vFactorOrder.size(); i++)
        {
            nCurFactor = vFactorOrder[i];
            // verify to make sure we have not already included it in our candidate
            if (IsValidLevel(Candidate[nCurFactor]) == false)
            {
                // now we need to determine which of the levels for this factor
                // will cover the greatest number of uncovered t-tuples
                LEVEL_T nMinFactorLevel = system.GetMinLevel(nCurFactor);
                LEVEL_T nMaxFactorLevel = system.GetMaxLevel(nCurFactor);
                size_t  nCoverage       = 0;
                size_t  nBestCoverage   = 0;
                int     iRandomMod      = 2;
                nBestLevel     = LEVEL_INVALID;
                // Check to see if the Candidate has T_WAY-1 number of valid factors assigned
                if (Candidate.GetNumValidFactors() < T_WAY-1)
                {  // no?
                   // then just iterate over the min..max levels for the factor
                   // and assign the one that has the highest level count
                    
                    for (int n = nMinFactorLevel; n < nMaxFactorLevel; n++)
                    {
                        if (m_rgLevelCount[n] > nBestCoverage)
                        {
                            nBestLevel    = n;
                            nBestCoverage = m_rgLevelCount[nBestLevel];
                        }
                        else if ((m_rgLevelCount[n] == nBestCoverage) && (nBestCoverage > 0))
                        {// let's randomly determine who is going to be considered the highest
                            if (( rand() % iRandomMod ) == 0)
                            {
                                nBestLevel = n;
                                iRandomMod++;
                            }
                        }
                    }
                    if (IsValidLevel(nBestLevel) == false)
                    {
                        std::uniform_int_distribution<LEVEL_T>  dist(nMinFactorLevel, nMaxFactorLevel);
                        nBestLevel = dist(g_mt);
                    }

                    Candidate[nCurFactor] = nBestLevel;
                }
                else
                {
                    for (int j = nMinFactorLevel; j <= nMaxFactorLevel; j++)
                    {
                        Candidate[nCurFactor] = j;

                        nCoverage = CalculateT3TestCaseCoverage(Candidate);

                        if (nCoverage > nBestCoverage)
                        {
                            nBestCoverage = nCoverage;
                            nBestLevel    = j;         // identify the level with the best coverage
                            nResult       = nCoverage; // return best coverage value to caller
                        }
                        else if ((nCoverage == nBestCoverage) && (nCoverage > 0))
                        {  // let's randomly determine who is going to be considered the highest
                            if (( rand( ) % iRandomMod ) == 0)
                            {
                                nBestLevel = j;
                                iRandomMod++;
                            }
                        }
                    }
                }

                // if we didn't find a level needing to be covered, go
                // ahead and assign something reasonable for the level
                if (IsValidLevel(nBestLevel) == false)
                {
                    std::uniform_int_distribution<LEVEL_T>  dist(nMinFactorLevel, nMaxFactorLevel);
                    nBestLevel = dist(g_mt);
                }

                Candidate[nCurFactor] = nBestLevel;
            }
        }
    }

    return nResult;
}

bool
CTestSuite::GenerateMinLevelCandidate(const CComponentSystem& system, CTestCase& Candidate) const
{
    bool bResult = false;

    if (m_nNumFactors > 1)
    {
        // initialize the test case candidate
        Candidate.Init(m_nNumFactors);

        for (FACTOR_T nCurFactor = 0; nCurFactor < m_nNumFactors; nCurFactor++)
            Candidate[nCurFactor] = system.GetMinLevel(nCurFactor);

        bResult = true;
    }

    return bResult;
};


bool
CTestSuite::GenerateMaxLevelCandidate(const CComponentSystem& system, CTestCase& Candidate) const
{
    bool bResult = false;

    if (m_nNumFactors > 1)
    { 
        // initialize the test case candidate
        Candidate.Init(m_nNumFactors);

        for (FACTOR_T nCurFactor  = 0; nCurFactor < m_nNumFactors; nCurFactor++)
            Candidate[nCurFactor] = system.GetMaxLevel(nCurFactor);

        bResult = true;
    }

    return bResult;
};

size_t
CTestSuite::SpliceTestSuite(std::list<CTestCase>& lstOther)
{
    lstOther.splice(lstOther.begin(), m_lstTestSuite);

    return lstOther.size();
}

size_t
CTestSuite::ClearTestSuite(void) noexcept
{
    m_lstTestSuite.clear();
    return m_lstTestSuite.size();
}

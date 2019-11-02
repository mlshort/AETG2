/**
* @file       AETG_Project_Main.cpp
* @brief      Main source file for the AETG_2 project
* @mainpage
* @author     Mark L. Short
* @date       February 9, 2015
*
* <b>Course:</b>     CSCE 5420 Software Engineering 
*
* <b>Instructor:</b> Dr. Renee Bryce
*
* <b>Assignment:</b>
*
*    In this assignment, you will extend your homework 1 submission to provide 3-way
*    combinational coverage.
*
*    1. For this assignment, you are not required to take strings as input. You 
*       may simply give every factor and every level a unique numerical identifier.
*    2. You should prompt the user for the number of factors and levels so that the 
*       grader can easily grade your program.
*    3. You may implement the program in your choice of programming language.
*    4. Your program may end up using too much time or memory for larger inputs, so 
*       you will need to be careful in your implementation. The papers do not describe 
*       the exact implementation details for the exact data structures that they use. 
*       It is expected that you can effectively make decisions that lead to a fast 
*       and efficient implementation.
*    5. There is randomization in your algorithm. You will run your program 100 times 
*       for each input and report the average size and execution time for each input. 
*       You should also use 50 candidates as done in the AETG literature.
*    6. You will report the results of your algorithm for several inputs listed in 
*       the table on the next page. Of course, it is possible that your results will 
*       slightly vary from their reported results since there is randomization in the 
*       algorithm. However, if your results are off by more than 20% for any inputs 
*       larger than 3^4, you have a bug in your program.
*    7. You should strive to implement an efficient solution. To receive credit for 
*       this assignment, your algorithm cannot run for longer than 20 minutes to find 
*       a solution.
*    8. You are only required to implement a solution for up to 3-way coverage.
*
* <b>AETG algorithm overview:</b>
*
*    Assume that we have a system with k test parameters and that the i-th parameter 
*    has li different values. Assume that we have already selected r test cases. We 
*    select the r+1 by first generating M different candidate test cases and then 
*    choosing one that covers the most new pairs. Each candidate test case is 
*    selected by the following greedy algorithm.
*
*    1. Choose a parameter f and a value l for f such that that parameter value 
*    appears in the greatest number of uncovered pairs.
*
*    2. Let f1 = f. Then choose a random order for the remaining parameters. Then we 
*    have an order for all k parameters f1, ... fk.
*
*    3. Assume that values have been selected for parameters f1, ..., fj. For 
*    1 <= i <= j, let the selected value for fi be called vi. Then choose a value 
*    vj+1 for fj+1 as follows. For each possible value v for fj, find the number of 
*    new pairs in the set of pairs { fj+1 = v and fi = vi for 1 <= i <= j }. Then 
*    let vj+1 be one of the values that appeared in the greatest number of new pairs.
*
*    Note that in this step, each parameter value is considered only once for 
*    inclusion in a candidate test case. Also, that when choosing a value for 
*    parameter fj+1, the possible values are compared with only the j values already 
*    chosen for parameters f1, ..., fj.
*
*
* <b>Cite:</b>
*       * RC Bryce and CJ Colbourn, "A density-based greedy algorithm for 
*         higher strength covering arrays", Softw. Test. Verif. Reliab. 2009
*       * Kuhn, Raghu N. Kacker, Yu Lei. "Practical Combinatorical Testing"
*         National Institute for Standards and Technology, NIST Special Publication 
*         800-142, October 2010, 
*         http://csrc.nist.gov/groups/SNS/acts/documents/SP800-142-101006.pdf
*       * DM Cohen, SR Dalal, ML Fredman, and GC Patton, "The AETG System: An Approach 
*         to Testing Based on Combinatorial Design", Appeared in IEEE Transactions On 
*         Software Engineering, volume 23, number 7, July 1997, pages 437-444, 
*         http://aetgweb.appcomsci.com/papers/1997-tse.html#heur
*       * Manchester, Bryce, Sampath, Samant, Kuhn, Kacker, "Applying higher strength 
*         combinatorial criteria to test case prioritization: a case study", July 27, 2012
*       * Grindal, Offutt, Andler, "Combination Testing Strategies: A Survey",
*         November 16th, 2004, http://csrc.nist.gov/acts/grindal-offutt-andler.pdf
*       * George, "Constructing Covering Arrays", Universitat Politecnica De Valencia,
*         https://riunet.upv.es/bitstream/handle/10251/17027/tesisUPV3917.pdf?sequence=1
*       * Lavavej, "rand() Considered Harmful", Microsoft Corp,
*         http://channel9.msdn.com/Events/GoingNative/2013/rand-Considered-Harmful
*       * Kuhn, Kacker, Yu, Lei, "Introduction to Combinatorial Testing", 2010
*       * James Bach, Patrick Schroeder, "Pairwise Testing: A Best Practice That Isn't", 
*         http://www.satisfice.com
*       * Ellims, Ince, Petre, "AETG vs. Man: an Assessment of the Effectiveness of 
*         Combinatorial Test Data Generation", 2008, Technical Report 2007/08, ISSN
*         1744-1986
*       * Bryce, Colbourn, "One-Test-at-a-Time Heuristic Search for Interaction Test Suites",
*         GECCO'07, July 7-11, 2007 
*       * Bryce, Colbourn, "Test Prioritization for Pairwise Interaction Coverage", A-MOST'05,
*         May 15-21, 2005
*
* <b>Implementation Details:</b>
*
*   1.  The overall greatest challenge in extending the previous assignment of AETG 2-way coverage 
*       to 3-way coverage was in the implementation of a systematic way to generate the set of 
*       uncovered (UC) 3-way tuples.  Ultimately, this was handled through a combinatorial 
*       algorithm.
*   2.  Since the tuple is being stored in an unordered set (or a hash-set), with the values of the
*       tuple being used in the hashing algorithm, the nature of extending the tuple from 2 elements
*       to 3 elements required some tweaking & fine tuning to the algorithm.  This was accomplished 
*       by equal parts research and experimentation.
*   3.  Another fundamental change from the previous assignment was in moving the target platform
*       from x32 to x64.
*   4.  There was extensive amount of time put into testing and performance optimization with various
*       tests performed on a much larger set of AETG configurations than as prescribed in the
*       assignment.  Unfortunately, the generated coverage arrays for those configurations exceeded
*       the limits of the <b>ThreeWay.jar</b>, which emitted an out-of-range exceptions beginning at
*       at test case (v = 6, t = 3, k = 10).
*/

#include "stdafx.h"

#include "DebugUtility.h"
#include "ComponentSystem.h"
#include "TestSuite.h"

#include <chrono>
#include <ostream>
#include <fstream>
#include <sstream>

#if defined(UNICODE) || defined(_UNICODE)
    #define tostream       std::wostream
    #define tofstream      std::wofstream
    #define tstringstream  std::wstringstream
#else
    #define tostream       std::ostream
    #define tofstream      std::ofstream
    #define tstringstream  std::stringstream
#endif

/**
  @brief  describes a type that represents a point in time

  It holds an object of type duration that stores the elapsed time.
*/
typedef std::chrono::time_point<std::chrono::system_clock>  TIME_POINT;

/**
  @brief  holds a time interval
  
  A time interval is an elapsed time between two time points.
*/
typedef std::chrono::duration<double>                       TIME_DURATION;

/// @todo make the following configurable
/// Global const specifying the number of repetitions
constexpr const int     NUM_REPETITIONS = 100;

/// Global component system object
CComponentSystem        g_System;
/// Global test suite object
CTestSuite              g_TestSuite;



/**
    @brief Outputs test suite to an ostream

    @param [in,out] os      reference to an ostream
    @param [in] testSuite   target

    @retval tostream
*/
tostream& OutputTestSuite(tostream& os, const CTestSuite& testSuite)
{
    os << testSuite.get_TestSuiteSize() << std::endl << std::endl;

    for (auto &it : testSuite )
        os << it << std::endl;

    return os;
}

/**
    @brief Outputs test suite to an ostream

    @param [in,out] os      reference to an ostream
    @param [in] testSuite   target

    @retval tostream
*/
tostream& OutputTestSuite(tostream& os, const std::list<CTestCase>& testSuite)
{
    os << testSuite.size() << std::endl << std::endl;

    for (auto& it : testSuite)
        os << it << std::endl;

    return os;
}

/**
 *   @brief  main application entry point
 */
int _tmain(int argc, _TCHAR* argv[])
{
    bool bExit    = false;

    int  iFactors = 0;
    int  iLevels  = 0;

    std::list<CTestCase> lstBestTestSuite;

    // Seed the random-number generator with the current time so that
    // the numbers will be different every time we run.
    srand((unsigned) std::chrono::system_clock::now( ).time_since_epoch( ).count( ));

    while (!bExit)
    {
        tcout << _T("Please enter factors [1..100]:");
        tcin  >> iFactors;
        tcout << _T("Please enter levels  [1..100]:");
        tcin  >> iLevels;

        TIME_DURATION durElapsed;           
        TIME_DURATION durCumulativeElapsedTime; // used to determine average
        size_t nCummulativeAETG = 0;            // used to determine average
        size_t nBestAETG        = 0;
        size_t nWorstAETG       = 0;

        g_System.Init(static_cast<FACTOR_T>(iFactors), 
                      static_cast<LEVEL_T> (iLevels));

        g_TestSuite.Init( g_System );

        CTestCase Candidate;

        for (int i = 0; i < NUM_REPETITIONS; i++)
        {
            size_t     nCurAETG = g_TestSuite.ClearTestSuite();

            TIME_POINT tpStart  = std::chrono::system_clock::now( );

            size_t nNumTuples   = g_TestSuite.GenerateUncoveredT3Tuples(g_System);

            tcout << std::setw(4) << i << _T(") ---------------------------------(") << nNumTuples 
                  << std::endl;

            g_TestSuite.GenerateMinLevelCandidate(g_System, Candidate);
#ifdef _DEBUG
            TCHAR szBuffer[256] = { 0 };

            DebugTrace(_T(" Min) Best ( %s), UC Tuples Covered: %d \n"),
                       Candidate.ToString(szBuffer, _countof(szBuffer)),
                       g_TestSuite.CalculateT3TestCaseCoverage(Candidate));
#endif
            g_TestSuite.AddToT3TestSuite(Candidate);

            g_TestSuite.GenerateMaxLevelCandidate(g_System, Candidate);
#ifdef _DEBUG
            DebugTrace(_T(" Max) Best ( %s), UC Tuples Covered: %d \n"),
                       Candidate.ToString(szBuffer, _countof(szBuffer)),
                       g_TestSuite.CalculateT3TestCaseCoverage(Candidate));
#endif
            g_TestSuite.AddToT3TestSuite(Candidate);

            while (g_TestSuite.get_NumUncoveredT3Tuples() > 0)
            {
                CTestCase            BestCandidate;

                size_t               nBestCoverage = 0;
                int                  iRandomMod    = 2;

                for (int j = 0; j < TEST_CASE_CANDIDATES; j++)
                {
                    size_t nCoverage = g_TestSuite.GenerateT3TestCaseCandidate(g_System, Candidate);
#ifdef _DEBUG
//                    DebugTrace(_T("  %2d) Candidate ( %s) \n"), j,
//                               Candidate.ToString(szBuffer, _countof(szBuffer) - 1) );
#endif
                    if (nCoverage > nBestCoverage)
                    {
                        nBestCoverage  = nCoverage;
                        BestCandidate  = Candidate;
                    }
                    else if ((nCoverage == nBestCoverage) && (nCoverage != 0))
                    { // two candidates with the same coverage?
                      // let's randomly decide who will be considered
                      // the best
                        if (( rand( ) % iRandomMod ) == 0)
                        {
                            BestCandidate = Candidate;
                            iRandomMod    = iRandomMod << 1;
                        }
                    }
                }

                nCurAETG = g_TestSuite.AddToT3TestSuite(BestCandidate);

#ifdef _DEBUG
                DebugTrace(_T("Best ( %s), UC Tuples Covered: %d Remaining: %d SuiteSize: %d\n"), 
                           BestCandidate.ToString(szBuffer, _countof(szBuffer)),
                           nBestCoverage, 
                           g_TestSuite.get_NumUncoveredT3Tuples(), 
                           g_TestSuite.get_TestSuiteSize() );
#endif
            } // end while (g_TestSuite.GetNumUncoveredT3Tuples() > 0)

            TIME_POINT tpEnd = std::chrono::system_clock::now( );

            durElapsed       = tpEnd - tpStart;

            durCumulativeElapsedTime += durElapsed;
            nCummulativeAETG         += nCurAETG;

            if (( nBestAETG == 0 ) || ( nCurAETG < nBestAETG ))
            {
                nBestAETG = nCurAETG;
                lstBestTestSuite.clear();
                g_TestSuite.SpliceTestSuite(lstBestTestSuite);
            }

            if (( nWorstAETG == 0 ) || ( nCurAETG > nWorstAETG ))
            {
                nWorstAETG = nCurAETG;
            }
        } // end for (int i = 0; i < NUM_REPETITIONS; i++)
        tcout << _T("For v = ") << iLevels << (" t = 3") << (" k = ") << iFactors 
              << std::endl;
        tcout << _T("-------------------------------------------------------") << std::endl;
        tcout << _T("Avg execution time: ") << (durCumulativeElapsedTime.count() / NUM_REPETITIONS) 
              << _T("s") << std::endl;
        tcout << _T("Best  mAETG: ")        << nBestAETG  << std::endl;
        tcout << _T("Avg   mAETG: ")        << (nCummulativeAETG / NUM_REPETITIONS) << std::endl;
        tcout << _T("Worst mAETG: ")        << nWorstAETG << std::endl;

        tofstream     of;
        tstringstream ss;

        TCHAR szModulePath[_MAX_PATH] = { 0 };

        GetModulePath(szModulePath, _countof(szModulePath) - 1);

        ss << szModulePath << _T("..\\Data\\CSCE5420_AETG_") 
           << iFactors << _T("_") << iLevels 
           << _T("_mls.txt");

        of.open(ss.str().c_str());

        OutputTestSuite(of, lstBestTestSuite);

        of.close();

        TCHAR iAnyKey;
        tcout << _T("press (c)ontinue or e(x)it ");
        tcin  >> iAnyKey;

        if (iAnyKey == 'x')
            bExit = true;
    }


    return 0;
}


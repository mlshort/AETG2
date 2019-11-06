/**
 *  @file       TestSuite.h
 *  @brief      CTestSuite class interface
 *
 *  Provides type definitions for T2_TUPLE_HASHSET, T3_TUPLE_HASHSET, CTestSuite
 *
 *  @author     Mark L. Short
 *  @date       February 9, 2015
 */

#pragma once

#if !defined(__TEST_SUITE_H__)
#define __TEST_SUITE_H__

#ifndef __COMMON_DEF_H__
    #include "CommonDef.h"
#endif

#ifndef _LIST_
    #include <list>
#endif

#ifndef _UNORDERED_SET_
    #include <unordered_set>
#endif

#ifndef __TEST_CASE_H__
    #include "TestCase.h"
#endif

#ifndef __TUPLE_H__
    #include "Tuple.h"
#endif

/// Global predetermined test case candidate generation
constexpr const int TEST_CASE_CANDIDATES = 50;

// forward declaration
class CComponentSystem;

/**
  @brief  A hash set type definition based on a T2_TUPLE hash algorithm
*/
typedef std::unordered_set<T2_TUPLE, T2_TUPLE_HASH> T2_TUPLE_HASHSET;

/**
  @brief  A hash set type definition based on a T3_TUPLE hash algorithm
*/
typedef std::unordered_set<T3_TUPLE, T3_TUPLE_HASH> T3_TUPLE_HASHSET;

/// @todo need to re-factor CTestSuite into a class hierarchy
/** 
  @brief  Manages test case generation and the and the collection of test cases
*/
class CTestSuite
{
    FACTOR_T               m_nNumFactors;          ///< number of configured system factors
    LEVEL_T                m_nMaxSystemLevel;      ///< maximum system level of any factor
    std::vector<LEVEL_T>   m_rgLevelCount;         ///< current count of uncovered tuple levels
    std::vector<T2_TUPLE>  m_rgTestCaseT2Tuples;   ///< working set of possible T2 tuple coverages
    std::vector<T3_TUPLE>  m_rgTestCaseT3Tuples;   ///< working set of possible T3 tuple coverages
    T2_TUPLE_HASHSET       m_setUncoveredT2Tuples; ///< collection of uncovered T2 tuples
    T3_TUPLE_HASHSET       m_setUncoveredT3Tuples; ///< collection of uncovered T3 tuples
    std::list<CTestCase>   m_lstTestSuite;         ///< collection of test cases

public:
    typedef std::list<CTestCase>::const_iterator const_iterator; ///< exposes the underlying typedef

    /// Default Constructor
    CTestSuite( ) noexcept
        : m_nNumFactors(0),
          m_nMaxSystemLevel(LEVEL_INVALID),
          m_rgLevelCount(),
          m_rgTestCaseT2Tuples(),
          m_rgTestCaseT3Tuples(),
          m_setUncoveredT2Tuples(),
          m_setUncoveredT3Tuples(),
          m_lstTestSuite()
    { };

    /// Default Destructor
    ~CTestSuite( );

/**
    @brief class initializer

    @param [in] system
*/
    void   Init(const CComponentSystem& system);

/**
  @brief  Generate the uncovered 2-way tuple set

  @param [in] system         target CComponentSystem

  @retval size_t             containing the number of uncovered tuples 
                             generated
*/
    size_t GenerateUncoveredT2Tuples(const CComponentSystem& system);

/**
  @brief  Generates the uncovered 3-way tuple set

  @param [in] system         target CComponentSystem

  @retval size_t             containing the number of uncovered tuples 
                             generated
*/
    size_t GenerateUncoveredT3Tuples(const CComponentSystem& system);

/**
  @brief  Calculates a coverage value for a given CTestCase object

  @param [in] TestCase       target CTestCase object

  @retval size_t             number of 2-way tuples covered in 
                             the [partial] test case
*/
    size_t CalculateT2TestCaseCoverage(const CTestCase& TestCase);

/**
  @brief  Calculates a coverage value for a given CTestCase object

  @param [in] TestCase       target CTestCase object

  @retval size_t             number of 3-way tuples covered in 
                             the [partial] test case
*/
    size_t CalculateT3TestCaseCoverage(const CTestCase& TestCase);

/**
  @brief Generates a minimum level CTestCase object

  This method generates a test case candidate using the minimum possible level 
  values for all factors given in a system

  @param [in]   system       CComponentSystem object, containing the inputs
  @param [out]  Candidate    the resultant CTestCase object

  @retval true               on success
  @retval false              on error
*/
    bool   GenerateMinLevelCandidate(const CComponentSystem& system, CTestCase& Candidate) const;

/**
  @brief Generates a maximum level CTestCase object

  This method generates a test case candidate using the maximum possible level 
  values for all factors given in a system

  @param [in]   system       CComponentSystem object, containing the inputs
  @param [out]  Candidate    the resultant CTestCase object

  @retval true               on success
  @retval false              on error
*/
    bool   GenerateMaxLevelCandidate(const CComponentSystem& system, CTestCase& Candidate) const;

/**
  @brief  Generates a 2-way test case candidate

  @param [in]   system       CComponentSystem object, containing the inputs
  @param [out]  Candidate    the resultant CTestCase object

  @retval size_t             containing the candidate coverage value
*/
    size_t GenerateT2TestCaseCandidate(const CComponentSystem& system, CTestCase& Candidate);

/**
  @brief  Generates a 3-way test case candidate

  @param [in]   system       CComponentSystem object, containing the inputs
  @param [out]  Candidate    the resultant CTestCase object

  @retval size_t             containing the candidate coverage value
*/
    size_t GenerateT3TestCaseCandidate(const CComponentSystem& system, CTestCase& Candidate);

/**
  @brief  Adds a new 2-way test case to the suite

  @param [in] TestCase       the new CTestCase object to be added

  @retval size_t             containing the updated number of test 
                             cases in the suite after adding the 
                             target test case
*/
    size_t AddToT2TestSuite           (const CTestCase& TestCase);

/**
  @brief  Adds a new 3-way test case to the suite

  @param [in] TestCase       the new CTestCase object to be added

  @retval size_t             on success, contains the updated number of test 
                             cases in the suite after adding the target test 
                             case
  @retval 0                  on error
*/
    size_t AddToT3TestSuite           (const CTestCase& TestCase);

/**
  @brief  Returns the number of 2-way tuples in the uncovered tuple set

  @retval size_t             containing the current number of 
                             uncovered 2-way tuples
*/
    inline size_t get_NumUncoveredT2Tuples    (void) const noexcept
    { return m_setUncoveredT2Tuples.size(); };

/**
  @brief  Returns the number of 3-way tuples in the uncovered tuple set

  @retval size_t             containing the current number of 
                             uncovered 3-way tuples
*/
    inline size_t get_NumUncoveredT3Tuples    (void) const noexcept
    { return m_setUncoveredT3Tuples.size(); };

/**
  @brief  Returns the current number of CTestCase objects 
          contained in the test suite collection

  @retval size_t             the number of test cases in the test 
                             suite
*/
    inline size_t get_TestSuiteSize         (void) const noexcept
    { return m_lstTestSuite.size(); };


/**
  @brief  Returns an iterator to the beginning of nonmutable sequence of 
          CTestCase objects contained in the current test suite collection

  @retval const_iterator     pointing to the start of the 
                             current test suite
*/
    const_iterator begin(void) const noexcept
    { return m_lstTestSuite.begin(); };

/**
  @brief  Returns an iterator to the end of nonmutable sequence of CTestCase 
          object contained in the current test suite collection

  @retval const_iterator     pointing to the end of the current
                             test suite
*/
    const_iterator end(void) const noexcept
    { return m_lstTestSuite.end(); };

/**
  @brief Transfers ownership of the test suite collection

  Efficiently detaches and transfers the current collection of CTestCase objects 
  to the target list. No elements are copied or moved, only the internal pointers 
  of the list nodes are re-pointed.

  @param [out] lstOther      destination list

  @retval size_t             containing the size of the destination list
*/
    size_t  SpliceTestSuite(std::list<CTestCase>& lstOther);

/**
  @brief Clears the underlying test case collection

  @retval size_t             containing the size of the test case collection
                             after it has been cleared (which should be 0)
*/
    size_t  ClearTestSuite (void) noexcept;

///////////////////////////////////////////////////////////////////////
// Following are internal helper methods used by the CTestSuite object
//
private:

/**
  @brief  Generates a set of tuples from a given test case

  @param [in] TestCase       target CTestCase

  @retval size_t             containing the number of test case 2-way tuples
*/
    size_t  GenerateTestCaseT2Tuples   (const CTestCase& TestCase);

/**
  @brief  Generates a set of tuples from a given test case

  @param [in] TestCase       target CTestCase

  @retval size_t             on success, contains the number of test case 
                             3-way tuples
  @retval 0                  on error
*/
    size_t  GenerateTestCaseT3Tuples   (const CTestCase& TestCase);
};

#endif

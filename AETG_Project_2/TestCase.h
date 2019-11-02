/**
 *  @file       TestCase.h
 *  @brief      CTestCase class interface
 *  @author     Mark L. Short
 *  @date       February 9, 2015
 */
#pragma once

#if !defined(__TEST_CASE_H__)
#define __TEST_CASE_H__

#ifndef __COMMON_DEF_H__
    #include "CommonDef.h"
#endif

#ifndef _VECTOR_
    #include <vector>
#endif

#ifndef _IOSTREAM_
    #include <iostream>
#endif

#ifndef tostream
    #if defined(UNICODE) || defined(_UNICODE)
        #define tostream std::wostream
        #define tistream std::wistream
    #else
        #define tostream std::ostream
        #define tistream std::istream
    #endif
#endif

/**
  @brief A primitive test case class implementation

  The class maintains a collection of (Factor, Level) pairs associated by using 
  a Factor as an index into a sequence of stored Levels.
*/
class CTestCase
{
    typedef std::vector<LEVEL_T> LEVEL_VECTOR;
  
    LEVEL_VECTOR m_rgData; ///< a collection of level values, indexed by factor

public:
    typedef LEVEL_VECTOR::reference       reference;       ///< exposes associated type definition
    typedef LEVEL_VECTOR::const_reference const_reference; ///< exposes associated type definition
    typedef LEVEL_VECTOR::const_iterator  const_iterator;  ///< exposes associated type definition
    typedef LEVEL_VECTOR::size_type       size_type;       ///< exposes associated type definition

    /// Default Constructor
    CTestCase()
        : m_rgData()
    { };
    /// Copy Constructor
    CTestCase(const CTestCase& o)
        : m_rgData(o.m_rgData)
    { };

    /// assignment operator
    CTestCase& operator=(const CTestCase& rhs)
    {
        if (this != &rhs)
        {
            m_rgData = rhs.m_rgData;
        }
        return *this;
    };

/**
  @brief class initializer

  Initializes internal data structures and and allocates storage for variable 
  number of levels to be stored in the test case

  @param [in] nSize          number of levels to be stored in stored in the 
                             CTestCase object
*/
    inline void Init(size_t nSize) noexcept
    {
        // m_rgData.resize(nSize, LEVEL_INVALID);
         m_rgData.assign(nSize, LEVEL_INVALID);
    };

/**
  @brief  Returns the length of the stored level sequence

  @retval size_t             containing number of elements
*/
    inline size_t get_Size(void) const noexcept
    { return m_rgData.size(); };

/**
  @brief subscript to the level nonmutable sequence

  @param [in] nPos           position index

  @retval const_reference    to nPos'th level in the test case
*/
    const_reference operator[](size_type nPos) const
    { return m_rgData.operator[](nPos); };

/**
  @brief subscript to the level mutable sequence

  @param [in] nPos           position index

  @retval reference          to nPos'th element level in the test case
*/
    reference operator[](size_type nPos)
    { return m_rgData.operator[](nPos); };

/**
  @brief Returns a const iterator to the begin of the stored level sequence

  @retval const_iterator     to beginning of nonmutable sequence
*/
    const_iterator begin(void) const noexcept
    { return m_rgData.begin(); };

/**
  @brief Returns a const iterator to the end of the stored level sequence

  @retval const_iterator     to the end of nonmutable sequence
*/
    const_iterator end(void) const noexcept
    { return m_rgData.end(); };

/**
  @brief Returns the number of valid factor entries

  @retval size_t             contains the number of valid factor
                             elements in the contained level collection
*/
    size_t         GetNumValidFactors(void) const noexcept;

/**
  @brief Writes formatted data to a destination buffer

  @param [out] szDest        storage location for output
  @param [in]  cchLen        count of characters to write

  @retval TCHAR*             address of the destination buffer
*/
    TCHAR*         ToString(TCHAR* szDest, size_t cchLen) const;

////////////////////////////////////////////////////////////////////
/// overloaded stream extraction operator
    friend tostream& operator <<(tostream& os, const CTestCase& rhs);

};

/**
  @brief overloaded stream extraction operator

  @param [in,out] os         reference to an ostream object
  @param [in] rhs            target CTestCase object to be written to the 
                             stream

  @retval tostream&          a reference to the resultant stream object
*/
tostream& operator <<(tostream& os, const CTestCase& rhs);

#endif

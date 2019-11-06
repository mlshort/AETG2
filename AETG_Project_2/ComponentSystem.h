/**
 *  @file       ComponentSystem.h
 *  @brief      CComponentSystem class interface
 *
 *  Provides type definitions for: TRange, TFactor, CComponentSystem
 *
 *  @author     Mark L. Short
 *  @date       February 9, 2015
 */

#pragma once

#if !defined(__COMPONENT_SYSTEM_H__)
#define __COMPONENT_SYSTEM_H__

#ifndef __COMMON_DEF_H__
    #include "CommonDef.h"
#endif

#ifndef _VECTOR_
    #include <vector>
#endif

#ifndef _LIST_
    #include <list>
#endif

/**
  @brief a primitive range implementation

  The TRange template class allows the underlying
  min & max data types to be determined when the 
  template class is fully declared.
 */
template<class Ty>
class TRange
{
    Ty m_nMin; ///< inclusive minimum value
    Ty m_nMax; ///< inclusive maximum value

public:
    /// Default Constructor
    constexpr TRange() noexcept
        : m_nMin(0),
          m_nMax(0)
    {};

    /// Initialization Constructor
    constexpr explicit TRange(Ty nMin, Ty nMax) noexcept
        : m_nMin(nMin),
          m_nMax(nMax)
    { };

/**
  @brief  Performs inclusive range test

  @param [in] nVal           target value

  @retval true               if nVal is within range
  @retval false              if nVal is out of range
*/
    constexpr bool IsInRange(Ty nVal) const noexcept
    { return ((m_nMin <= nVal) && (nVal <= m_nMax)); };

//////////////////////////////////////////////
// Primitive data accessors
/**
  @brief  Sets the minimum inclusive range

  @param [in] nSet           minimum range value
*/
    inline void  set_Min(Ty nSet) noexcept
    { m_nMin = nSet; };

/**
  @brief Sets the maximum inclusive range

  @param [in] nSet           maximum range value
*/
    inline void  set_Max(Ty nSet) noexcept
    { m_nMax = nSet; };

/**
  @brief  Returns the minimum inclusive range value

  @retval Ty                 value that contains the minimum inclusive range
*/
    constexpr Ty   get_Min(void) const noexcept
    { return m_nMin; };

/**
  @brief  Returns the maximum inclusive range value

  @retval Ty                 value that contains the maximum inclusive range
*/
    constexpr Ty   get_Max(void) const noexcept
    { return m_nMax; };

};


/**
  @brief manages ranges associated with a factor
  
  The TFactor class template is a primitive abstraction
  of a factor and associated permissible level ranges.
*/
template <class Ty>
class TFactor
{
    TRange<Ty> m_Levels;  ///< associated range of valid levels

public:
    /// Default Constructor
    constexpr TFactor() noexcept
        : m_Levels(LEVEL_INVALID, LEVEL_INVALID)
    { };

    /// Initialization Constructor
    constexpr explicit TFactor(Ty nMin, Ty nMax) noexcept
        : m_Levels(nMin, nMax)
    { };

/**
  @brief  sets the inclusive level range values

  @param [in] nMin           inclusive minimum range value
  @param [in] nMax           inclusive maximum range value
*/
    inline void SetLevelRange(Ty nMin, Ty nMax) noexcept
    {
        m_Levels.set_Min(nMin);
        m_Levels.set_Max(nMax);
    };

/**
  @brief  Tests to see if a given level value is within the configured inclusive
          range values

  @param [in] nLevel         target level value

  @retval true               if nLevel is within configured level range
  @retval false              if nLevel is not within range
*/
    constexpr bool IsInRange(Ty nLevel) const noexcept
    { return m_Levels.IsInRange(nLevel); };

////////////////////////////////////////////////
// Primitive data accessors

/**
  @brief Returns the minimum inclusive range value

  @retval Ty                 containing the minimum inclusive level
*/
    inline Ty get_MinLevel(void) const noexcept
    { return m_Levels.get_Min(); };

/**
  @brief Returns the maximum inclusive range value

  @retval Ty                 containing the maximum inclusive level
*/
    constexpr Ty get_MaxLevel(void) const noexcept
    { return m_Levels.get_Max(); };
};

/**
  @brief  Facilitates the management of the underlying component system

  In essence, the CComponentSystem class models the valid variable-value 
  configurations as described below:

  Definition. For a set of t variables, a variable-value configuration is a set 
  of t valid values for the variables. 

  Example. Given four binary variables, a, b, c, and d, a=0, c=1, d=0 is a 
  variable-value configuration, and a=1, c=1, d=0 is a different variable-value 
  configuration for the same three variables a, c, and d.

  http://csrc.nist.gov/groups/SNS/acts/coverage_measure.html

*/
class CComponentSystem
{
    FACTOR_T                        m_nNumFactors; ///<  number of factors
    LEVEL_T                         m_nNumLevels;  ///<  number of levels configured per factor
    std::vector<TFactor<LEVEL_T>>   m_rgFactors;   ///<  array of TFactors

public:
    /// Default Constructor
    CComponentSystem() noexcept
        :m_nNumFactors(0),
         m_nNumLevels(0),
         m_rgFactors()
    { };

    /// Destructor
    ~CComponentSystem( );

/**
  @brief  class initializer

  @param [in] nFactors       the max number of system factors permitted
  @param [in] nLevels        the max number of associated levels per factor 
                             permitted

  @retval true               on success and the system was successfully 
                             initialized
  @retval false              on error or invalid parameter values
*/
    bool     Init         (FACTOR_T nFactors, LEVEL_T nLevels);

/**
  @brief  Sets the level range for a particular factor

  @param [in] nFactor        target factor
  @param [in] nMinLevel      minimum level value for this factor
  @param [in] nMaxLevel      maximum level value for this factor

  @retval true               on success and the factor's level range 
                             successfully set
  @retval false              on error, target factor is invalid or out-of-range
*/
    bool     SetLevelRange(FACTOR_T nFactor, LEVEL_T nMinLevel, LEVEL_T nMaxLevel) noexcept;

/**
  @brief  Returns the minimum inclusive level value assigned to a particular
          factor

  @param [in] nFactor        target factor 

  @retval LEVEL_T            on success containing the factor's min level value
  @retval LEVEL_INVALID      on error
*/
    LEVEL_T  GetMinLevel      (FACTOR_T nFactor) const noexcept;

/**
  @brief  Returns the maximum inclusive level value assigned to a particular
          factor

  @param [in] nFactor        target factor

  @retval LEVEL_INVALID      on error
  @retval LEVEL_T            on success containing the factor's max level value
*/
    LEVEL_T  GetMaxLevel      (FACTOR_T nFactor) const noexcept;

/**
  @brief  Returns the overall maximum inclusive level value assigned to any
          factor

  @retval LEVEL_T            on success containing the system's max level value
  @retval LEVEL_INVALID      on error
*/
    LEVEL_T  GetMaxSystemLevel(void) const noexcept;

/**
  @brief  Returns a random level value given a target factor

  A random number is generated given the range of a factor's configured 
  [min ... max] level value, using the Mersenne Twister engine.

  @param [in] nFactor        target factor

  @retval LEVEL_T            on success containing a random generated level
  @retval LEVEL_INVALID      on error
*/
    LEVEL_T  GetRandomLevel   (FACTOR_T nFactor) const;

/**
  @brief Returns the factor associated with a given level

  @param  [in] nLevel        target level

  @retval FACTOR_T           on success containing a factor value
  @retval FACTOR_INVALID     on error or if target level is invalid
*/
    FACTOR_T GetFactor        (LEVEL_T nLevel) const noexcept;

/**
  @brief Returns a random factor value

  A random factor is generated, given the current configuration in the range of 
  [0..m_nNumFactors - 1], using the Mersenne Twister engine.

  @retval FACTOR_T           on success containing a random factor value
  @retval FACTOR_INVALID     on error
*/
    FACTOR_T GetRandomFactor  (void) const;

/**
  @brief Performs an in-place shuffling of factors

  This method initializes an sequence of factors based on the current system 
  configuration as [0 .. m_nNumFactors - 1].  It then performs a random in-place 
  shuffling of the sequence using the Mersenne Twister engine.

  @param [out] vShuffledFactors  target destination

  @retval size_t             containing number of factors returned
  @retval 0                  on error
*/
    size_t     GetShuffledFactors(std::vector<FACTOR_T>& vShuffledFactors) const;

/**
  @brief  Returns the number of T-way system variable combinations 

  The number of t-way combinations in an array of n variables is 
  C(n,t) = n!/(n-t)!t!, or “n choose t” in combinatorics, the number 
  of ways of taking t out of n things at a time.

  @sa
  http://csrc.nist.gov/groups/SNS/acts/documents/kuhn-kacker-lei-isse-14-preprint.pdf

  In this implementation, m_nNumFactors used for 'n' and nT for 't', resulting 
  in a calculation of C(m_nFactors, nT).

  @param [in] nT             the T-way value

  @retval size_t             containing the number of combinations
  @retval 0                  on error
*/
    size_t   CalcNumberOfVariableCombinations(WORD nT) const;

/**
  @brief  Returns the number of T-way variable-value configurations

  @param [in] nT             the T-way value

  @retval size_t             containing the number of configurations
*/
    size_t   CalcNumberOfTWayConfigurations(WORD nT) const;

// Primitive data accessors
/**
  @brief  Returns the number of factors currently configured
          in the system.

  @retval FACTOR_T           containing the number of factors
*/
    constexpr FACTOR_T get_NumFactors(void) const noexcept
        { return m_nNumFactors; };
};

#endif

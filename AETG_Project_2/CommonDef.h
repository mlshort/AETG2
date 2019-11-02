/**
 *  @file       CommonDef.h
 *  @brief      Common type definitions
 *  @author     Mark L. Short
 *  @date       February 9, 2015
 */
#pragma once

#if !defined(__COMMON_DEF_H__)
#define __COMMON_DEF_H__

#ifndef _LIMITS_
    #include <limits>
#endif

typedef unsigned __int8  BYTE;   ///< 8-bit unsigned type
typedef unsigned __int16 WORD;   ///< 16-bit unsigned type
typedef unsigned __int32 DWORD;  ///< 32-bit unsigned type

typedef WORD           FACTOR_T; ///< factor value type
typedef WORD           LEVEL_T;  ///< level value type

/// used for level validation
constexpr const LEVEL_T  LEVEL_INVALID  = std::numeric_limits<LEVEL_T>::max(); 

/// used for factor validation
constexpr const FACTOR_T FACTOR_INVALID = std::numeric_limits<FACTOR_T>::max();

/**
 *  @brief  Performs basic validation of a level value 
 *
 *  @param [in] nLevel         value to be verified
 *
 *  @retval true               if nLevel is valid
 *  @retval false              if nLevel is invalid
 */
constexpr bool IsValidLevel(LEVEL_T nLevel) noexcept
{ return (nLevel != LEVEL_INVALID); };

/**
 *  @brief Performs basic validation of a factor value 
 *
 *  @param [in] nFactor        value to be verified
 *
 *  @retval true               if nFactor is valid
 *  @retval false              if nFactor is invalid
*/
constexpr bool IsValidFactor(FACTOR_T nFactor) noexcept
{ return (nFactor != FACTOR_INVALID); };


#endif

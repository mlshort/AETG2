/**
 *  @file       Tuple.cpp
 *  @brief      Tuple utility methods
 *  @author     Mark L. Short
 *  @date       February 9, 2015
 */

#include "stdafx.h"
#include "Tuple.h"

void PrintTuple(const T2_TUPLE& tpl)
{
   tcout << _T("( ")
         << std::get<0>(tpl) << _T(", ") 
         << std::get<1>(tpl) << _T(" )") << std::endl;
}

void PrintTuple(const T3_TUPLE& tpl)
{
   tcout << _T("( ")
         << std::get<0>(tpl) << _T(", ")
         << std::get<1>(tpl) << _T(", ")
         << std::get<2>(tpl) << _T(" )") << std::endl;
}
/**
 * @brief This file provides some utilities for template meta-programming.
 * @file TemplateUtilities.h
 * @author Skyler Saleh
 **/
#ifndef TemplateUtilities_h
#define TemplateUtilities_h
#ifdef _WIN32
#define INLINE __forceinline
#else
#define INLINE inline __attribute__((always_inline))
#endif
#include <stdint.h>
namespace Argon {
/**
 * @brief This class is used to rank the precision of various intrinsic types, to determine the conversion order.
 * The rank value is higher if the variable holds more precision.
 * @tparam T The type to rank.
 **/
template<typename T>struct TypePromotionRank;
template<>struct TypePromotionRank<bool>{enum {rank=0}; };
template<>struct TypePromotionRank<uint8_t>{enum {rank=1}; };
template<>struct TypePromotionRank<int8_t>{enum {rank=2}; };
template<>struct TypePromotionRank<uint16_t>{enum {rank=3}; };
template<>struct TypePromotionRank<int16_t>{enum {rank=4}; };
template<>struct TypePromotionRank<uint32_t>{enum {rank=5}; };
template<>struct TypePromotionRank<int32_t>{enum {rank=6}; };
template<>struct TypePromotionRank<uint64_t>{enum {rank=7}; };
template<>struct TypePromotionRank<int64_t>{enum {rank=8}; };
template<>struct TypePromotionRank<float>{enum {rank=9}; };
template<>struct TypePromotionRank<double>{enum {rank=10}; };
template<>struct TypePromotionRank<long double>{enum {rank=11}; };

/**
 * @brief This class acts as a compile time if statement.
 * type will be T2 if b is true, or T if b is false.
 * @tparam T The type to typedef type as if b is false.
 * @tparam T2 The type to typedef type as if b is true.
 * @tparam b Determines whether to typedef type as T or T2.
 **/
template<typename T, typename T2, bool b> struct TemplateSwitch;
template<typename T, typename T2> struct TemplateSwitch<T,T2,true>{typedef T2 type; };
template<typename T, typename T2> struct TemplateSwitch<T,T2,false>{typedef T type; };
/**
 * @brief This class uses the TypePromotionRank to determine the necessary return type of an operation so that
 * precision will not be lost, and typedefs type to the necessary type.
 * @tparam T The first type involved in the operation.
 * @tparam T2 The second type involved in the operation.
 **/
template<typename T, typename T2>
struct TypePromotion {
    typedef typename TemplateSwitch<T,T2, TypePromotionRank<T>::rank<TypePromotionRank<T2>::rank>::type type;
};

};

#endif // ifndef TemplateUtilities_h

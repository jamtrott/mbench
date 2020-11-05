/*
 * Benchmark program for common mathematical functions
 * Copyright (C) 2020 James D. Trotter
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Authors: James D. Trotter <james@simula.no>
 *
 * Exceptions for error handling in floating-point calculations.
 */

#include "fexcept.h"

#include <errno.h>

#include <fenv.h>
#include <math.h>

/**
 * `fexcept_clear()` clears stored floating-point exceptions.
 */
void fexcept_clear(
    fexcept_t * fexcept)
{
    if (!(math_errhandling & MATH_ERREXCEPT))
        return;
    feclearexcept(FE_ALL_EXCEPT);
    fegetexceptflag(fexcept, FE_ALL_EXCEPT);
}

/**
 * `fexcept_store()` stores current floating-point exceptions.
 */
void fexcept_store(
    fexcept_t * fexcept,
    int excepts)
{
    if (!(math_errhandling & MATH_ERREXCEPT))
        return;
    fegetexceptflag(fexcept, excepts);
}

/**
 * `fexcept_str()` converts floating-point exceptions to a string.
 */
const char * fexcept_str(
    fexcept_t fexcept)
{
    if (!(math_errhandling & MATH_ERREXCEPT))
        return "disabled";

    fesetexceptflag(&fexcept, FE_ALL_EXCEPT);
    if (fetestexcept(FE_DIVBYZERO)) {
        return "divide-by-zero";
    } else if (fetestexcept(FE_INEXACT)) {
        return "inexact";
    } else if (fetestexcept(FE_INVALID)) {
        return "invalid";
    } else if (fetestexcept(FE_OVERFLOW)) {
        return "overflow";
    } else if (fetestexcept(FE_UNDERFLOW)) {
        return "underflow";
    } else if (fetestexcept(FE_DIVBYZERO | FE_INEXACT)) {
        return "divide-by-zero,inexact";
    } else if (fetestexcept(FE_DIVBYZERO | FE_INVALID)) {
        return "divide-by-zero,invalid";
    } else if (fetestexcept(FE_DIVBYZERO | FE_OVERFLOW)) {
        return "divide-by-zero,overflow";
    } else if (fetestexcept(FE_DIVBYZERO | FE_UNDERFLOW)) {
        return "divide-by-zero,underflow";
    } else if (fetestexcept(FE_INEXACT | FE_INVALID)) {
        return "inexact,invalid";
    } else if (fetestexcept(FE_INEXACT | FE_OVERFLOW)) {
        return "inexact,overflow";
    } else if (fetestexcept(FE_INEXACT | FE_UNDERFLOW)) {
        return "inexact,underflow";
    } else if (fetestexcept(FE_INVALID | FE_OVERFLOW)) {
        return "invalid,overflow";
    } else if (fetestexcept(FE_INVALID | FE_UNDERFLOW)) {
        return "invalid,underflow";
    } else if (fetestexcept(FE_OVERFLOW | FE_UNDERFLOW)) {
        return "overflow,underflow";
    } else if (fetestexcept(FE_DIVBYZERO | FE_INEXACT | FE_INVALID)) {
        return "divide-by-zero,inexact,invalid";
    } else if (fetestexcept(FE_DIVBYZERO | FE_INEXACT | FE_OVERFLOW)) {
        return "divide-by-zero,inexact,overflow";
    } else if (fetestexcept(FE_DIVBYZERO | FE_INEXACT | FE_UNDERFLOW)) {
        return "divide-by-zero,inexact,underflow";
    } else if (fetestexcept(FE_DIVBYZERO | FE_INVALID | FE_OVERFLOW)) {
        return "divide-by-zero,invalid,overflow";
    } else if (fetestexcept(FE_DIVBYZERO | FE_INVALID | FE_UNDERFLOW)) {
        return "divide-by-zero,invalid,underflow";
    } else if (fetestexcept(FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW)) {
        return "divide-by-zero,overflow,underflow";
    } else if (fetestexcept(FE_INEXACT | FE_INVALID | FE_OVERFLOW)) {
        return "inexact,invalid,overflow";
    } else if (fetestexcept(FE_INEXACT | FE_INVALID | FE_UNDERFLOW)) {
        return "inexact,invalid,underflow";
    } else if (fetestexcept(FE_INEXACT | FE_OVERFLOW | FE_UNDERFLOW)) {
        return "inexact,overflow,underflow";
    } else if (fetestexcept(FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW)) {
        return "invalid,overflow,underflow";
    } else if (fetestexcept(FE_DIVBYZERO | FE_INEXACT | FE_INVALID | FE_OVERFLOW)) {
        return "divide-by-zero,inexact,invalid,overflow";
    } else if (fetestexcept(FE_DIVBYZERO | FE_INEXACT | FE_INVALID | FE_UNDERFLOW)) {
        return "divide-by-zero,inexact,invalid,underflow";
    } else if (fetestexcept(FE_DIVBYZERO | FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW)) {
        return "divide-by-zero,invalid,overflow,underflow";
    } else if (fetestexcept(FE_DIVBYZERO | FE_INEXACT | FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW)) {
        return "divide-by-zero,inexact,invalid,overflow,underflow";
    } else {
        return "none";
    }
}

/**
 * `fexcept_is_exception()` returns `true` if there is a
 * floating-point exception.
 */
bool fexcept_is_exception(
    fexcept_t fexcept,
    int excepts)
{
    if (!(math_errhandling & MATH_ERREXCEPT))
        return false;
    feclearexcept(FE_ALL_EXCEPT);
    fesetexceptflag(&fexcept, excepts);
    return fetestexcept(excepts);
}

#ifdef HAVE_MPFR
/**
 * `mpfr_except_str()` converts MPFR floating-point exceptions to a
 * string.
 */
const char * mpfr_except_str(
    mpfr_flags_t mask)
{
    if (mask & MPFR_FLAGS_DIVBY0) {
        return "divide-by-zero";
    } else if (mask & MPFR_FLAGS_INEXACT) {
        return "inexact";
    } else if (mask & MPFR_FLAGS_NAN) {
        return "invalid";
    } else if (mask & MPFR_FLAGS_OVERFLOW) {
        return "overflow";
    } else if (mask & MPFR_FLAGS_UNDERFLOW) {
        return "underflow";
    } else if (mask & MPFR_FLAGS_ERANGE) {
        return "range";
    } else if (mask & MPFR_FLAGS_DIVBY0 && mask & MPFR_FLAGS_INEXACT) {
        return "divide-by-zero,inexact";
    } else if (mask & MPFR_FLAGS_DIVBY0 && mask & MPFR_FLAGS_NAN) {
        return "divide-by-zero,invalid";
    } else if (mask & MPFR_FLAGS_DIVBY0 && mask & MPFR_FLAGS_OVERFLOW) {
        return "divide-by-zero,overflow";
    } else if (mask & MPFR_FLAGS_DIVBY0 && mask & MPFR_FLAGS_UNDERFLOW) {
        return "divide-by-zero,underflow";
    } else if (mask & MPFR_FLAGS_DIVBY0 && mask & MPFR_FLAGS_ERANGE) {
        return "divide-by-zero,range";
    } else if (mask & MPFR_FLAGS_INEXACT && mask & MPFR_FLAGS_NAN) {
        return "inexact,invalid";
    } else if (mask & MPFR_FLAGS_INEXACT && mask & MPFR_FLAGS_OVERFLOW) {
        return "inexact,overflow";
    } else if (mask & MPFR_FLAGS_INEXACT && mask & MPFR_FLAGS_UNDERFLOW) {
        return "inexact,underflow";
    } else if (mask & MPFR_FLAGS_INEXACT && mask & MPFR_FLAGS_ERANGE) {
        return "inexact,range";
    } else if (mask & MPFR_FLAGS_NAN && mask & MPFR_FLAGS_OVERFLOW) {
        return "invalid,overflow";
    } else if (mask & MPFR_FLAGS_NAN && mask & MPFR_FLAGS_UNDERFLOW) {
        return "invalid,underflow";
    } else if (mask & MPFR_FLAGS_NAN && mask & MPFR_FLAGS_ERANGE) {
        return "invalid,range";
    } else if (mask & MPFR_FLAGS_OVERFLOW && mask & MPFR_FLAGS_UNDERFLOW) {
        return "overflow,underflow";
    } else if (mask & MPFR_FLAGS_OVERFLOW && mask & MPFR_FLAGS_ERANGE) {
        return "overflow,range";
    } else if (mask & MPFR_FLAGS_UNDERFLOW && mask & MPFR_FLAGS_ERANGE) {
        return "underflow,range";
    } else if (mask & MPFR_FLAGS_DIVBY0 && mask & MPFR_FLAGS_INEXACT && mask & MPFR_FLAGS_NAN) {
        return "divide-by-zero,inexact,invalid";
    } else if (mask & MPFR_FLAGS_DIVBY0 && mask & MPFR_FLAGS_INEXACT && mask & MPFR_FLAGS_OVERFLOW) {
        return "divide-by-zero,inexact,overflow";
    } else if (mask & MPFR_FLAGS_DIVBY0 && mask & MPFR_FLAGS_INEXACT && mask & MPFR_FLAGS_UNDERFLOW) {
        return "divide-by-zero,inexact,underflow";
    } else if (mask & MPFR_FLAGS_DIVBY0 && mask & MPFR_FLAGS_INEXACT && mask & MPFR_FLAGS_ERANGE) {
        return "divide-by-zero,inexact,range";
    } else if (mask & MPFR_FLAGS_DIVBY0 && mask & MPFR_FLAGS_NAN && mask & MPFR_FLAGS_OVERFLOW) {
        return "divide-by-zero,invalid,overflow";
    } else if (mask & MPFR_FLAGS_DIVBY0 && mask & MPFR_FLAGS_NAN && mask & MPFR_FLAGS_UNDERFLOW) {
        return "divide-by-zero,invalid,underflow";
    } else if (mask & MPFR_FLAGS_DIVBY0 && mask & MPFR_FLAGS_NAN && mask & MPFR_FLAGS_ERANGE) {
        return "divide-by-zero,invalid,range";
    } else if (mask & MPFR_FLAGS_DIVBY0 && mask & MPFR_FLAGS_OVERFLOW && mask & MPFR_FLAGS_UNDERFLOW) {
        return "divide-by-zero,overflow,underflow";
    } else if (mask & MPFR_FLAGS_DIVBY0 && mask & MPFR_FLAGS_OVERFLOW && mask & MPFR_FLAGS_ERANGE) {
        return "divide-by-zero,overflow,range";
    } else if (mask & MPFR_FLAGS_INEXACT && mask & MPFR_FLAGS_NAN && mask & MPFR_FLAGS_OVERFLOW) {
        return "inexact,invalid,overflow";
    } else if (mask & MPFR_FLAGS_INEXACT && mask & MPFR_FLAGS_NAN && mask & MPFR_FLAGS_UNDERFLOW) {
        return "inexact,invalid,underflow";
    } else if (mask & MPFR_FLAGS_INEXACT && mask & MPFR_FLAGS_NAN && mask & MPFR_FLAGS_ERANGE) {
        return "inexact,invalid,range";
    } else if (mask & MPFR_FLAGS_INEXACT && mask & MPFR_FLAGS_OVERFLOW && mask & MPFR_FLAGS_UNDERFLOW) {
        return "inexact,overflow,underflow";
    } else if (mask & MPFR_FLAGS_INEXACT && mask & MPFR_FLAGS_OVERFLOW && mask & MPFR_FLAGS_ERANGE) {
        return "inexact,overflow,range";
    } else if (mask & MPFR_FLAGS_INEXACT && mask & MPFR_FLAGS_UNDERFLOW && mask & MPFR_FLAGS_ERANGE) {
        return "inexact,underflow,range";
    } else if (mask & MPFR_FLAGS_NAN && mask & MPFR_FLAGS_OVERFLOW && mask & MPFR_FLAGS_UNDERFLOW) {
        return "invalid,overflow,underflow";
    } else if (mask & MPFR_FLAGS_NAN && mask & MPFR_FLAGS_OVERFLOW && mask & MPFR_FLAGS_ERANGE) {
        return "invalid,overflow,range";
    } else if (mask & MPFR_FLAGS_NAN && mask & MPFR_FLAGS_UNDERFLOW && mask & MPFR_FLAGS_ERANGE) {
        return "invalid,underflow,range";
    } else if (mask & MPFR_FLAGS_DIVBY0 && mask & MPFR_FLAGS_INEXACT && mask & MPFR_FLAGS_NAN && mask & MPFR_FLAGS_OVERFLOW) {
        return "divide-by-zero,inexact,invalid,overflow";
    } else if (mask & MPFR_FLAGS_DIVBY0 && mask & MPFR_FLAGS_INEXACT && mask & MPFR_FLAGS_NAN && mask & MPFR_FLAGS_UNDERFLOW) {
        return "divide-by-zero,inexact,invalid,underflow";
    } else if (mask & MPFR_FLAGS_DIVBY0 && mask & MPFR_FLAGS_INEXACT && mask & MPFR_FLAGS_NAN && mask & MPFR_FLAGS_ERANGE) {
        return "divide-by-zero,inexact,invalid,range";
    } else if (mask & MPFR_FLAGS_DIVBY0 && mask & MPFR_FLAGS_NAN && mask & MPFR_FLAGS_OVERFLOW && mask & MPFR_FLAGS_UNDERFLOW) {
        return "divide-by-zero,invalid,overflow,underflow";
    } else if (mask & MPFR_FLAGS_DIVBY0 && mask & MPFR_FLAGS_NAN && mask & MPFR_FLAGS_OVERFLOW && mask & MPFR_FLAGS_ERANGE) {
        return "divide-by-zero,invalid,overflow,range";
    } else if (mask & MPFR_FLAGS_DIVBY0 && mask & MPFR_FLAGS_INEXACT && mask & MPFR_FLAGS_NAN && mask & MPFR_FLAGS_OVERFLOW && mask & MPFR_FLAGS_UNDERFLOW) {
        return "divide-by-zero,inexact,invalid,overflow,underflow";
    } else if (mask & MPFR_FLAGS_DIVBY0 && mask & MPFR_FLAGS_INEXACT && mask & MPFR_FLAGS_NAN && mask & MPFR_FLAGS_OVERFLOW && mask & MPFR_FLAGS_ERANGE) {
        return "divide-by-zero,inexact,invalid,overflow,range";
    } else if (mask & MPFR_FLAGS_DIVBY0 && mask & MPFR_FLAGS_INEXACT && mask & MPFR_FLAGS_NAN && mask & MPFR_FLAGS_UNDERFLOW && mask & MPFR_FLAGS_ERANGE) {
        return "divide-by-zero,inexact,invalid,underflow,range";
    } else if (mask & MPFR_FLAGS_DIVBY0 && mask & MPFR_FLAGS_INEXACT && mask & MPFR_FLAGS_NAN && mask & MPFR_FLAGS_OVERFLOW && mask & MPFR_FLAGS_UNDERFLOW && mask & MPFR_FLAGS_ERANGE) {
        return "divide-by-zero,inexact,invalid,overflow,underflow,range";
    } else {
        return "none";
    }
}
#endif

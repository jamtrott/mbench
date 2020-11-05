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

#ifndef FEXCEPT_H
#define FEXCEPT_H

#ifdef HAVE_MPFR
#include <mpfr.h>
#endif

#include <fenv.h>
#include <stdbool.h>

/**
 * `fexcept_clear()` clears stored floating-point exceptions.
 */
void fexcept_clear(
    fexcept_t * fexcept);

/**
 * `fexcept_store()` stores current floating-point exceptions.
 */
void fexcept_store(
    fexcept_t * fexcept,
    int excepts);

/**
 * `fexcept_str()` converts floating-point exceptions to a string.
 */
const char * fexcept_str(
    fexcept_t fexcept);

/**
 * `fexcept_is_exception()` returns `true` if there is a
 * floating-point exception.
 */
bool fexcept_is_exception(
    fexcept_t fexcept,
    int excepts);

#ifdef HAVE_MPFR
/**
 * `mpfr_except_str()` converts MPFR floating-point exceptions to a
 * string.
 */
const char * mpfr_except_str(
    mpfr_flags_t mask);
#endif

#endif

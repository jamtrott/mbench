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
 * Rounding modes for floating-point calculations.
 */

#ifndef ROUND_H
#define ROUND_H

#ifdef HAVE_MPFR
#include <mpfr.h>
#endif

/**
 * `round_mode` is used to enumerate different rounding modes for
 * floating-point calculations.
 */
enum round_mode
{
    round_downward = 0, /* round downward */
    round_tonearest,    /* round to nearest */
    round_towardzero,   /* round toward zero */
    round_upward,       /* round upward */

    /* A final dummy entry, equal to the number of enum values. */
    num_round_modes
};

/**
 * `round_mode_str()` is a string representing a given rounding mode.
 */
const char * round_mode_str(
    enum round_mode round_mode);

/**
 * `parse_round_mode()` parses a string designating a rounding mode.
 *
 * On success, `parse_round_mode()` returns `0`. If the string does
 * not correspond to a valid rounding mode, then `parse_round_mode()`
 * returns `EINVAL`.
 */
int parse_round_mode(
    const char * s,
    enum round_mode * round_mode);

/**
 * `set_round_mode()` sets the rounding mode for subsequent
 * floating-point calculations.
 */
int set_round_mode(
    enum round_mode round_mode);

#ifdef HAVE_MPFR
/**
 * `round_mode_mpfr()` returns the corresponding MPFR rounding mode.
 */
int round_mode_mpfr(
    enum round_mode round_mode,
    mpfr_rnd_t * mpfr_round_mode);
#endif

#endif

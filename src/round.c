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

#include "round.h"

#ifdef HAVE_MPFR
#include <mpfr.h>
#endif

#include <errno.h>

#include <fenv.h>
#include <string.h>

#pragma STDC FENV_ACCESS on

/**
 * `round_mode_str()` is a string representing a given rounding mode.
 */
const char * round_mode_str(
    enum round_mode round_mode)
{
    switch (round_mode) {
    case round_downward: return "downward";
    case round_tonearest: return "tonearest";
    case round_towardzero: return "towardzero";
    case round_upward: return "upward";
    default: return "unknown";
    }
}

/**
 * `parse_round_mode()` parses a string designating a rounding mode.
 *
 * On success, `parse_round_mode()` returns `0`. If the string does
 * not correspond to a valid rounding mode, then `parse_round_mode()`
 * returns `EINVAL`.
 */
int parse_round_mode(
    const char * s,
    enum round_mode * round_mode)
{
    if (strcmp(s, "downward") == 0) {
        *round_mode = round_downward;
    } else if (strcmp(s, "tonearest") == 0) {
        *round_mode = round_tonearest;
    } else if (strcmp(s, "towardzero") == 0) {
        *round_mode = round_towardzero;
    } else if (strcmp(s, "upward") == 0) {
        *round_mode = round_upward;
    } else {
        return EINVAL;
    }
    return 0;
}

/**
 * `set_round_mode()` sets the rounding mode for subsequent
 * floating-point calculations.
 */
int set_round_mode(
    enum round_mode round_mode)
{
    int err;

    switch (round_mode) {
    case round_downward:
        err = fesetround(FE_DOWNWARD);
        break;
    case round_tonearest:
        err = fesetround(FE_TONEAREST);
        break;
    case round_towardzero:
        err = fesetround(FE_TOWARDZERO);
        break;
    case round_upward:
        err = fesetround(FE_UPWARD);
        break;
    default: return EINVAL;
    }

    if (err)
        return errno;
    return 0;
}

#ifdef HAVE_MPFR
/**
 * `round_mode_mpfr()` returns the corresponding MPFR rounding mode.
 */
int round_mode_mpfr(
    enum round_mode round_mode,
    mpfr_rnd_t * mpfr_round_mode)
{
    switch (round_mode) {
    case round_downward: *mpfr_round_mode = MPFR_RNDD; break;
    case round_tonearest: *mpfr_round_mode = MPFR_RNDN; break;
    case round_towardzero: *mpfr_round_mode = MPFR_RNDZ; break;
    case round_upward: *mpfr_round_mode = MPFR_RNDU; break;
    default: return EINVAL;
    }
    return 0;
}
#endif

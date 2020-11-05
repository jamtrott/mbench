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
 * Common mathematical operations.
 */

#ifndef MATHOP_H
#define MATHOP_H

#include "round.h"

#include <fenv.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/**
 * `mathop_input_type` is used to enumerate different types of inputs
 * for common mathematical operations.
 */
enum mathop_input_type
{
    mathop_input_f32,
    mathop_input_f64,

    /* A final dummy entry, equal to the number of enum values. */
    num_mathop_input_types
};

/**
 * `mathop_input_type_str()` is a string representing the input type
 * of a math operation.
 */
const char * mathop_input_type_str(
    enum mathop_input_type mathop_input_type);

/**
 * `parse_mathop_input_type()` parses a string designating the input
 * type of a math operation.
 *
 * On success, `parse_mathop_input_type()` returns `0`. If the string
 * does not correspond to a valid operation, then
 * `parse_mathop_input_type()` returns `EINVAL`.
 */
int parse_mathop_input_type(
    const char * s,
    enum mathop_input_type * input);

/**
 * `mathop` is used to enumerate different common mathematical
 * operations.
 */
enum mathop
{
    /* Trigonometric functions */
    mathop_cos = 0,
    mathop_cosf,
    mathop_sin,
    mathop_sinf,
    mathop_tan,
    mathop_tanf,
    mathop_acos,
    mathop_acosf,
    mathop_asin,
    mathop_asinf,
    mathop_atan,
    mathop_atanf,

    /* Hyperbolic functions */
    mathop_cosh,
    mathop_coshf,
    mathop_sinh,
    mathop_sinhf,
    mathop_tanh,
    mathop_tanhf,
    mathop_acosh,
    mathop_acoshf,
    mathop_asinh,
    mathop_asinhf,
    mathop_atanh,
    mathop_atanhf,

    /* Exponential and logarithmic functions */
    mathop_exp,
    mathop_expf,
    mathop_log,
    mathop_logf,
    mathop_log10,
    mathop_log10f,
    mathop_exp2,
    mathop_exp2f,
    mathop_expm1,
    mathop_expm1f,
    mathop_log1p,
    mathop_log1pf,
    mathop_log2,
    mathop_log2f,

    /* Power functions */
    mathop_sqrt,
    mathop_sqrtf,
    mathop_cbrt,
    mathop_cbrtf,

    /* Error and gamma functions */
    mathop_erf,
    mathop_erff,
    mathop_erfc,
    mathop_erfcf,
    mathop_tgamma,
    mathop_tgammaf,
    mathop_lgamma,
    mathop_lgammaf,

    /* A final dummy entry, equal to the number of enum values. */
    num_mathops
};

/**
 * `mathop_str()` is a string representing a given math operation.
 */
const char * mathop_str(
    enum mathop mathop);

/**
 * `parse_mathop()` parses a string designating a math operation.
 *
 * On success, `parse_mathop()` returns `0`. If the string does not
 * correspond to a valid operation, then `parse_mathop()` returns
 * `EINVAL`.
 */
int parse_mathop(
    const char * s,
    enum mathop * mathop);

/**
 * `mathop_input()` returns the input type associated with a math
 * operation.
 */
int mathop_input(
    enum mathop mathop,
    enum mathop_input_type * input_type);

/**
 * `mathop_input` is a data structure used to represent inputs for
 * common mathematical operations.
 */
struct mathop_input
{
    enum mathop_input_type type;
    int64_t size;
    float * f32;
    double * f64;
};

/**
 * `mathop_input_init()` reads input for a math operation from a file
 * stream.
 */
int mathop_input_init(
    struct mathop_input * input,
    enum mathop mathop,
    FILE * f,
    int alignment);

/**
 * `mathop_input_free()` frees resources associated with an input for
 * a math operation.
 */
int mathop_input_free(
    struct mathop_input * input);

/**
 * `mathop_input_print()` prints the input of a math operation.
 */
int mathop_input_print(
    const struct mathop_input * input,
    FILE * f,
    int width,
    int prec,
    const char * delimiter);

/**
 * `mathop_result_type` is used to enumerate different types of
 * results from common mathematical operations.
 */
enum mathop_result_type
{
    mathop_result_f32,
    mathop_result_f64,

    /* A final dummy entry, equal to the number of enum values. */
    num_mathop_result_types
};

/**
 * `mathop_result_type_str()` is a string representing the result type
 * of a math operation.
 */
const char * mathop_result_type_str(
    enum mathop_result_type mathop_result_type);

/**
 * `parse_mathop_result_type()` parses a string designating the result
 * type of a math operation.
 *
 * On success, `parse_mathop_result_type()` returns `0`. If the string
 * does not correspond to a valid operation, then
 * `parse_mathop_result_type()` returns `EINVAL`.
 */
int parse_mathop_result_type(
    const char * s,
    enum mathop_result_type * result);

/**
 * `mathop_result` is a data structure used to represent results from
 * common mathematical operations.
 */
struct mathop_result
{
    enum mathop_result_type type;
    fexcept_t fexcept;
    int64_t size;
    float * f32;
    double * f64;
};

/**
 * `mathop_result_init()` allocates storage for the result for a math
 * operation.
 */
int mathop_result_init(
    struct mathop_result * result,
    enum mathop mathop,
    int64_t size,
    int alignment);

/**
 * `mathop_result_free()` frees resources associated with the result
 * of a math operation.
 */
int mathop_result_free(
    struct mathop_result * result);

/**
 * `mathop_result_has_exception()` returns `true` if there is a
 * floating-point exception associated with the given result.
 */
bool mathop_result_has_exception(
    const struct mathop_result * result);

/**
 * `mathop_result_print()` prints the result of a math operation.
 */
int mathop_result_print(
    const struct mathop_result * result,
    FILE * f,
    int width,
    int prec,
    const char * delimiter);

/**
 * `benchmark_mathop()` benchmarks a math operation.
 */
int benchmark_mathop(
    enum mathop mathop,
    struct mathop_input * input,
    struct mathop_result * result,
    int64_t * num_ops);

/**
 * `mathop_error()` computes the error associated with the result
 * obtained after benchmarking a math operation.
 */
int mathop_error(
    enum mathop mathop,
    const struct mathop_input * input,
    const struct mathop_result * result,
    enum round_mode round_mode,
    int precision,
    double * abs_error,
    double * rel_error,
    const char ** exceptions);

#endif

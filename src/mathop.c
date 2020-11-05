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

#include "mathop.h"
#include "fexcept.h"
#include "parse.h"
#include "round.h"

#ifdef HAVE_MPFR
#include <mpfr.h>
#endif

#include <errno.h>
#include <unistd.h>

#include <ctype.h>
#include <fenv.h>
#include <math.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * `mathop_input_type_str()` is a string representing the input type
 * of a math operation.
 */
const char * mathop_input_type_str(
    enum mathop_input_type mathop_input_type)
{
    switch (mathop_input_type) {
    case mathop_input_f32: return "f32";
    case mathop_input_f64: return "f64";
    default: return "unknown";
    }
}

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
    enum mathop_input_type * type)
{
    if (strcmp("f32", s) == 0) {
        *type = mathop_input_f32;
    } else if (strcmp("f64", s) == 0) {
        *type = mathop_input_f64;
    } else {
        return EINVAL;
    }
    return 0;
}

/**
 * `mathop_str()` is a string representing a given math operation.
 */
const char * mathop_str(
    enum mathop mathop)
{
    switch (mathop) {
    case mathop_cos: return "cos";
    case mathop_cosf: return "cosf";
    case mathop_sin: return "sin";
    case mathop_sinf: return "sinf";
    case mathop_tan: return "tan";
    case mathop_tanf: return "tanf";
    case mathop_acos: return "acos";
    case mathop_acosf: return "acosf";
    case mathop_asin: return "asin";
    case mathop_asinf: return "asinf";
    case mathop_atan: return "atan";
    case mathop_atanf: return "atanf";
    case mathop_cosh: return "cosh";
    case mathop_coshf: return "coshf";
    case mathop_sinh: return "sinh";
    case mathop_sinhf: return "sinhf";
    case mathop_tanh: return "tanh";
    case mathop_tanhf: return "tanhf";
    case mathop_acosh: return "acosh";
    case mathop_acoshf: return "acoshf";
    case mathop_asinh: return "asinh";
    case mathop_asinhf: return "asinhf";
    case mathop_atanh: return "atanh";
    case mathop_atanhf: return "atanhf";
    case mathop_exp: return "exp";
    case mathop_expf: return "expf";
    case mathop_log: return "log";
    case mathop_logf: return "logf";
    case mathop_log10: return "log10";
    case mathop_log10f: return "log10f";
    case mathop_exp2: return "exp2";
    case mathop_exp2f: return "exp2f";
    case mathop_expm1: return "expm1";
    case mathop_expm1f: return "expm1f";
    case mathop_log1p: return "log1p";
    case mathop_log1pf: return "log1pf";
    case mathop_log2: return "log2";
    case mathop_log2f: return "log2f";
    case mathop_sqrt: return "sqrt";
    case mathop_sqrtf: return "sqrtf";
    case mathop_cbrt: return "cbrt";
    case mathop_cbrtf: return "cbrtf";
    case mathop_erf: return "erf";
    case mathop_erff: return "erff";
    case mathop_erfc: return "erfc";
    case mathop_erfcf: return "erfcf";
    case mathop_tgamma: return "tgamma";
    case mathop_tgammaf: return "tgammaf";
    case mathop_lgamma: return "lgamma";
    case mathop_lgammaf: return "lgammaf";
    default: return "unknown";
    }
}

/**
 * `parse_mathop()` parses a string designating a math operation.
 *
 * On success, `parse_mathop()` returns `0`. If the string does not
 * correspond to a valid operation, then `parse_mathop()` returns
 * `EINVAL`.
 */
int parse_mathop(
    const char * s,
    enum mathop * mathop)
{
    if (strcmp(s, "cos") == 0) {
        *mathop = mathop_cos;
    } else if (strcmp(s, "cosf") == 0) {
        *mathop = mathop_cosf;
    } else if (strcmp(s, "sin") == 0) {
        *mathop = mathop_sin;
    } else if (strcmp(s, "sinf") == 0) {
        *mathop = mathop_sinf;
    } else if (strcmp(s, "tan") == 0) {
        *mathop = mathop_tan;
    } else if (strcmp(s, "tanf") == 0) {
        *mathop = mathop_tanf;
    } else if (strcmp(s, "acos") == 0) {
        *mathop = mathop_acos;
    } else if (strcmp(s, "acosf") == 0) {
        *mathop = mathop_acosf;
    } else if (strcmp(s, "asin") == 0) {
        *mathop = mathop_asin;
    } else if (strcmp(s, "asinf") == 0) {
        *mathop = mathop_asinf;
    } else if (strcmp(s, "atan") == 0) {
        *mathop = mathop_atan;
    } else if (strcmp(s, "atanf") == 0) {
        *mathop = mathop_atanf;
    } else if (strcmp(s, "cosh") == 0) {
        *mathop = mathop_cosh;
    } else if (strcmp(s, "coshf") == 0) {
        *mathop = mathop_coshf;
    } else if (strcmp(s, "sinh") == 0) {
        *mathop = mathop_sinh;
    } else if (strcmp(s, "sinhf") == 0) {
        *mathop = mathop_sinhf;
    } else if (strcmp(s, "tanh") == 0) {
        *mathop = mathop_tanh;
    } else if (strcmp(s, "tanhf") == 0) {
        *mathop = mathop_tanhf;
    } else if (strcmp(s, "acosh") == 0) {
        *mathop = mathop_acosh;
    } else if (strcmp(s, "acoshf") == 0) {
        *mathop = mathop_acoshf;
    } else if (strcmp(s, "asinh") == 0) {
        *mathop = mathop_asinh;
    } else if (strcmp(s, "asinhf") == 0) {
        *mathop = mathop_asinhf;
    } else if (strcmp(s, "atanh") == 0) {
        *mathop = mathop_atanh;
    } else if (strcmp(s, "atanhf") == 0) {
        *mathop = mathop_atanhf;
    } else if (strcmp(s, "exp") == 0) {
        *mathop = mathop_exp;
    } else if (strcmp(s, "expf") == 0) {
        *mathop = mathop_expf;
    } else if (strcmp(s, "log") == 0) {
        *mathop = mathop_log;
    } else if (strcmp(s, "logf") == 0) {
        *mathop = mathop_logf;
    } else if (strcmp(s, "log10") == 0) {
        *mathop = mathop_log10;
    } else if (strcmp(s, "log10f") == 0) {
        *mathop = mathop_log10f;
    } else if (strcmp(s, "exp2") == 0) {
        *mathop = mathop_exp2;
    } else if (strcmp(s, "exp2f") == 0) {
        *mathop = mathop_exp2f;
    } else if (strcmp(s, "expm1") == 0) {
        *mathop = mathop_expm1;
    } else if (strcmp(s, "expm1f") == 0) {
        *mathop = mathop_expm1f;
    } else if (strcmp(s, "log1p") == 0) {
        *mathop = mathop_log1p;
    } else if (strcmp(s, "log1pf") == 0) {
        *mathop = mathop_log1pf;
    } else if (strcmp(s, "log2") == 0) {
        *mathop = mathop_log2;
    } else if (strcmp(s, "log2f") == 0) {
        *mathop = mathop_log2f;
    } else if (strcmp(s, "sqrt") == 0) {
        *mathop = mathop_sqrt;
    } else if (strcmp(s, "sqrtf") == 0) {
        *mathop = mathop_sqrtf;
    } else if (strcmp(s, "cbrt") == 0) {
        *mathop = mathop_cbrt;
    } else if (strcmp(s, "cbrtf") == 0) {
        *mathop = mathop_cbrtf;
    } else if (strcmp(s, "erf") == 0) {
        *mathop = mathop_erf;
    } else if (strcmp(s, "erff") == 0) {
        *mathop = mathop_erff;
    } else if (strcmp(s, "erfc") == 0) {
        *mathop = mathop_erfc;
    } else if (strcmp(s, "erfcf") == 0) {
        *mathop = mathop_erfcf;
    } else if (strcmp(s, "tgamma") == 0) {
        *mathop = mathop_tgamma;
    } else if (strcmp(s, "tgammaf") == 0) {
        *mathop = mathop_tgammaf;
    } else if (strcmp(s, "lgamma") == 0) {
        *mathop = mathop_lgamma;
    } else if (strcmp(s, "lgammaf") == 0) {
        *mathop = mathop_lgammaf;
    } else {
        return EINVAL;
    }
    return 0;
}

/**
 * `mathop_input()` returns the input type associated with a math
 * operation.
 */
int mathop_input(
    enum mathop mathop,
    enum mathop_input_type * input_type)
{
    switch (mathop) {
    case mathop_cos:
    case mathop_sin:
    case mathop_tan:
    case mathop_acos:
    case mathop_asin:
    case mathop_atan:
    case mathop_cosh:
    case mathop_sinh:
    case mathop_tanh:
    case mathop_acosh:
    case mathop_asinh:
    case mathop_atanh:
    case mathop_exp:
    case mathop_log:
    case mathop_log10:
    case mathop_exp2:
    case mathop_expm1:
    case mathop_log1p:
    case mathop_log2:
    case mathop_sqrt:
    case mathop_cbrt:
    case mathop_erf:
    case mathop_erfc:
    case mathop_tgamma:
    case mathop_lgamma:
        *input_type = mathop_input_f64;
        break;

    case mathop_cosf:
    case mathop_sinf:
    case mathop_tanf:
    case mathop_acosf:
    case mathop_asinf:
    case mathop_atanf:
    case mathop_coshf:
    case mathop_sinhf:
    case mathop_tanhf:
    case mathop_acoshf:
    case mathop_asinhf:
    case mathop_atanhf:
    case mathop_expf:
    case mathop_logf:
    case mathop_log10f:
    case mathop_exp2f:
    case mathop_expm1f:
    case mathop_log1pf:
    case mathop_log2f:
    case mathop_sqrtf:
    case mathop_cbrtf:
    case mathop_erff:
    case mathop_erfcf:
    case mathop_tgammaf:
    case mathop_lgammaf:
        *input_type = mathop_input_f32;
        break;

    default:
        return EINVAL;
    }

    return 0;
}

/**
 * `read_floats()` reads a sequence of values from the given stream
 * and converts them to single-precision floating-point numbers.
 */
static int read_floats(
    FILE * f,
    int alignment,
    int64_t * out_num_values,
    float ** out_values)
{
    int err;
    int64_t max_values = 32768;
    int64_t num_values = 0;

    /* Allocate storage for values. */
    float * values = (float *) aligned_alloc(
        alignment, max_values * sizeof(float));
    if (!values)
        return errno;

#pragma omp parallel for
    for (int64_t i = 0; i < max_values; i++)
        values[i] = 0.0;

    /* Allocate storage for reading lines. */
    long int line_max = sysconf(_SC_LINE_MAX);
    char * linebuf = malloc(line_max+1);
    if (!linebuf) {
        free(values);
        return errno;
    }

    /* Read float values. */
    while (true) {
        int c;
        size_t len = 0;

        /* Skip whitespace. */
        while ((c = fgetc(f)) != EOF && isspace(c));
        if (c == EOF && ferror(f)) {
            free(linebuf);
            free(values);
            return errno ? errno : -1;
        } else if (c == EOF) {
            break;
        }
        linebuf[len] = c;
        len++;

        /* Read until next whitespace character. */
        while (len < line_max && (c = fgetc(f)) != EOF && !isspace(c)) {
            linebuf[len] = c;
            len++;
        }
        if (c == EOF && ferror(f)) {
            free(linebuf);
            free(values);
            return errno ? errno : -1;
        } else if (len == line_max) {
            free(linebuf);
            free(values);
            return ENOMEM;
        }
        linebuf[len] = '\0';

        /* If needed, allocate more storage. */
        if (num_values == max_values) {
            max_values *= 2;
            float * new_values = (float *) aligned_alloc(
                alignment, max_values * sizeof(float));
            if (!new_values) {
                free(linebuf);
                free(values);
                return errno;
            }

#pragma omp parallel for
            for (int64_t i = 0; i < max_values; i++)
                new_values[i] = 0.0;

#pragma omp parallel for
            for (int64_t i = 0; i < num_values; i++)
                new_values[i] = values[i];
            free(values);
            values = new_values;
        }

        /* Parse the string as a float. */
        float x;
        err = parse_float(linebuf, NULL, &x, NULL);
        if (err) {
            free(linebuf);
            free(values);
            return err;
        }

        values[num_values] = x;
        num_values++;
    }

    free(linebuf);
    *out_num_values = num_values;
    *out_values = values;
    return 0;
}

/**
 * `read_doubles()` reads a sequence of values from the given stream
 * and converts them to double-precision floating-point numbers.
 */
static int read_doubles(
    FILE * f,
    int alignment,
    int64_t * out_num_values,
    double ** out_values)
{
    int err;
    int64_t max_values = 32768;
    int64_t num_values = 0;

    /* Allocate storage for values. */
    double * values = (double *) aligned_alloc(
        alignment, (((max_values * sizeof(double)) + alignment-1) /
                    alignment) * alignment);
    if (!values)
        return errno;

#pragma omp parallel for
    for (int64_t i = 0; i < max_values; i++)
        values[i] = 0.0;

    /* Allocate storage for reading lines. */
    long int line_max = sysconf(_SC_LINE_MAX);
    char * linebuf = malloc(line_max+1);
    if (!linebuf) {
        free(values);
        return errno;
    }

    /* Read float values. */
    while (true) {
        int c;
        size_t len = 0;

        /* Skip whitespace. */
        while ((c = fgetc(f)) != EOF && isspace(c));
        if (c == EOF && ferror(f)) {
            free(linebuf);
            free(values);
            return errno ? errno : -1;
        } else if (c == EOF) {
            break;
        }
        linebuf[len] = c;
        len++;

        /* Read until next whitespace character. */
        while (len < line_max && (c = fgetc(f)) != EOF && !isspace(c)) {
            linebuf[len] = c;
            len++;
        }
        if (c == EOF && ferror(f)) {
            free(linebuf);
            free(values);
            return errno ? errno : -1;
        } else if (len == line_max) {
            free(linebuf);
            free(values);
            return ENOMEM;
        }
        linebuf[len] = '\0';

        /* If needed, allocate more storage. */
        if (num_values == max_values) {
            max_values *= 2;
            double * new_values = (double *) aligned_alloc(
                alignment, (((max_values * sizeof(double)) + alignment-1) /
                            alignment) * alignment);
            if (!new_values) {
                free(linebuf);
                free(values);
                return errno;
            }

#pragma omp parallel for
            for (int64_t i = 0; i < max_values; i++)
                new_values[i] = 0.0;

#pragma omp parallel for
            for (int64_t i = 0; i < num_values; i++)
                new_values[i] = values[i];
            free(values);
            values = new_values;
        }

        /* Parse the string as a float. */
        double x;
        err = parse_double(linebuf, NULL, &x, NULL);
        if (err) {
            free(linebuf);
            free(values);
            return err;
        }

        values[num_values] = x;
        num_values++;
    }

    free(linebuf);
    *out_num_values = num_values;
    *out_values = values;
    return 0;
}

/**
 * `mathop_input_init()` sets up the input for a math operation.
 */
int mathop_input_init(
    struct mathop_input * input,
    enum mathop mathop,
    FILE * f,
    int alignment)
{
    int err;

    /* Determine data type associated with the math operation. */
    enum mathop_input_type input_type;
    err = mathop_input(mathop, &input_type);
    if (err)
        return err;

    /* Read float values from the stream. */
    input->type = input_type;
    switch (input_type) {
    case mathop_input_f32:
        err = read_floats(f, alignment, &input->size, &input->f32);
        if (err)
            return err;
        break;

    case mathop_input_f64:
        err = read_doubles(f, alignment, &input->size, &input->f64);
        if (err)
            return err;
        break;

    default:
        return EINVAL;
    }
    return 0;
}

/**
 * `mathop_input_free()` frees resources associated with an input for
 * a math operation.
 */
int mathop_input_free(
    struct mathop_input * input)
{
    switch (input->type) {
    case mathop_input_f32:
        free(input->f32);
        break;
    case mathop_input_f64:
        free(input->f64);
        break;
    default:
        return EINVAL;
    }
    return 0;
}

/**
 * `mathop_input_print()` prints the input of a math operation.
 */
int mathop_input_print(
    const struct mathop_input * input,
    FILE * f,
    int width,
    int prec,
    const char * delimiter)
{
    switch (input->type) {
    case mathop_input_f32:
        if (input->size > 0)
            fprintf(f, "%*.*f", width, prec, input->f32[0]);
        for (int64_t i = 1; i < input->size; i++)
            fprintf(f, "%s%*.*f", delimiter, width, prec, input->f32[i]);
        break;

    case mathop_input_f64:
        if (input->size > 0)
            fprintf(f, "%*.*f", width, prec, input->f64[0]);
        for (int64_t i = 1; i < input->size; i++)
            fprintf(f, "%s%*.*f", delimiter, width, prec, input->f64[i]);
        break;

    default:
        return EINVAL;
    }
    return 0;
}

/**
 * `mathop_result_type_str()` is a string representing the result type
 * of a math operation.
 */
const char * mathop_result_type_str(
    enum mathop_result_type mathop_result_type)
{
    switch (mathop_result_type) {
    case mathop_result_f32: return "f32";
    case mathop_result_f64: return "f64";
    default: return "unknown";
    }
}

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
    enum mathop_result_type * type)
{
    if (strcmp("f32", s) == 0) {
        *type = mathop_result_f32;
    } else if (strcmp("f64", s) == 0) {
        *type = mathop_result_f64;
    } else {
        return EINVAL;
    }
    return 0;
}

/**
 * `mathop_result_init_float_array()` allocates an array of floats and
 * initialises them to zero.
 */
int mathop_result_init_float_array(
    struct mathop_result * result,
    int64_t size,
    int alignment)
{
    result->type = mathop_result_f32;
    result->size = size;
    int64_t aligned_size =
        (((size * sizeof(float)) + alignment-1) /
         alignment) * alignment;
    result->f32 = (float *) aligned_alloc(
        alignment, aligned_size);
    if (!result->f32)
        return errno;

#pragma omp parallel for
    for (int64_t i = 0; i < result->size; i++)
        result->f32[i] = 0.0f;
    return 0;
}

/**
 * `mathop_result_init_double_array()` allocates an array of doubles
 * and initialises them to zero.
 */
int mathop_result_init_double_array(
    struct mathop_result * result,
    int64_t size,
    int alignment)
{
    result->type = mathop_result_f64;
    result->size = size;
    int64_t aligned_size =
        (((size * sizeof(double)) + alignment-1) /
         alignment) * alignment;
    result->f64 = (double *) aligned_alloc(
        alignment, aligned_size);
    if (!result->f64)
        return errno;

#pragma omp parallel for
    for (int64_t i = 0; i < result->size; i++)
        result->f64[i] = 0.0;
    return 0;
}

/**
 * `mathop_result_init()` allocates storage for the result for a math
 * operation.
 */
int mathop_result_init(
    struct mathop_result * result,
    enum mathop mathop,
    int64_t size,
    int alignment)
{
    int err;
    fexcept_clear(&result->fexcept);

    switch (mathop) {
    case mathop_cos:
    case mathop_sin:
    case mathop_tan:
    case mathop_acos:
    case mathop_asin:
    case mathop_atan:
    case mathop_cosh:
    case mathop_sinh:
    case mathop_tanh:
    case mathop_acosh:
    case mathop_asinh:
    case mathop_atanh:
    case mathop_exp:
    case mathop_log:
    case mathop_log10:
    case mathop_exp2:
    case mathop_expm1:
    case mathop_log1p:
    case mathop_log2:
    case mathop_sqrt:
    case mathop_cbrt:
    case mathop_erf:
    case mathop_erfc:
    case mathop_tgamma:
    case mathop_lgamma:
        err = mathop_result_init_double_array(
            result, size, alignment);
        if (err)
            return err;
        break;

    case mathop_cosf:
    case mathop_sinf:
    case mathop_tanf:
    case mathop_acosf:
    case mathop_asinf:
    case mathop_atanf:
    case mathop_coshf:
    case mathop_sinhf:
    case mathop_tanhf:
    case mathop_acoshf:
    case mathop_asinhf:
    case mathop_atanhf:
    case mathop_expf:
    case mathop_logf:
    case mathop_log10f:
    case mathop_exp2f:
    case mathop_expm1f:
    case mathop_log1pf:
    case mathop_log2f:
    case mathop_sqrtf:
    case mathop_cbrtf:
    case mathop_erff:
    case mathop_erfcf:
    case mathop_tgammaf:
    case mathop_lgammaf:
        err = mathop_result_init_float_array(
            result, size, alignment);
        if (err)
            return err;
        break;

    default:
        return EINVAL;
    }

    return 0;
}

/**
 * `mathop_result_free()` frees resources associated with the result
 * of a math operation.
 */
int mathop_result_free(
    struct mathop_result * result)
{
    switch (result->type) {
    case mathop_result_f32:
        free(result->f32);
        break;
    case mathop_result_f64:
        free(result->f64);
        break;
    default:
        return EINVAL;
    }
    return 0;
}

/**
 * `mathop_result_has_exception()` returns `true` if there is a
 * floating-point exception associated with the given result.
 */
bool mathop_result_has_exception(
    const struct mathop_result * result)
{
    return fexcept_is_exception(result->fexcept, FE_ALL_EXCEPT);
}

/**
 * `mathop_result_print()` prints the result of a math operation.
 */
int mathop_result_print(
    const struct mathop_result * result,
    FILE * f,
    int width,
    int prec,
    const char * delimiter)
{
    switch (result->type) {
    case mathop_result_f32:
        if (result->size > 0)
            fprintf(f, "%*.*f", width, prec, result->f32[0]);
        for (int64_t i = 1; i < result->size; i++)
            fprintf(f, "%s%*.*f", delimiter, width, prec, result->f32[i]);
        break;

    case mathop_result_f64:
        if (result->size > 0)
            fprintf(f, "%*.*f", width, prec, result->f64[0]);
        for (int64_t i = 1; i < result->size; i++)
            fprintf(f, "%s%*.*f", delimiter, width, prec, result->f64[i]);
        break;

    default:
        return EINVAL;
    }

    return 0;
}

/*
 * Functions for benchmarking common math operations.
 */

#define benchmark_mathop_fn_float(OPNAME)                               \
    static int benchmark_mathop_ ## OPNAME(                             \
        int64_t N,                                                      \
        const float * restrict x,                                       \
        struct mathop_result * restrict result,                         \
        int64_t * restrict num_ops)                                     \
    {                                                                   \
        if (N != result->size || result->type != mathop_result_f32)     \
            return EINVAL;                                              \
        _Pragma("omp for simd")                                         \
        for (int64_t i = 0; i < N; i++)                                 \
            result->f32[i] = OPNAME(x[i]);                              \
        (*num_ops) += N;                                                \
        return 0;                                                       \
    }                                                                   \


#define benchmark_mathop_fn_double(OPNAME)                              \
    static int benchmark_mathop_ ## OPNAME(                             \
        int64_t N,                                                      \
        const double * restrict x,                                      \
        struct mathop_result * restrict result,                         \
        int64_t * restrict num_ops)                                     \
    {                                                                   \
        if (N != result->size || result->type != mathop_result_f64)     \
            return EINVAL;                                              \
        _Pragma("omp for simd")                                         \
        for (int64_t i = 0; i < N; i++)                                 \
            result->f64[i] = OPNAME(x[i]);                              \
        (*num_ops) += N;                                                \
        return 0;                                                       \
    }                                                                   \


benchmark_mathop_fn_double(cos)
benchmark_mathop_fn_float(cosf)
benchmark_mathop_fn_double(sin)
benchmark_mathop_fn_float(sinf)
benchmark_mathop_fn_double(tan)
benchmark_mathop_fn_float(tanf)
benchmark_mathop_fn_double(acos)
benchmark_mathop_fn_float(acosf)
benchmark_mathop_fn_double(asin)
benchmark_mathop_fn_float(asinf)
benchmark_mathop_fn_double(atan)
benchmark_mathop_fn_float(atanf)
benchmark_mathop_fn_double(cosh)
benchmark_mathop_fn_float(coshf)
benchmark_mathop_fn_double(sinh)
benchmark_mathop_fn_float(sinhf)
benchmark_mathop_fn_double(tanh)
benchmark_mathop_fn_float(tanhf)
benchmark_mathop_fn_double(acosh)
benchmark_mathop_fn_float(acoshf)
benchmark_mathop_fn_double(asinh)
benchmark_mathop_fn_float(asinhf)
benchmark_mathop_fn_double(atanh)
benchmark_mathop_fn_float(atanhf)
benchmark_mathop_fn_double(exp)
benchmark_mathop_fn_float(expf)
benchmark_mathop_fn_double(log)
benchmark_mathop_fn_float(logf)
benchmark_mathop_fn_double(log10)
benchmark_mathop_fn_float(log10f)
benchmark_mathop_fn_double(exp2)
benchmark_mathop_fn_float(exp2f)
benchmark_mathop_fn_double(expm1)
benchmark_mathop_fn_float(expm1f)
benchmark_mathop_fn_double(log1p)
benchmark_mathop_fn_float(log1pf)
benchmark_mathop_fn_double(log2)
benchmark_mathop_fn_float(log2f)
benchmark_mathop_fn_double(sqrt)
benchmark_mathop_fn_float(sqrtf)
benchmark_mathop_fn_double(cbrt)
benchmark_mathop_fn_float(cbrtf)
benchmark_mathop_fn_double(erf)
benchmark_mathop_fn_float(erff)
benchmark_mathop_fn_double(erfc)
benchmark_mathop_fn_float(erfcf)
benchmark_mathop_fn_double(tgamma)
benchmark_mathop_fn_float(tgammaf)
benchmark_mathop_fn_double(lgamma)
benchmark_mathop_fn_float(lgammaf)

/**
 * `benchmark_mathop()` benchmarks a math operation.
 */
int benchmark_mathop(
    enum mathop mathop,
    struct mathop_input * input,
    struct mathop_result * result,
    int64_t * num_ops)
{
    int err;

    errno = 0;
    if (math_errhandling & MATH_ERREXCEPT)
        feclearexcept(FE_ALL_EXCEPT);

    switch (mathop) {
    case mathop_cos:
        err = benchmark_mathop_cos(input->size, input->f64, result, num_ops);
        break;
    case mathop_cosf:
        err = benchmark_mathop_cosf(input->size, input->f32, result, num_ops);
        break;
    case mathop_sin:
        err = benchmark_mathop_sin(input->size, input->f64, result, num_ops);
        break;
    case mathop_sinf:
        err = benchmark_mathop_sinf(input->size, input->f32, result, num_ops);
        break;
    case mathop_tan:
        err = benchmark_mathop_tan(input->size, input->f64, result, num_ops);
        break;
    case mathop_tanf:
        err = benchmark_mathop_tanf(input->size, input->f32, result, num_ops);
        break;
    case mathop_acos:
        err = benchmark_mathop_acos(input->size, input->f64, result, num_ops);
        break;
    case mathop_acosf:
        err = benchmark_mathop_acosf(input->size, input->f32, result, num_ops);
        break;
    case mathop_asin:
        err = benchmark_mathop_asin(input->size, input->f64, result, num_ops);
        break;
    case mathop_asinf:
        err = benchmark_mathop_asinf(input->size, input->f32, result, num_ops);
        break;
    case mathop_atan:
        err = benchmark_mathop_atan(input->size, input->f64, result, num_ops);
        break;
    case mathop_atanf:
        err = benchmark_mathop_atanf(input->size, input->f32, result, num_ops);
        break;
    case mathop_cosh:
        err = benchmark_mathop_cosh(input->size, input->f64, result, num_ops);
        break;
    case mathop_coshf:
        err = benchmark_mathop_coshf(input->size, input->f32, result, num_ops);
        break;
    case mathop_sinh:
        err = benchmark_mathop_sinh(input->size, input->f64, result, num_ops);
        break;
    case mathop_sinhf:
        err = benchmark_mathop_sinhf(input->size, input->f32, result, num_ops);
        break;
    case mathop_tanh:
        err = benchmark_mathop_tanh(input->size, input->f64, result, num_ops);
        break;
    case mathop_tanhf:
        err = benchmark_mathop_tanhf(input->size, input->f32, result, num_ops);
        break;
    case mathop_acosh:
        err = benchmark_mathop_acosh(input->size, input->f64, result, num_ops);
        break;
    case mathop_acoshf:
        err = benchmark_mathop_acoshf(input->size, input->f32, result, num_ops);
        break;
    case mathop_asinh:
        err = benchmark_mathop_asinh(input->size, input->f64, result, num_ops);
        break;
    case mathop_asinhf:
        err = benchmark_mathop_asinhf(input->size, input->f32, result, num_ops);
        break;
    case mathop_atanh:
        err = benchmark_mathop_atanh(input->size, input->f64, result, num_ops);
        break;
    case mathop_atanhf:
        err = benchmark_mathop_atanhf(input->size, input->f32, result, num_ops);
        break;
    case mathop_exp:
        err = benchmark_mathop_exp(input->size, input->f64, result, num_ops);
        break;
    case mathop_expf:
        err = benchmark_mathop_expf(input->size, input->f32, result, num_ops);
        break;
    case mathop_log:
        err = benchmark_mathop_log(input->size, input->f64, result, num_ops);
        break;
    case mathop_logf:
        err = benchmark_mathop_logf(input->size, input->f32, result, num_ops);
        break;
    case mathop_log10:
        err = benchmark_mathop_log10(input->size, input->f64, result, num_ops);
        break;
    case mathop_log10f:
        err = benchmark_mathop_log10f(input->size, input->f32, result, num_ops);
        break;
    case mathop_exp2:
        err = benchmark_mathop_exp2(input->size, input->f64, result, num_ops);
        break;
    case mathop_exp2f:
        err = benchmark_mathop_exp2f(input->size, input->f32, result, num_ops);
        break;
    case mathop_expm1:
        err = benchmark_mathop_expm1(input->size, input->f64, result, num_ops);
        break;
    case mathop_expm1f:
        err = benchmark_mathop_expm1f(input->size, input->f32, result, num_ops);
        break;
    case mathop_log1p:
        err = benchmark_mathop_log1p(input->size, input->f64, result, num_ops);
        break;
    case mathop_log1pf:
        err = benchmark_mathop_log1pf(input->size, input->f32, result, num_ops);
        break;
    case mathop_log2:
        err = benchmark_mathop_log2(input->size, input->f64, result, num_ops);
        break;
    case mathop_log2f:
        err = benchmark_mathop_log2f(input->size, input->f32, result, num_ops);
        break;
    case mathop_sqrt:
        err = benchmark_mathop_sqrt(input->size, input->f64, result, num_ops);
        break;
    case mathop_sqrtf:
        err = benchmark_mathop_sqrtf(input->size, input->f32, result, num_ops);
        break;
    case mathop_cbrt:
        err = benchmark_mathop_cbrt(input->size, input->f64, result, num_ops);
        break;
    case mathop_cbrtf:
        err = benchmark_mathop_cbrtf(input->size, input->f32, result, num_ops);
        break;
    case mathop_erf:
        err = benchmark_mathop_erf(input->size, input->f64, result, num_ops);
        break;
    case mathop_erff:
        err = benchmark_mathop_erff(input->size, input->f32, result, num_ops);
        break;
    case mathop_erfc:
        err = benchmark_mathop_erfc(input->size, input->f64, result, num_ops);
        break;
    case mathop_erfcf:
        err = benchmark_mathop_erfcf(input->size, input->f32, result, num_ops);
        break;
    case mathop_tgamma:
        err = benchmark_mathop_tgamma(input->size, input->f64, result, num_ops);
        break;
    case mathop_tgammaf:
        err = benchmark_mathop_tgammaf(input->size, input->f32, result, num_ops);
        break;
    case mathop_lgamma:
        err = benchmark_mathop_lgamma(input->size, input->f64, result, num_ops);
        break;
    case mathop_lgammaf:
        err = benchmark_mathop_lgammaf(input->size, input->f32, result, num_ops);
        break;
    default:
        return EINVAL;
    }
    if (err)
        return err;

    fexcept_store(&result->fexcept, FE_ALL_EXCEPT & ~FE_INEXACT);
    if (math_errhandling & MATH_ERRNO) {
        if (errno)
            return errno;
    }

    return 0;
}

#ifdef HAVE_MPFR

/*
 * Functions for computing error for common math operations by
 * comparing to a reference computed by the GNU MPFR library.
 */

#define mathop_error_fn_float(OPNAME)                                   \
    static int mathop_error_float_ ## OPNAME(                           \
        int64_t N,                                                      \
        const float * _x,                                               \
        const float * _y,                                               \
        enum round_mode round_mode,                                     \
        int precision,                                                  \
        double * out_abs_error,                                         \
        double * out_rel_error,                                         \
        const char ** exceptions)                                       \
    {                                                                   \
        mpfr_rnd_t mpfr_round_mode;                                     \
        int err = round_mode_mpfr(round_mode, &mpfr_round_mode);        \
        if (err) return err;                                            \
        mpfr_t x, y, z, abs_error, rel_error;                           \
        mpfr_init2(x, precision);                                       \
        mpfr_init2(y, precision);                                       \
        mpfr_init2(z, precision);                                       \
        mpfr_init2(abs_error, precision);                               \
        mpfr_init2(rel_error, precision);                               \
        mpfr_set_zero(abs_error, 0);                                    \
        mpfr_set_zero(rel_error, 0);                                    \
        for (int64_t i = 0; i < N; i++) {                               \
            mpfr_set_flt(x, _x[i], mpfr_round_mode);                    \
            mpfr_set_flt(y, _y[i], mpfr_round_mode);                    \
            mpfr_ ## OPNAME(x, x, mpfr_round_mode);                     \
            mpfr_set(z, x, mpfr_round_mode);                            \
            mpfr_sub(x, x, y, mpfr_round_mode);                         \
            mpfr_abs(x, x, mpfr_round_mode);                            \
            if (mpfr_cmp(x, abs_error) > 0)                             \
                mpfr_set(abs_error, x, mpfr_round_mode);                \
            mpfr_abs(z, z, mpfr_round_mode);                            \
            mpfr_div(z, x, z, mpfr_round_mode);                         \
            if (mpfr_cmp(z, rel_error) > 0)                             \
                mpfr_set(rel_error, z, mpfr_round_mode);                \
        }                                                               \
        *out_abs_error = mpfr_get_d(abs_error, mpfr_round_mode);        \
        *out_rel_error = mpfr_get_d(rel_error, mpfr_round_mode);        \
        mpfr_flags_t flags = mpfr_flags_save();                         \
        *exceptions = mpfr_except_str(flags);                           \
        mpfr_clears(x, y, z, abs_error, rel_error, (mpfr_ptr) 0);       \
        return 0;                                                       \
    }                                                                   \


#define mathop_error_fn_double(OPNAME)                                  \
    static int mathop_error_double_ ## OPNAME(                          \
        int64_t N,                                                      \
        const double * _x,                                              \
        const double * _y,                                              \
        enum round_mode round_mode,                                     \
        int precision,                                                  \
        double * out_abs_error,                                         \
        double * out_rel_error,                                         \
        const char ** exceptions)                                       \
    {                                                                   \
        mpfr_rnd_t mpfr_round_mode;                                     \
        int err = round_mode_mpfr(round_mode, &mpfr_round_mode);        \
        if (err) return err;                                            \
        mpfr_t x, y, z, abs_error, rel_error;                           \
        mpfr_init2(x, precision);                                       \
        mpfr_init2(y, precision);                                       \
        mpfr_init2(z, precision);                                       \
        mpfr_init2(abs_error, precision);                               \
        mpfr_init2(rel_error, precision);                               \
        mpfr_set_zero(abs_error, 0);                                    \
        mpfr_set_zero(rel_error, 0);                                    \
        for (int64_t i = 0; i < N; i++) {                               \
            mpfr_set_d(x, _x[i], mpfr_round_mode);                      \
            mpfr_set_d(y, _y[i], mpfr_round_mode);                      \
            mpfr_ ## OPNAME(x, x, mpfr_round_mode);                     \
            mpfr_set(z, x, mpfr_round_mode);                            \
            mpfr_sub(x, x, y, mpfr_round_mode);                         \
            mpfr_abs(x, x, mpfr_round_mode);                            \
            if (mpfr_cmp(x, abs_error) > 0)                             \
                mpfr_set(abs_error, x, mpfr_round_mode);                \
            mpfr_abs(z, z, mpfr_round_mode);                            \
            mpfr_div(z, x, z, mpfr_round_mode);                         \
            if (mpfr_cmp(z, rel_error) > 0)                             \
                mpfr_set(rel_error, z, mpfr_round_mode);                \
        }                                                               \
        *out_abs_error = mpfr_get_d(abs_error, mpfr_round_mode);        \
        *out_rel_error = mpfr_get_d(rel_error, mpfr_round_mode);        \
        mpfr_flags_t flags = mpfr_flags_save();                         \
        *exceptions = mpfr_except_str(flags);                           \
        mpfr_clears(x, y, z, abs_error, rel_error, (mpfr_ptr) 0);       \
        return 0;                                                       \
    }                                                                   \


mathop_error_fn_double(cos)
mathop_error_fn_float(cos)
mathop_error_fn_double(sin)
mathop_error_fn_float(sin)
mathop_error_fn_double(tan)
mathop_error_fn_float(tan)
mathop_error_fn_double(acos)
mathop_error_fn_float(acos)
mathop_error_fn_double(asin)
mathop_error_fn_float(asin)
mathop_error_fn_double(atan)
mathop_error_fn_float(atan)
mathop_error_fn_double(cosh)
mathop_error_fn_float(cosh)
mathop_error_fn_double(sinh)
mathop_error_fn_float(sinh)
mathop_error_fn_double(tanh)
mathop_error_fn_float(tanh)
mathop_error_fn_double(acosh)
mathop_error_fn_float(acosh)
mathop_error_fn_double(asinh)
mathop_error_fn_float(asinh)
mathop_error_fn_double(atanh)
mathop_error_fn_float(atanh)
mathop_error_fn_double(exp)
mathop_error_fn_float(exp)
mathop_error_fn_double(log)
mathop_error_fn_float(log)
mathop_error_fn_double(log10)
mathop_error_fn_float(log10)
mathop_error_fn_double(exp2)
mathop_error_fn_float(exp2)
mathop_error_fn_double(expm1)
mathop_error_fn_float(expm1)
mathop_error_fn_double(log1p)
mathop_error_fn_float(log1p)
mathop_error_fn_double(log2)
mathop_error_fn_float(log2)
mathop_error_fn_double(sqrt)
mathop_error_fn_float(sqrt)
mathop_error_fn_double(cbrt)
mathop_error_fn_float(cbrt)
mathop_error_fn_double(erf)
mathop_error_fn_float(erf)
mathop_error_fn_double(erfc)
mathop_error_fn_float(erfc)
mathop_error_fn_double(gamma)
mathop_error_fn_float(gamma)

static int mathop_error_float_lgamma(
    int64_t N,
    const float * _x,
    const float * _y,
    enum round_mode round_mode,
    int precision,
    double * out_abs_error,
    double * out_rel_error,
    const char ** exceptions)
{
    mpfr_rnd_t mpfr_round_mode;
    int err = round_mode_mpfr(round_mode, &mpfr_round_mode);
    if (err) return err;
    mpfr_t x, y, z, abs_error, rel_error;
    mpfr_clear_flags();
    mpfr_init2(x, precision);
    mpfr_init2(y, precision);
    mpfr_init2(z, precision);
    mpfr_init2(abs_error, precision);
    mpfr_init2(rel_error, precision);
    mpfr_set_zero(abs_error, 0);
    mpfr_set_zero(rel_error, 0);
    #pragma omp for simd
    for (int64_t i = 0; i < N; i++) {
        mpfr_set_flt(x, _x[i], mpfr_round_mode);
        mpfr_set_flt(y, _y[i], mpfr_round_mode);
        int sign;
        mpfr_lgamma(x, &sign, x, mpfr_round_mode);
        mpfr_set(z, x, mpfr_round_mode);
        mpfr_sub(x, x, y, mpfr_round_mode);
        mpfr_abs(x, x, mpfr_round_mode);
        if (mpfr_cmp(x, abs_error) > 0)
            mpfr_set(abs_error, x, mpfr_round_mode);
        mpfr_abs(z, z, mpfr_round_mode);
        mpfr_div(z, x, z, mpfr_round_mode);
        if (mpfr_cmp(z, rel_error) > 0)
            mpfr_set(rel_error, z, mpfr_round_mode);
    }
    *out_abs_error = mpfr_get_d(abs_error, mpfr_round_mode);
    *out_rel_error = mpfr_get_d(rel_error, mpfr_round_mode);
    mpfr_flags_t flags = mpfr_flags_save();
    *exceptions = mpfr_except_str(flags);
    mpfr_clears(x, y, z, abs_error, rel_error, (mpfr_ptr) 0);
    return 0;
}

static int mathop_error_double_lgamma(
    int64_t N,
    const double * _x,
    const double * _y,
    enum round_mode round_mode,
    int precision,
    double * out_abs_error,
    double * out_rel_error,
    const char ** exceptions)
{
    mpfr_rnd_t mpfr_round_mode;
    int err = round_mode_mpfr(round_mode, &mpfr_round_mode);
    if (err) return err;
    mpfr_t x, y, z, abs_error, rel_error;
    mpfr_init2(x, precision);
    mpfr_init2(y, precision);
    mpfr_init2(z, precision);
    mpfr_init2(abs_error, precision);
    mpfr_init2(rel_error, precision);
    mpfr_set_zero(abs_error, 0);
    mpfr_set_zero(rel_error, 0);
    #pragma omp for simd
    for (int64_t i = 0; i < N; i++) {
        mpfr_set_d(x, _x[i], mpfr_round_mode);
        mpfr_set_d(y, _y[i], mpfr_round_mode);
        int sign;
        mpfr_lgamma(x, &sign, x, mpfr_round_mode);
        mpfr_set(z, x, mpfr_round_mode);
        mpfr_sub(x, x, y, mpfr_round_mode);
        mpfr_abs(x, x, mpfr_round_mode);
        if (mpfr_cmp(x, abs_error) > 0)
            mpfr_set(abs_error, x, mpfr_round_mode);
        mpfr_abs(z, z, mpfr_round_mode);
        mpfr_div(z, x, z, mpfr_round_mode);
        if (mpfr_cmp(z, rel_error) > 0)
            mpfr_set(rel_error, z, mpfr_round_mode);
    }
    *out_abs_error = mpfr_get_d(abs_error, mpfr_round_mode);
    *out_rel_error = mpfr_get_d(rel_error, mpfr_round_mode);
    mpfr_flags_t flags = mpfr_flags_save();
    *exceptions = mpfr_except_str(flags);
    mpfr_clears(x, y, z, abs_error, rel_error, (mpfr_ptr) 0);
    return 0;
}

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
    const char ** exceptions)
{
    int err;
    *exceptions = NULL;

    switch (mathop) {
    case mathop_cos:
        err = mathop_error_double_cos(
            result->size, input->f64, result->f64,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_cosf:
        err = mathop_error_float_cos(
            result->size, input->f32, result->f32,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_sin:
        err = mathop_error_double_sin(
            result->size, input->f64, result->f64,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_sinf:
        err = mathop_error_float_sin(
            result->size, input->f32, result->f32,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_tan:
        err = mathop_error_double_tan(
            result->size, input->f64, result->f64,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_tanf:
        err = mathop_error_float_tan(
            result->size, input->f32, result->f32,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_acos:
        err = mathop_error_double_acos(
            result->size, input->f64, result->f64,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_acosf:
        err = mathop_error_float_acos(
            result->size, input->f32, result->f32,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_asin:
        err = mathop_error_double_asin(
            result->size, input->f64, result->f64,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_asinf:
        err = mathop_error_float_asin(
            result->size, input->f32, result->f32,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_atan:
        err = mathop_error_double_atan(
            result->size, input->f64, result->f64,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_atanf:
        err = mathop_error_float_atan(
            result->size, input->f32, result->f32,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_cosh:
        err = mathop_error_double_cosh(
            result->size, input->f64, result->f64,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_coshf:
        err = mathop_error_float_cosh(
            result->size, input->f32, result->f32,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_sinh:
        err = mathop_error_double_sinh(
            result->size, input->f64, result->f64,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_sinhf:
        err = mathop_error_float_sinh(
            result->size, input->f32, result->f32,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_tanh:
        err = mathop_error_double_tanh(
            result->size, input->f64, result->f64,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_tanhf:
        err = mathop_error_float_tanh(
            result->size, input->f32, result->f32,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_acosh:
        err = mathop_error_double_acosh(
            result->size, input->f64, result->f64,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_acoshf:
        err = mathop_error_float_acosh(
            result->size, input->f32, result->f32,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_asinh:
        err = mathop_error_double_asinh(
            result->size, input->f64, result->f64,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_asinhf:
        err = mathop_error_float_asinh(
            result->size, input->f32, result->f32,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_atanh:
        err = mathop_error_double_atanh(
            result->size, input->f64, result->f64,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_atanhf:
        err = mathop_error_float_atanh(
            result->size, input->f32, result->f32,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_exp:
        err = mathop_error_double_exp(
            result->size, input->f64, result->f64,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_expf:
        err = mathop_error_float_exp(
            result->size, input->f32, result->f32,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_log:
        err = mathop_error_double_log(
            result->size, input->f64, result->f64,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_logf:
        err = mathop_error_float_log(
            result->size, input->f32, result->f32,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_log10:
        err = mathop_error_double_log10(
            result->size, input->f64, result->f64,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_log10f:
        err = mathop_error_float_log10(
            result->size, input->f32, result->f32,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_exp2:
        err = mathop_error_double_exp2(
            result->size, input->f64, result->f64,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_exp2f:
        err = mathop_error_float_exp2(
            result->size, input->f32, result->f32,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_expm1:
        err = mathop_error_double_expm1(
            result->size, input->f64, result->f64,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_expm1f:
        err = mathop_error_float_expm1(
            result->size, input->f32, result->f32,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_log1p:
        err = mathop_error_double_log1p(
            result->size, input->f64, result->f64,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_log1pf:
        err = mathop_error_float_log1p(
            result->size, input->f32, result->f32,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_log2:
        err = mathop_error_double_log2(
            result->size, input->f64, result->f64,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_log2f:
        err = mathop_error_float_log2(
            result->size, input->f32, result->f32,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_sqrt:
        err = mathop_error_double_sqrt(
            result->size, input->f64, result->f64,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_sqrtf:
        err = mathop_error_float_sqrt(
            result->size, input->f32, result->f32,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_cbrt:
        err = mathop_error_double_cbrt(
            result->size, input->f64, result->f64,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_cbrtf:
        err = mathop_error_float_cbrt(
            result->size, input->f32, result->f32,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_erf:
        err = mathop_error_double_erf(
            result->size, input->f64, result->f64,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_erff:
        err = mathop_error_float_erf(
            result->size, input->f32, result->f32,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_erfc:
        err = mathop_error_double_erfc(
            result->size, input->f64, result->f64,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_erfcf:
        err = mathop_error_float_erfc(
            result->size, input->f32, result->f32,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_tgamma:
        err = mathop_error_double_gamma(
            result->size, input->f64, result->f64,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_tgammaf:
        err = mathop_error_float_gamma(
            result->size, input->f32, result->f32,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_lgamma:
        err = mathop_error_double_lgamma(
            result->size, input->f64, result->f64,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    case mathop_lgammaf:
        err = mathop_error_float_lgamma(
            result->size, input->f32, result->f32,
            round_mode, precision, abs_error, rel_error, exceptions);
        break;
    default:
        return EINVAL;
    }

    return 0;
}

#else

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
    const char ** exceptions)
{
    return ENOTSUP;
}

#endif

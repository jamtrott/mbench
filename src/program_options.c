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
 * Parsing of program options.
 */

#include "program_options.h"
#include "mathop.h"
#include "parse.h"

#ifdef HAVE_MPFR
#include <mpfr.h>
#endif

#include <errno.h>

#include <fenv.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * `program_options_init()` configures the default program options.
 */
static int program_options_init(
    struct program_options * args)
{
    args->filename = NULL;
    args->mathop = mathop_exp;
    args->rounding_mode = fegetround();
    args->alignment = sizeof(void *);
    args->repeat = 1;
    args->min_ops = 0;
#ifdef HAVE_MPFR
    args->error_precision = mpfr_get_default_prec();
#else
    args->error_precision = -1;
#endif
    args->output_field_width = 0;
    args->output_precision = -1;
    args->verbose = 1;
    args->help = false;
    args->version = false;
    return 0;
}

/**
 * `program_options_free()` frees memory and other resources
 * associated with parsing program options.
 */
void program_options_free(
    struct program_options * args)
{
    if (args->filename)
        free(args->filename);
}

/**
 * `program_options_print_help()` prints a help text.
 */
void program_options_print_help(
    FILE * f)
{
    fprintf(f, "Usage: %s [OPTION..] [FILE]\n", program_name);
    fprintf(f, "Benchmark common mathematical functions\n");
    fprintf(f, "\n");
    fprintf(f, "The options are:\n");
    fprintf(f, "  --op=OP\t\tmath operation: cos, cosf, sin, sinf, tan, tanf, acos,\n");
    fprintf(f, "\t\t\tacosf, asin, asinf, atan, atanf, cosh, coshf, sinh,\n");
    fprintf(f, "\t\t\tsinhf, tanh, tanhf, acosh, acoshf, asinh, asinhf,\n");
    fprintf(f, "\t\t\tatanh, atanhf, exp, expf, log, logf, log10, log10f,\n");
    fprintf(f, "\t\t\texp2, exp2f, expm1, expm1f, log1p, log1pf, log2,\n");
    fprintf(f, "\t\t\tlog2f, sqrt, sqrtf, cbrt,cbrtf, erf, erff, erfc,\n");
    fprintf(f, "\t\t\terfcf, tgamma, tgammaf, lgamma or lgammaf.\n");
    fprintf(f, "  --round=MODE\t\trounding mode: downward, tonearest, towardzero or\n");
    fprintf(f, "\t\t\tupward.\n");
    fprintf(f, "  --alignment=N\t\talignment in bytes of allocated memory (default: %ld)\n", sizeof(void *));
    fprintf(f, "  --min-ops=N\t\trepeat until a minimum number of operations performed\n");
    fprintf(f, "  --repeat=N\t\trepeat benchmark\n");
    fprintf(f, "  --error-precision=N\tprecision to use when computing error\n");
    fprintf(f, "  --out-field-width=N\tfield width for output\n");
    fprintf(f, "  --out-precision=N\tprecision for output\n");
    fprintf(f, "  -v, --verbose\t\tbe more verbose\n");
    fprintf(f, "  -q, --quiet\t\tsuppress output\n");
    fprintf(f, "\n");
    fprintf(f, "  -h, --help\t\tdisplay this help and exit\n");
    fprintf(f, "  --version\t\tdisplay version information and exit\n");
    fprintf(f, "\n");
    fprintf(f, "A list of numerical values, separated by whitespace, are read from FILE\n");
    fprintf(f, "and used as input to the benchmark. If no file is given or FILE is '-',\n");
    fprintf(f, "then standard input is read.\n");
    fprintf(f, "\n");
    fprintf(f, "Report bugs to: <james@simula.no>\n");
}

/**
 * `program_options_print_version()` prints version information.
 */
static void program_options_print_version(
    FILE * f)
{
    fprintf(f, "%s %s\n", program_name, program_version);
    fprintf(f, "%s\n", program_copyright);
    fprintf(f, "%s\n", program_license);
}

/**
 * `parse_program_options()` parses program options.
 */
int parse_program_options(
    int * argc,
    char *** argv,
    struct program_options * args,
    int * num_error_args)
{
    int err;
    *num_error_args = 1;

    /* Set program invocation name. */
    program_invocation_name = (*argv)[0];
    program_invocation_short_name = (
        strrchr(program_invocation_name, '/')
        ? strrchr(program_invocation_name, '/') + 1
        : program_invocation_name);
    (*argc)--; (*argv)++;

    /* Set default program options. */
    err = program_options_init(args);
    if (err)
        return err;

    /* Parse program options. */
    int num_arguments_consumed = 0;
    while (*argc > 0) {
        *argc -= num_arguments_consumed;
        *argv += num_arguments_consumed;
        num_arguments_consumed = 0;
        if (*argc <= 0)
            break;

        /* Parse math operation. */
        if (strcmp((*argv)[0], "--op") == 0) {
            if (*argc < 2) {
                program_options_free(args);
                return EINVAL;
            }
            err = parse_mathop((*argv)[1], &args->mathop);
            if (err) {
                *num_error_args = 2;
                program_options_free(args);
                return err;
            }
            num_arguments_consumed += 2;
            continue;
        } else if (strstr((*argv)[0], "--op=") == (*argv)[0]) {
            err = parse_mathop(
                (*argv)[0] + strlen("--op="), &args->mathop);
            if (err) {
                program_options_free(args);
                return err;
            }
            num_arguments_consumed++;
            continue;
        }

        /* Parse rounding mode. */
        if (strcmp((*argv)[0], "--round") == 0) {
            if (*argc < 2) {
                program_options_free(args);
                return EINVAL;
            }
            err = parse_round_mode((*argv)[1], &args->rounding_mode);
            if (err) {
                *num_error_args = 2;
                program_options_free(args);
                return err;
            }
            num_arguments_consumed += 2;
            continue;
        } else if (strstr((*argv)[0], "--round=") == (*argv)[0]) {
            err = parse_round_mode(
                (*argv)[0] + strlen("--round="), &args->rounding_mode);
            if (err) {
                program_options_free(args);
                return err;
            }
            num_arguments_consumed++;
            continue;
        }

        /* Parse alignment requirements. */
        if (strcmp((*argv)[0], "--alignment") == 0) {
            if (*argc < 2) {
                program_options_free(args);
                return EINVAL;
            }
            err = parse_int32((*argv)[1], NULL, &args->alignment, NULL);
            if (err) {
                program_options_free(args);
                return err;
            }
            if (args->alignment <= 0) {
                *num_error_args = 2;
                program_options_free(args);
                return EINVAL;
            }
            num_arguments_consumed += 2;
            continue;
        } else if (strstr((*argv)[0], "--alignment=") == (*argv)[0]) {
            err = parse_int32(
                (*argv)[0] + strlen("--alignment="), NULL, &args->alignment, NULL);
            if (err) {
                program_options_free(args);
                return err;
            }
            if (args->alignment <= 0) {
                program_options_free(args);
                return EINVAL;
            }
            num_arguments_consumed++;
            continue;
        }

        /* Parse minimum number of operations. */
        if (strcmp((*argv)[0], "--min-ops") == 0) {
            if (*argc < 2) {
                program_options_free(args);
                return EINVAL;
            }
            err = parse_int64((*argv)[1], NULL, &args->min_ops, NULL);
            if (err) {
                *num_error_args = 2;
                program_options_free(args);
                return err;
            }
            num_arguments_consumed += 2;
            continue;
        } else if (strstr((*argv)[0], "--min-ops=") == (*argv)[0]) {
            err = parse_int64(
                (*argv)[0] + strlen("--min-ops="), NULL, &args->min_ops, NULL);
            if (err) {
                program_options_free(args);
                return err;
            }
            num_arguments_consumed++;
            continue;
        }

        /* Parse benchmark repeats. */
        if (strcmp((*argv)[0], "--repeat") == 0) {
            if (*argc < 2) {
                program_options_free(args);
                return EINVAL;
            }
            err = parse_int32((*argv)[1], NULL, &args->repeat, NULL);
            if (err) {
                *num_error_args = 2;
                program_options_free(args);
                return err;
            }
            num_arguments_consumed += 2;
            continue;
        } else if (strstr((*argv)[0], "--repeat=") == (*argv)[0]) {
            err = parse_int32(
                (*argv)[0] + strlen("--repeat="), NULL, &args->repeat, NULL);
            if (err) {
                program_options_free(args);
                return err;
            }
            num_arguments_consumed++;
            continue;
        }

        /* Parse error precision. */
        if (strcmp((*argv)[0], "--error-precision") == 0) {
            if (*argc < 2) {
                program_options_free(args);
                return EINVAL;
            }
            err = parse_int32((*argv)[1], NULL, &args->error_precision, NULL);
            if (err) {
                *num_error_args = 2;
                program_options_free(args);
                return err;
            }
            num_arguments_consumed += 2;
            continue;
        } else if (strstr((*argv)[0], "--error-precision=") == (*argv)[0]) {
            err = parse_int32(
                (*argv)[0] + strlen("--error-precision="), NULL,
                &args->error_precision, NULL);
            if (err) {
                program_options_free(args);
                return err;
            }
            num_arguments_consumed++;
            continue;
        }

        /* Parse output options. */
        if (strcmp((*argv)[0], "--out-field-width") == 0) {
            if (*argc < 2) {
                program_options_free(args);
                return EINVAL;
            }
            err = parse_int32((*argv)[1], NULL, &args->output_field_width, NULL);
            if (err) {
                *num_error_args = 2;
                program_options_free(args);
                return err;
            }
            num_arguments_consumed += 2;
            continue;
        } else if (strstr((*argv)[0], "--out-field-width=") == (*argv)[0]) {
            err = parse_int32(
                (*argv)[0] + strlen("--out-field-width="), NULL,
                &args->output_field_width, NULL);
            if (err) {
                program_options_free(args);
                return err;
            }
            num_arguments_consumed++;
            continue;
        }

        if (strcmp((*argv)[0], "--out-precision") == 0) {
            if (*argc < 2) {
                program_options_free(args);
                return EINVAL;
            }
            err = parse_int32((*argv)[1], NULL, &args->output_precision, NULL);
            if (err) {
                *num_error_args = 2;
                program_options_free(args);
                return err;
            }
            num_arguments_consumed += 2;
            continue;
        } else if (strstr((*argv)[0], "--out-precision=") == (*argv)[0]) {
            err = parse_int32(
                (*argv)[0] + strlen("--out-precision="), NULL,
                &args->output_precision, NULL);
            if (err) {
                program_options_free(args);
                return err;
            }
            num_arguments_consumed++;
            continue;
        }

        if (strcmp((*argv)[0], "-v") == 0 || strcmp((*argv)[0], "--verbose") == 0) {
            args->verbose++;
            num_arguments_consumed++;
            continue;
        }

        if (strcmp((*argv)[0], "-q") == 0 || strcmp((*argv)[0], "--quiet") == 0) {
            args->verbose = 0;
            num_arguments_consumed++;
            continue;
        }

        /* If requested, print program help text. */
        if (strcmp((*argv)[0], "-h") == 0 || strcmp((*argv)[0], "--help") == 0) {
            program_options_free(args);
            program_options_print_help(stdout);
            exit(EXIT_SUCCESS);
        }

        /* If requested, print program version information. */
        if (strcmp((*argv)[0], "--version") == 0) {
            program_options_free(args);
            program_options_print_version(stdout);
            exit(EXIT_SUCCESS);
        }

        /* Stop parsing options after '--'.  */
        if (strcmp((*argv)[0], "--") == 0) {
            (*argc)--; (*argv)++;
            break;
        }

        /* Parse file name, where '-' means to use standard input. */
        if (strcmp((*argv)[0], "-") == 0) {
            num_arguments_consumed++;
            continue;
        }
        if (strlen((*argv)[0]) > 0 && (*argv)[0][0] != '-') {
            if (args->filename)
                free(args->filename);
            args->filename = strdup((*argv)[0]);
            num_arguments_consumed++;
            continue;
        }

        /* Unrecognised option. */
        program_options_free(args);
        return EINVAL;
    }

    return 0;
}

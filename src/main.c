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
 * Benchmarking program for common mathematical functions.
 */

#include "program_options.h"
#include "fexcept.h"

#include <errno.h>

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const char * program_name = "mbench";
const char * program_version = "1.0";
const char * program_copyright =
    "Copyright (C) 2020 James D. Trotter";
const char * program_license =
    "License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>\n"
    "This is free software: you are free to change and redistribute it.\n"
    "There is NO WARRANTY, to the extent permitted by law.";

/**
 * `timespec_duration()` is the duration, in seconds, elapsed between
 * two given time points.
 */
static double timespec_duration(
    struct timespec t0,
    struct timespec t1)
{
    return (t1.tv_sec - t0.tv_sec) +
        (t1.tv_nsec - t0.tv_nsec) * 1e-9;
}

/*
 * Custom OpenMP reduction operator for combining errors from
 * different threads.
 */
#pragma omp declare reduction(                                          \
    err_add : int :                                                     \
    omp_out = omp_out ? omp_out : omp_in)                               \
    initializer (omp_priv=0)

/**
 * `main()`.
 */
int main(int argc, char *argv[])
{
    int err;
    struct timespec t0, t1;

    /* Parse program options. */
    struct program_options args;
    int argc_copy = argc;
    char ** argv_copy = argv;
    int num_error_args;
    err = parse_program_options(&argc_copy, &argv_copy, &args, &num_error_args);
    if (err && num_error_args == 2) {
        fprintf(stderr, "%s: %s `%s %s'\n", program_invocation_short_name,
                strerror(err), argv_copy[0], argv_copy[1]);
        return EXIT_FAILURE;
    } else if (err) {
        fprintf(stderr, "%s: %s `%s'\n", program_invocation_short_name,
                strerror(err), argv_copy[0]);
        return EXIT_FAILURE;
    }

    /* Set rounding mode for floating-point calculations. */
    err = set_round_mode(args.rounding_mode);
    if (err) {
        fprintf(stderr, "%s: %s\n", program_invocation_short_name, strerror(err));
        program_options_free(&args);
        return EXIT_FAILURE;
    }

    /* Allocate storage and read input for the benchmark. */
    FILE * f = stdin;
    if (args.filename) {
        if ((f = fopen(args.filename, "r")) == NULL) {
            fprintf(stderr, "%s: %s: %s\n",
                    program_invocation_short_name, args.filename, strerror(errno));
            program_options_free(&args);
            return EXIT_FAILURE;
        }
    }
    struct mathop_input input;
    err = mathop_input_init(&input, args.mathop, f, args.alignment);
    if (err) {
        fprintf(stderr, "%s: %s\n", program_invocation_short_name, strerror(err));
        if (args.filename)
            fclose(f);
        program_options_free(&args);
        return EXIT_FAILURE;
    }
    if (args.filename)
        fclose(f);

    /* Allocate storage for results. */
    struct mathop_result result;
    err = mathop_result_init(&result, args.mathop, input.size, args.alignment);
    if (err) {
        fprintf(stderr, "%s: %s\n", program_invocation_short_name, strerror(err));
        mathop_input_free(&input);
        program_options_free(&args);
        return EXIT_FAILURE;
    }

    /* Start a timer. */
    if (args.verbose > 0) {
        fprintf(stdout, "%s: ", mathop_str(args.mathop));
        fflush(stdout);
        clock_gettime(CLOCK_MONOTONIC, &t0);
    }

    /* Benchmark the mathematical function. */
    int repeat = 0;
    int64_t num_ops = 0;
#pragma omp parallel reduction(err_add:err) reduction(max:num_ops) reduction(max:repeat)
    for (repeat = 0, num_ops = 0; (repeat < args.repeat) || (num_ops < args.min_ops); repeat++) {
        err = benchmark_mathop(args.mathop, &input, &result, &num_ops);
        if (err)
            break;
    }
    if (err) {
        fprintf(stderr, "%s: %s\n", program_invocation_name,
                strerror(err));
        if (args.verbose > 1) {
            mathop_result_print(
                &result, stderr, args.output_field_width,
                args.output_precision, " ");
            fputc('\n', stderr);
        }
        mathop_result_free(&result);
        mathop_input_free(&input);
        program_options_free(&args);
        return EXIT_FAILURE;
    }

    /* Display benchmark results. */
    if (args.verbose > 0) {
        clock_gettime(CLOCK_MONOTONIC, &t1);
        double duration = timespec_duration(t0, t1);
        double throughput = (double) num_ops / duration / 1000000.0;
        double abs_error, rel_error;
        const char * exceptions = NULL;
        err = mathop_error(
            args.mathop, &input, &result,
            args.rounding_mode, args.error_precision,
            &abs_error, &rel_error, &exceptions);
        if (!err) {
            fprintf(stdout, "%.6f seconds %d repetitions "
                    "%"PRId64" ops %.6f Mops/s exceptions: %s "
                    "absolute error: %e relative error: %e (exceptions: %s)\n",
                    duration, repeat, num_ops, throughput, fexcept_str(result.fexcept),
                    abs_error, rel_error, exceptions);
        } else if (err == ENOTSUP) {
            fprintf(stdout, "%.6f seconds %d repetitions %"PRId64" ops %.6f Mops/s "
                    "exceptions: %s\n",
                    duration, repeat, num_ops, throughput,
                    fexcept_str(result.fexcept));
        } else {
            fprintf(stderr, "%s: %s\n", program_invocation_name,
                    strerror(err));
            mathop_result_free(&result);
            mathop_input_free(&input);
            program_options_free(&args);
            return EXIT_FAILURE;
        }
        fflush(stdout);
    }

    if (args.verbose > 1) {
        mathop_result_print(
            &result, stderr, args.output_field_width,
            args.output_precision, " ");
        fputc('\n', stderr);
    }

    /* Clean up. */
    mathop_result_free(&result);
    mathop_input_free(&input);
    program_options_free(&args);
    return EXIT_SUCCESS;
}

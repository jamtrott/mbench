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

#ifndef PROGRAM_OPTIONS_H
#define PROGRAM_OPTIONS_H

#include "mathop.h"
#include "round.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

extern const char * program_name;
extern const char * program_version;
extern const char * program_copyright;
extern const char * program_license;
extern const char * program_invocation_name;
extern const char * program_invocation_short_name;

/**
 * `program_options` contains data related program options.
 */
struct program_options
{
    char * filename;
    enum mathop mathop;
    enum round_mode rounding_mode;
    int alignment;
    int repeat;
    int64_t min_ops;
    int error_precision;
    int output_field_width;
    int output_precision;
    int verbose;
    bool help;
    bool version;
};

/**
 * `parse_program_options()` parses program options.
 */
int parse_program_options(
    int * argc,
    char *** argv,
    struct program_options * args,
    int * num_error_args);

/**
 * `program_options_free()` frees memory and other resources
 * associated with parsing program options.
 */
void program_options_free(
    struct program_options * args);

#endif

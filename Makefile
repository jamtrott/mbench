# Benchmark program for common mathematical functions
# Copyright (C) 2020 James D. Trotter
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
#
# Authors: James D. Trotter <james@simula.no>
#
# Benchmarking program for common mathematical functions.

mbench = mbench

all: $(mbench)
clean:
	rm -f $(mbench_c_objects) $(mbench)
.PHONY: all clean

CFLAGS += -g -Wall -iquote src

mbench_c_sources = \
	src/fexcept.c \
	src/main.c \
	src/mathop.c \
	src/parse.c \
	src/program_options.c \
	src/round.c
mbench_c_headers = \
	src/fexcept.h \
	src/mathop.h \
	src/parse.h \
	src/program_options.h \
	src/round.h
mbench_c_objects := $(foreach x,$(mbench_c_sources),$(x:.c=.o))
$(mbench_c_objects): %.o: %.c $(mbench_c_headers)
	$(CC) -c $(CFLAGS) $< -o $@
$(mbench): $(mbench_c_objects)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

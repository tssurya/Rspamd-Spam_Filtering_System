# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/linux/rspamd/rspamd-0.8.1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/linux/rspamd/rspamd.build

# Include any dependencies generated for this target.
include src/ucl/cmake/CMakeFiles/ucl.dir/depend.make

# Include the progress variables for this target.
include src/ucl/cmake/CMakeFiles/ucl.dir/progress.make

# Include the compile flags for this target's objects.
include src/ucl/cmake/CMakeFiles/ucl.dir/flags.make

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_util.c.o: src/ucl/cmake/CMakeFiles/ucl.dir/flags.make
src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_util.c.o: /home/linux/rspamd/rspamd-0.8.1/src/ucl/src/ucl_util.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/linux/rspamd/rspamd.build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_util.c.o"
	cd /home/linux/rspamd/rspamd.build/src/ucl/cmake && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/ucl.dir/__/src/ucl_util.c.o   -c /home/linux/rspamd/rspamd-0.8.1/src/ucl/src/ucl_util.c

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_util.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ucl.dir/__/src/ucl_util.c.i"
	cd /home/linux/rspamd/rspamd.build/src/ucl/cmake && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/linux/rspamd/rspamd-0.8.1/src/ucl/src/ucl_util.c > CMakeFiles/ucl.dir/__/src/ucl_util.c.i

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_util.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ucl.dir/__/src/ucl_util.c.s"
	cd /home/linux/rspamd/rspamd.build/src/ucl/cmake && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/linux/rspamd/rspamd-0.8.1/src/ucl/src/ucl_util.c -o CMakeFiles/ucl.dir/__/src/ucl_util.c.s

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_util.c.o.requires:
.PHONY : src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_util.c.o.requires

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_util.c.o.provides: src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_util.c.o.requires
	$(MAKE) -f src/ucl/cmake/CMakeFiles/ucl.dir/build.make src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_util.c.o.provides.build
.PHONY : src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_util.c.o.provides

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_util.c.o.provides.build: src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_util.c.o

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_parser.c.o: src/ucl/cmake/CMakeFiles/ucl.dir/flags.make
src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_parser.c.o: /home/linux/rspamd/rspamd-0.8.1/src/ucl/src/ucl_parser.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/linux/rspamd/rspamd.build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_parser.c.o"
	cd /home/linux/rspamd/rspamd.build/src/ucl/cmake && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/ucl.dir/__/src/ucl_parser.c.o   -c /home/linux/rspamd/rspamd-0.8.1/src/ucl/src/ucl_parser.c

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_parser.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ucl.dir/__/src/ucl_parser.c.i"
	cd /home/linux/rspamd/rspamd.build/src/ucl/cmake && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/linux/rspamd/rspamd-0.8.1/src/ucl/src/ucl_parser.c > CMakeFiles/ucl.dir/__/src/ucl_parser.c.i

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_parser.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ucl.dir/__/src/ucl_parser.c.s"
	cd /home/linux/rspamd/rspamd.build/src/ucl/cmake && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/linux/rspamd/rspamd-0.8.1/src/ucl/src/ucl_parser.c -o CMakeFiles/ucl.dir/__/src/ucl_parser.c.s

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_parser.c.o.requires:
.PHONY : src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_parser.c.o.requires

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_parser.c.o.provides: src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_parser.c.o.requires
	$(MAKE) -f src/ucl/cmake/CMakeFiles/ucl.dir/build.make src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_parser.c.o.provides.build
.PHONY : src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_parser.c.o.provides

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_parser.c.o.provides.build: src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_parser.c.o

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_emitter.c.o: src/ucl/cmake/CMakeFiles/ucl.dir/flags.make
src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_emitter.c.o: /home/linux/rspamd/rspamd-0.8.1/src/ucl/src/ucl_emitter.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/linux/rspamd/rspamd.build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_emitter.c.o"
	cd /home/linux/rspamd/rspamd.build/src/ucl/cmake && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/ucl.dir/__/src/ucl_emitter.c.o   -c /home/linux/rspamd/rspamd-0.8.1/src/ucl/src/ucl_emitter.c

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_emitter.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ucl.dir/__/src/ucl_emitter.c.i"
	cd /home/linux/rspamd/rspamd.build/src/ucl/cmake && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/linux/rspamd/rspamd-0.8.1/src/ucl/src/ucl_emitter.c > CMakeFiles/ucl.dir/__/src/ucl_emitter.c.i

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_emitter.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ucl.dir/__/src/ucl_emitter.c.s"
	cd /home/linux/rspamd/rspamd.build/src/ucl/cmake && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/linux/rspamd/rspamd-0.8.1/src/ucl/src/ucl_emitter.c -o CMakeFiles/ucl.dir/__/src/ucl_emitter.c.s

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_emitter.c.o.requires:
.PHONY : src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_emitter.c.o.requires

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_emitter.c.o.provides: src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_emitter.c.o.requires
	$(MAKE) -f src/ucl/cmake/CMakeFiles/ucl.dir/build.make src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_emitter.c.o.provides.build
.PHONY : src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_emitter.c.o.provides

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_emitter.c.o.provides.build: src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_emitter.c.o

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_emitter_streamline.c.o: src/ucl/cmake/CMakeFiles/ucl.dir/flags.make
src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_emitter_streamline.c.o: /home/linux/rspamd/rspamd-0.8.1/src/ucl/src/ucl_emitter_streamline.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/linux/rspamd/rspamd.build/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_emitter_streamline.c.o"
	cd /home/linux/rspamd/rspamd.build/src/ucl/cmake && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/ucl.dir/__/src/ucl_emitter_streamline.c.o   -c /home/linux/rspamd/rspamd-0.8.1/src/ucl/src/ucl_emitter_streamline.c

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_emitter_streamline.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ucl.dir/__/src/ucl_emitter_streamline.c.i"
	cd /home/linux/rspamd/rspamd.build/src/ucl/cmake && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/linux/rspamd/rspamd-0.8.1/src/ucl/src/ucl_emitter_streamline.c > CMakeFiles/ucl.dir/__/src/ucl_emitter_streamline.c.i

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_emitter_streamline.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ucl.dir/__/src/ucl_emitter_streamline.c.s"
	cd /home/linux/rspamd/rspamd.build/src/ucl/cmake && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/linux/rspamd/rspamd-0.8.1/src/ucl/src/ucl_emitter_streamline.c -o CMakeFiles/ucl.dir/__/src/ucl_emitter_streamline.c.s

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_emitter_streamline.c.o.requires:
.PHONY : src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_emitter_streamline.c.o.requires

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_emitter_streamline.c.o.provides: src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_emitter_streamline.c.o.requires
	$(MAKE) -f src/ucl/cmake/CMakeFiles/ucl.dir/build.make src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_emitter_streamline.c.o.provides.build
.PHONY : src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_emitter_streamline.c.o.provides

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_emitter_streamline.c.o.provides.build: src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_emitter_streamline.c.o

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_emitter_utils.c.o: src/ucl/cmake/CMakeFiles/ucl.dir/flags.make
src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_emitter_utils.c.o: /home/linux/rspamd/rspamd-0.8.1/src/ucl/src/ucl_emitter_utils.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/linux/rspamd/rspamd.build/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_emitter_utils.c.o"
	cd /home/linux/rspamd/rspamd.build/src/ucl/cmake && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/ucl.dir/__/src/ucl_emitter_utils.c.o   -c /home/linux/rspamd/rspamd-0.8.1/src/ucl/src/ucl_emitter_utils.c

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_emitter_utils.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ucl.dir/__/src/ucl_emitter_utils.c.i"
	cd /home/linux/rspamd/rspamd.build/src/ucl/cmake && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/linux/rspamd/rspamd-0.8.1/src/ucl/src/ucl_emitter_utils.c > CMakeFiles/ucl.dir/__/src/ucl_emitter_utils.c.i

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_emitter_utils.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ucl.dir/__/src/ucl_emitter_utils.c.s"
	cd /home/linux/rspamd/rspamd.build/src/ucl/cmake && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/linux/rspamd/rspamd-0.8.1/src/ucl/src/ucl_emitter_utils.c -o CMakeFiles/ucl.dir/__/src/ucl_emitter_utils.c.s

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_emitter_utils.c.o.requires:
.PHONY : src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_emitter_utils.c.o.requires

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_emitter_utils.c.o.provides: src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_emitter_utils.c.o.requires
	$(MAKE) -f src/ucl/cmake/CMakeFiles/ucl.dir/build.make src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_emitter_utils.c.o.provides.build
.PHONY : src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_emitter_utils.c.o.provides

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_emitter_utils.c.o.provides.build: src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_emitter_utils.c.o

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_hash.c.o: src/ucl/cmake/CMakeFiles/ucl.dir/flags.make
src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_hash.c.o: /home/linux/rspamd/rspamd-0.8.1/src/ucl/src/ucl_hash.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/linux/rspamd/rspamd.build/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_hash.c.o"
	cd /home/linux/rspamd/rspamd.build/src/ucl/cmake && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/ucl.dir/__/src/ucl_hash.c.o   -c /home/linux/rspamd/rspamd-0.8.1/src/ucl/src/ucl_hash.c

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_hash.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ucl.dir/__/src/ucl_hash.c.i"
	cd /home/linux/rspamd/rspamd.build/src/ucl/cmake && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/linux/rspamd/rspamd-0.8.1/src/ucl/src/ucl_hash.c > CMakeFiles/ucl.dir/__/src/ucl_hash.c.i

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_hash.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ucl.dir/__/src/ucl_hash.c.s"
	cd /home/linux/rspamd/rspamd.build/src/ucl/cmake && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/linux/rspamd/rspamd-0.8.1/src/ucl/src/ucl_hash.c -o CMakeFiles/ucl.dir/__/src/ucl_hash.c.s

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_hash.c.o.requires:
.PHONY : src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_hash.c.o.requires

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_hash.c.o.provides: src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_hash.c.o.requires
	$(MAKE) -f src/ucl/cmake/CMakeFiles/ucl.dir/build.make src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_hash.c.o.provides.build
.PHONY : src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_hash.c.o.provides

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_hash.c.o.provides.build: src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_hash.c.o

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_schema.c.o: src/ucl/cmake/CMakeFiles/ucl.dir/flags.make
src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_schema.c.o: /home/linux/rspamd/rspamd-0.8.1/src/ucl/src/ucl_schema.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/linux/rspamd/rspamd.build/CMakeFiles $(CMAKE_PROGRESS_7)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_schema.c.o"
	cd /home/linux/rspamd/rspamd.build/src/ucl/cmake && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/ucl.dir/__/src/ucl_schema.c.o   -c /home/linux/rspamd/rspamd-0.8.1/src/ucl/src/ucl_schema.c

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_schema.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ucl.dir/__/src/ucl_schema.c.i"
	cd /home/linux/rspamd/rspamd.build/src/ucl/cmake && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/linux/rspamd/rspamd-0.8.1/src/ucl/src/ucl_schema.c > CMakeFiles/ucl.dir/__/src/ucl_schema.c.i

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_schema.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ucl.dir/__/src/ucl_schema.c.s"
	cd /home/linux/rspamd/rspamd.build/src/ucl/cmake && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/linux/rspamd/rspamd-0.8.1/src/ucl/src/ucl_schema.c -o CMakeFiles/ucl.dir/__/src/ucl_schema.c.s

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_schema.c.o.requires:
.PHONY : src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_schema.c.o.requires

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_schema.c.o.provides: src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_schema.c.o.requires
	$(MAKE) -f src/ucl/cmake/CMakeFiles/ucl.dir/build.make src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_schema.c.o.provides.build
.PHONY : src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_schema.c.o.provides

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_schema.c.o.provides.build: src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_schema.c.o

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/xxhash.c.o: src/ucl/cmake/CMakeFiles/ucl.dir/flags.make
src/ucl/cmake/CMakeFiles/ucl.dir/__/src/xxhash.c.o: /home/linux/rspamd/rspamd-0.8.1/src/ucl/src/xxhash.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/linux/rspamd/rspamd.build/CMakeFiles $(CMAKE_PROGRESS_8)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/ucl/cmake/CMakeFiles/ucl.dir/__/src/xxhash.c.o"
	cd /home/linux/rspamd/rspamd.build/src/ucl/cmake && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/ucl.dir/__/src/xxhash.c.o   -c /home/linux/rspamd/rspamd-0.8.1/src/ucl/src/xxhash.c

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/xxhash.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ucl.dir/__/src/xxhash.c.i"
	cd /home/linux/rspamd/rspamd.build/src/ucl/cmake && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/linux/rspamd/rspamd-0.8.1/src/ucl/src/xxhash.c > CMakeFiles/ucl.dir/__/src/xxhash.c.i

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/xxhash.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ucl.dir/__/src/xxhash.c.s"
	cd /home/linux/rspamd/rspamd.build/src/ucl/cmake && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/linux/rspamd/rspamd-0.8.1/src/ucl/src/xxhash.c -o CMakeFiles/ucl.dir/__/src/xxhash.c.s

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/xxhash.c.o.requires:
.PHONY : src/ucl/cmake/CMakeFiles/ucl.dir/__/src/xxhash.c.o.requires

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/xxhash.c.o.provides: src/ucl/cmake/CMakeFiles/ucl.dir/__/src/xxhash.c.o.requires
	$(MAKE) -f src/ucl/cmake/CMakeFiles/ucl.dir/build.make src/ucl/cmake/CMakeFiles/ucl.dir/__/src/xxhash.c.o.provides.build
.PHONY : src/ucl/cmake/CMakeFiles/ucl.dir/__/src/xxhash.c.o.provides

src/ucl/cmake/CMakeFiles/ucl.dir/__/src/xxhash.c.o.provides.build: src/ucl/cmake/CMakeFiles/ucl.dir/__/src/xxhash.c.o

# Object files for target ucl
ucl_OBJECTS = \
"CMakeFiles/ucl.dir/__/src/ucl_util.c.o" \
"CMakeFiles/ucl.dir/__/src/ucl_parser.c.o" \
"CMakeFiles/ucl.dir/__/src/ucl_emitter.c.o" \
"CMakeFiles/ucl.dir/__/src/ucl_emitter_streamline.c.o" \
"CMakeFiles/ucl.dir/__/src/ucl_emitter_utils.c.o" \
"CMakeFiles/ucl.dir/__/src/ucl_hash.c.o" \
"CMakeFiles/ucl.dir/__/src/ucl_schema.c.o" \
"CMakeFiles/ucl.dir/__/src/xxhash.c.o"

# External object files for target ucl
ucl_EXTERNAL_OBJECTS =

src/ucl/cmake/libucl.a: src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_util.c.o
src/ucl/cmake/libucl.a: src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_parser.c.o
src/ucl/cmake/libucl.a: src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_emitter.c.o
src/ucl/cmake/libucl.a: src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_emitter_streamline.c.o
src/ucl/cmake/libucl.a: src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_emitter_utils.c.o
src/ucl/cmake/libucl.a: src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_hash.c.o
src/ucl/cmake/libucl.a: src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_schema.c.o
src/ucl/cmake/libucl.a: src/ucl/cmake/CMakeFiles/ucl.dir/__/src/xxhash.c.o
src/ucl/cmake/libucl.a: src/ucl/cmake/CMakeFiles/ucl.dir/build.make
src/ucl/cmake/libucl.a: src/ucl/cmake/CMakeFiles/ucl.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C static library libucl.a"
	cd /home/linux/rspamd/rspamd.build/src/ucl/cmake && $(CMAKE_COMMAND) -P CMakeFiles/ucl.dir/cmake_clean_target.cmake
	cd /home/linux/rspamd/rspamd.build/src/ucl/cmake && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ucl.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/ucl/cmake/CMakeFiles/ucl.dir/build: src/ucl/cmake/libucl.a
.PHONY : src/ucl/cmake/CMakeFiles/ucl.dir/build

src/ucl/cmake/CMakeFiles/ucl.dir/requires: src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_util.c.o.requires
src/ucl/cmake/CMakeFiles/ucl.dir/requires: src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_parser.c.o.requires
src/ucl/cmake/CMakeFiles/ucl.dir/requires: src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_emitter.c.o.requires
src/ucl/cmake/CMakeFiles/ucl.dir/requires: src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_emitter_streamline.c.o.requires
src/ucl/cmake/CMakeFiles/ucl.dir/requires: src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_emitter_utils.c.o.requires
src/ucl/cmake/CMakeFiles/ucl.dir/requires: src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_hash.c.o.requires
src/ucl/cmake/CMakeFiles/ucl.dir/requires: src/ucl/cmake/CMakeFiles/ucl.dir/__/src/ucl_schema.c.o.requires
src/ucl/cmake/CMakeFiles/ucl.dir/requires: src/ucl/cmake/CMakeFiles/ucl.dir/__/src/xxhash.c.o.requires
.PHONY : src/ucl/cmake/CMakeFiles/ucl.dir/requires

src/ucl/cmake/CMakeFiles/ucl.dir/clean:
	cd /home/linux/rspamd/rspamd.build/src/ucl/cmake && $(CMAKE_COMMAND) -P CMakeFiles/ucl.dir/cmake_clean.cmake
.PHONY : src/ucl/cmake/CMakeFiles/ucl.dir/clean

src/ucl/cmake/CMakeFiles/ucl.dir/depend:
	cd /home/linux/rspamd/rspamd.build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/linux/rspamd/rspamd-0.8.1 /home/linux/rspamd/rspamd-0.8.1/src/ucl/cmake /home/linux/rspamd/rspamd.build /home/linux/rspamd/rspamd.build/src/ucl/cmake /home/linux/rspamd/rspamd.build/src/ucl/cmake/CMakeFiles/ucl.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/ucl/cmake/CMakeFiles/ucl.dir/depend


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
include contrib/hiredis/CMakeFiles/hiredis.dir/depend.make

# Include the progress variables for this target.
include contrib/hiredis/CMakeFiles/hiredis.dir/progress.make

# Include the compile flags for this target's objects.
include contrib/hiredis/CMakeFiles/hiredis.dir/flags.make

contrib/hiredis/CMakeFiles/hiredis.dir/async.c.o: contrib/hiredis/CMakeFiles/hiredis.dir/flags.make
contrib/hiredis/CMakeFiles/hiredis.dir/async.c.o: /home/linux/rspamd/rspamd-0.8.1/contrib/hiredis/async.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/linux/rspamd/rspamd.build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object contrib/hiredis/CMakeFiles/hiredis.dir/async.c.o"
	cd /home/linux/rspamd/rspamd.build/contrib/hiredis && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/hiredis.dir/async.c.o   -c /home/linux/rspamd/rspamd-0.8.1/contrib/hiredis/async.c

contrib/hiredis/CMakeFiles/hiredis.dir/async.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/hiredis.dir/async.c.i"
	cd /home/linux/rspamd/rspamd.build/contrib/hiredis && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/linux/rspamd/rspamd-0.8.1/contrib/hiredis/async.c > CMakeFiles/hiredis.dir/async.c.i

contrib/hiredis/CMakeFiles/hiredis.dir/async.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/hiredis.dir/async.c.s"
	cd /home/linux/rspamd/rspamd.build/contrib/hiredis && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/linux/rspamd/rspamd-0.8.1/contrib/hiredis/async.c -o CMakeFiles/hiredis.dir/async.c.s

contrib/hiredis/CMakeFiles/hiredis.dir/async.c.o.requires:
.PHONY : contrib/hiredis/CMakeFiles/hiredis.dir/async.c.o.requires

contrib/hiredis/CMakeFiles/hiredis.dir/async.c.o.provides: contrib/hiredis/CMakeFiles/hiredis.dir/async.c.o.requires
	$(MAKE) -f contrib/hiredis/CMakeFiles/hiredis.dir/build.make contrib/hiredis/CMakeFiles/hiredis.dir/async.c.o.provides.build
.PHONY : contrib/hiredis/CMakeFiles/hiredis.dir/async.c.o.provides

contrib/hiredis/CMakeFiles/hiredis.dir/async.c.o.provides.build: contrib/hiredis/CMakeFiles/hiredis.dir/async.c.o

contrib/hiredis/CMakeFiles/hiredis.dir/dict.c.o: contrib/hiredis/CMakeFiles/hiredis.dir/flags.make
contrib/hiredis/CMakeFiles/hiredis.dir/dict.c.o: /home/linux/rspamd/rspamd-0.8.1/contrib/hiredis/dict.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/linux/rspamd/rspamd.build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object contrib/hiredis/CMakeFiles/hiredis.dir/dict.c.o"
	cd /home/linux/rspamd/rspamd.build/contrib/hiredis && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/hiredis.dir/dict.c.o   -c /home/linux/rspamd/rspamd-0.8.1/contrib/hiredis/dict.c

contrib/hiredis/CMakeFiles/hiredis.dir/dict.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/hiredis.dir/dict.c.i"
	cd /home/linux/rspamd/rspamd.build/contrib/hiredis && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/linux/rspamd/rspamd-0.8.1/contrib/hiredis/dict.c > CMakeFiles/hiredis.dir/dict.c.i

contrib/hiredis/CMakeFiles/hiredis.dir/dict.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/hiredis.dir/dict.c.s"
	cd /home/linux/rspamd/rspamd.build/contrib/hiredis && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/linux/rspamd/rspamd-0.8.1/contrib/hiredis/dict.c -o CMakeFiles/hiredis.dir/dict.c.s

contrib/hiredis/CMakeFiles/hiredis.dir/dict.c.o.requires:
.PHONY : contrib/hiredis/CMakeFiles/hiredis.dir/dict.c.o.requires

contrib/hiredis/CMakeFiles/hiredis.dir/dict.c.o.provides: contrib/hiredis/CMakeFiles/hiredis.dir/dict.c.o.requires
	$(MAKE) -f contrib/hiredis/CMakeFiles/hiredis.dir/build.make contrib/hiredis/CMakeFiles/hiredis.dir/dict.c.o.provides.build
.PHONY : contrib/hiredis/CMakeFiles/hiredis.dir/dict.c.o.provides

contrib/hiredis/CMakeFiles/hiredis.dir/dict.c.o.provides.build: contrib/hiredis/CMakeFiles/hiredis.dir/dict.c.o

contrib/hiredis/CMakeFiles/hiredis.dir/hiredis.c.o: contrib/hiredis/CMakeFiles/hiredis.dir/flags.make
contrib/hiredis/CMakeFiles/hiredis.dir/hiredis.c.o: /home/linux/rspamd/rspamd-0.8.1/contrib/hiredis/hiredis.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/linux/rspamd/rspamd.build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object contrib/hiredis/CMakeFiles/hiredis.dir/hiredis.c.o"
	cd /home/linux/rspamd/rspamd.build/contrib/hiredis && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/hiredis.dir/hiredis.c.o   -c /home/linux/rspamd/rspamd-0.8.1/contrib/hiredis/hiredis.c

contrib/hiredis/CMakeFiles/hiredis.dir/hiredis.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/hiredis.dir/hiredis.c.i"
	cd /home/linux/rspamd/rspamd.build/contrib/hiredis && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/linux/rspamd/rspamd-0.8.1/contrib/hiredis/hiredis.c > CMakeFiles/hiredis.dir/hiredis.c.i

contrib/hiredis/CMakeFiles/hiredis.dir/hiredis.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/hiredis.dir/hiredis.c.s"
	cd /home/linux/rspamd/rspamd.build/contrib/hiredis && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/linux/rspamd/rspamd-0.8.1/contrib/hiredis/hiredis.c -o CMakeFiles/hiredis.dir/hiredis.c.s

contrib/hiredis/CMakeFiles/hiredis.dir/hiredis.c.o.requires:
.PHONY : contrib/hiredis/CMakeFiles/hiredis.dir/hiredis.c.o.requires

contrib/hiredis/CMakeFiles/hiredis.dir/hiredis.c.o.provides: contrib/hiredis/CMakeFiles/hiredis.dir/hiredis.c.o.requires
	$(MAKE) -f contrib/hiredis/CMakeFiles/hiredis.dir/build.make contrib/hiredis/CMakeFiles/hiredis.dir/hiredis.c.o.provides.build
.PHONY : contrib/hiredis/CMakeFiles/hiredis.dir/hiredis.c.o.provides

contrib/hiredis/CMakeFiles/hiredis.dir/hiredis.c.o.provides.build: contrib/hiredis/CMakeFiles/hiredis.dir/hiredis.c.o

contrib/hiredis/CMakeFiles/hiredis.dir/net.c.o: contrib/hiredis/CMakeFiles/hiredis.dir/flags.make
contrib/hiredis/CMakeFiles/hiredis.dir/net.c.o: /home/linux/rspamd/rspamd-0.8.1/contrib/hiredis/net.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/linux/rspamd/rspamd.build/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object contrib/hiredis/CMakeFiles/hiredis.dir/net.c.o"
	cd /home/linux/rspamd/rspamd.build/contrib/hiredis && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/hiredis.dir/net.c.o   -c /home/linux/rspamd/rspamd-0.8.1/contrib/hiredis/net.c

contrib/hiredis/CMakeFiles/hiredis.dir/net.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/hiredis.dir/net.c.i"
	cd /home/linux/rspamd/rspamd.build/contrib/hiredis && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/linux/rspamd/rspamd-0.8.1/contrib/hiredis/net.c > CMakeFiles/hiredis.dir/net.c.i

contrib/hiredis/CMakeFiles/hiredis.dir/net.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/hiredis.dir/net.c.s"
	cd /home/linux/rspamd/rspamd.build/contrib/hiredis && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/linux/rspamd/rspamd-0.8.1/contrib/hiredis/net.c -o CMakeFiles/hiredis.dir/net.c.s

contrib/hiredis/CMakeFiles/hiredis.dir/net.c.o.requires:
.PHONY : contrib/hiredis/CMakeFiles/hiredis.dir/net.c.o.requires

contrib/hiredis/CMakeFiles/hiredis.dir/net.c.o.provides: contrib/hiredis/CMakeFiles/hiredis.dir/net.c.o.requires
	$(MAKE) -f contrib/hiredis/CMakeFiles/hiredis.dir/build.make contrib/hiredis/CMakeFiles/hiredis.dir/net.c.o.provides.build
.PHONY : contrib/hiredis/CMakeFiles/hiredis.dir/net.c.o.provides

contrib/hiredis/CMakeFiles/hiredis.dir/net.c.o.provides.build: contrib/hiredis/CMakeFiles/hiredis.dir/net.c.o

contrib/hiredis/CMakeFiles/hiredis.dir/sds.c.o: contrib/hiredis/CMakeFiles/hiredis.dir/flags.make
contrib/hiredis/CMakeFiles/hiredis.dir/sds.c.o: /home/linux/rspamd/rspamd-0.8.1/contrib/hiredis/sds.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/linux/rspamd/rspamd.build/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object contrib/hiredis/CMakeFiles/hiredis.dir/sds.c.o"
	cd /home/linux/rspamd/rspamd.build/contrib/hiredis && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/hiredis.dir/sds.c.o   -c /home/linux/rspamd/rspamd-0.8.1/contrib/hiredis/sds.c

contrib/hiredis/CMakeFiles/hiredis.dir/sds.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/hiredis.dir/sds.c.i"
	cd /home/linux/rspamd/rspamd.build/contrib/hiredis && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/linux/rspamd/rspamd-0.8.1/contrib/hiredis/sds.c > CMakeFiles/hiredis.dir/sds.c.i

contrib/hiredis/CMakeFiles/hiredis.dir/sds.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/hiredis.dir/sds.c.s"
	cd /home/linux/rspamd/rspamd.build/contrib/hiredis && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/linux/rspamd/rspamd-0.8.1/contrib/hiredis/sds.c -o CMakeFiles/hiredis.dir/sds.c.s

contrib/hiredis/CMakeFiles/hiredis.dir/sds.c.o.requires:
.PHONY : contrib/hiredis/CMakeFiles/hiredis.dir/sds.c.o.requires

contrib/hiredis/CMakeFiles/hiredis.dir/sds.c.o.provides: contrib/hiredis/CMakeFiles/hiredis.dir/sds.c.o.requires
	$(MAKE) -f contrib/hiredis/CMakeFiles/hiredis.dir/build.make contrib/hiredis/CMakeFiles/hiredis.dir/sds.c.o.provides.build
.PHONY : contrib/hiredis/CMakeFiles/hiredis.dir/sds.c.o.provides

contrib/hiredis/CMakeFiles/hiredis.dir/sds.c.o.provides.build: contrib/hiredis/CMakeFiles/hiredis.dir/sds.c.o

# Object files for target hiredis
hiredis_OBJECTS = \
"CMakeFiles/hiredis.dir/async.c.o" \
"CMakeFiles/hiredis.dir/dict.c.o" \
"CMakeFiles/hiredis.dir/hiredis.c.o" \
"CMakeFiles/hiredis.dir/net.c.o" \
"CMakeFiles/hiredis.dir/sds.c.o"

# External object files for target hiredis
hiredis_EXTERNAL_OBJECTS =

contrib/hiredis/libhiredis.a: contrib/hiredis/CMakeFiles/hiredis.dir/async.c.o
contrib/hiredis/libhiredis.a: contrib/hiredis/CMakeFiles/hiredis.dir/dict.c.o
contrib/hiredis/libhiredis.a: contrib/hiredis/CMakeFiles/hiredis.dir/hiredis.c.o
contrib/hiredis/libhiredis.a: contrib/hiredis/CMakeFiles/hiredis.dir/net.c.o
contrib/hiredis/libhiredis.a: contrib/hiredis/CMakeFiles/hiredis.dir/sds.c.o
contrib/hiredis/libhiredis.a: contrib/hiredis/CMakeFiles/hiredis.dir/build.make
contrib/hiredis/libhiredis.a: contrib/hiredis/CMakeFiles/hiredis.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C static library libhiredis.a"
	cd /home/linux/rspamd/rspamd.build/contrib/hiredis && $(CMAKE_COMMAND) -P CMakeFiles/hiredis.dir/cmake_clean_target.cmake
	cd /home/linux/rspamd/rspamd.build/contrib/hiredis && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/hiredis.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
contrib/hiredis/CMakeFiles/hiredis.dir/build: contrib/hiredis/libhiredis.a
.PHONY : contrib/hiredis/CMakeFiles/hiredis.dir/build

contrib/hiredis/CMakeFiles/hiredis.dir/requires: contrib/hiredis/CMakeFiles/hiredis.dir/async.c.o.requires
contrib/hiredis/CMakeFiles/hiredis.dir/requires: contrib/hiredis/CMakeFiles/hiredis.dir/dict.c.o.requires
contrib/hiredis/CMakeFiles/hiredis.dir/requires: contrib/hiredis/CMakeFiles/hiredis.dir/hiredis.c.o.requires
contrib/hiredis/CMakeFiles/hiredis.dir/requires: contrib/hiredis/CMakeFiles/hiredis.dir/net.c.o.requires
contrib/hiredis/CMakeFiles/hiredis.dir/requires: contrib/hiredis/CMakeFiles/hiredis.dir/sds.c.o.requires
.PHONY : contrib/hiredis/CMakeFiles/hiredis.dir/requires

contrib/hiredis/CMakeFiles/hiredis.dir/clean:
	cd /home/linux/rspamd/rspamd.build/contrib/hiredis && $(CMAKE_COMMAND) -P CMakeFiles/hiredis.dir/cmake_clean.cmake
.PHONY : contrib/hiredis/CMakeFiles/hiredis.dir/clean

contrib/hiredis/CMakeFiles/hiredis.dir/depend:
	cd /home/linux/rspamd/rspamd.build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/linux/rspamd/rspamd-0.8.1 /home/linux/rspamd/rspamd-0.8.1/contrib/hiredis /home/linux/rspamd/rspamd.build /home/linux/rspamd/rspamd.build/contrib/hiredis /home/linux/rspamd/rspamd.build/contrib/hiredis/CMakeFiles/hiredis.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : contrib/hiredis/CMakeFiles/hiredis.dir/depend


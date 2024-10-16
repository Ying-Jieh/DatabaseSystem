# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /tmp/SP24-ECE141B-Database-Team12

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /tmp/SP24-ECE141B-Database-Team12/cmake-build-docker-arm

# Include any dependencies generated for this target.
include CMakeFiles/ECE141db.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/ECE141db.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/ECE141db.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ECE141db.dir/flags.make

CMakeFiles/ECE141db.dir/src/controller/AppController.cpp.o: CMakeFiles/ECE141db.dir/flags.make
CMakeFiles/ECE141db.dir/src/controller/AppController.cpp.o: ../src/controller/AppController.cpp
CMakeFiles/ECE141db.dir/src/controller/AppController.cpp.o: CMakeFiles/ECE141db.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/SP24-ECE141B-Database-Team12/cmake-build-docker-arm/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ECE141db.dir/src/controller/AppController.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ECE141db.dir/src/controller/AppController.cpp.o -MF CMakeFiles/ECE141db.dir/src/controller/AppController.cpp.o.d -o CMakeFiles/ECE141db.dir/src/controller/AppController.cpp.o -c /tmp/SP24-ECE141B-Database-Team12/src/controller/AppController.cpp

CMakeFiles/ECE141db.dir/src/controller/AppController.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ECE141db.dir/src/controller/AppController.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/SP24-ECE141B-Database-Team12/src/controller/AppController.cpp > CMakeFiles/ECE141db.dir/src/controller/AppController.cpp.i

CMakeFiles/ECE141db.dir/src/controller/AppController.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ECE141db.dir/src/controller/AppController.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/SP24-ECE141B-Database-Team12/src/controller/AppController.cpp -o CMakeFiles/ECE141db.dir/src/controller/AppController.cpp.s

CMakeFiles/ECE141db.dir/src/database/Database.cpp.o: CMakeFiles/ECE141db.dir/flags.make
CMakeFiles/ECE141db.dir/src/database/Database.cpp.o: ../src/database/Database.cpp
CMakeFiles/ECE141db.dir/src/database/Database.cpp.o: CMakeFiles/ECE141db.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/SP24-ECE141B-Database-Team12/cmake-build-docker-arm/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/ECE141db.dir/src/database/Database.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ECE141db.dir/src/database/Database.cpp.o -MF CMakeFiles/ECE141db.dir/src/database/Database.cpp.o.d -o CMakeFiles/ECE141db.dir/src/database/Database.cpp.o -c /tmp/SP24-ECE141B-Database-Team12/src/database/Database.cpp

CMakeFiles/ECE141db.dir/src/database/Database.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ECE141db.dir/src/database/Database.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/SP24-ECE141B-Database-Team12/src/database/Database.cpp > CMakeFiles/ECE141db.dir/src/database/Database.cpp.i

CMakeFiles/ECE141db.dir/src/database/Database.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ECE141db.dir/src/database/Database.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/SP24-ECE141B-Database-Team12/src/database/Database.cpp -o CMakeFiles/ECE141db.dir/src/database/Database.cpp.s

CMakeFiles/ECE141db.dir/src/database/RowCache.cpp.o: CMakeFiles/ECE141db.dir/flags.make
CMakeFiles/ECE141db.dir/src/database/RowCache.cpp.o: ../src/database/RowCache.cpp
CMakeFiles/ECE141db.dir/src/database/RowCache.cpp.o: CMakeFiles/ECE141db.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/SP24-ECE141B-Database-Team12/cmake-build-docker-arm/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/ECE141db.dir/src/database/RowCache.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ECE141db.dir/src/database/RowCache.cpp.o -MF CMakeFiles/ECE141db.dir/src/database/RowCache.cpp.o.d -o CMakeFiles/ECE141db.dir/src/database/RowCache.cpp.o -c /tmp/SP24-ECE141B-Database-Team12/src/database/RowCache.cpp

CMakeFiles/ECE141db.dir/src/database/RowCache.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ECE141db.dir/src/database/RowCache.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/SP24-ECE141B-Database-Team12/src/database/RowCache.cpp > CMakeFiles/ECE141db.dir/src/database/RowCache.cpp.i

CMakeFiles/ECE141db.dir/src/database/RowCache.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ECE141db.dir/src/database/RowCache.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/SP24-ECE141B-Database-Team12/src/database/RowCache.cpp -o CMakeFiles/ECE141db.dir/src/database/RowCache.cpp.s

CMakeFiles/ECE141db.dir/src/database/Schema.cpp.o: CMakeFiles/ECE141db.dir/flags.make
CMakeFiles/ECE141db.dir/src/database/Schema.cpp.o: ../src/database/Schema.cpp
CMakeFiles/ECE141db.dir/src/database/Schema.cpp.o: CMakeFiles/ECE141db.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/SP24-ECE141B-Database-Team12/cmake-build-docker-arm/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/ECE141db.dir/src/database/Schema.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ECE141db.dir/src/database/Schema.cpp.o -MF CMakeFiles/ECE141db.dir/src/database/Schema.cpp.o.d -o CMakeFiles/ECE141db.dir/src/database/Schema.cpp.o -c /tmp/SP24-ECE141B-Database-Team12/src/database/Schema.cpp

CMakeFiles/ECE141db.dir/src/database/Schema.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ECE141db.dir/src/database/Schema.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/SP24-ECE141B-Database-Team12/src/database/Schema.cpp > CMakeFiles/ECE141db.dir/src/database/Schema.cpp.i

CMakeFiles/ECE141db.dir/src/database/Schema.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ECE141db.dir/src/database/Schema.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/SP24-ECE141B-Database-Team12/src/database/Schema.cpp -o CMakeFiles/ECE141db.dir/src/database/Schema.cpp.s

CMakeFiles/ECE141db.dir/src/database/Table.cpp.o: CMakeFiles/ECE141db.dir/flags.make
CMakeFiles/ECE141db.dir/src/database/Table.cpp.o: ../src/database/Table.cpp
CMakeFiles/ECE141db.dir/src/database/Table.cpp.o: CMakeFiles/ECE141db.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/SP24-ECE141B-Database-Team12/cmake-build-docker-arm/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/ECE141db.dir/src/database/Table.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ECE141db.dir/src/database/Table.cpp.o -MF CMakeFiles/ECE141db.dir/src/database/Table.cpp.o.d -o CMakeFiles/ECE141db.dir/src/database/Table.cpp.o -c /tmp/SP24-ECE141B-Database-Team12/src/database/Table.cpp

CMakeFiles/ECE141db.dir/src/database/Table.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ECE141db.dir/src/database/Table.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/SP24-ECE141B-Database-Team12/src/database/Table.cpp > CMakeFiles/ECE141db.dir/src/database/Table.cpp.i

CMakeFiles/ECE141db.dir/src/database/Table.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ECE141db.dir/src/database/Table.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/SP24-ECE141B-Database-Team12/src/database/Table.cpp -o CMakeFiles/ECE141db.dir/src/database/Table.cpp.s

CMakeFiles/ECE141db.dir/src/database/TableRow.cpp.o: CMakeFiles/ECE141db.dir/flags.make
CMakeFiles/ECE141db.dir/src/database/TableRow.cpp.o: ../src/database/TableRow.cpp
CMakeFiles/ECE141db.dir/src/database/TableRow.cpp.o: CMakeFiles/ECE141db.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/SP24-ECE141B-Database-Team12/cmake-build-docker-arm/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/ECE141db.dir/src/database/TableRow.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ECE141db.dir/src/database/TableRow.cpp.o -MF CMakeFiles/ECE141db.dir/src/database/TableRow.cpp.o.d -o CMakeFiles/ECE141db.dir/src/database/TableRow.cpp.o -c /tmp/SP24-ECE141B-Database-Team12/src/database/TableRow.cpp

CMakeFiles/ECE141db.dir/src/database/TableRow.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ECE141db.dir/src/database/TableRow.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/SP24-ECE141B-Database-Team12/src/database/TableRow.cpp > CMakeFiles/ECE141db.dir/src/database/TableRow.cpp.i

CMakeFiles/ECE141db.dir/src/database/TableRow.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ECE141db.dir/src/database/TableRow.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/SP24-ECE141B-Database-Team12/src/database/TableRow.cpp -o CMakeFiles/ECE141db.dir/src/database/TableRow.cpp.s

CMakeFiles/ECE141db.dir/src/main.cpp.o: CMakeFiles/ECE141db.dir/flags.make
CMakeFiles/ECE141db.dir/src/main.cpp.o: ../src/main.cpp
CMakeFiles/ECE141db.dir/src/main.cpp.o: CMakeFiles/ECE141db.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/SP24-ECE141B-Database-Team12/cmake-build-docker-arm/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/ECE141db.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ECE141db.dir/src/main.cpp.o -MF CMakeFiles/ECE141db.dir/src/main.cpp.o.d -o CMakeFiles/ECE141db.dir/src/main.cpp.o -c /tmp/SP24-ECE141B-Database-Team12/src/main.cpp

CMakeFiles/ECE141db.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ECE141db.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/SP24-ECE141B-Database-Team12/src/main.cpp > CMakeFiles/ECE141db.dir/src/main.cpp.i

CMakeFiles/ECE141db.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ECE141db.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/SP24-ECE141B-Database-Team12/src/main.cpp -o CMakeFiles/ECE141db.dir/src/main.cpp.s

CMakeFiles/ECE141db.dir/src/misc/Faked.cpp.o: CMakeFiles/ECE141db.dir/flags.make
CMakeFiles/ECE141db.dir/src/misc/Faked.cpp.o: ../src/misc/Faked.cpp
CMakeFiles/ECE141db.dir/src/misc/Faked.cpp.o: CMakeFiles/ECE141db.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/SP24-ECE141B-Database-Team12/cmake-build-docker-arm/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/ECE141db.dir/src/misc/Faked.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ECE141db.dir/src/misc/Faked.cpp.o -MF CMakeFiles/ECE141db.dir/src/misc/Faked.cpp.o.d -o CMakeFiles/ECE141db.dir/src/misc/Faked.cpp.o -c /tmp/SP24-ECE141B-Database-Team12/src/misc/Faked.cpp

CMakeFiles/ECE141db.dir/src/misc/Faked.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ECE141db.dir/src/misc/Faked.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/SP24-ECE141B-Database-Team12/src/misc/Faked.cpp > CMakeFiles/ECE141db.dir/src/misc/Faked.cpp.i

CMakeFiles/ECE141db.dir/src/misc/Faked.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ECE141db.dir/src/misc/Faked.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/SP24-ECE141B-Database-Team12/src/misc/Faked.cpp -o CMakeFiles/ECE141db.dir/src/misc/Faked.cpp.s

CMakeFiles/ECE141db.dir/src/misc/Filters.cpp.o: CMakeFiles/ECE141db.dir/flags.make
CMakeFiles/ECE141db.dir/src/misc/Filters.cpp.o: ../src/misc/Filters.cpp
CMakeFiles/ECE141db.dir/src/misc/Filters.cpp.o: CMakeFiles/ECE141db.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/SP24-ECE141B-Database-Team12/cmake-build-docker-arm/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/ECE141db.dir/src/misc/Filters.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ECE141db.dir/src/misc/Filters.cpp.o -MF CMakeFiles/ECE141db.dir/src/misc/Filters.cpp.o.d -o CMakeFiles/ECE141db.dir/src/misc/Filters.cpp.o -c /tmp/SP24-ECE141B-Database-Team12/src/misc/Filters.cpp

CMakeFiles/ECE141db.dir/src/misc/Filters.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ECE141db.dir/src/misc/Filters.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/SP24-ECE141B-Database-Team12/src/misc/Filters.cpp > CMakeFiles/ECE141db.dir/src/misc/Filters.cpp.i

CMakeFiles/ECE141db.dir/src/misc/Filters.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ECE141db.dir/src/misc/Filters.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/SP24-ECE141B-Database-Team12/src/misc/Filters.cpp -o CMakeFiles/ECE141db.dir/src/misc/Filters.cpp.s

CMakeFiles/ECE141db.dir/src/misc/Join.cpp.o: CMakeFiles/ECE141db.dir/flags.make
CMakeFiles/ECE141db.dir/src/misc/Join.cpp.o: ../src/misc/Join.cpp
CMakeFiles/ECE141db.dir/src/misc/Join.cpp.o: CMakeFiles/ECE141db.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/SP24-ECE141B-Database-Team12/cmake-build-docker-arm/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/ECE141db.dir/src/misc/Join.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ECE141db.dir/src/misc/Join.cpp.o -MF CMakeFiles/ECE141db.dir/src/misc/Join.cpp.o.d -o CMakeFiles/ECE141db.dir/src/misc/Join.cpp.o -c /tmp/SP24-ECE141B-Database-Team12/src/misc/Join.cpp

CMakeFiles/ECE141db.dir/src/misc/Join.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ECE141db.dir/src/misc/Join.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/SP24-ECE141B-Database-Team12/src/misc/Join.cpp > CMakeFiles/ECE141db.dir/src/misc/Join.cpp.i

CMakeFiles/ECE141db.dir/src/misc/Join.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ECE141db.dir/src/misc/Join.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/SP24-ECE141B-Database-Team12/src/misc/Join.cpp -o CMakeFiles/ECE141db.dir/src/misc/Join.cpp.s

CMakeFiles/ECE141db.dir/src/misc/Logger.cpp.o: CMakeFiles/ECE141db.dir/flags.make
CMakeFiles/ECE141db.dir/src/misc/Logger.cpp.o: ../src/misc/Logger.cpp
CMakeFiles/ECE141db.dir/src/misc/Logger.cpp.o: CMakeFiles/ECE141db.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/SP24-ECE141B-Database-Team12/cmake-build-docker-arm/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/ECE141db.dir/src/misc/Logger.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ECE141db.dir/src/misc/Logger.cpp.o -MF CMakeFiles/ECE141db.dir/src/misc/Logger.cpp.o.d -o CMakeFiles/ECE141db.dir/src/misc/Logger.cpp.o -c /tmp/SP24-ECE141B-Database-Team12/src/misc/Logger.cpp

CMakeFiles/ECE141db.dir/src/misc/Logger.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ECE141db.dir/src/misc/Logger.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/SP24-ECE141B-Database-Team12/src/misc/Logger.cpp > CMakeFiles/ECE141db.dir/src/misc/Logger.cpp.i

CMakeFiles/ECE141db.dir/src/misc/Logger.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ECE141db.dir/src/misc/Logger.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/SP24-ECE141B-Database-Team12/src/misc/Logger.cpp -o CMakeFiles/ECE141db.dir/src/misc/Logger.cpp.s

CMakeFiles/ECE141db.dir/src/misc/ParseHelper.cpp.o: CMakeFiles/ECE141db.dir/flags.make
CMakeFiles/ECE141db.dir/src/misc/ParseHelper.cpp.o: ../src/misc/ParseHelper.cpp
CMakeFiles/ECE141db.dir/src/misc/ParseHelper.cpp.o: CMakeFiles/ECE141db.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/SP24-ECE141B-Database-Team12/cmake-build-docker-arm/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/ECE141db.dir/src/misc/ParseHelper.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ECE141db.dir/src/misc/ParseHelper.cpp.o -MF CMakeFiles/ECE141db.dir/src/misc/ParseHelper.cpp.o.d -o CMakeFiles/ECE141db.dir/src/misc/ParseHelper.cpp.o -c /tmp/SP24-ECE141B-Database-Team12/src/misc/ParseHelper.cpp

CMakeFiles/ECE141db.dir/src/misc/ParseHelper.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ECE141db.dir/src/misc/ParseHelper.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/SP24-ECE141B-Database-Team12/src/misc/ParseHelper.cpp > CMakeFiles/ECE141db.dir/src/misc/ParseHelper.cpp.i

CMakeFiles/ECE141db.dir/src/misc/ParseHelper.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ECE141db.dir/src/misc/ParseHelper.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/SP24-ECE141B-Database-Team12/src/misc/ParseHelper.cpp -o CMakeFiles/ECE141db.dir/src/misc/ParseHelper.cpp.s

CMakeFiles/ECE141db.dir/src/parsing/TokenParser.cpp.o: CMakeFiles/ECE141db.dir/flags.make
CMakeFiles/ECE141db.dir/src/parsing/TokenParser.cpp.o: ../src/parsing/TokenParser.cpp
CMakeFiles/ECE141db.dir/src/parsing/TokenParser.cpp.o: CMakeFiles/ECE141db.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/SP24-ECE141B-Database-Team12/cmake-build-docker-arm/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/ECE141db.dir/src/parsing/TokenParser.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ECE141db.dir/src/parsing/TokenParser.cpp.o -MF CMakeFiles/ECE141db.dir/src/parsing/TokenParser.cpp.o.d -o CMakeFiles/ECE141db.dir/src/parsing/TokenParser.cpp.o -c /tmp/SP24-ECE141B-Database-Team12/src/parsing/TokenParser.cpp

CMakeFiles/ECE141db.dir/src/parsing/TokenParser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ECE141db.dir/src/parsing/TokenParser.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/SP24-ECE141B-Database-Team12/src/parsing/TokenParser.cpp > CMakeFiles/ECE141db.dir/src/parsing/TokenParser.cpp.i

CMakeFiles/ECE141db.dir/src/parsing/TokenParser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ECE141db.dir/src/parsing/TokenParser.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/SP24-ECE141B-Database-Team12/src/parsing/TokenParser.cpp -o CMakeFiles/ECE141db.dir/src/parsing/TokenParser.cpp.s

CMakeFiles/ECE141db.dir/src/storage/BlockIO.cpp.o: CMakeFiles/ECE141db.dir/flags.make
CMakeFiles/ECE141db.dir/src/storage/BlockIO.cpp.o: ../src/storage/BlockIO.cpp
CMakeFiles/ECE141db.dir/src/storage/BlockIO.cpp.o: CMakeFiles/ECE141db.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/SP24-ECE141B-Database-Team12/cmake-build-docker-arm/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building CXX object CMakeFiles/ECE141db.dir/src/storage/BlockIO.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ECE141db.dir/src/storage/BlockIO.cpp.o -MF CMakeFiles/ECE141db.dir/src/storage/BlockIO.cpp.o.d -o CMakeFiles/ECE141db.dir/src/storage/BlockIO.cpp.o -c /tmp/SP24-ECE141B-Database-Team12/src/storage/BlockIO.cpp

CMakeFiles/ECE141db.dir/src/storage/BlockIO.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ECE141db.dir/src/storage/BlockIO.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/SP24-ECE141B-Database-Team12/src/storage/BlockIO.cpp > CMakeFiles/ECE141db.dir/src/storage/BlockIO.cpp.i

CMakeFiles/ECE141db.dir/src/storage/BlockIO.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ECE141db.dir/src/storage/BlockIO.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/SP24-ECE141B-Database-Team12/src/storage/BlockIO.cpp -o CMakeFiles/ECE141db.dir/src/storage/BlockIO.cpp.s

CMakeFiles/ECE141db.dir/src/storage/Storage.cpp.o: CMakeFiles/ECE141db.dir/flags.make
CMakeFiles/ECE141db.dir/src/storage/Storage.cpp.o: ../src/storage/Storage.cpp
CMakeFiles/ECE141db.dir/src/storage/Storage.cpp.o: CMakeFiles/ECE141db.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/SP24-ECE141B-Database-Team12/cmake-build-docker-arm/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Building CXX object CMakeFiles/ECE141db.dir/src/storage/Storage.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ECE141db.dir/src/storage/Storage.cpp.o -MF CMakeFiles/ECE141db.dir/src/storage/Storage.cpp.o.d -o CMakeFiles/ECE141db.dir/src/storage/Storage.cpp.o -c /tmp/SP24-ECE141B-Database-Team12/src/storage/Storage.cpp

CMakeFiles/ECE141db.dir/src/storage/Storage.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ECE141db.dir/src/storage/Storage.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/SP24-ECE141B-Database-Team12/src/storage/Storage.cpp > CMakeFiles/ECE141db.dir/src/storage/Storage.cpp.i

CMakeFiles/ECE141db.dir/src/storage/Storage.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ECE141db.dir/src/storage/Storage.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/SP24-ECE141B-Database-Team12/src/storage/Storage.cpp -o CMakeFiles/ECE141db.dir/src/storage/Storage.cpp.s

CMakeFiles/ECE141db.dir/src/tokenizer/Tokenizer.cpp.o: CMakeFiles/ECE141db.dir/flags.make
CMakeFiles/ECE141db.dir/src/tokenizer/Tokenizer.cpp.o: ../src/tokenizer/Tokenizer.cpp
CMakeFiles/ECE141db.dir/src/tokenizer/Tokenizer.cpp.o: CMakeFiles/ECE141db.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/SP24-ECE141B-Database-Team12/cmake-build-docker-arm/CMakeFiles --progress-num=$(CMAKE_PROGRESS_16) "Building CXX object CMakeFiles/ECE141db.dir/src/tokenizer/Tokenizer.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ECE141db.dir/src/tokenizer/Tokenizer.cpp.o -MF CMakeFiles/ECE141db.dir/src/tokenizer/Tokenizer.cpp.o.d -o CMakeFiles/ECE141db.dir/src/tokenizer/Tokenizer.cpp.o -c /tmp/SP24-ECE141B-Database-Team12/src/tokenizer/Tokenizer.cpp

CMakeFiles/ECE141db.dir/src/tokenizer/Tokenizer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ECE141db.dir/src/tokenizer/Tokenizer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/SP24-ECE141B-Database-Team12/src/tokenizer/Tokenizer.cpp > CMakeFiles/ECE141db.dir/src/tokenizer/Tokenizer.cpp.i

CMakeFiles/ECE141db.dir/src/tokenizer/Tokenizer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ECE141db.dir/src/tokenizer/Tokenizer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/SP24-ECE141B-Database-Team12/src/tokenizer/Tokenizer.cpp -o CMakeFiles/ECE141db.dir/src/tokenizer/Tokenizer.cpp.s

# Object files for target ECE141db
ECE141db_OBJECTS = \
"CMakeFiles/ECE141db.dir/src/controller/AppController.cpp.o" \
"CMakeFiles/ECE141db.dir/src/database/Database.cpp.o" \
"CMakeFiles/ECE141db.dir/src/database/RowCache.cpp.o" \
"CMakeFiles/ECE141db.dir/src/database/Schema.cpp.o" \
"CMakeFiles/ECE141db.dir/src/database/Table.cpp.o" \
"CMakeFiles/ECE141db.dir/src/database/TableRow.cpp.o" \
"CMakeFiles/ECE141db.dir/src/main.cpp.o" \
"CMakeFiles/ECE141db.dir/src/misc/Faked.cpp.o" \
"CMakeFiles/ECE141db.dir/src/misc/Filters.cpp.o" \
"CMakeFiles/ECE141db.dir/src/misc/Join.cpp.o" \
"CMakeFiles/ECE141db.dir/src/misc/Logger.cpp.o" \
"CMakeFiles/ECE141db.dir/src/misc/ParseHelper.cpp.o" \
"CMakeFiles/ECE141db.dir/src/parsing/TokenParser.cpp.o" \
"CMakeFiles/ECE141db.dir/src/storage/BlockIO.cpp.o" \
"CMakeFiles/ECE141db.dir/src/storage/Storage.cpp.o" \
"CMakeFiles/ECE141db.dir/src/tokenizer/Tokenizer.cpp.o"

# External object files for target ECE141db
ECE141db_EXTERNAL_OBJECTS =

ECE141db: CMakeFiles/ECE141db.dir/src/controller/AppController.cpp.o
ECE141db: CMakeFiles/ECE141db.dir/src/database/Database.cpp.o
ECE141db: CMakeFiles/ECE141db.dir/src/database/RowCache.cpp.o
ECE141db: CMakeFiles/ECE141db.dir/src/database/Schema.cpp.o
ECE141db: CMakeFiles/ECE141db.dir/src/database/Table.cpp.o
ECE141db: CMakeFiles/ECE141db.dir/src/database/TableRow.cpp.o
ECE141db: CMakeFiles/ECE141db.dir/src/main.cpp.o
ECE141db: CMakeFiles/ECE141db.dir/src/misc/Faked.cpp.o
ECE141db: CMakeFiles/ECE141db.dir/src/misc/Filters.cpp.o
ECE141db: CMakeFiles/ECE141db.dir/src/misc/Join.cpp.o
ECE141db: CMakeFiles/ECE141db.dir/src/misc/Logger.cpp.o
ECE141db: CMakeFiles/ECE141db.dir/src/misc/ParseHelper.cpp.o
ECE141db: CMakeFiles/ECE141db.dir/src/parsing/TokenParser.cpp.o
ECE141db: CMakeFiles/ECE141db.dir/src/storage/BlockIO.cpp.o
ECE141db: CMakeFiles/ECE141db.dir/src/storage/Storage.cpp.o
ECE141db: CMakeFiles/ECE141db.dir/src/tokenizer/Tokenizer.cpp.o
ECE141db: CMakeFiles/ECE141db.dir/build.make
ECE141db: CMakeFiles/ECE141db.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/tmp/SP24-ECE141B-Database-Team12/cmake-build-docker-arm/CMakeFiles --progress-num=$(CMAKE_PROGRESS_17) "Linking CXX executable ECE141db"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ECE141db.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ECE141db.dir/build: ECE141db
.PHONY : CMakeFiles/ECE141db.dir/build

CMakeFiles/ECE141db.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ECE141db.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ECE141db.dir/clean

CMakeFiles/ECE141db.dir/depend:
	cd /tmp/SP24-ECE141B-Database-Team12/cmake-build-docker-arm && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /tmp/SP24-ECE141B-Database-Team12 /tmp/SP24-ECE141B-Database-Team12 /tmp/SP24-ECE141B-Database-Team12/cmake-build-docker-arm /tmp/SP24-ECE141B-Database-Team12/cmake-build-docker-arm /tmp/SP24-ECE141B-Database-Team12/cmake-build-docker-arm/CMakeFiles/ECE141db.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ECE141db.dir/depend


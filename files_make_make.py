import def_make_make

make_release_str = ""
make_debug_str = ""

flags = """
EXE = mandelbrot
EXE_ARGS =
EXE_LOG = 2>log
"""

include = "\nINCLUDE = -Isrc/include/ -Iglm/glm/     -Iglad_fork/include/\n"

cpp_files = "src/source/main.cpp src/source/mandelbrot.cpp src/source/opengl_log.cpp src/source/shader.cpp glad_fork/src/glad.c"
cpp_objects = "src/build/main.o src/build/mandelbrot.o src/build/opengl_log.o src/build/shader.o src/build/glad.o"

debug_target = """debug: all
all: {}
\t@$(CXX) {} -o $(EXE) $(LINK_FLAGS_DEBUG)\n""".format(cpp_objects, cpp_objects)

release_target = """release: all
all: {}
\t@$(CXX) {} -o $(EXE) $(LINK_FLAGS_RELEASE)""".format(cpp_objects, cpp_objects)
# ------------------------------------------------------------------------------

# objects for debug: -----------------------------------------------------------

cpp_obj_debug = ''
cpp_sources_splited = cpp_files.split()
cpp_objects_splited = cpp_objects.split()

cpp_ns_files = len(cpp_sources_splited)
cpp_no_files = len(cpp_objects_splited)

if (cpp_no_files != cpp_ns_files):
    print("fuck")
    exit(0)

for i in range(cpp_no_files):
    cpp_obj_debug += "{}: {}\n\t@$(CXX) -c $^ $(DEBUG_FLAGS) $(INCLUDE) -o $@\n\n".format(cpp_objects_splited[i], cpp_sources_splited[i])

# objects for release: ---------------------------------------------------------

cpp_obj_release = ''
cpp_sources_splited = cpp_files.split()
cpp_objects_splited = cpp_objects.split()

cpp_ns_files = len(cpp_sources_splited)
cpp_no_files = len(cpp_objects_splited)

if (cpp_no_files != cpp_ns_files):
    print("Warning number of .o files doenst match number of .c files")
    exit(0)

for i in range(cpp_no_files):
    cpp_obj_release += "{}: {}\n\t@$(CXX) -c $^ $(RELEASE_FLAGS) $(INCLUDE) -o $@\n\n".format(cpp_objects_splited[i], cpp_sources_splited[i])

make_release_str += flags + def_make_make.def_frags + include + def_make_make.sep + release_target + def_make_make.sep + cpp_obj_release
make_debug_str += flags + def_make_make.def_frags + include + def_make_make.sep + debug_target + def_make_make.sep + cpp_obj_debug

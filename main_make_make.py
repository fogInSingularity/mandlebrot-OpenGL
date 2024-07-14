# sep_str = "# ------------------------------------------------------------------------------\n"

clean_cmd = "rm -rf src/build"

main_make_str = """
all:
\t@echo 'available options> release debug clean'

release:
\t@echo 'Compiler $(CXX)'
\tmkdir src/build
\tmake -f release.mk CXX=$(CXX) -j8

debug:
\t@echo 'Compiler $(CXX)'
\tmkdir src/build
\tmake -f debug.mk CXX=$(CXX) -j8
clean:
\t{}
""".format(clean_cmd)
# FIXME

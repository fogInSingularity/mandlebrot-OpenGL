add_library(mandelbrot-defaults INTERFACE)

target_compile_features(mandelbrot-defaults
    INTERFACE
        cxx_std_20
)

target_compile_options(mandelbrot-defaults
    INTERFACE
        -fdiagnostics-color=always

        -Wall
        -Wextra
        -fstack-protector-strong
        -fcheck-new
        -fstrict-overflow
        $<$<CONFIG:Debug>:
            -Og
            -g3
            -ggdb
            -fsanitize=address,leak,undefined
        >

        $<$<CONFIG:Release>:
            -O2
            -march=native
            -flto
            -DNDEBUG
        >
)

target_link_options(mandelbrot-defaults
    INTERFACE
        $<$<CONFIG:Debug>:
            -Og
            -g3
            -ggdb
            -fsanitize=address,leak,undefined
        >

        $<$<CONFIG:Release>:
            -O2
            -march=native
            -flto
        >
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # to generate compile_commands.json

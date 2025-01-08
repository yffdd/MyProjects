if not exist build (
    mkdir build
)

@REM cmake -B build -S . -G "MinGW Makefiles"

cmake -B build -S .

cmake --build build

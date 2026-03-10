Remove-Item build -Recurse -Force -ErrorAction Ignore
cmake -S . -B build -G "Ninja"
cmake --build build --parallel 1
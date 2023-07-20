bash debug_compile.sh
valgrind --tool=callgrind ./main < test/open_49.txt > output.txt
kcachegrind

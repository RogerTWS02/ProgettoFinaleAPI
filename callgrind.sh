bash debug_compile.sh
valgrind --tool=callgrind ./main < test/open_111.txt > output.txt
kcachegrind

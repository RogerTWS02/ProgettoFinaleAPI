bash debug_compile.sh
valgrind --tool=massif --stacks=yes --massif-out-file=massif.out.txt ./main < LALA > LALA
massif-visualizer massif.out.txt
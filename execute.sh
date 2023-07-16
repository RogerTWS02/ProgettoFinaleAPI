bash compile.sh
./main < test/open_1.txt > output.txt
diff test/open_1.output.txt output.txt

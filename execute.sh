bash compile.sh
./main < test/open_110.txt > output.txt
diff test/open_110.output.txt output.txt

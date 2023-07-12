bash compile.sh
./main < test/open_11.txt > output.txt
diff test/open_11.output.txt output.txt

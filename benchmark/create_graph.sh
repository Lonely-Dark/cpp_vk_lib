for file in *.txt; do
    echo "file: ${file%.txt}"
    gnuplot -e "filename='${file%.txt}.txt';name='${file%.txt}.pdf';" ../benchmark/benchmark.gnuplot
done

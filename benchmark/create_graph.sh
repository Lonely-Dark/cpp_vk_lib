# gnuplot -e "filename='$1.txt';name='$1.pdf';" ../benchmark/benchmark.gnuplot

echo "./benchmark/convert_ascii"
./benchmark/convert_ascii
echo "./benchmark/convert_utf8"
./benchmark/convert_utf8
echo "./benchmark/join"
./benchmark/join
echo "./benchmark/lazy_split"
./benchmark/lazy_split
echo "./benchmark/split"
./benchmark/split
echo "./benchmark/message_new"
./benchmark/message_new
echo "./benchmark/wall_post_new"
./benchmark/wall_post_new
echo "./benchmark/wall_reply_new"
./benchmark/wall_reply_new

for file in *.txt; do
    echo "file: ${file%.txt}"
    gnuplot -e "filename='${file%.txt}.txt';name='${file%.txt}.pdf';" ../benchmark/benchmark.gnuplot
done

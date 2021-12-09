set term pdfcairo fontscale 0.35 noenhanced
set output name
set style fill solid
set ylabel "processing speed in seconds (S)"
set xlabel "events count"

# Line style for grid
set style line 81 lt 0  # dashed
set style line 81 lt rgb "#808080"  # grey

plot filename using 2:1 with lines notitle

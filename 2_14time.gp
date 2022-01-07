set terminal eps color enhanced
set output "2_n_14_time.eps"
set xlabel "number of edges"
set ylabel "time"
set key 
plot "kadai1_n_14.txt" using 1:2 title "{path all}" with lines  linewidth 5 
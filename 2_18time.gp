set terminal eps color enhanced
set output "2_n_18_time.eps"
set xlabel "number of edges"
set ylabel "time"
set key 
plot "kadai1_n_18.txt" using 1:2 title "{path all}" with lines  linewidth 5 
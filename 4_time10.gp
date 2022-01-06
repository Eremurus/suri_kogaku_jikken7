set terminal eps color enhanced
set output "4_n_10_time.eps"
set xlabel "number of edges"
set ylabel "time"
set key 
plot "kadai1_n_10.txt" using 1:2 title "{path all}" with lines  linewidth 5 ,\
"kadai2_n_10.txt" using 1:2 title "{branch and bound}" with lines  linewidth 5 

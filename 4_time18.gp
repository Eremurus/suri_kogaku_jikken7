set terminal eps color enhanced
set output "4_n_18_time.eps"
set xlabel "number of edges"
set ylabel "time"
set key 
plot "kadai1_n_18.txt" using 1:2 title "{path all}" with lines  linewidth 5 ,\
"kadai2_n_18.txt" using 1:2 title "{branch and bound}" with lines  linewidth 5 

set terminal eps color enhanced
set output "2_n_14_nodes.eps"
set xlabel "number of edges"
set ylabel "number of searched nodes"
set key 
plot "kadai1_n_14.txt" using 1:3 title "{path all}" with lines  linewidth 5 

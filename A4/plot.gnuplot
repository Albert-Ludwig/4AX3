set terminal pngcairo size 1800,1200 enhanced font "Arial,22"
set output "landing.png"
set grid
set border lw 1.5
set key top right
set multiplot layout 2,1 title "Controlled lander trajectory" font "Arial,30"

set xlabel "Time (s)"
set ylabel "Height (m)"
plot "landing_data.dat" using 1:2 with lines lw 3 lc rgb "#1769aa" title "True height", \
     "" using 1:4 with lines lw 2 dt 2 lc rgb "#e27d19" title "Estimated height", \
     0 with lines lw 1 dt 3 lc rgb "#555555" title "Ground"

set xlabel "Time (s)"
set ylabel "Vertical velocity (m/s)"
unset key
plot "landing_data.dat" using 1:3 with lines lw 3 lc rgb "#1769aa" title "True velocity", \
     "" using 1:5 with lines lw 2 dt 2 lc rgb "#e27d19" title "Estimated velocity", \
     0 with lines lw 1 dt 3 lc rgb "#555555" notitle

unset multiplot
set output

set terminal pngcairo size 1800,900 enhanced font "Arial,22"
set output "gravity_estimate.png"
set title "Gravity estimate"
set xlabel "Time (s)"
set ylabel "Gravity (m/s^2)"
set key bottom right
plot "landing_data.dat" using 1:6 with lines lw 3 lc rgb "#1769aa" title "Estimated gravity", \
     9.81 with lines lw 2 dt 2 lc rgb "#e27d19" title "True gravity"
set output

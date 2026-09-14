set terminal pdfcairo enhanced color font "Helvetica,11" size 7in,5in background rgb "white"
set grid
set key top right

set output "A1_Q2_trajectory.pdf"
set title "Ball Trajectory: Continuous and Discrete Models"
set xlabel "Horizontal position, x (m)"
set ylabel "Vertical position, y (m)"
set yrange [0:*]
plot "A1_Q2_data.dat" using 2:4 with lines linewidth 2 title "Continuous (RK4)", \
     "A1_Q2_data.dat" using 6:8 every 10 with points pointtype 7 pointsize 0.35 \
     title "Discrete (Forward Euler)"

set output "A1_Q2_position.pdf"
set title "Position versus Time"
set xlabel "Time (s)"
set ylabel "Position (m)"
set autoscale y
plot "A1_Q2_data.dat" using 1:2 with lines linewidth 2 title "x (RK4)", \
     "A1_Q2_data.dat" using 1:4 with lines linewidth 2 title "y (RK4)", \
     "A1_Q2_data.dat" using 1:6 every 10 with points pointtype 5 pointsize 0.3 \
     title "x (discrete)", \
     "A1_Q2_data.dat" using 1:8 every 10 with points pointtype 7 pointsize 0.3 \
     title "y (discrete)"

set output "A1_Q2_velocity.pdf"
set title "Velocity versus Time"
set xlabel "Time (s)"
set ylabel "Velocity (m/s)"
plot "A1_Q2_data.dat" using 1:3 with lines linewidth 2 title "vx (RK4)", \
     "A1_Q2_data.dat" using 1:5 with lines linewidth 2 title "vy (RK4)", \
     "A1_Q2_data.dat" using 1:7 every 10 with points pointtype 5 pointsize 0.3 \
     title "vx (discrete)", \
     "A1_Q2_data.dat" using 1:9 every 10 with points pointtype 7 pointsize 0.3 \
     title "vy (discrete)"

unset output

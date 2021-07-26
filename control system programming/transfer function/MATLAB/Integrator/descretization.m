A = [0 1; -1/4 0];
B = [0; 0];
C = [1 0];
D = 0;

ds1 = 1/5;
ds2 = 1/50;
ds3 = 1/100;

y = ss(A,B,C,D);
yd1 = c2d(y,ds1);
yd2 = c2d(y,ds2);
yd3 = c2d(y,ds3);
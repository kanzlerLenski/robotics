% jacobian is used to find the velocities of an end-effector.
function [J] = jacob(q)

% symbolic computation.
syms q1 q2 q3 q4 q5 q6

a = [0 1 0 0 0 0];
alpha = [pi/2 0 pi/2 -pi/2 pi/2 0];
d = [1 0 0 1 0 1];
teta = [0 0 pi/2 0 0 0];

q = q + teta;

% transformation matrices from one frame to another.
T00 = eye(4);
T01 = ht(a(1), alpha(1), d(1), q1);
T12 = ht(a(2), alpha(2), d(2), q2);
T23 = ht(a(3), alpha(3), d(3), q3);
T34 = ht(a(4), alpha(4), d(4), q4);
T45 = ht(a(5), alpha(5), d(5), q5);
T56 = ht(a(6), alpha(6), d(6), q6);

T02 = T01*T12;
T03 = T02*T23;
T04 = T03*T34;
T05 = T04*T45;
T06 = T05*T56;

% angular part of Jacobian.
z0 = T00(1:3, 3);
z1 = T01(1:3, 3);
z2 = T02(1:3, 3);
z3 = T03(1:3, 3);
z4 = T04(1:3, 3);
z5 = T05(1:3, 3);

% jacobian elements (partial derivatives of q).
j1 = [diff(T06(1:3, 4), q1); z0];
j2 = [diff(T06(1:3, 4), q2); z1];
j3 = [diff(T06(1:3, 4), q3); z2];
j4 = [diff(T06(1:3, 4), q4); z3];
j5 = [diff(T06(1:3, 4), q5); z4];
j6 = [diff(T06(1:3, 4), q6); z5];

J = [j1 j2 j3 j4 j5 j6];

% substitute numbers.
J = double(subs(J, [q1, q2, q3, q4, q5, q6], q));

end


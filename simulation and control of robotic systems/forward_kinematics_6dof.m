% forward kinematics: compute the position of the end-effector from 
% specified values for the joint parameters.
function [xi] = forward_kinematics_6dof(q)

% DH parameters.
a = [0 1 0 0 0 0];
alpha = [pi/2 0 pi/2 -pi/2 pi/2 0];
d = [1 0 0 1 0 1];
teta = [0 0 pi/2 0 0 0];

% current joints position.
q = q + teta;

% transformation matrices from one frame to another.
T01 = ht(a(1), alpha(1), d(1), q(1));
T12 = ht(a(2), alpha(2), d(2), q(2));
T23 = ht(a(3), alpha(3), d(3), q(3));
T34 = ht(a(4), alpha(4), d(4), q(4));
T45 = ht(a(5), alpha(5), d(5), q(5));
T56 = ht(a(6), alpha(6), d(6), q(6));

T02 = T01*T12;
T03 = T02*T23;
T04 = T03*T34;
T05 = T04*T45;
T06 = T05*T56;  % final transformation matrix from the base to the end-effector coordinates.

x = T06(1,4);
y = T06(2,4);
z = T06(3,4);
R06 = T06(1:3, 1:3);

% euler angles computation.
[phi, theta, psi] = euler_angles(R06);

xi = [x y z phi theta psi];



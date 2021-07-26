% inverse kinematics: compute the variable joint parameters needed to place 
% the end-effector in a given position and orientation relative to the 
% start of the chain.
function [q] = inverse_kinematics_6dof(xi)

x = xi(1);
y = xi(2);
z = xi(3);
phi = xi(4);
theta = xi(5);
psi = xi(6);

% DH parameters.
a = [0 1 0 0 0 0];
alpha = [pi/2 0 pi/2 -pi/2 pi/2 0];
d = [1 0 0 1 0 1];
teta = [0 0 pi/2 0 0 0];

% first compute position coordinates.

p06 = [x; y; z]; % position of the end-effector.

% rotation.
R06 = [cos(phi) -sin(phi) 0;
    sin(phi) cos(phi) 0;
    0 0 1] * ... % rotation around z-axes.
    [cos(theta) 0 sin(theta); 
    0 1 0; 
    -sin(theta) 0 cos(theta)] * ... % rotation around y-axes.
    [cos(psi) -sin(psi) 0;
    sin(psi) cos(psi) 0;
    0 0 1]; % rotation around z-axes.

% kinematic decoupling approach applied to standard 6-DOF robot with
% spherical wrist (a kinematic scheme of the last three rotational
% joints such that their axes of rotation intersect at the same point) 
% is comprised of two subtasks:
% I position IK (to compute q1, q2 and q3);
% II orientation IK (to compute q4, q5 and q6).
% p06 (point) = p04 + p46 (d(6) - direction * R06 - orientation).
p04 = p06 - d(6)*R06*[0; 0; 1];

% the point of intersection at the wrist.
xc = p04(1);
yc = p04(2);
zc = p04(3);

q(1) = atan2(yc, xc);

% compute q(3) first since there is sign uncertainty which influences
% q(2) computation; the computation is geometrically derived.
cosq3 = ((zc-d(1))^2+xc^2+yc^2-a(2)^2-d(4)^2)/(2*a(2)*d(4));

if fix(cosq3) == 1 % rounds each element to the nearest integer toward zero.
    q(3) = 0;
    q(2) = atan2(zc-d(1), sqrt(xc^2+yc^2));
elseif fix(cosq3) == -1
    q(3) = pi;
elseif fix(cosq3) < 1
    q(3) = atan2(sqrt(1-cosq3^2), cosq3);
end

q(2) = atan2(zc-d(1), sqrt(xc^2+yc^2))-atan2(d(4)*sin(q(3)), a(2)+d(4)*cos(q(3)));

% now compute orientation coordinates.

% solve fk to find R03.
T01 = ht(a(1), alpha(1), d(1), q(1));
T12 = ht(a(2), alpha(2), d(2), q(2));
T23 = ht(a(3), alpha(3), d(3), q(3) + pi/2); % add pi/2 because of DH parameters of the system.

T02 = T01*T12;
T03 = T02*T23;

R03 = T03(1:3, 1:3);

% find R36.
R36 = inv(R03)*R06;

% find the angles.
[q(4), q(5), q(6)] = euler_angles(R36);


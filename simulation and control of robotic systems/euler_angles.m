function [phi, theta, psi] = euler_angles(R)

% depending on the cos(theta) computation, there are 3 possible options to
% consider.
if abs(R(3,3)) < 1 % abs returns modulus.
    phi = atan2(R(2,3), R(1,3));
    theta = atan2(sqrt(1-R(3,3)^2), R(3,3));
    psi = atan2(R(3,2), -R(3,1));
    
elseif R(3,3) == 1
    phi = 0;
    theta = 0;
    psi = atan2(R(2,1), R(1,1));
    
elseif R(3,3) == -1
    phi = -atan2(-R(1,2), -R(1,1));
    theta = pi;
    psi = 0;

end


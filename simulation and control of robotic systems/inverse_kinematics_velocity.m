function [q_dot] = inverse_kinematics_velocity(q, xi)

J = jacob(q);
J_inv = inv(J);

q_dot = (J_inv*xi')';

end


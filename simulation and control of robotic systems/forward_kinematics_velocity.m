function [xi] = forward_kinematics_velocity(q, q_dot)

% q is joints' positions; q_dot is joints' velocities.
% xi contains linear and angular velocities from the base to the
% end-effector frame.

J = jacob(q);
xi = (J*q_dot')';

end


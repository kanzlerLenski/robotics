% trajectory planning using spline function.

% four data points (опорные точки, через которые нужно пройти).
xi1 = [2 0 0 0 1.3090 3.1416];
xi2 = [2 0 2 0 0.5236 3.1416];
xi3 = [0 2 2 1.5708 0.5236 3.1416];
xi4 = [0 2 0 1.5708 1.3090 3.1416];

% initial velocities.
q1_dot = [0 0 0 0 0 0]; % linear.
q1d_dot = [0 0 0 0 0 0]; % angular.

% final velocities.
q4_dot = [0 0 0 0 0 0];
q4d_dot = [0 0 0 0 0 0];

% absolute time for each data point.
t1 = 0;
t2 = 2;
t3 = 4;
t4 = 6;

% how many steps between the first and the last point.
inc = 600;

% positions.
q1 = inverse_kinematics_6dof(xi1);
q2 = inverse_kinematics_6dof(xi2);
q3 = inverse_kinematics_6dof(xi3);
q4 = inverse_kinematics_6dof(xi4);

% matrix representation of the polinomyal system.
M = [0 0 0 0 1 0 0 0 0 0 0 0 0 0;
    0 0 0 1 0 0 0 0 0 0 0 0 0 0;
    0 0 2 0 0 0 0 0 0 0 0 0 0 0;
    1 1 1 1 1 0 0 0 0 0 0 0 0 0;
    0 0 0 0 0 0 0 0 1 0 0 0 0 0;
    4 3 2 1 0 0 0 -1 0 0 0 0 0 0;
    12 6 2 0 0 0 -2 0 0 0 0 0 0 0;
    0 0 0 0 0 1 1 1 1 0 0 0 0 0;
    0 0 0 0 0 0 0 0 0 0 0 0 0 1;
    0 0 0 0 0 3 2 1 0 0 0 0 -1 0;
    0 0 0 0 0 6 2 0 0 0 0 -2 0 0;
    0 0 0 0 0 0 0 0 0 1 1 1 1 1;
    0 0 0 0 0 0 0 0 0 4 3 2 1 0;
    0 0 0 0 0 0 0 0 0 12 6 2 0 0];

% define the constraints. 
% zero values impose continuity requirments: velocities and accelerations
% at the end of the subtrajectory must be the same in the beginning of the
% next one.
k1 = inv(M)*[q1(1) q1_dot(1) q1d_dot(1) q2(1) q2(1) 0 0 q3(1) q3(1) 0 0 q4(1) q4_dot(1) q4d_dot(1)]';
k2 = inv(M)*[q1(2) q1_dot(2) q1d_dot(2) q2(2) q2(2) 0 0 q3(2) q3(2) 0 0 q4(2) q4_dot(2) q4d_dot(2)]';
k3 = inv(M)*[q1(3) q1_dot(3) q1d_dot(3) q2(3) q2(3) 0 0 q3(3) q3(3) 0 0 q4(3) q4_dot(3) q4d_dot(3)]';
k4 = inv(M)*[q1(4) q1_dot(4) q1d_dot(4) q2(4) q2(4) 0 0 q3(4) q3(4) 0 0 q4(4) q4_dot(4) q4d_dot(4)]';
k5 = inv(M)*[q1(5) q1_dot(5) q1d_dot(5) q2(5) q2(5) 0 0 q3(5) q3(5) 0 0 q4(5) q4_dot(5) q4d_dot(5)]';
k6 = inv(M)*[q1(6) q1_dot(6) q1d_dot(6) q2(6) q2(6) 0 0 q3(6) q3(6) 0 0 q4(6) q4_dot(6) q4d_dot(6)]';

q = zeros(6, inc);
q_dot = zeros(6, inc);
qd_dot = zeros(6, inc);

i = 1;
for t = linspace(t1, t4, inc)
    
    % relative time for each subtrajectory.
    tau1 = (t - t1)/(t2 - t1);
    tau2 = (t - t2)/(t3 - t2);
    tau3 = (t - t3)/(t4 - t3);
    
    if (t >= t1) && (t < t2)
        
        % position.
        q(1, i) = k1(1)*tau1^4 + k1(2)*tau1^3 + k1(3)*tau1^2 + k1(4)*tau1 + k1(5);
        q(2, i) = k2(1)*tau1^4 + k2(2)*tau1^3 + k2(3)*tau1^2 + k2(4)*tau1 + k2(5);
        q(3, i) = k3(1)*tau1^4 + k3(2)*tau1^3 + k3(3)*tau1^2 + k3(4)*tau1 + k3(5);
        q(4, i) = k4(1)*tau1^4 + k4(2)*tau1^3 + k4(3)*tau1^2 + k4(4)*tau1 + k4(5);
        q(5, i) = k5(1)*tau1^4 + k5(2)*tau1^3 + k5(3)*tau1^2 + k5(4)*tau1 + k5(5);
        q(6, i) = k6(1)*tau1^4 + k6(2)*tau1^3 + k6(3)*tau1^2 + k6(4)*tau1 + k6(5);
        
        % velocity.
        q_dot(1, i) = 4*k1(1)*tau1^3 + 3*k1(2)*tau1^2 + 2*k1(3)*tau1 + k1(4);
        q_dot(2, i) = 4*k2(1)*tau1^3 + 3*k2(2)*tau1^2 + 2*k2(3)*tau1 + k2(4);
        q_dot(3, i) = 4*k3(1)*tau1^3 + 3*k3(2)*tau1^2 + 2*k3(3)*tau1 + k3(4);
        q_dot(4, i) = 4*k4(1)*tau1^3 + 3*k4(2)*tau1^2 + 2*k4(3)*tau1 + k4(4);
        q_dot(5, i) = 4*k5(1)*tau1^3 + 3*k5(2)*tau1^2 + 2*k5(3)*tau1 + k5(4);
        q_dot(6, i) = 4*k6(1)*tau1^3 + 3*k6(2)*tau1^2 + 2*k6(3)*tau1 + k6(4);
        
        % acceleration.
        qd_dot(1,i) = 12*k1(1)*tau1^2 + 6*k1(2)*tau1 + 2*k1(3);
        qd_dot(2,i) = 12*k2(1)*tau1^2 + 6*k2(2)*tau1 + 2*k2(3);
        qd_dot(3,i) = 12*k3(1)*tau1^2 + 6*k3(2)*tau1 + 2*k3(3);
        qd_dot(4,i) = 12*k4(1)*tau1^2 + 6*k4(2)*tau1 + 2*k4(3);
        qd_dot(5,i) = 12*k5(1)*tau1^2 + 6*k5(2)*tau1 + 2*k5(3);
        qd_dot(6,i) = 12*k6(1)*tau1^2 + 6*k6(2)*tau1 + 2*k6(3);
        
    elseif (t >= t2) && (t < t3)
        
        q(1, i) = k1(6)*tau2^3 + k1(7)*tau2^2 + k1(8)*tau2 + k1(9);
        q(2, i) = k2(6)*tau2^3 + k2(7)*tau2^2 + k2(8)*tau2 + k2(9);
        q(3, i) = k3(6)*tau2^3 + k3(7)*tau2^2 + k3(8)*tau2 + k3(9);
        q(4, i) = k4(6)*tau2^3 + k4(7)*tau2^2 + k4(8)*tau2 + k4(9);
        q(5, i) = k5(6)*tau2^3 + k5(7)*tau2^2 + k5(8)*tau2 + k5(9);
        q(6, i) = k6(6)*tau2^3 + k6(7)*tau2^2 + k6(8)*tau2 + k6(9);

        q_dot(1, i) = 3*k1(6)*tau2^2 +2*k1(7)*tau2 + k1(8);
        q_dot(2, i) = 3*k2(6)*tau2^2 +2*k2(7)*tau2 + k2(8);
        q_dot(3, i) = 3*k3(6)*tau2^2 +2*k3(7)*tau2 + k3(8);
        q_dot(4, i) = 3*k4(6)*tau2^2 +2*k4(7)*tau2 + k4(8);
        q_dot(5, i) = 3*k5(6)*tau2^2 +2*k5(7)*tau2 + k5(8);
        q_dot(6, i) = 3*k6(6)*tau2^2 +2*k6(7)*tau2 + k6(8);

        qd_dot(1,i) = 6*k1(6)*tau2 + 2*k1(7);
        qd_dot(2,i) = 6*k2(6)*tau2 + 2*k2(7);
        qd_dot(3,i) = 6*k3(6)*tau2 + 2*k3(7);
        qd_dot(4,i) = 6*k4(6)*tau2 + 2*k4(7);
        qd_dot(5,i) = 6*k5(6)*tau2 + 2*k5(7);
        qd_dot(6,i) = 6*k6(6)*tau2 + 2*k6(7);
        
    elseif (t >= t3) && (t <=t4)

        q(1, i) = k1(10)*tau3^4 + k1(11)*tau3^3 + k1(12)*tau3^2 + k1(13)*tau3 + k1(14);
        q(2, i) = k2(10)*tau3^4 + k2(11)*tau3^3 + k2(12)*tau3^2 + k2(13)*tau3 + k2(14);
        q(3, i) = k3(10)*tau3^4 + k3(11)*tau3^3 + k3(12)*tau3^2 + k3(13)*tau3 + k3(14);
        q(4, i) = k4(10)*tau3^4 + k4(11)*tau3^3 + k4(12)*tau3^2 + k4(13)*tau3 + k4(14);
        q(5, i) = k5(10)*tau3^4 + k5(11)*tau3^3 + k5(12)*tau3^2 + k5(13)*tau3 + k5(14);
        q(6, i) = k6(10)*tau3^4 + k6(11)*tau3^3 + k6(12)*tau3^2 + k6(13)*tau3 + k6(14);

        q_dot(1, i) = 4*k1(10)*tau3^3 + 3*k1(11)*tau3^2 +2*k1(12)*tau3 + k1(13);
        q_dot(2, i) = 4*k2(10)*tau3^3 + 3*k2(11)*tau3^2 +2*k2(12)*tau3 + k2(13);
        q_dot(3, i) = 4*k3(10)*tau3^3 + 3*k3(11)*tau3^2 +2*k3(12)*tau3 + k3(13);
        q_dot(4, i) = 4*k4(10)*tau3^3 + 3*k4(11)*tau3^2 +2*k4(12)*tau3 + k4(13);
        q_dot(5, i) = 4*k5(10)*tau3^3 + 3*k5(11)*tau3^2 +2*k5(12)*tau3 + k5(13);
        q_dot(6, i) = 4*k6(10)*tau3^3 + 3*k6(11)*tau3^2 +2*k6(12)*tau3 + k6(13);

        qd_dot(1,i) = 12*k1(10)*tau3^2 + 6*k1(11)*tau3 + 2*k1(12);
        qd_dot(2,i) = 12*k2(10)*tau3^2 + 6*k2(11)*tau3 + 2*k2(12);
        qd_dot(3,i) = 12*k3(10)*tau3^2 + 6*k3(11)*tau3 + 2*k3(12);
        qd_dot(4,i) = 12*k4(10)*tau3^2 + 6*k4(11)*tau3 + 2*k4(12);
        qd_dot(5,i) = 12*k5(10)*tau3^2 + 6*k5(11)*tau3 + 2*k5(12);
        qd_dot(6,i) = 12*k6(10)*tau3^2 + 6*k6(11)*tau3 + 2*k6(12);     
    end
    
    i = i + 1;
    
end

% solve fk.
xi = zeros(6, inc);
for i = 1:inc
    xi(:, i) = forward_kinematics_6dof(q(:,i)');
end

% plot the results.
figure
plot3(xi(1,:), xi(2, :), xi(3, :), 'Color', 'blue', 'Linewidth', 3)
hold on
grid on
plot3(xi1(1), xi1(2), xi1(3), 'Color', 'red', 'Linewidth', 3, 'Marker', 'o')
plot3(xi2(1), xi2(2), xi2(3), 'Color', 'red', 'Linewidth', 3, 'Marker', 'o')
plot3(xi3(1), xi3(2), xi3(3), 'Color', 'red', 'Linewidth', 3, 'Marker', 'o')
plot3(xi4(1), xi4(2), xi4(3), 'Color', 'red', 'Linewidth', 3, 'Marker', 'o')
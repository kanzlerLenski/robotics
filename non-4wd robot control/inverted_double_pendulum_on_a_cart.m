% the double pendulum parameters.
m0 = 1000;
m1 = 100;
m2 = 100;
l1 = 20;
l2 = 10;
g = 9.8;

% intial conditions.
x = 0;
theta1 = deg2rad(20);
theta2 = deg2rad(20);
x_dot = 0;
theta1_dot = 0;
theta2_dot = 0;

% linearized A matrix.
A = [0 0 0 0 ((-m2)*g*m1-m1^2*g)/(m1*m0) 0;
    0 0 0 0 ((-m2)*g*m0-m2*g*m1-m1*g*m0-m1^2*g)/(l1*m1*m0) (m2*g)/(m1*l1);
    0 0 0 0 -m2*g-g*m1 (g*m1+g*m2)/(m1*l2);
    1 0 0 0 0 0;
    0 1 0 0 0 0;
    0 0 1 0 0 0];

% linearized B matrix.
B = [1 -1/l1 0 0 0 0]';

% C matrix.
C = eye(6);

% D matrix.
D = zeros(1, 6)';

% weight matrices for a cost function.
R = 1;
Q = diag([5 50 50 700 700 700]);

% convert to a state-space model.
sys = ss(A, B, C, 0);

% calculate the optimal gain matrix K (the state-feedback law u = -Kx).
K = lqr(sys, Q, R);

% define the new system.
sysnew=ss(A-B*K, B, C, 0);

% define initial conditions.
initial_conditions = [x theta1 theta2 x_dot theta1_dot theta2_dot];

% simulate the system for defined initial conditions.
[y,t,x]=initial(sysnew, initial_conditions, 10);

% plot the results.
for i=1:1:6
    subplot(2,3,i)
    plot(t,y(:,i))
    xlabel('Time')
    
    if i == 1
        title('x')
    elseif i == 2
        title('theta1')
    elseif i == 3
        title('theta2')
    elseif i == 4
        title('x dot')
    elseif i == 5
        title('theta1 dot')
    elseif i == 6
        title('theta2 dot')
    end
    
    if i == 1
        ylabel('Position')
    elseif i == 4
        ylabel('Velocity')
    end
    
    grid
    
end

#!/usr/bin/env python
# coding: utf-8
'''
The program represents a numerical calculation of Lagrangian and Hamiltonian
of the inverted double pendulum on a cart using the formulas studied during
the course. It also calculates and then plots tragectory between
initial and end generalized coordinates (each of them) with respect to given time.
'''

from numpy import *
from matplotlib.pyplot import *

# interactive interface block for gathering parameters and coordinates values
# from a user.

print('Рассматриваемая робототехническая система представляет собой двухзвенный обратный маятник на тележке.')
print('Введите параметры Денавита-Хартенберга для первого звена в формате: [a, alpha, d, theta].')
DH_1 = matrix(input())

print('Введите параметры Денавита-Хартенберга для второго звена в формате: [a, alpha, d, theta].')
DH_2 = matrix(input())

print('Введите параметры Денавита-Хартенберга для третьего звена в формате: [a, alpha, d, theta].')
DH_3 = matrix(input())

DHs = [DH_1, DH_2, DH_3]

print('Введите начальную обобщённую координату q1 в формате: [0, 0, 0].')
q1_start = matrix(input())

print('Введите начальную обобщённую координату q2 в формате: [0, 0, 0].')
q2_start = matrix(input())

print('Введите начальную обобщённую координату q3 в формате: [0, 0, 0].')
q3_start = matrix(input())

q_starts = [q1_start, q2_start, q3_start]

print('Введите конечную обобщённую координату q1 в формате: [0, 0, 0].')
q1_end = matrix(input())

print('Введите конечную обобщённую координату q2 в формате: [0, 0, 0].')
q2_end = matrix(input())

print('Введите конечную обобщённую координату q3 в формате: [0, 0, 0].')
q3_end = matrix(input())

q_ends = [q1_end, q2_end, q3_end]

print('Введите обобщённую скорость для первого звена в формате: 1.')
q_dot1 = float(input())

print('Введите обобщённую скорость для второго звена в формате: 1.')
q_dot2 = float(input())

print('Введите обобщённую скорость для третьего звена в формате: 1.')
q_dot3 = float(input())

q_dots = [q_dot1, q_dot2, q_dot3]

print('Введите массу первого звена в формате: 1.')
m1 = float(input())

print('Введите массу второго звена в формате: 1.')
m2 = float(input())

print('Введите массу третьего звена в формате: 1.')
m3 = float(input())

ms = [m1, m2, m3]

print('Введите длину первого звена в формате: 1.')
l1 = float(input())

print('Введите длину второго звена в формате: 1.')
l2 = float(input())

print('Введите длину третьего звена в формате: 1.')
l3 = float(input())

ls = [l1, l2, l3]

print('Введите момент инерции первого звена в формате: 1.')
I1 = float(input())

print('Введите момент инерции второго звена в формате: 1.')
I2 = float(input())

print('Введите момент инерции третьего звена в формате: 1.')
I3 = float(input())

Is = [I1, I2, I3]

print('Введите положение центра масс первого звена в формате: 1.')
d_c1 = float(input())

print('Введите положение центра масс второго звена в формате: 1.')
d_c2 = float(input())

print('Введите положение центра масс второго звена в формате: 1.')
d_c3 = float(input())

d_cs = [d_c1, d_c2, d_c3]

print('Введите значение вектора гравитации в формате: [0, 0, 0].')
g = matrix(input())

print('Введите время для расчёта траектории в формате: 1.')
t = float(input())

# projection vector

z = matrix([0,0,1]).T

# lists to gather kinetic and potential energy values for each joint.

kinetics = []
potentials = []

# i-1 velocities for the first joint. 

w = 0
v = 0

# get sigma
# sigma describes the nature of the joint;
# in our case the first one (a cart) is prismatic, so sigma is equal to 1,
# and the rest two are revolute, so sigma for them is 0.

def check_motion_type(i):
    
    if i == 0:
        sigma = 1
    else:
        sigma = 0
        
    return sigma

# get rotation matrix
# according to the formula rotation matrices are calculated for each joint
# depending on the set Denavit-Hartenberg parameters.

def calculate_rotation_matrix(DH):

    # a is for the distance between z(i-1) and z(i) along x(i).
    
    a = DH.item(0)

    # alpha defines the angle of rotation around x(i) from z(i-1) to z(i).
    
    alpha = DH.item(1)

    # d is the distance along z(i-1) from x(i-1) to x(i).
    d = DH.item(2)

    # theta defines the angle around z(i-1) from x(i-i) to x(i).
    
    theta = DH.item(3)
    
    R = matrix([[cos(theta), -cos(alpha)*sin(theta), sin(alpha)*sin(theta)],
                  [sin(theta), cos(alpha)*cos(theta), -sin(alpha)*cos(theta)],
                  [0, sin(alpha), cos(alpha)]])

    # radius-vectors between O(i-1) and O(i) in O(i-1)x(i-1)y(i-1)z(i-1) frame.
    
    r_0 = matrix([[a*cos(theta),a*sin(theta),d]]).T
    
    return R, r_0

# get vectors for mass center offset
# this function returns coordinates of mass centers
# defined as the offset from the link end assuming
# that they are located on kinematic axis of each link:
# z for prismatic joint and x for revolute ones.

def calculate_offset(i, l, d_c):
    
    if i == 0:
        r_c_0 = matrix([0,0,-l + d_c]).T
    else:
        r_c_0 = matrix([-l + d_c,0,0]).T
    
    return r_c_0

# angular velocity calculation.

def calculate_angular_velocity(R, q_dot, sigma, z, w_prev):

   
    w = R.T.dot(w_prev + (1-sigma)*(q_dot*z))
    
    return w

# linear velocity calculation.

def calculate_linear_velocity(R, sigma, q_dot, z, w, v_prev, r_0):

    v = R.T.dot(v_prev + sigma*q_dot*z + cross(w, r_0, axis=0))
    
    return v

# center velocity calculation.

def calculate_v_c(v, w, r_c):
    
    v_c = v + cross(w, r_c, axis=0)
    
    return v_c

# kinetic energy calculation.

def calculate_kinetic_energy(m, v_c, w, I):
    
    K = 1/2*m*(v_c.T.dot(v_c)) + 1/2*(w.T.dot(I*w))
    
    return K

# full kinetic energy of the system is calculated
# as the sum of kinetic energy of each joint.

def calculate_full_kinetic_energy(kinetics):
    
    K = sum(kinetics)

    print('Полная кинетическая энергия системы:\n{}\n'.format(K))
    
    return K

# vectors of mass centers are calculated.

def calculate_mass_center(R, r_0, r_c_0):
    
    H = append(append(R, r_0, axis=1), matrix('0 0 0 1'), axis=0)
    r_c_0 = matrix([r_c_0.item(0), r_c_0.item(1), r_c_0.item(2), 1]).T
    
    r_c = H.dot(r_c_0)
    r_c = matrix([r_c.item(0), r_c.item(1), r_c.item(2)])
    
    return r_c

# potential energy calculation.

def calculate_potential_energy(m, g, r_c):
    
    P = -m*g.T.dot(r_c)
    
    return P

# full potential energy of the system is calculated
# as the sum of potential energy of each joint.

def calculate_full_potential_energy(potentials):
    
    P = sum(potentials)
    
    print('Полная потенциальная энергия системы:\n{}\n'.format(P))
    
    return P

# lagrangian is calculated as subtraction of full
# potential energy of the system from its full kinetic energy.

def calculate_lagrangian(K, P):
    
    L = K - P
    
    print('Лагранжиан системы:\n{}\n'.format(L))
    
    return L

# hamiltonian is calculated as a sum of full kinetic
# and potential energy of the system.

def calculate_hamiltonian(K, P):
    
    H = K + P
    
    print('Гамильтониан системы:\n{}\n'.format(H))
    
    return H

# compute trajectory between initial and end position
# of each generalized coordinate for a given time.
# as in the 'jtraj' function from MATLAB robotics-toolbox by Corke
# a 7th order polynomial is used. 

def jtraj(q0, q1, tv, qd0=None, qd1=None):

    # time normalization (from 0 -> 1) if it's given as a scalar value
    
    if isinstance(tv,(int,int32,float,float64)):
        tscal = float(1)
        t = matrix(range(0,round(tv))).T/(tv-1.)

    # and if otherwise.
    
    else:
        tv = arg2array(tv);
        tscal = float(max(tv))
        t = mat(tv).T / tscal
    
    q0 = arg2array(q0)
    q1 = arg2array(q1)

    # given velocity is transformed to vector form.
    
    if qd0 == None:
        qd0 = zeros((shape(q0)))
    else:
        qd0 = arg2array(qd0);
    if qd1 == None:
        qd1 = zeros((shape(q1)))
    else:
        qd1 = arg2array(qd1)
    
    # compute the polynomial coefficients.
    
    A = 6*(q1 - q0) - 3*(qd1 + qd0)*tscal
    B = -15*(q1 - q0) + (8*qd0 + 7*qd1)*tscal
    C = 10*(q1 - q0) - (6*qd0 + 4*qd1)*tscal
    E = zeros(shape(A))
    E[0] = float(qd0*tscal)
    E = matrix(E)
    F = q0

    tt = concatenate((power(t,5),power(t,4),power(t,3),power(t,2),t,ones(shape(t))),1)
    c = vstack((A, B, C, zeros(shape(A)), E, F))

    # matrix of position changing.
    
    qt = tt * c

    # velocity depending on time calculation.
    
    c = vstack((zeros(shape(A)),5*A,4*B,3*C,zeros(shape(A)),E))
    qdt = tt * c / tscal

    # acceleration depending on time calculation.
    
    c = vstack((zeros(shape(A)),zeros(shape(A)),20*A,12*B,6*C,zeros(shape(A))))
    qddt = tt * c / (tscal**2)

    return qt, qdt, t #,qddt

# normalize input
# function for scalar-to-vector transformations.

def arg2array(arg):

    if isinstance(arg, (matrix, ndarray)):
        s = arg.shape;
        if len(s) == 1:
            return array(arg);
        if min(s) == 1:
            return array(arg).flatten();

    elif isinstance(arg, list):
        return array(arg);

    elif isinstance(arg, (int, float, float32, float64)):
        return array([arg]);
        
    raise ValueError;

# plot trajectory from the initial to end generalized coordinate. 

def plot_a_trajectory(q1_start, q1_end, t, q_dot1, q):

    # calculate trajectory.
    
    result = jtraj(q1_start, q1_end, t, q_dot1)

    # position coordinates.
    
    trajectory = result[0]

    # velocity.
    
    velocity = result[1]

    # time.
    
    t = result[2]

    # gathering of x's, y's and z's for plotting along y-axis,
    # while x-axis is time.
    
    if trajectory.shape[0] == 1:
        x = trajectory.item(0)
        y = trajectory.item(1)
        z = trajectory.item(2)
    else:
        trajectory = trajectory.T.tolist()
        x = trajectory[0]
        y = trajectory[1]
        z = trajectory[2]   
       
    lines = [plot(t, x), plot(t, y), plot(t, z)]
    xlabel('Время')
    ylabel('Положение')
    leg = legend(('{}(x)'.format(q), '{}(y)'.format(q), '{}(z)'.format(q)))
    show()

    # the same is for velocity plotting.
    
    if velocity.shape[0] == 1:
        v_x = trajectory.item(0)
        v_y = trajectory.item(1)
        v_z = trajectory.item(2)
    else:
        velocity = velocity.T.tolist()
        v_x = trajectory[0]
        v_y = trajectory[1]
        v_z = trajectory[2]

    lines = [plot(t, v_x), plot(t, v_y), plot(t, v_z)]
    xlabel('Время')
    ylabel('Скорость')
    leg = legend(('{}(v_x)'.format(q), '{}(v_y)'.format(q), '{}(v_z)'.format(q)))
    show()

# process input data
# through interactive interface all the coordinates and parameters are given;
# after that they are grouped according to their joint and
# given to each of the described function for computations.

for i in range(0,3):
        
    sigma = check_motion_type(i)
    mat = calculate_rotation_matrix(DHs[i])
    R = mat[0]
    r_0 = mat[1]
    r_c_0 = calculate_offset(i, ls[i], d_cs[i])
    w_prev = w
    w = calculate_angular_velocity(R, q_dots[i], sigma, z, w_prev)
    v_prev = v
    v = calculate_linear_velocity(R, sigma, q_dots[i], z, w, v_prev, r_0)
    v_c = calculate_v_c(v, w, r_c_0)
    K = calculate_kinetic_energy(ms[i], v_c, w, Is[i])
    kinetics.append(K)
    r_c = calculate_mass_center(R, r_0, r_c_0)
    P = calculate_potential_energy(ms[i], g, r_c)
    potentials.append(P)
    
K_full = calculate_full_kinetic_energy(kinetics)
P_full = calculate_full_potential_energy(potentials)
L = calculate_lagrangian(K_full, P_full)
H = calculate_hamiltonian(K_full, P_full)
plot_a_trajectory(q1_start, q1_end, t, q_dot1, q='q1')
plot_a_trajectory(q2_start, q2_end, t, q_dot2, q='q2')
plot_a_trajectory(q3_start, q3_end, t, q_dot3, q='q3')

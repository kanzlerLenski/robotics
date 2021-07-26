from numpy import *
from matplotlib.pyplot import *

# interactive interface block for gathering parameters and coordinates values
# from a user.

print('Рассматриваемая робототехническая система представляет собой двухзвенный обратный маятник на тележке.')

print('Введите начальные обобщённые координаты через пробел в формате: 0 0 0.')
q_start = list(map(float, input().split()))
    
print('Введите конечные обобщённые координаты через пробел в формате: 0 0 0.')
q_end = list(map(float, input().split()))

print('Введите начальные обобщённые линейные скорости через пробел в формате: 0 0 0.')
qd_start = list(map(float, input().split()))

print('Введите конечные обобщённые линейные скорости через пробел в формате: 0 0 0.')
qd_end = list(map(float, input().split()))

print('Введите начальные обобщённые линейные ускорения через пробел в формате: 0 0 0.')
qd_dot_start = list(map(float, input().split()))

print('Введите конечные обобщённые линейные ускорения через пробел в формате: 0 0 0.')
qd_dot_end = list(map(float, input().split()))

print('Введите длины звеньев через пробел в формате: 0 0 0.')
l = list(map(float, input().split()))

print('Введите массы звеньев через пробел в формате:  0 0 0.')
m = list(map(float, input().split()))

print('Введите моменты инерции звеньев через пробел в формате: 0 0 0.')
I = list(map(float, input().split()))

print('Введите положения центров масс звеньев через пробел в формате: 0 0 0.')
d_c = list(map(float, input().split()))

print('Введите начальное значение внешней силы, приложенной к терминальному звену в формате: 0.')
f_prev = float(input())
f_prev_g = f_prev_c = f_prev_mi = f_prev

print('Введите начальное значение внешнего момента терминального звена в формате: 0.')
tau_prev = float(input())
tau_prev_g = tau_prev_c = tau_prev_mi = tau_prev

print('Введите коэффициент вязкого трения в формате: 0.')
n_eta = float(input())

print('Введите значение вектора гравитации в формате: 0.')
g = float(input())

print('Введите время начала и окончания расчёта траектории через пробел в формате: 0 1.')
time = list(map(float, input().split()))

z = matrix([0, 0, 1]).T

w_prev = w_prev_g = w_prev_c = w_prev_mi = matrix([0, 0, 0]).T
w_dot_prev = w_dot_prev_g = w_dot_prev_c = w_dot_prev_mi = matrix([0, 0, 0]).T
a_prev = a_prev_g = a_prev_c = a_prev_mi = 0


history = {'1':{'inverse dynamics': [], 'gravity':[], 'coriolis':[], 'inertia matrix':[]},
           '2':{'inverse dynamics': [], 'gravity':[], 'coriolis':[], 'inertia matrix':[]},
           '3':{'inverse dynamics': [], 'gravity':[], 'coriolis':[], 'inertia matrix':[]}}

final = {'1':{'inverse dynamics': [], 'gravity':[], 'coriolis':[], 'inertia matrix':[]},
           '2':{'inverse dynamics': [], 'gravity':[], 'coriolis':[], 'inertia matrix':[]},
           '3':{'inverse dynamics': [], 'gravity':[], 'coriolis':[], 'inertia matrix':[]}}

u_list = [[], [], [], []]

def jtraj(q0, q1, qd_0, qd_1, qd_dot0, qd_dot1, t0, t1):

    t = linspace(t0, t1, 10*(round(t1)-round(t0)))

    c = ones(size(t))

    M = matrix([[1, t0, t0**2, t0**3, t0**4, t0**5],
              [0, 1, 2*t0, 3*t0**2, 4*t0**3, 5*t0**4],
              [0, 0, 2, 6*t0, 12*t0**2, 20*t0**3],
             [1, t1, t1**2, t1**3, t1**4, t1**5],
             [0, 1, 2*t1, 3*t1**2, 4*t1**3, 5*t1**5],
               [0, 0, 2, 6*t1, 12*t1**2, 20*t1**3]])

    b = matrix([q0, qd_0, qd_dot0, q1, qd_1, qd_dot1]).T

    a = linalg.inv(M)*b

    qd = a.item(0)*c + a.item(1)*t + a.item(3)*t**2 + a.item(4)*t**3 + a.item(5)*t**5

    vd = a.item(1)*c + 2*a.item(2)*t + 3*a.item(3)*t**2 + 4*a.item(4)*t**3 + 5*a.item(5)*t**5

    ad = 2*a.item(2)*c + 6*a.item(3)*t + 12*a.item(4)*t**2 + 20*a.item(5)*t**3

    return qd, vd, ad, t

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

def calculate_rotation_matrix(i, q, l):

    if i == 0:

        # a is for the distance between z(i-1) and z(i) along x(i).
    
        a = 0

        # alpha defines the angle of rotation around x(i) from z(i-1) to z(i).
    
        alpha = -1.57

        # d is the distance along z(i-1) from x(i-1) to x(i).
        
        d = q

        # theta defines the angle around z(i-1) from x(i-i) to x(i).
    
        theta = 0

    else:
        a = l
        alpha = 0
        d = 0
        theta = q
    
    R = matrix([[cos(theta), -cos(alpha)*sin(theta), sin(alpha)*sin(theta)],
                 [sin(theta), cos(alpha)*cos(theta), -sin(alpha)*cos(theta)],
                 [0, sin(alpha), cos(alpha)]])

    # radius-vectors between O(i-1) and O(i) in O(i-1)x(i-1)y(i-1)z(i-1) frame.
    
    r = matrix([[a*cos(theta),a*sin(theta),d]]).T
    
    return R, r

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

# vectors of mass centers are calculated.

def calculate_mass_center(R, r_0, r_c_0):
    
    H = append(append(R, r_0, axis=1), matrix('0 0 0 1'), axis=0)
    r_c_0 = matrix([r_c_0.item(0), r_c_0.item(1), r_c_0.item(2), 1]).T
    
    r_c = H.dot(r_c_0)
    r_c = matrix([r_c.item(0), r_c.item(1), r_c.item(2)]).T
    
    return r_c

def forward_recursion(R, z, q_dot, qddot, r, r_c, w_prev, w_dot_prev, a_prev):

    w_i = R.T.dot(w_prev + q_dot*z)

    w_dot_i = R.T.dot(w_dot_prev + qddot*z + q_dot*(cross(w_prev, z, axis=0)))

    a_i = R.T.dot(a_prev + cross(w_dot_i, r, axis=0) + cross(w_i, cross(w_i, r, axis=0), axis=0))

    a_ci = a_i + cross(w_dot_i, r_c, axis=0) + cross(w_i, cross(w_i, r_c, axis=0), axis=0)

    return w_i, w_dot_i, a_i, a_ci

def backward_recursion(f, m, g, I, tau, r, r_c, w_i, w_dot_i, a_ci):

    f_i = f + m*(a_ci - g)

    try:
        tau_i = tau - cross(f_i, (r + r_c), axis=0) + cross(f, r_c, axis=0) + I*w_dot_i + cross(w_i, I*w_i, axis=0)
    except Exception:
        tau_i = tau - cross(f_i, (r + r_c), axis=0) + f*r_c + I*w_dot_i + cross(w_i, I*w_i, axis=0)
    return f_i, tau_i

def project(f, tau, z, n_eta, q_dot):

    if sigma == 0:
        u = f.T.dot(z) + n_eta*q_dot

    else:
        u = tau.T.dot(z) + n_eta*q
    return u

def plot_trajectory(x, x_name, t, name):

    lines = [plot(t, final['1'][x]),
             plot(t, final['2'][x]),
             plot(t, final['3'][x])]
    xlabel('Время')
    ylabel(name)
    leg = legend(('{}_1'.format(x_name), '{}_2'.format(x_name), '{}_3'.format(x_name)))
    show()

for i in range(0, 3):
    qd, vd, ad, t = jtraj(q_start[i], q_end[i], qd_start[i], qd_end[i],
                             qd_dot_start[i], qd_dot_end[i], time[0], time[1])
    sigma = check_motion_type(i)

    for i_t in range(0, len(t)):

        R, r = calculate_rotation_matrix(i, qd[i_t], l[i])
        r_c_0 = calculate_offset(i, l[i], d_c[i])
        r_c = calculate_mass_center(R, r, r_c_0)
        
        w_i, w_dot_i, a_i, a_ci = forward_recursion(R=R, z=z, q_dot=vd[i_t],
                                qddot=ad[i_t], r=r, r_c=r_c, w_prev=w_prev,
                                w_dot_prev=w_dot_prev, a_prev=a_prev)
        w_prev = w_i
        w_dot_prev = w_dot_i
        a_prev = a_i
        history[str(i+1)]['inverse dynamics'].append([vd[i_t], r, r_c, w_i, w_dot_i, a_ci])

        w_i_g, w_dot_i_g, a_i_g, a_ci_g = forward_recursion(R=R, z=z, q_dot=0, qddot=0,
                                          r=r, r_c=r_c, w_prev=w_prev_g,
                                          w_dot_prev=w_dot_prev_g, a_prev=a_prev_g)
        w_prev_g = w_i_g
        w_dot_prev_g = w_dot_i_g
        a_prev_g = a_i_g
        history[str(i+1)]['gravity'].append([vd[i_t], r, r_c, w_i_g, w_dot_i_g, a_ci_g])

        w_i_c, w_dot_i_c, a_i_c, a_ci_c = forward_recursion(R=R, z=z, q_dot=vd[i_t], qddot=0,
                                          r=r, r_c=r_c, w_prev=w_prev_c,
                                          w_dot_prev=w_dot_prev_c, a_prev=a_prev_c)
        w_prev_c = w_i_c
        w_dot_prev_c = w_dot_i_c
        a_prev_c = a_i_c
        history[str(i+1)]['coriolis'].append([vd[i_t], r, r_c, w_i_c, w_dot_i_c, a_ci_c])

        w_i_mi, w_dot_i_mi, a_i_mi, a_ci_mi = forward_recursion(R=R, z=z, q_dot=0,
                                              qddot=1, r=r, r_c=r_c, w_prev=w_prev_mi,
                                              w_dot_prev=w_dot_prev_mi, a_prev=a_prev_mi)
        w_prev_mi = w_i_mi
        w_dot_prev_mi = w_dot_i_mi
        a_prev_mi = a_i_mi
        history[str(i+1)]['inertia matrix'].append([vd[i_t], r, r_c, w_i_mi, w_dot_i_mi, a_ci_mi])

for i in range(2, -1, -1):

    for i_t in range(0, len(t)):
    
        f_i, tau_i = backward_recursion(f=f_prev, m=m[i], g=g, I=I[i], tau=tau_prev,
                    r=history[str(i+1)]['inverse dynamics'][i_t][1], 
                    r_c=history[str(i+1)]['inverse dynamics'][i_t][2],
                    w_i=history[str(i+1)]['inverse dynamics'][i_t][3],
                    w_dot_i=history[str(i+1)]['inverse dynamics'][i_t][4],
                    a_ci=history[str(i+1)]['inverse dynamics'][i_t][5])
        u = project(f_i, tau_i, z, n_eta, history[str(i+1)]['inverse dynamics'][i_t][0])
        f_prev = f_i
        tau_prev = tau_i
        final[str(i+1)]['inverse dynamics'].append(round(float(u), 2))

        f_i_g, tau_i_g = backward_recursion(f=f_prev_g, m=m[i], g=g, I=I[i],
                        tau=tau_prev_g,
                        r=history[str(i+1)]['gravity'][i_t][1], 
                        r_c=history[str(i+1)]['gravity'][i_t][2],
                        w_i=history[str(i+1)]['gravity'][i_t][3],
                        w_dot_i=history[str(i+1)]['gravity'][i_t][4],
                        a_ci=history[str(i+1)]['gravity'][i_t][5])
        u_g = project(f_i, tau_i, z, n_eta, history[str(i+1)]['gravity'][i_t][0])
        f_prev_g = f_i_g
        tau_prev_g = tau_i_g
        final[str(i+1)]['gravity'].append(round(float(u_g), 2))

        f_i_c, tau_i_c = backward_recursion(f=f_prev_c, m=m[i], g=0, I=I[i],
                        tau=tau_prev_c,
                        r=history[str(i+1)]['coriolis'][i_t][1], 
                        r_c=history[str(i+1)]['coriolis'][i_t][2],
                        w_i=history[str(i+1)]['coriolis'][i_t][3],
                        w_dot_i=history[str(i+1)]['coriolis'][i_t][4],
                        a_ci=history[str(i+1)]['coriolis'][i_t][5])
        u_c = project(f_i_c, tau_i_c, z, n_eta, history[str(i+1)]['coriolis'][i_t][0])
        f_prev_c = f_i_c
        tau_prev_c = tau_i_c
        final[str(i+1)]['coriolis'].append(round(float(u_c), 2))

        f_i_mi, tau_i_mi = backward_recursion(f=f_prev_mi, m=m[i], g=0, I=I[i],
                        tau=tau_prev_mi,
                        r=history[str(i+1)]['inertia matrix'][i_t][1], 
                        r_c=history[str(i+1)]['inertia matrix'][i_t][2],
                        w_i=history[str(i+1)]['inertia matrix'][i_t][3],
                        w_dot_i=history[str(i+1)]['inertia matrix'][i_t][4],
                        a_ci=history[str(i+1)]['inertia matrix'][i_t][5])
        u_mi = project(f_i_mi, tau_i_mi, z, n_eta,
                       history[str(i+1)]['inertia matrix'][i_t][0])
        f_prev_mi = f_i_mi
        tau_prev_mi = tau_i_mi
        final[str(i+1)]['inertia matrix'].append(round(float(u_mi), 2))

plot_trajectory('inverse dynamics', 'u', t, 'Обобщённые силы')
plot_trajectory('gravity', 'g', t, 'Гравитация')
plot_trajectory('coriolis', 'c', t, 'Кориолисовы/центробежные силы')
plot_trajectory('inertia matrix', 'I', t, 'Инерция')

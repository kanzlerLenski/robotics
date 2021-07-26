import numpy as np

def calculate_j(x, u):
	return x**2 + 6*u**2 + 3*x*u + 8*x + 3*u - 5

def calculate_x(u, x=[]):
    for u_i in u:
        x.append(4*u_i**2 + 3)
    return x

def calculate_lambda(u, lamb=[]):
    for u_i in u:
        lamb.append(8*u_i**2 + 3*u_i + 14)
    return lamb

def calculate_ja(x, u, lam, ja=[]):
    for i in range(0, 3):
            ja.append(x[i]**2 + 6*u[i]**2 + 3*x[i]*u[i] + 8*x[i] + 3*u[i] - 5 + \
                  lam[i]*(-x[i] + 4*u[i]**2 + 3))
    return ja

def check_inequation(all_x, all_u, res=[]):
    for i in range(0, 4):
        if -all_x[i] + 4*all_u[i]**2 + 3 <= 0:
                res.append((all_x[i], all_u[i]))
    return res

# 1.1.
calculate_j(-5.8, 1.2)

# 1.2.
coeff = [16, 9, 31, 3]
u = np.roots(coeff)

x = calculate_x(u)
lamb = calculate_lambda(u)
ja = calculate_ja(x, u, lamb)

print(f"u = {u}\nx = {x}\nlamb = {lamb}\nJa = {ja}")

# 1.3.
all_x = x
all_x.append(-5.8)

all_u = list(u)
all_u.append(1.2)

res = check_inequation(all_x, all_u)

print(res)









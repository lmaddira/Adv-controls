# MPC tracking horizon problem we solved in matlab
import osqp
import numpy as np
import scipy as sp
from scipy import sparse
from scipy.linalg import block_diag
from scipy import signal
import matplotlib.pyplot as plt

# plant model
Ad = sparse.csc_matrix([[1,1],[0, 1]])
Bd = sparse.csc_matrix([[0],[1]])
A = np.array([[1,1],[0, 1]])
B = sparse.csc_matrix([[0],[1]])
[nx,nu] = Bd.shape
C = np.array([1,0])
R = 1/10
RD = 1
Q = 1
# MPC horizon
N = 10 

Sx = []
CAB = []
# print(A)
Qbar = np.kron(np.eye(N,dtype=np.float),Q)
Rbar = np.kron(np.eye(N,dtype=np.float),R)
RDbar = np.kron(np.eye(N,dtype=np.float),RD)
for i in range(1,N+1):
    Sx.append(C @ np.linalg.matrix_power(A, i))
    CAB.append(C @ np.linalg.matrix_power(A, i-1) @ B)
Sx = np.asarray(Sx)
CAB = np.asarray(CAB)

# need to redefine this for a global problem this Su initiation will only work for this example
Su = np.zeros([N,N])

for i in range(N):
    for j in range(i):
        Su[i,j] = np.sum(CAB[:i-j+1])
Su1 = Su[:][1]
LL = np.tril(np.ones(N))
# P is same as H in Matlab formulation for quadprog
P = 2 * ((LL.T @ Rbar @ LL) + (RDbar) + (Su.T @ Qbar @ Su))
P = sparse.csc_matrix(P)
Fu = 2* (((LL.T @ Rbar.T).diagonal()).T + Su1.T @ Qbar @ Su).T
Fr = -2 * (Qbar @ Su).T
Fx = 2 *(Sx.T @ Qbar @ Su).T
# Instead of G, w and S we have l , A, u but we already have A so using G for that var
G = np.tril(np.ones(N))
G = sparse.csc_matrix(G)
umax = 12
umin = -12
U = 0 # initial control 
l = np.ones(N)*(umin - U)
u = np.ones(N)*(umax - U)

# Create an OSQP object
prob = osqp.OSQP()

X = np.zeros(2)
T = 40 # time steps 
# reference 
t = np.linspace(0,T+N+1)
r = signal.square(1/6 * t)
# plt.plot(t, r)
# plt.show()
i = 0
# print(r[i:i+N].T)
print(len(r))
q = (Fx @ X + Fu * U + Fr @ r[i:i+N].T)
# print(" q ", q )
# Setup workspace
prob.setup(P, q, G, l, u, warm_start=True)
res = prob.solve()
U = res.x[0]
# print("res ", u)
Xact = []
uact = []
# Simulate in closed loop
for i in range(T-1):
    Xact.append(X[0])
    uact.append(U)
    q = Fx @ X + Fu * u + Fr @ r[i:i+N].T
    l = np.ones(N)*(umin - U)
    u = np.ones(N)*(umax - U)
    print(" min ", umin-U)
    print(" max ", umax-U)
    prob.update(q = q, l=l, u=u)
    res = prob.solve()
    U = U + res.x[0]
    print(" new u ", U)
    # print(A.shape)
    # print(B.shape)
    # print(X.shape)
    X = (A@X.reshape(2,1) + B * U).reshape(2,1)
    # print("res ", res)
plt.plot(t[:T-1],Xact)
plt.plot(t[:T-1], r[:T-1])
plt.show()
# print(" u ", u)
plt.plot(uact)
plt.show()


# pron


# print("Qbar :\n",Qbar)
# print("Rbar :\n",Rbar)
# print("RDbar :\n",RDbar)
# print("Sx :\n",Sx)
# print("CAB :\n", CAB)
# print("Su :\n",Su)
# print("Su1 :\n",Su1)
# print("LL :\n",LL)
# print("P :\n",P)
# print("Fu :\n",Fu)
# print("Fr :\n",Fr)
# print("Fx :\n",Fx)

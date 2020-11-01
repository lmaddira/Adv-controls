import osqp
import numpy as np
from scipy import sparse

# Define problem data
P = sparse.csc_matrix([[4, 1], [1, 2]])
q = np.array([1, 1])
A = sparse.csc_matrix([[1, 1], [1, 0], [0, 1]])
l = np.array([1, 0, 0])
u = np.array([1, 0.7, 0.7])

# Create an OSQP object
prob = osqp.OSQP()

# Setup workspace and change alpha parameter
prob.setup(P, q, A, l, u, alpha=1.0)

# Solve problem
res = prob.solve()

# Update problem (vectors)
q_new = np.array([2, 3])
l_new = np.array([2, -1, -1])
u_new = np.array([2, 2.5, 2.5])
prob.update(q=q_new, l=l_new, u=u_new)

# Solve updated problem
res = prob.solve()


# Update problem (matrices)
# NB: Update only upper triangular part of P
P_new = sparse.csc_matrix([[5, 1.5], [1.5, 1]])
A_new = sparse.csc_matrix([[1.2, 1.1], [1.5, 0], [0, 0.8]])
prob.update(Px=sparse.triu(P_new).data, Ax=A_new.data)

# Solve updated problem
res = prob.solve()
import numpy as np
import numpy.linalg as LA

def naiveGaussElim(A, n):
	for i in range(n-1):
		for j in range(i, n):
			m = A[j][i] / A[i][i]
			for k in range(i+1, n):
				A[j][k] = A[j][k] - m * A[i][k]
			b[j] = b[j] - m * b[i]
	return b  # ?

def gaussian_elim_pivot(Aorig, borig):
	A = Aorig.astype('f')
	print(A)
	b = borig.astype('f')
	#copies and converts the original matrix to float
	n, m = A.shape
	print (n, m)
	print ("blen", len(b))
	for i in range(n-1):
		print("i", i)
		print(A)
		colmax = abs(A[i, i])
		pivot = i
		for j in range(i+1, n):
			if abs(A[j, i]) > colmax:
				colmax = abs(A[j, i])
				pivot = j
		if pivot > i:
			for k in range( n):
				hold = A[i, k]
				A[i, k] = A[pivot, k]
				A[pivot, k] = hold
			hold = b[i]
			b[i] = b[pivot]
			b[pivot] = hold
		for j in range(i+1, n):
			m = A[j, i] / A[i, i]#the diagonals can't be 0
			for k in range(i+1, n):
				A[j, k] = A[j, k] - (m * A[i, k])
			b[j] = b[j] - (m * b[i])
	return A, b

def vec_p_norm(v, p):
	sum = 0
	for i in range(len(v)):
		sum += ( v[i] ** p)
	return ( np.power(sum, ( 1 / p ) ))

'''
def vec_L2_norm(v):
	sum = 0
	for i in range(len(v)):
		sum += ( v[i] ** 2 )
	return ( np.power(sum, .5) )
'''
def vec_L2_norm(v):
	return np.sqrt(np.vdot(v.T, v)) 
def vec_inf_norm(v):
	max = 0
	for i in range(len(v)):
		if abs(v[i]) > max:
			max = abs(v[i])
	return max

def mat_inf_norm(A):
	max = 0
	rows = A.shape[0]
	cols = A.shape[1]
	for i in range(rows):
		rowSum = 0
		for j in range(cols):
			rowSum += abs(A[i,j])
		if rowSum > max:
			max = rowSum
	return max

def mat_L2_norm(A):
	#A must be a numpy matrix
	At = A.transpose()
	AtA = np.dot(At, A)
	AtA_eigenvalues = LA.eig(AtA)
	maxeig = 0
	for i in range( len( AtA_eigenvalues )):
		if AtA_eigenvalues[i] > maxeig:
			maxeig = AtA_eigenvalues[i]
	ret = (maxeig ** 1/2)
	return ret


def cond_num(A):
	Ainv = LA.inv(A)
	infN = mat_inf_norm(A)
	invInfN = mat_inf_norm(Ainv)
	kappa = infN * invInfN
	return kappa

def est_cond_num(A):
	n, m = A.shape
	y0 = [1 for i in range(n)] #array of vectors
	y = [ [] for i in range(5)]
	y[0] = y0
	rows = A.shape[0]
	cols = A.shape[1]


	for i in range(1, 5):
		b = [ ( y[i-1][j] / vec_inf_norm(y[i-1]) ) for j in range(n)]
		y[i] = LA.solve(A, b)

	alpha = mat_inf_norm(A)
	v = vec_inf_norm(y[4])
	kappa = alpha * v
	return kappa

def LUdecomp(A):
	n, m = A.shape
	for k in range(n-1):
		for i in range(k+1,n):
			if abs(A[i,k]) > 1.0e-9:
				lam = A[i,k] / A[k,k]
				A[i,k+1:n] = A[i,k+1:n] - lam * A[k,k+1:n]
				A[i,k] = lam
	return A

def LUsolve(A,b):
	n, m = A.shape
	for k in range(1,n):
		b[k] = b[k] - np.dot(A[k,0:k],b[0:k])
	b[n-1] = b[n-1] / A[n-1,n-1]
	for k in range(n-2,-1,-1):
		Ab = np.dot(A[k,k+1:n], b[k+1:n])
		b[k] = (b[k] - Ab ) / A[k,k]
	return b

def LUP(matrix):
	#stack overflow version
	n, m = matrix.shape
	P = np.identity(n)
	L = np.identity(n)
	U = matrix.copy()
	PF = np.identity(n)
	LF = np.zeros((n,n))
	
	for k in range(0, n - 1):
		index = max(abs(U[k:,k]))
		index = index + k
		if index != k:
			P = np.identity(n)
			P[[index,k],k:n] = P[[k,index],k:n]
			U[[index,k],k:n] = U[[k,index],k:n]
			PF = np.dot(P,PF)
			LF = np.dot(P,LF)
		L = np.identity(n)
		for j in range(k+1,n):
			L[j,k] = -(U[j,k] / U[k,k])
			LF[j,k] = (U[j,k] / U[k,k])
		U = np.dot(L,U)
	np.fill_diagonal(LF, 1)
	return PF, LF, U
def LUP_combo(Aorig):

	A = Aorig.astype('f') #copies and converts the original matrix to float

	n, m = A.shape

	L = np.identity(n)
	U = np.zeros( (n, n) )
	swap = [i for i in range(n)]


	for i in range( n):
		colmax = abs(A[i, i])
		pivot = i
		#find column max
		for j in range(i+1, n):
			if abs(A[j,i]) > colmax:
				colmax = abs(A[j,i])
				pivot = j
		if pivot > i:
			#swap the rows
			for k in range(n):
				hold = A[i, k]
				A[i, k] = A[pivot, k]
				A[pivot, k] = hold
			s = swap[i]
			swap[i] = swap[pivot]
			swap[pivot] = s
		for j in range(i+1, n):
			A[j, i] = A[j, i] / A[i, i]
			for k in range(i+1, n):
				A[j, k] = A[j, k] - A[j, i] * A[i, k]
	return A, swap

def LU_explode(A):
	n, m = A.shape
	L = np.identity(n)
	U = np.zeros( (n, n) )
	for i in range(n):
		for j in range(n):
			if i > j:
				L[i, j] = A[i, j]
			if i <= j:
				U[i, j] = A[i, j]
	return L, U
def bSwap(b, swap):
	x = [0 for i in range(n)]
	for k in range(n):
		x[k] = b[swap[k]]
	for k in range(n):
		b[k] = x[k]
	return b

def LUP_solver(L, U, b, swap, A):
	n, m = L.shape
	x = [0 for i in range(n)]
	y = [0 for i in range(n)]

	# for i in range(n):
	# 	indx[i] = np.where(P[i] == 1.0)[0][0]

	for k in range(n):
		#print(swap[k])
		x[k] = b[swap[k]]

	for k in range(n):
		b[k] = x[k]

	print("b{}".format(b))
	# Solve Ly = b
	y[0] = b[0]
	for i in range( n):
		sum = 0.0
		for j in range(i-1):
			sum = sum + L[i, j]*y[j]
		y[i] = b[i] - sum
	print(y)
	print(x)
	# Next, solve Ux = y
	x[n-1] = y[n-1]/A[n-1, n-1]
	for i in range(n-1, 1, -1):
		sum = 0.0
		for j in range(i+1, n):
			sum = sum + U[i, j]*x[j]
		x[i] = (y[i] - sum)/A[i, i]
	return x

def H(n):
	H = np.zeros( (n, n) )
	for i in range(n):
		for j in range(n):
			H[i, j] = 1/ (i + j + 1)
	return H
def K(n):
	K = np.zeros( (n, n) ) 
	for i in range(n):
		for j in range(n):
			if i == j:
				K[i, j] = 2
			elif abs(i - j) == 1:
				K[i, j] = -1
			else:
				K[i, j] = 0
	return K
def T(n):
	T = np.zeros( (n, n) )
	for i in range(n):
		for j in range(n):
			if i == j:
				T[i, j] = 4
			elif abs(i - j) == 1:
				T[i, j] = 1
			else:
				T[i, j] = 0
	return T
def A(n):
	T = np.zeros( (n, n) )
	for i in range(n):
		for j in range(n):
			if i == j:
				T[i, j] = 1
			elif i - j == 1:
				T[i, j] = 4
			elif i - j == -1:
				T[i, j] = -4
			else:
				T[i, j] = 0
	return T

def S(n):
	S = np.zeros( (n, n) )
	for i in range(n):
		for j in range(n):
			if i == j or abs(i - j) == 1:
				S[i, j] =  1/ (i + j + 1)
			else:
				S[i, j] = 0
	return S
def diagWpadding(n, mainDiag, numPads, padding):
	D = np.zeros( (n, n) )
	for i in range(n):
		for j in range(n):
			if i == j:
				D[i, j] =  mainDiag
			elif abs(i - j) <= numPads:
				D[i, j] = padding
	return D
def is_pos_def(x):
	return np.all(np.linalg.eigvals(x) > 0)

def cholesky(A):
	n, m = A.shape
	L = np.zeros((n, n))
	# Perform the Cholesky decomposition
	for i in range(n):
		for j in range(i+1):
			tmp_sum = sum(L[i][k] * L[j][k] for k in range(j))
			if (i == j): # Diagonal elements
				L[i, j] = np.sqrt(max(A[i,i] - tmp_sum, 0))
			else:
				L[i,j] = (1.0 / L[j,j]) * (A[i,j] - tmp_sum)
	return L

#diagonal simplification version - O(n)
def jacobi(A, b, x, n):

	D = np.diag(A)
	R = A - np.diagflat(D)

	for i in range(n):
		x = (b - np.dot(R.A,x))/ D
	return x

#book's inverse version O(n^2)
#%%
def invJacobi(A, b, x, iter):
	n, m = A.shape
	if n != m:
		print("not square")
		return -1

	#copy all of the diagonal values of A into M, and set all of the diagonals of N to be 0
	#3 * O(n^2)
	Diag = np.identity(n)
	Rest = A.copy()
	for i in range(n):
		Diag[i, i] = A[i, i]
		Rest[i, i] = 0
	x = [1] * n
	#iterate
	for i in range(iter):
		ax = np.dot(A, x)
		dinv = LA.inv(Diag)
		dinvax = np.dot( dinv, ax)
		dinvb = np.dot(dinv, b)
		x = np.add(np.subtract(x, dinax), x)
	
def gauss_seidel(A, b, x, iter):
	L = np.tril(A)
	for i in range(iter):
		Linv = LA.inv(L)
		Ax = A @ x
		print("Ax", Ax)
		Linvb =Linv @ b
		print("Linvb", Linvb)
		LinvAx = Linv @ Ax
		print("LinvAx", LinvAx)
		x = x - LinvAx + Linvb
		print("x", x)
	return x
def SOR(A, b, x, w, iter):
	L = np.tril(A, -1)
	D = np.diag(A)
	U = np.triu(A, 1)
	
	M = LA.inv(D - ( w * L )) @ (( ( 1 - w ) * D ) + ( w * U))
	C = ( w * LA.inv(D - ( w * L )) ) @ b
	#Dw = D/w
	#LDinv = LA.inv(L + Dw)
	#DwU = Dw - D - U
	for i in range(iter):
		xOld = x
		x = M @ x + C
		#x = LDinv @ ( ( DwU @ x ) + b)
	return x

def sumSOR(A, b, x0, tol=1.e-6, max_iter=50, w=1): 
	n, m = A.shape 
	x = x0 
	i = 0
	for i in range (max_iter): 
		for i in range(n): 
			xOld = x
			upper_sum = A[i, :i] @ x[:i]
			lower_sum = A[i, i+1 :] @ xOld[i+1:]
			x[i] = (b[i] - (lower_sum + upper_sum)) / A[i, i] 
			x[i] = (x[i] @ w) + (xOld[i] @ (1 - w))
		
		#if (np.linalg.norm(x - x0) < tol): 
		if (vec_L2_norm((A @ x) - b ) < tol):
			print("Converged after {} iterations".format(i))
			break 
		
	return x
def multiDimNM_err(f, gradF, x, err=1.e-5, maxIter=50, verbose=False):
	xOld = x*2
	iter = 0
	for i in range(maxIter):

		xOld = x
		x = x - np.dot(LA.inv(gradF(x)) , f(x))
		iter+=1
		if verbose:
			print(x)
		if ch7.vec_L2_norm(x - xOld) < err:
			break
	if verbose:
		print("Converge after {} iterations".format(iter))
	return x


def f(x):
	return 6*x**5-5*x**4-4*x**3+3*x**2

def df(x):
	return 30*x**4-20*x**3-12*x**2+6*x


	

e = lambda i, n: np.array([ 1 if indx==i-1 else 0 for indx in range(n) ])
diagAry = lambda A, offset=0: [ A[i+offset,i+offset] for i in range(A.shape[0]) ]
residual = lambda x, y: abs(LA.norm(x, np.inf) - LA.norm(y, np.inf))
sign = lambda x: 1 if np.real(x) >= 0 else -1
I = lambda n: np.eye(n, dtype='float')

def signed_normed(v):
	e1 = e(1, len(v))
	a = sign(v[0]) * vec_L2_norm(v)
	u = v + ( a* e1 )
	u = u / vec_L2_norm(u)
	return u

def hh_reflect(v):
	u = signed_normed(v)
	V = 2* ( np.outer(u, u) )
	H = I(len(v)) - V
	return H

def hh_reflect_complex(v):
	u = signed_normed(v)
	V = 2* ( np.dot(u.conj(), u.conj().T) )
	H = I(len(v)) - V
	return H


def hhInsert(A, B):
#implants A into B in the bottom right corner
	nB, mB = B.shape
	nA, mA = A.shape
	offset = nB - nA
	Apad = B.copy()#astype('complex')
	if( offset < 0):
		print("matrix is too big")
	else:
		for i in range(nB):
			for j in range(mB):
				if i >= offset and j >= offset:
					Apad[i,j] = A[i-offset, j - offset]

	return Apad

def qr(A, verbose=False):
	if verbose:
		print("-----------------------Decomposing---------------------")
	n,m = A.shape
	Hs = []
	As = []
	As.append(A)
	
	for i in range(n-1):
		Ax = As[i]
		a1 = np.array(Ax[i:,i] ).flatten() #get the first col of the subarray
		Hsub = hh_reflect(a1) #compute the householder reflection matrix
		#print("As", As)
		Hs.append( hhInsert( Hsub, I(n) )) #pad the H matrix and add it to the list
		As.append(np.dot( Hs[i].conj(), As[i] ))
		if verbose:
			print("\nH[{}]\n".format(i), Hs[i])
			print("\nA[{}]\n".format(i), As[i])
			print("\nA[{}]\n".format(i+1), As[i+1])
		
	R = As[-1]
	Q = I(n)
	for H in Hs:
		Q = np.dot(Q.conj(), H)
	if verbose:
		print("\n------------\nQ\n", Q)
		print("\n------------\nR\n", R)
		print("-----------------------Done Decomposing---------------------")
	return Q, R


def wilkinsonMu(A):
	d = (A[-2, -2] + A[-1, -1])/2
	t = np.sqrt(d**2 + A[-1, -2]**2)
	mu = A[-1, -1] + d - sign(d) * t
	return mu

def doubleShift(A):
	a = A[-2, -2]
	b = A[-2, -1]
	c = A[-1, -2]
	d = A[-1, -1]
	trace = a + d
	det = a*d - b*c
	eig1 = trace/2 + np.sqrt(trace**2 / 4-det)
	eig2 = trace/2 - np.sqrt(trace**2 / 4-det)
	return eig1, eig2

def qr_iter(A, muType='wilk', tol=1.e-5, maxIter=1000, verbose=False):
	n,m = A.shape
	
	Q = np.empty((n,m))
	R = np.empty((n,m))
	
	Ak = sLA.hessenberg(A).astype('cfloat')
	
	err = 1
	i = 0
	doubShiftCounter = 0
	mu1 = 0
	mu2 = 0
	for i in range(maxIter):
		if muType == 'wilk':
			mu = wilkinsonMu(Ak)
		if muType == 'br': #bottom right
			mu = Ak[-1, -1]
		if muType== 'double':
			if doubShiftCounter == 0:
				mu1, mu2 = doubleShift(Ak)
				mu = mu1
				doubShiftCounter+=1
			else:
				mu = mu2
				doubShiftCounter = 0
				
		#mu = vec_p_norm(diagAry(Ak), 7) #eigs converge to the 
		#mu = Ak[i+1, i+1] #idk
		#mu = max(diagAry(Ak))
		Aold = Ak
		Ak = Ak - mu * I(n)
		Q, R = qr(Ak, verbose)
		Ak = np.dot(R.conj(), Q) + mu * I(n)
		err = residual(Ak, Aold)
		#err = vec_inf_norm(diagAry(R, -1) )
		if verbose:
			print("Ak1: \n{}".format(Ak))
			print("i:{} err:{}".format(i, err) )
		if err < tol:
			break
	return Ak, i, err
'''
def qr_gramschmidt(A):
	n,m = A.shape
	e = []
	for i in range(n):
		a = A[:,0]
		print(a)
		#print(LA.norm(a))
		e.append( a / vec_L2_norm(a) )
		#print(e)
'''
def gershCircles(A):
	n, m = A.shape
	circles = []
	maxRadi = 1
	for i in range(n):
		center = ( A[i, i], 0 )
		rowSum = 0
		colSum = 0
		for j in range(m):
			if i != j:
				rowSum += abs(A[i, j])
				colSum += abs(A[j, i])
		gershRadi = min(rowSum, colSum)		
		circles.append( (center, gershRadi) )
	#print(circles)
	return circles

def plotGersh(A):
	name = A[1]
	A = A[0]
	colors = ['blue', 'green', 'red', 'purple', 'orange', 'white', 'black', 'gray']
	circles = gershCircles(A)
	fig, ax = plt.subplots()
	maxRadi = 0
	for i in range(len(circles)):
		circle = circles[i]
		maxRadi = max(circle[1], maxRadi)
		c = plt.Circle(circle[0], circle[1], color=str(random.random()), fill=False)
		ax.add_artist(c)
	
	axes = plt.gca()
	axes.set_xlim([-2*maxRadi,4*maxRadi])
	axes.set_ylim([-2*maxRadi,2*maxRadi])
	ax.plot()
	
	eigs = LA.eigvals(A)
	eigX = [eigs[i].real for i in range(len(eigs))]
	eigY = [eigs[i].imag for i in range(len(eigs))]
	plt.plot(eigX, eigY, 'bo')
	plt.title(name)
	plt.show()
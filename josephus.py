import math
import sys

from collections import deque

# Josephus problem is defined as follows
# Given [0, N-1] list in a circle, initially remove K-th item in the list,
# which is K-1. Repeatly removing K-th item from the previously removed
# item, compute the item remains at the end.
def josephus_naive(N, K):
	# O(NK)
	nums = deque(range(0, N))
	for i in range(0, N-1):
		nums.rotate(-(K-1))
		nums.popleft()
	return nums.pop()

def josephus(N, K):
	# O(N)
	if N == 1: return 0
	return (josephus(N-1, K) + K) % N

def josephus_iterative(N, K):
	# O(N)
	a = 0
	for i in range(2, N+1):
		a = (a + K) % i
	return a

def josephus_better(N, K):
	# O(min(N, K log N))
	if N == 1: return 0
	if K == 1: return N-1
	if N < K: return (josephus(N-1, K) + K) % N
	n = N - N // K
	nK = josephus_better(n, K)
	
	if nK < N % K:
		return (nK + (N - N % K)) % N
	return (((nK - (N % K))) % n // (K - 1) + nK + (N - (N % K))) % N

def josephus_better_iterative(N, K):
	# O(min(N, K log N))
	if N == 1: return 0
	if K == 1: return N-1
	if N < K: return (josephus_iterative(N-1, K) + K) % N

	Ns = [] # Size can be O(min(N, K log N))
	while N >= K:
		Ns.append(N)
		N -= N // K

	nK = josephus_iterative(N, K)
	n = N
	for N in reversed(Ns):
		if nK < N % K:
			nK = nK + (N - N % K)
		else:
			nK = (((nK - (N % K))) % n // (K - 1) + nK + (N - (N % K))) % N
		n = N
	return nK

def josephus_general(N, K, M, S):
	# This solves a general version of Josephus problem
	# Given [0, N-1] list in a circle, initially remove S-th item in the list.
	# Then repeatly remove K-th item from the previously removed item.
	# This returns the M-th removed item.
	# O(M)
	if M == 1: return (S - 1) % N
	return (josephus_general(N-1, K, M-1, K) + S) % N

def josephus_general_iterative(N, K, M, S):
	# O(M)
	if M == 1: return (S - 1) % N
	a = (K - 1) % (N - M + 1)
	for i in range(N - M + 2, N):
		a = (a + K) % i
	return (a + S) % N;

print josephus_general_iterative(6, 3, 6, 1), josephus_general_iterative(6, 3, 5, 1) # 4, 2
print josephus_general_iterative(40, 3, 40, 0), josephus_general_iterative(40, 3, 39, 0) # 24, 9

print josephus_iterative(10000000, 10000)
print josephus_better_iterative(10000000, 10000)

for N in range(1, 10000):
	for K in range(1, int(math.sqrt(N))):
		assert josephus_iterative(N, K) == josephus_better_iterative(N, K)

# 0, 1, 2, 3, 4, 5, 6, 7
#       x        x
# 2  3     4  5     0  1
#                   x

# 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
#       x        x        x   
# 2  3     4  5     6  7     0   1
#                   x


#((josephus(n, K) - (N % K)) // (K - 1) + josephus(n, K) + (N - N % K)) % N


import sys


def Eratosphen(n):
    mask = [True for i in range(n + 1)]
    primes = []
    for i in range(2, n + 1):
        if not mask[i]:
            continue
        primes.append(i)
        for j in range(2 * i, n + 1, i):
            mask[j] = False
    return primes


def Sqrt(n):
    a, b = 1, n
    while a <= b:
        t = (a + b) // 2
        m = t * t
        if m == n:
            return t
        if m < n:
            a = t + 1
        else:
            b = t - 1
    return a


def TripplesFast(n):
    if n <= 1:
        return 1
    primes = {}
    for k in Eratosphen(1000000):
        exponent = 0
        while n % k == 0:
            n = n // k
            exponent += 1
        if exponent // 2 > 0:
            primes[k] = exponent // 2
        if n == 1:
            break
    if n != 1:
        k = Sqrt(n)
        if k * k == n:
            primes[k] = 1
    odd = 1
    for k, v in primes.items():
        if k != 2:
            odd *= (k ** v)
    odd = (odd + 1) // 2
    result = odd
    if 2 not in primes:
        return result
    exp2 = primes[2]
    result *= 2
    if exp2 == 1:
        return result
    add = odd * 2 - 1
    for _ in range(exp2 - 1):
        result += add
        add *= 2
    return result

print(TripplesFast(int(sys.stdin.read())))


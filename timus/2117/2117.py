import math
import sys

"""
a*a*b=n => b<pow(n, 1/3)
"""

def Tripples(c):
    result = 0
    for a in range(c + 1):
        for b in range(a, c + 1):
            #if abs(math.sqrt(a)+math.sqrt(b)-math.sqrt(c)) < 1e-5:
            if c*c+a*a+b*b-2*a*c-2*b*c-2*a*b==0:
                result += 1
    return result


def Eratosphen(n):
    mask = [True for i in range(n + 1)]
    primes = []
    for i in range(2, n + 1):
        if not mask[i]:
            continue
        primes.append(i)
        for j in range(2 * i, n + 1, i):
            mask[j] = False
    #print(len(primes))
    return primes


def TripplesFast(n):
    if n <= 1:
        return 1
    primes = {}
    for k in Eratosphen(1000000): #2642245):
        exponent = 0
        while n % k == 0:
            n = int(n / k)
            exponent += 1
        if int(exponent / 2) > 0:
            primes[k] = int(exponent / 2)
        if n == 1:
            break
    if n != 1:
        k = int(math.sqrt(n))
        if k * k == n:
            primes[k] = 1
    odd = 1
    for k, v in primes.items():
        if k != 2:
            odd *= (k ** v)
    odd = int((odd + 1) / 2)
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

#for i in range(1, 100):
#    for j in range(7):
#        k = (2 * i + 1) * (2 * i + 1) * (2 * j + 1) * (2 * j + 1)
#        print("{}\t{}\t{}\t{}\t{}".format(k, 2 * i + 1, 2 * j + 1, Tripples(k), Factorize(k)))
#    print("")

#prev = 0
#for i in range(15):
#    k = (2 ** (2*i)) * 5 * 5
#    cur = Tripples(k)
#    print("{}\t{}\t{}\t{}\t{}".format(k, i, cur, cur - prev, TripplesFast(k)))
#    prev = cur

#for i in range(1, 1000):
#    print("{}\t{}\t{}".format(i, Tripples(i), TripplesFast(i)))

#for i in range(1, 1000):
#    k = 2 * i + 1
#    k = k * k
#    print("{}\t{}\t{}\t{}".format(k, i, Tripples(k), Factorize(k)))

#print(Factorize(794569207093795778))
#print(Factorize(2242187))



import sys

"""
 a   k-1 k-1   a
 b    1   0    b
"""

n, k, m = map(int, sys.stdin.read().split())

"""
 a b
 c d
"""
class TMatrix:
    def __init__(self, a, b, c, d, m):
        self.a, self.b, self.c, self.d = a % m, b % m, c % m, d % m


"""
 a b  a b
 c d  c d
"""
def Mul(x, y, m):
    return TMatrix(x.a * y.a + x.b * y.c, x.a * y.b + x.b * y.d, x.c * y.a + x.d * y.c, x.c * y.b + x.d * y.d, m)


def Pow(a, k, m):
    result = TMatrix(1, 0, 0, 1, m)
    while k != 0:
        if k % 2 == 1:
            result = Mul(result, a, m)
        a = Mul(a, a, m)
        k //= 2
    return result


result = Pow(TMatrix(k - 1, k - 1, 1, 0, m), n, m)

print(result.a)

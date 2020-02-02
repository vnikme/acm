#include <ios>
#include <iostream>
#include <cmath>


constexpr double EPS = 1e-6;
constexpr double R = 100.0;
constexpr double STEP = 0.001;
constexpr double LIMIT = 200;


struct TPoint {
    double X = 0.0, Y = 0.0, Z = 0.0;

    TPoint()
        : X(0.0), Y(0.0), Z(0.0)
    {
    }


    TPoint(double x, double y, double z)
        : X(x), Y(y), Z(z)
    {
    }
};

std::istream &operator >> (std::istream &fin, TPoint &p) {
    fin >> p.X >> p.Y >> p.Z;
    return fin;
}

std::ostream &operator << (std::ostream &fout, const TPoint &p) {
    fout << p.X << ' ' << p.Y << ' ' << p.Z;
    return fout;
}


/*
    x1 y1 z1
    x2 y2 z2
    x3 y3 z3
*/
double Det3(const TPoint &p1, const TPoint &p2, const TPoint &p3) {
    return p1.X*p2.Y*p3.Z + p3.X*p1.Y*p2.Z + p1.Z*p2.X*p3.Y - p1.Z*p2.Y*p3.X - p1.X*p2.Z*p3.Y - p3.Z*p1.Y*p2.X;
}

/*
    1  x   y   z
    1  x1  y1  z1
    1  x2  y2  z2
    1  x3  y3  z3
*/
void GetPlaneEquation(const TPoint &p1, const TPoint &p2, const TPoint &p3, double &a, double &b, double &c, double &d) {
    d = Det3(p1, p2, p3);
    a = -Det3(TPoint(1.0, p1.Y, p1.Z), TPoint(1.0, p2.Y, p2.Z), TPoint(1.0, p3.Y, p3.Z));
    b = Det3(TPoint(1.0, p1.X, p1.Z), TPoint(1.0, p2.X, p2.Z), TPoint(1.0, p3.X, p3.Z));
    c = -Det3(TPoint(1.0, p1.X, p1.Y), TPoint(1.0, p2.X, p2.Y), TPoint(1.0, p3.X, p3.Y));
}

double DotProduct(const TPoint &a, const TPoint &b) {
    return a.X*b.X + a.Y*b.Y + a.Z*b.Z;
}

/*
    m1x m1y m1z | px
    m2x m2y m2z | py
    m3x m3y m3z | pz

*/
TPoint Multiply(const TPoint &m1, const TPoint &m2, const TPoint &m3, const TPoint &p) {
    return TPoint(DotProduct(m1, p), DotProduct(m2, p), DotProduct(m3, p));
}

void Normalize(TPoint n, TPoint &p1, TPoint &p2, TPoint &p3) {
    double l = sqrt(n.X*n.X + n.Z*n.Z);
    if (l > EPS) {
        auto m1 = TPoint(n.Z / l, 0.0, -n.X / l);
        auto m2 = TPoint(0.0, 1.0, 0.0);
        auto m3 = TPoint(n.X / l, 0.0, n.Z / l);
        p1 = Multiply(m1, m2, m3, p1);
        p2 = Multiply(m1, m2, m3, p2);
        p3 = Multiply(m1, m2, m3, p3);
        n = Multiply(m1, m2, m3, n);
    }
    l = sqrt(n.Y*n.Y + n.Z*n.Z);
    if (l > EPS) {
        auto m1 = TPoint(1.0, 0.0, 0.0);
        auto m2 = TPoint(0.0, n.Z / l, -n.Y / l);
        auto m3 = TPoint(0.0, n.Y / l, n.Z / l);
        p1 = Multiply(m1, m2, m3, p1);
        p2 = Multiply(m1, m2, m3, p2);
        p3 = Multiply(m1, m2, m3, p3);
        n = Multiply(m1, m2, m3, n);
    }
    //std::cout << "n: " << n << std::endl;
}

double Distance(const TPoint &a, const TPoint &b) {
    return sqrt((a.X - b.X) * (a.X - b.X) + (a.Y - b.Y) * (a.Y - b.Y) + (a.Z - b.Z) * (a.Z - b.Z));
}

double Square(const TPoint &p1, const TPoint &p2, const TPoint &p3) {
    double d1 = Distance(p1, p2);
    double d2 = Distance(p1, p3);
    double d3 = Distance(p2, p3);
    double p = (d1 + d2 + d3) / 2;
    return sqrt(p * (p - d1) * (p - d2) * (p - d3));
}

bool Intersect(double x1, double y1, double x2, double y2, double x, double &y) {
    if (fabs(x1 - x2) < EPS)
        return false;
    double t = (x - x2) / (x1 - x2);
    if (t < 0.0 || t > 1.0)
        return false;
    y = (y1 - y2) * t + y2;
    return true;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    TPoint p1, p2, p3;
    std::cin >> p1 >> p2 >> p3;
    double a, b, c, d;
    GetPlaneEquation(p1, p2, p3, a, b, c, d);
    //std::cout << a << ' ' << b << ' ' << c << ' ' << d << std::endl;
    //std::cout << "Was: " << Square(p1, p2, p3) << ' ' << Distance(TPoint(0.0, 0.0, 0.0), p1) << ' ' << Distance(TPoint(0.0, 0.0, 0.0), p2) << ' ' << Distance(TPoint(0.0, 0.0, 0.0), p3) << std::endl;
    Normalize(TPoint(a, b, c), p1, p2, p3);
    //std::cout << p1 << std::endl << p2 << std::endl << p3 << std::endl;
    //std::cout << "Became: " << Square(p1, p2, p3) << ' ' << Distance(TPoint(0.0, 0.0, 0.0), p1) << ' ' << Distance(TPoint(0.0, 0.0, 0.0), p2) << ' ' << Distance(TPoint(0.0, 0.0, 0.0), p3) << std::endl;
    GetPlaneEquation(p1, p2, p3, a, b, c, d);
    //std::cout << a << ' ' << b << ' ' << c << ' ' << d << std::endl;
    double result = 0.0;
    for (double x = -LIMIT; x <= LIMIT; x += STEP) {
        double pointExists = false;
        double y, ym = 0.0, yM = 0.0;
        if (Intersect(p1.X, p1.Y, p2.X, p2.Y, x, y)) {
            if (!pointExists || y < ym)
                ym = y;
            if (!pointExists || y > yM)
                yM = y;
            pointExists = true;
        }
        if (Intersect(p1.X, p1.Y, p3.X, p3.Y, x, y)) {
            if (!pointExists || y < ym)
                ym = y;
            if (!pointExists || y > yM)
                yM = y;
            pointExists = true;
        }
        if (Intersect(p2.X, p2.Y, p3.X, p3.Y, x, y)) {
            if (!pointExists || y < ym)
                ym = y;
            if (!pointExists || y > yM)
                yM = y;
            pointExists = true;
        }
        if (!pointExists)
            continue;
        y = R*R - p1.Z * p1.Z - x * x;
        if (y < 0)
            continue;
        y = sqrt(y);
        ym = std::max(ym, -y);
        yM = std::min(yM, y);
        result += (std::max(yM - ym, 0.0) * STEP);
    }
    std::cout << result << std::endl;
    //std::cout << std::endl;
    return 0;
}



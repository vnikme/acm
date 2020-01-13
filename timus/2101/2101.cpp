#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>

constexpr double EPS = 1e-5;

struct TPoint {
    double x = 1000000.0, y = 1000000.0;
};

struct TLineEquation {
    double a = 0.0, b = 0.0, c = 0.0;
};

TLineEquation GetLineFrom2Points(const TPoint &a, const TPoint &b) {
    TLineEquation result;
    result.a = b.y - a.y;
    result.b = a.x - b.x;
    result.c = a.y * b.x - b.y * a.x;
    return result;
}

TLineEquation GetParallelLineWithPoint(const TLineEquation &line, const TPoint &p) {
    TLineEquation result(line);
    result.c = -(line.a * p.x + line.b * p.y);
    return result;
}

TLineEquation GetOrthogonalLineWithPoint(const TLineEquation &line, const TPoint &p) {
    TLineEquation result;
    result.a = -line.b;
    result.b = line.a;
    result.c = -(result.a * p.x + result.b * p.y);
    return result;
}

bool IntersectLines(const TLineEquation &l1, const TLineEquation &l2, TPoint &result) {
    double d = l1.a * l2.b - l1.b * l2.a;
    if (fabs(d) < EPS)
        return false;
    result.x = - (l1.c * l2.b - l1.b * l2.c) / d;
    result.y = - (l1.a * l2.c - l1.c * l2.a) / d;
    return true;
}

double Distance(const TPoint &a, const TPoint &b) {
    return sqrt((a.x - b.x) * (a.x - b.x)+ (a.y - b.y) * (a.y - b.y));
}

bool IsInsideSegment(const TPoint &a, const TPoint &b, const TPoint &c) {
    return fabs(Distance(a, b) - Distance(a, c) - Distance(c, b)) < EPS;
}

double ScalarProduct(const TPoint &a, const TPoint &b, const TPoint &c) {
    return (a.x - b.x) * (c.x - b.x) + (a.y - b.y) * (c.y - b.y);
}

double CalcAreaParallel(const TPoint &leftDown, const TPoint &rightDown, const TPoint &upper, const TPoint &hole) {
    TLineEquation downLine = GetLineFrom2Points(leftDown, rightDown);
    TLineEquation upperLine = GetParallelLineWithPoint(downLine, hole);
    TLineEquation rightTriangleLine = GetLineFrom2Points(rightDown, upper);
    TLineEquation leftTriangleLine = GetLineFrom2Points(leftDown, upper);
    TPoint rectangleRightUpper, rectangleLeftUpper, rectangleRightDown, rectangleLeftDown;
    if (!IntersectLines(upperLine, rightTriangleLine, rectangleRightUpper))
        return -1.0;
    if (!IntersectLines(upperLine, leftTriangleLine, rectangleLeftUpper))
        return -1.0;
    TLineEquation rightRectangleLine = GetOrthogonalLineWithPoint(downLine, rectangleRightUpper);
    TLineEquation leftRectangleLine = GetOrthogonalLineWithPoint(downLine, rectangleLeftUpper);
    if (!IntersectLines(rightRectangleLine, downLine, rectangleRightDown))
        return -1.0;
    if (!IntersectLines(leftRectangleLine, downLine, rectangleLeftDown))
        return -1.0;
    if (!IsInsideSegment(leftDown, rightDown, rectangleRightDown) || !IsInsideSegment(leftDown, rightDown, rectangleLeftDown))
        return -1.0;
    double square = Distance(rectangleLeftDown, rectangleRightDown) * Distance(rectangleLeftDown, rectangleLeftUpper);
    return square > EPS ? square : -1.0;
}

double CalcAreaOrthogonal(TPoint leftDown, TPoint rightDown, const TPoint &upper, const TPoint &hole) {
    if (fabs(ScalarProduct(upper, leftDown, rightDown)) < EPS)
        return -1.0;
    if (fabs(ScalarProduct(upper, rightDown, leftDown)) < EPS)
        return -1.0;
    TLineEquation downLine = GetLineFrom2Points(leftDown, rightDown);
    TLineEquation verticalLine = GetOrthogonalLineWithPoint(downLine, hole);
    TLineEquation rightTriangleLine = GetLineFrom2Points(rightDown, upper);
    TLineEquation leftTriangleLine = GetLineFrom2Points(leftDown, upper);
    TPoint rectangleRightUpper, rectangleLeftUpper, rectangleRightDown, rectangleLeftDown;
    if (!IntersectLines(verticalLine, rightTriangleLine, rectangleRightUpper) || !IsInsideSegment(rightDown, upper, rectangleRightUpper)) {
        std::swap(rightTriangleLine, leftTriangleLine);
        std::swap(leftDown, rightDown);
    }
    if (!IntersectLines(verticalLine, rightTriangleLine, rectangleRightUpper) || !IsInsideSegment(rightDown, upper, rectangleRightUpper))
        return -1.0;
    TLineEquation upperLine = GetParallelLineWithPoint(downLine, rectangleRightUpper);
    if (!IntersectLines(upperLine, leftTriangleLine, rectangleLeftUpper) || !IsInsideSegment(leftDown, upper, rectangleLeftUpper))
        return -1.0;
    TLineEquation leftRectangleLine = GetOrthogonalLineWithPoint(downLine, rectangleLeftUpper);
    if (!IntersectLines(leftRectangleLine, downLine, rectangleLeftDown) || !IsInsideSegment(leftDown, rightDown, rectangleLeftDown))
        return -1.0;
    if (!IntersectLines(verticalLine, downLine, rectangleRightDown) || !IsInsideSegment(leftDown, rightDown, rectangleRightDown))
        return -1.0;
    //std::cout << '(' << rectangleLeftDown.x << ' ' << rectangleLeftDown.y << ") (" << rectangleLeftUpper.x << ' ' << rectangleLeftUpper.y << ") (" << rectangleRightUpper.x << ' ' << rectangleRightUpper.y << ") (" << rectangleRightDown.x << ' ' << rectangleRightDown.y << ")" << std::endl;
    double square = Distance(rectangleLeftDown, rectangleRightDown) * Distance(rectangleLeftDown, rectangleLeftUpper);
    return square > EPS ? square : -1.0;
}

int main() {
    TPoint V[3], H;
    for (size_t i = 0; i < 3; ++i)
        std::cin >> V[i].x >> V[i].y;
    std::cin >> H.x >> H.y;
    double maxArea = 0.0;
    size_t validRectangles = 0;
    for (size_t i = 0; i < 3; ++i) {
        double area = CalcAreaParallel(V[i], V[(i + 1) % 3], V[(i + 2) % 3], H);
        maxArea = std::max(maxArea, area);
        if (area >= 0.0)
            ++validRectangles;
        area = CalcAreaOrthogonal(V[i], V[(i + 1) % 3], V[(i + 2) % 3], H);
        maxArea = std::max(maxArea, area);
        if (area >= 0.0)
            ++validRectangles;
    }
    std::cout << std::fixed << std::setprecision(8) << maxArea << std::endl << validRectangles << std::endl;
    return 0;
}


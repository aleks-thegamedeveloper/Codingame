// https://www.codingame.com/training/hard/encounter-surface

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <map>
#include <cfloat>

using namespace std;

struct vec2f
{
    vec2f operator-(const vec2f& a) const
    {
        return { x - a.x, y - a.y };
    }

    vec2f operator+(const vec2f& a) const
    {
        return { x + a.x, y + a.y };
    }

    vec2f operator*(float k) const
    {
        return { x * k, y * k };
    }

    static float getSize(const vec2f& v)
    {
        return sqrt(v.x * v.x + v.y * v.y);
    }

    static vec2f getNormal(const vec2f& v)
    {
        const float sz{ getSize(v) };
        return { v.x / sz, v.y / sz };
    }

    static float dot(const vec2f& a, const vec2f& b)
    {
        return a.x * b.x + a.y * b.y;
    }

    float x;
    float y;
};
constexpr vec2f up{ 0.f, 1.f };

string toString(const vec2f& v)
{
    ostringstream stream;
    stream << "(x: " << v.x << "; y: " << v.y << ")";
    return stream.str();
}

bool IsNearlyZero(float f)
{
    return abs(f) <= 1e-8f;
}

class PolygonIntersectionAreaSolver
{
public:
    PolygonIntersectionAreaSolver()
    {
        int n;
        cin >> n; cin.ignore();
        int m;
        cin >> m; cin.ignore();
        vec2f temp;
        for (int i = 0; i < n; i++) {
            cin >> temp.x >> temp.y; cin.ignore();

            m_verticesA.emplace_back(temp);
        }
        for (int i = 0; i < m; i++) {
            cin >> temp.x >> temp.y; cin.ignore();

            m_verticesB.emplace_back(temp);
        }
    }

    float CalculatePolygonIntersectionArea()
    {
        CorrectPolygonWindingOrder(m_verticesA);
        CorrectPolygonWindingOrder(m_verticesB);

        ComputePolygonEdges();

        TestPolygonIntersections();

        ComputeIntersectionPolygonPoints();

        return ComputeIntersectionPolygonArea();
    }

private:
    struct IntersectionTest
    {
        const tuple<vec2f, vec2f>& edgeA;
        const tuple<vec2f, vec2f>& edgeB;
        float fa{ 0.f };
        float fb{ 0.f };
        bool bIntersect{ false };
    };

    void ComputePolygonEdges()
    {
        for (int i{ 1 }; i < m_verticesA.size(); ++i)
        {
            m_edgesA.push_back({ m_verticesA[i - 1], m_verticesA[i] });
        }
        m_edgesA.push_back({ m_verticesA.back(), m_verticesA.front() });

        for (int i{ 1 }; i < m_verticesB.size(); ++i)
        {
            m_edgesB.push_back({ m_verticesB[i - 1], m_verticesB[i] });
        }
        m_edgesB.push_back({ m_verticesB.back(), m_verticesB.front() });
    }

    void TestPolygonIntersections()
    {
        for (const tuple<vec2f, vec2f>& edgeA : m_edgesA)
        {
            m_intersectionTests.emplace_back();
            vector<IntersectionTest>& newRow = m_intersectionTests.back();

            for (const tuple<vec2f, vec2f>& edgeB : m_edgesB)
            {
                const IntersectionTest& test = CalculateIntersection(edgeA, edgeB);
                newRow.emplace_back(test);
            }
        }
    }

    void ComputeIntersectionPolygonPoints()
    {
        for (const vector<IntersectionTest>& intersectionTestRow : m_intersectionTests)
        {
            for (const IntersectionTest& intersectionTest : intersectionTestRow)
            {
                if (!intersectionTest.bIntersect)
                    continue;

                if (intersectionTest.fa >= 0 && intersectionTest.fa <= 1 &&
                    intersectionTest.fb >= 0 && intersectionTest.fb <= 1)
                {
                    const auto& [beginA, endA] = intersectionTest.edgeA;
                    m_intersectionPolygonPoints.emplace_back(beginA + (endA - beginA) * intersectionTest.fa);
                }
            }
        }

        for (int aIdx{ 0 }; aIdx < m_edgesA.size(); ++aIdx)
        {
            uint8_t intersectionCases{ 0 };
            for (int bIdx{ 0 }; bIdx < m_edgesB.size(); ++bIdx)
            {
                const IntersectionTest& test = m_intersectionTests[aIdx][bIdx];
                if (!test.bIntersect)
                    continue;

                if (test.fb >= 0 && test.fb <= 1)
                {
                    if (test.fa > 1)
                        intersectionCases |= 0b001; // end point is inside the intersection
                    else if (test.fa < 0)
                        intersectionCases |= 0b100; // start point is inside the intersection
                    else
                        intersectionCases |= 0b010;

                    if (intersectionCases == 0b110 || intersectionCases == 0b101)
                    {
                        const auto& [beginA, endA] = m_edgesA[aIdx];
                        m_intersectionPolygonPoints.emplace_back(beginA);
                        break;
                    }
                    if (intersectionCases == 0b011 || intersectionCases == 0b101)
                    {
                        const auto& [beginA, endA] = m_edgesA[aIdx];
                        m_intersectionPolygonPoints.emplace_back(endA);
                        break;
                    }
                }
            }
        }

        for (int bIdx{ 0 }; bIdx < m_edgesB.size(); ++bIdx)
        {
            uint8_t intersectionCases{ 0 };
            for (int aIdx{ 0 }; aIdx < m_edgesA.size(); ++aIdx)
            {
                const IntersectionTest& test = m_intersectionTests[aIdx][bIdx];
                if (!test.bIntersect)
                    continue;

                if (test.fa >= 0 && test.fa <= 1)
                {
                    if (test.fb > 1)
                        intersectionCases |= 0b001; // end point is inside the intersection
                    else if (test.fb < 0)
                        intersectionCases |= 0b100; // start point is inside the intersection
                    else
                        intersectionCases |= 0b010;

                    if (intersectionCases == 0b110 || intersectionCases == 0b101)
                    {
                        const auto& [beginB, endB] = m_edgesB[bIdx];
                        m_intersectionPolygonPoints.emplace_back(beginB);
                        break;
                    }
                    if (intersectionCases == 0b011 || intersectionCases == 0b101)
                    {
                        const auto& [beginB, endB] = m_edgesB[bIdx];
                        m_intersectionPolygonPoints.emplace_back(endB);
                        break;
                    }
                }
            }
        }

        for (int i{ int(m_intersectionPolygonPoints.size()) - 1 }; i >= 0; --i)
        {
            const vec2f& lhs = m_intersectionPolygonPoints[i];
            if (find_if(
                m_intersectionPolygonPoints.begin(), m_intersectionPolygonPoints.begin() + i, [lhs](const vec2f& rhs)
                {
                    return IsNearlyZero(vec2f::getSize(rhs - lhs));
                }) != m_intersectionPolygonPoints.begin() + i)
            {
                m_intersectionPolygonPoints.erase(m_intersectionPolygonPoints.begin() + i);
            }
        }

        CorrectPolygonWindingOrder(m_intersectionPolygonPoints);
    }

    float ComputeIntersectionPolygonArea()
    {
        float intersectionArea{};
        for (int i{ 2 }; i < m_intersectionPolygonPoints.size(); ++i)
        {
            const vec2f& triangleEdge1 = m_intersectionPolygonPoints[0] - m_intersectionPolygonPoints[i];
            const vec2f& triangleEdge2 = m_intersectionPolygonPoints[0] - m_intersectionPolygonPoints[i - 1];
            const vec2f& triangleEdge3 = m_intersectionPolygonPoints[i - 1] - m_intersectionPolygonPoints[i];

            const float triangleSide1Length = vec2f::getSize(triangleEdge1);
            const float triangleSide2Length = vec2f::getSize(triangleEdge2);
            const float triangleSide3Length = vec2f::getSize(triangleEdge3);

            const float triangleSemiPerimeter = (triangleSide1Length + triangleSide2Length + triangleSide3Length) * 0.5f;
            intersectionArea += sqrt(
                triangleSemiPerimeter *
                (triangleSemiPerimeter - triangleSide1Length) *
                (triangleSemiPerimeter - triangleSide2Length) *
                (triangleSemiPerimeter - triangleSide3Length));
        }

        return intersectionArea;
    }

    static IntersectionTest CalculateIntersection(const tuple<vec2f, vec2f>& a, const tuple<vec2f, vec2f>& b)
    {
        IntersectionTest result{ a, b, 0.f, 0.f, false };

        const auto& [beginA, endA] = a;
        const auto& [beginB, endB] = b;
        const vec2f aTranslation = endA - beginA;
        const vec2f bTranslation = endB - beginB;

        const std::vector<float> cramerMatrix{
            aTranslation.x, -bTranslation.x, beginB.x - beginA.x,
                aTranslation.y, -bTranslation.y, beginB.y - beginA.y };

        const float fa_divident{ cramerMatrix[2] * cramerMatrix[4] - cramerMatrix[1] * cramerMatrix[5] };
        const float fa_divisor{ cramerMatrix[0] * cramerMatrix[4] - cramerMatrix[1] * cramerMatrix[3] };

        const float fb_divident{ cramerMatrix[0] * cramerMatrix[5] - cramerMatrix[2] * cramerMatrix[3] };
        const float fb_divisor{ cramerMatrix[0] * cramerMatrix[4] - cramerMatrix[1] * cramerMatrix[3] };


        if (IsNearlyZero(fa_divisor) || IsNearlyZero(fb_divisor))
            return result;

        result.fa = fa_divident / fa_divisor;
        result.fb = fb_divident / fb_divisor;
        result.bIntersect = true;

        return result;
    }

    static void CorrectPolygonWindingOrder(vector<vec2f>& polygon)
    {
        float minX{ FLT_MAX }, maxX{ -FLT_MAX }, minY{ FLT_MAX }, maxY{ -FLT_MAX };

        for (const vec2f& p : polygon)
        {
            minX = min(minX, p.x);
            minY = min(minY, p.y);
            maxX = max(minX, p.x);
            maxY = max(maxY, p.y);
        }
        const vec2f estimatedOrigin{ minX + (maxX - minX) * 0.5f, minY + (maxY - minY) * 0.5f };

        sort(polygon.begin(), polygon.end(), [estimatedOrigin](const vec2f& a, const vec2f& b)
            {
                const vec2f& dirA{ vec2f::getNormal(a - estimatedOrigin) };

                const float dotUpA = dirA.x * up.x + dirA.y * up.y;
                const float detUpA = dirA.x * up.y - dirA.y * up.x;
                const float angleA = atan2(detUpA, dotUpA);

                const vec2f& dirB{ vec2f::getNormal(b - estimatedOrigin) };

                const float dotUpB = dirB.x * up.x + dirB.y * up.y;
                const float detUpB = dirB.x * up.y - dirB.y * up.x;
                const float angleB = atan2(detUpB, dotUpB);

                return angleA < angleB;
            });
    }

    vector<vec2f> m_verticesA;
    vector<vec2f> m_verticesB;

    vector<tuple<vec2f, vec2f>> m_edgesA;
    vector<tuple<vec2f, vec2f>> m_edgesB;

    vector<vector<IntersectionTest>> m_intersectionTests;

    vector<vec2f> m_intersectionPolygonPoints;
};

struct IntersectionTest
{
    const tuple<vec2f, vec2f>& edgeA;
    const tuple<vec2f, vec2f>& edgeB;
    float fa{ 0.f };
    float fb{ 0.f };
    bool bIntersect{ false };
};

IntersectionTest CalculateIntersection(const tuple<vec2f, vec2f>& a, const tuple<vec2f, vec2f>& b)
{
    IntersectionTest result{ a, b, 0.f, 0.f, false };

    const auto& [beginA, endA] = a;
    const auto& [beginB, endB] = b;
    const vec2f aTranslation = endA - beginA;
    const vec2f bTranslation = endB - beginB;

    const std::vector<float> cramerMatrix{
        aTranslation.x, -bTranslation.x, beginB.x - beginA.x,
            aTranslation.y, -bTranslation.y, beginB.y - beginA.y };

    const float fa_divident{ cramerMatrix[2] * cramerMatrix[4] - cramerMatrix[1] * cramerMatrix[5] };
    const float fa_divisor{ cramerMatrix[0] * cramerMatrix[4] - cramerMatrix[1] * cramerMatrix[3] };

    const float fb_divident{ cramerMatrix[0] * cramerMatrix[5] - cramerMatrix[2] * cramerMatrix[3] };
    const float fb_divisor{ cramerMatrix[0] * cramerMatrix[4] - cramerMatrix[1] * cramerMatrix[3] };


    if (IsNearlyZero(fa_divisor) || IsNearlyZero(fb_divisor))
        return result;

    result.fa = fa_divident / fa_divisor;
    result.fb = fb_divident / fb_divisor;
    result.bIntersect = true;

    return result;
}

void CorrectPolygonWindingOrder(vector<vec2f>& polygon)
{
    float minX{ FLT_MAX }, maxX{ -FLT_MAX }, minY{ FLT_MAX }, maxY{ -FLT_MAX };

    for (const vec2f& p : polygon)
    {
        minX = min(minX, p.x);
        minY = min(minY, p.y);
        maxX = max(minX, p.x);
        maxY = max(maxY, p.y);
    }
    const vec2f estimatedOrigin{ minX + (maxX - minX) * 0.5f, minY + (maxY - minY) * 0.5f };

    sort(polygon.begin(), polygon.end(), [estimatedOrigin](const vec2f& a, const vec2f& b)
        {
            const vec2f& dirA{ vec2f::getNormal(a - estimatedOrigin) };

            const float dotUpA = dirA.x * up.x + dirA.y * up.y;
            const float detUpA = dirA.x * up.y - dirA.y * up.x;
            const float angleA = atan2(detUpA, dotUpA);

            const vec2f& dirB{ vec2f::getNormal(b - estimatedOrigin) };

            const float dotUpB = dirB.x * up.x + dirB.y * up.y;
            const float detUpB = dirB.x * up.y - dirB.y * up.x;
            const float angleB = atan2(detUpB, dotUpB);

            return angleA < angleB;
        });
}

int main()
{
    PolygonIntersectionAreaSolver solver;

    cout << ceil(solver.CalculatePolygonIntersectionArea());

    return 0;
}

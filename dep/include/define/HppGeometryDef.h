#pragma once
#include <vector>
#define MAX_NUM_3D 128
#define MAX_NUM_2D 128

#define ABS_DIFF(x, y) (x>=y ? (x-y) : (y-x))
inline double square(double a) { return a * a; }
#pragma pack(push,1)
namespace HPP
{
    struct Point2d
    {
        double x, y;
        int id;
        Point2d() :x(0.), y(0.), id(-1) {}
        Point2d(double mx, double my, int iid = -1) : x(mx), y(my), id(iid) {}

        bool operator == (const Point2d& p1)
        {
            return (ABS_DIFF(x, p1.x) < 1e-6) && (ABS_DIFF(y, p1.y) < 1e-6);
        }
        bool operator != (const Point2d& p1)
        {
            return (ABS_DIFF(x, p1.x) >= 1e-6) || (ABS_DIFF(y, p1.y) >= 1e-6);
        }
        Point2d operator - (const Point2d& p)
        {
            return Point2d(x - p.x, y - p.y, -1);
        }
    };

    struct Point3d
    {
        double x, y, z;
        int id;
        Point3d() :x(0.), y(0.), z(0.), id(-1) {}
        Point3d(double mx, double my, double mz, int iid = -1) : x(mx), y(my), z(mz), id(iid) {}
    };

    struct Point2dSet
    {
        Point2d data[MAX_NUM_2D];
        unsigned int nPoints;
        Point2dSet() : nPoints(0) {}
    };

    struct Point2dPack
    {
        Point2dSet set[2];
        char frameid;
        unsigned long long timestampUs;
    };

    struct Point3dSet
    {
        Point3d data[MAX_NUM_3D];
        unsigned int nPoints;
        Point3dSet() : nPoints(0) {}
    };

    struct Point3dPack
    {
        Point3dSet set[2];
        char frameid;
        unsigned long long timestampUs;
    };

    struct Roi
    {
        int x, y;
        int width, height;
        Roi() : x(0), y(0), width(0), height(0) {}
        Roi(int x_, int y_, int w_, int h_) :
            x(x_), y(y_), width(w_), height(h_) {}
    };

    struct RoiPack
    {
        Roi set[2];
    };

    struct Line
    {
        double a, b, c;
    };

    struct Triangle
    {
        HPP::Point2d node[3];
        Triangle() {};
        Triangle(HPP::Point2d p0, HPP::Point2d p1, HPP::Point2d p2)
        {
            node[0] = p0; node[1] = p1; node[2] = p2;
        }
    };

    struct SurfaceArea
    {
        // 0-------1
        // |       |
        // |       |
        // 3-------2
        Point3d corner[4];
    };

    struct Volume
    {
        SurfaceArea slice[3];
    };
}

//struct MultiPoint2dPack
//{
//    HPP::Point2dPack observ;
//    HPP::Point2dPack pred0;
//    HPP::Point2dPack pred1;
//};
//typedef void (*p2dPackListener)(const MultiPoint2dPack*);

#pragma pack(pop)
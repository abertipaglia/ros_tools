#ifndef ros_tools_SPLINE_H
#define ros_tools_SPLINE_H

#include <third_party/tkspline.h>

#include <Eigen/Dense>

namespace RosTools
{
    class Spline2D
    {

    public:
        Spline2D(const std::vector<double> &x, const std::vector<double> &y);
        Spline2D(const tk::spline &x, const tk::spline &y, const std::vector<double> &s_vector);

        Eigen::Vector2d getPoint(double s) const;
        Eigen::Vector2d getVelocity(double s) const;
        Eigen::Vector2d getAcceleration(double s) const;

        /** @brief Check the entire spline for the closest point */
        void findClosestPoint(const Eigen::Vector2d &point, int &segment_out, double &s_out) const;

        void getParameters(int segment_index,
                           double &ax, double &bx, double &cx, double &dx,
                           double &ay, double &by, double &cy, double &dy) const;

        int numSegments() const { return _x_spline.m_x_.size() - 1; }
        double getSegmentStart(int index) const { return _s_vector[index]; };
        // double getSegmentEnd(int index) const { return _s_vector[index]; };
        double length() const { return _s_vector.back(); }

        tk::spline &getXSpline() { return _x_spline; }
        tk::spline &getYSpline() { return _y_spline; }

    private:
        tk::spline _x_spline, _y_spline;

        std::vector<double> _s_vector; // Holds distances at which each spline begins

        void computeDistanceVector(const std::vector<double> &x, const std::vector<double> &y, std::vector<double> &out);

        double findClosestSRecursively(const Eigen::Vector2d &point, double low, double high, int num_recursions) const;
    };
}

#endif // MPC_PLANNER_UTIL_SPLINE_H
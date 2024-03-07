#ifndef ros_tools_VISUALS_H
#define ros_tools_VISUALS_H

#define VISUALS Visuals::get()

// Still requires ROS support for now, but can be modified to use other visualisation tools

// #define MPC_PLANNER_ROS 1

#ifdef MPC_PLANNER_ROS

#include <ros_tools/ros_visuals.h>
#include <unordered_map>

class Visuals
{
public:
    static Visuals &get()
    {
        static Visuals instance;
        return instance;
    }

    void init(ros::NodeHandle *nh)
    {
        _name = ros::this_node::getName();
        _nh = nh;
    }

    RosTools::ROSMarkerPublisher &getPublisher(const std::string &topic_name)
    {
        if (_publishers.find(topic_name) == _publishers.end())
        {
            std::string node_topic_name = _name + std::string("/") + topic_name;
            _publishers.insert(std::make_pair(topic_name, RosTools::ROSMarkerPublisher(*_nh, node_topic_name.c_str(), frame_id, 10)));
        }
        return _publishers.at(topic_name); // Retrieve the publisher from the map
    }

private:
    std::unordered_map<std::string, RosTools::ROSMarkerPublisher> _publishers;

    std::string _name{""};
    ros::NodeHandle *_nh; // ROS1

    std::string frame_id{"map"};

    Visuals() {}                                  // Private constructor to prevent instantiation
    ~Visuals() {}                                 // Private destructor to prevent deletion
    Visuals(const Visuals &) = delete;            // Delete copy constructor
    Visuals &operator=(const Visuals &) = delete; // Delete assignment operator
};

#else
#include <ros_tools/ros_visuals.h>
#include <rclcpp/rclcpp.hpp>

class Visuals
{
public:
    static Visuals &get()
    {
        static Visuals instance;
        return instance;
    }

    void init(rclcpp::Node *node)
    {
        _node = node;
    }

    RosTools::ROSMarkerPublisher &getPublisher(std::string topic_name)
    {
        if (_publishers.find(topic_name) == _publishers.end())
        {
            std::string node_topic_name = _node->get_name() + std::string("/") + topic_name;
            _publishers.insert(std::make_pair(topic_name, RosTools::ROSMarkerPublisher(_node, node_topic_name.c_str(), frame_id, 10)));
        }
        return _publishers.at(topic_name); // Retrieve the publisher from the map
    }

private:
    std::unordered_map<std::string, RosTools::ROSMarkerPublisher> _publishers;

    rclcpp::Node *_node{nullptr}; // Necessary for ROS2

    std::string frame_id{"map"};

    Visuals() {}                                  // Private constructor to prevent instantiation
    ~Visuals() {}                                 // Private destructor to prevent deletion
    Visuals(const Visuals &) = delete;            // Delete copy constructor
    Visuals &operator=(const Visuals &) = delete; // Delete assignment operator
};
#endif

#endif // VISUALS_H

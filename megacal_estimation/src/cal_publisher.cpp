


#include <sstream>
#include <ros/ros.h>
#include <yaml-cpp/yaml.h>
#include <tf/transform_listener.h>
#include <tf/transform_broadcaster.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseStamped.h>
#include <megacal_estimation/CameraPose.h>

using namespace megacal_estimation;


void operator >> (const YAML::Node& node, geometry_msgs::)
{
  node["camera_id"]        >> cam_pose.camera_id;
  node["position"]["x"]    >> cam_pose.position.x;
  node["position"]["y"]    >> cam_pose.position.y;
  node["position"]["z"]    >> cam_pose.position.z;
  node["orientation"]["x"] >> cam_pose.orientation.x;
  node["orientation"]["y"] >> cam_pose.orientation.y;
  node["orientation"]["z"] >> cam_pose.orientation.z;
  node["orientation"]["w"] >> cam_pose.orientation.w;
}

void generateCameraList(const std::string& cam_yaml, std::vector<CameraPose>& cameras)
{
  std::stringstream cal_yaml_stream(cal_yaml);
  YAML::Parser parser(cal_yaml_stream);
  YAML::Node doc;
  parser.GetNextDocument(doc);
  cameras.resize(doc.size());
  for(unsigned int i=0; i<doc.size(); i++)
    doc[i] >> cameras[i];
}

struct CameraConfig
{
  std::string calibrated_frame;
  std::string transform_child;
  std::string transform_parent;
};

void generateConfigList(const std::string& config_yaml, std::vector<CameraConfig>& camera_configs)
{

}

class CalPublisher
{
private:
  ros::NodeHandle nh_;
  ros::TransformListener listener_;
  std::vector<CameraPose> cameras_;
  std::vector<geometry_msgs::PoseStamped> transform_cache_;
  std::vector<bool> transform_found_;
  boost::mutex mutex_;

public:
  CalPublisher()
  {
    ros::NodeHandle pnh("~");

    // Extract the cameras poses from the yaml string on param server
    std::string cal_yaml;
    if(!pnh.getParam("cal_estimate", cal_yaml))
      ROS_FATAL("Could not find parameter [~cal_estimate]. Shutting down cal_publisher node");
    generateCameraList(cal_yaml, cameras_);

    // Extract the configuration for each of the calibrated cameras

    // Get the root frame from the calibrated transforms
    std::string root_id;
    pnh.param<std::string>("root_id", root_id, "world_frame");

    transform_cache_.resize(cameras_.size());
    transform_found_.resize(cameras_.size(), false);

    timer_ = nh_.createTimer(ros::Duration(1.0), boost::bind(&CalPublisher::publishTF, this, _1));
  }

private:
  void publishTF(const ros::TimerEvent& event)
  {
    for (unsigned int i=0; i < transform_found_.size(); i++)
    {
      if (!transform_found_)
      {
        // See if we can generate this transform
        if (listener_.frameExists(cameras_[i].camera_id))
        {
          while(true)
          {

          }
        }
      }
      else
        transform_cache_[i].header.stamp = event.current_real;
    }
  }
};




int main(int argc, char** argv)
{
  ros::init(argc, argv, "cal_publisher");
  CalPublisher cal_publisher;
  ros::spin();
}




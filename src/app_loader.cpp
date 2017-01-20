/**
Software License Agreement (BSD)
\file      app_loader.cpp
\authors Xuefeng Chang <changxuefengcn@163.com>
\copyright Copyright (c) 2016, the micROS Team, HPCL (National University of Defense Technology), All rights reserved.
Redistribution and use in source and binary forms, with or without modification, are permitted provided that
the following conditions are met:
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the
   following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
   following disclaimer in the documentation and/or other materials provided with the distribution.
 * Neither the name of micROS Team, HPCL, nor the names of its contributors may be used to endorse or promote
   products derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WAR-
RANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, IN-
DIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <iostream>
#include <string>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <stack>
#include <map>
#include <set>
#include <queue>
#include <algorithm>
#include <functional>

#include <ros/ros.h>
#include <pluginlib/class_list_macros.h>

#include "micros_swarm_framework/app_loader.h"

namespace micros_swarm_framework{

    AppLoader::AppLoader()
    {
        ros::NodeHandle private_nh("~");
        ros::NodeHandle nh;

        //get app name and type
        private_nh.param("app_name", app_name_, std::string("app_demo"));
        private_nh.param("app_type", app_type_, std::string("micros_swarm_framework/AppDemo"));

        ros::ServiceClient client = nh.serviceClient<micros_swarm_framework::AppLoad>("micros_swarm_framework_load_app");
        micros_swarm_framework::AppLoad srv;
        srv.request.name = app_name_;
        srv.request.type = app_type_;
        if (client.call(srv))
        {
            ROS_INFO("App %s loaded successfully.", app_name_.c_str());
        }
        else
        {
            ROS_ERROR("Failed to load App %s.", app_name_.c_str());
        }
    }

    AppLoader::~AppLoader()
    {
        ros::NodeHandle nh;

        ros::ServiceClient client = nh.serviceClient<micros_swarm_framework::AppUnload>("micros_swarm_framework_unload_app");
        micros_swarm_framework::AppUnload srv;
        srv.request.name = app_name_;
        srv.request.type = app_type_;

        if (client.call(srv))
        {
            ROS_INFO("App %s was unloaded successfully.", app_name_.c_str());
        }
        else
        {
            ROS_ERROR("Failed to unload App %s.", app_name_.c_str());
        }
    }
};



/**
Software License Agreement (BSD)
\file      communication_interface.h
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

#ifndef COMMUNICATION_INTERFACE_H_
#define COMMUNICATION_INTERFACE_H_

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

#include "ros/ros.h"

#ifdef ROS
#include "micros_swarm_framework/MSFPPacket.h"
#endif

#ifdef OPENSPLICE_DDS
#include "opensplice_dds/MSFPPacket.h"
#include "opensplice_dds/check_status.h"
#include "opensplice_dds/publisher.h"
#include "opensplice_dds/subscriber.h"
#endif

namespace micros_swarm_framework{

    class CommunicationInterface{
        public:   
            std::string name_;
            std::queue<MSFPPacket> in_queue_;
            std::queue<MSFPPacket> out_queue_;
            
            virtual void broadcast(micros_swarm_framework::MSFPPacket msfp_packet)=0;
            virtual void receive(void (*callback)(const MSFPPacket& packet))=0;
    };
    
    class ROSCommunication : public CommunicationInterface{
        private:
            ros::NodeHandle node_handle_;
            ros::Publisher packet_publisher_;
            ros::Subscriber packet_subscriber_;
        public:
            ROSCommunication(ros::NodeHandle node_handle)
            {
                name_="ROS";
                node_handle_=node_handle;
                packet_publisher_ = node_handle_.advertise<micros_swarm_framework::MSFPPacket>("/micros_swarm_framework_topic", 1000, true);
            }
            
            void broadcast(micros_swarm_framework::MSFPPacket msfp_packet)
            {
                //packet_publisher_ = node_handle_.advertise<micros_swarm_framework::MSFPPacket>("/micros_swarm_framework_topic", 1000, true);
        
                static bool flag=false;
                if(!flag)
                {
                    ros::Duration(1).sleep();
                    if(!packet_publisher_)
                    {
                        ROS_INFO("packet_publisher could not initialize");
                    }
                    flag=true;
                }
        
                if(ros::ok())
                {
                    packet_publisher_.publish(msfp_packet);
                }
            }
            
            void receive(void (*callback)(const MSFPPacket& packet))
            {
                packet_subscriber_ = node_handle_.subscribe("/micros_swarm_framework_topic", 1000, callback, ros::TransportHints().udp());
            }
    };
    
};
#endif
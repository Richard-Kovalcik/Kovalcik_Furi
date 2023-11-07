# Copyright 2016 Open Source Robotics Foundation, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import rclpy
import socket
import time
import ctypes

from rclpy.node import Node

from std_msgs.msg import String

#my custom message works!!
#have to source install/setup.sh before running code
from custom_msgs.msg import FuriStruct

class MinimalPublisher(Node):

    def __init__(self):
        super().__init__('minimal_publisher')
        self.publisher_ = self.create_publisher(FuriStruct, 'Angular_Velocity', 10)
        #self.publisher_ = self.create_publisher(String, 'Angular_Velocity', 10)
        
def main(args=None):
    rclpy.init(args=args)
    
    minimal_publisher = MinimalPublisher()
    
    #create String for output for now
    #msg = String()
    msg = FuriStruct()
    
    #create struct for storing recieved UDP data
    class ExampleStruct(ctypes.Structure):
     _pack_ = 1
     _fields_ = [
                ('timestamp', ctypes.c_uint32),
                ('index', ctypes.c_uint32),
                ('extremely_important_data', ctypes.c_uint32),
                
                # motor 1
                ('counter_1', ctypes.c_double),
                ('revolutions_1', ctypes.c_double),
                ('recent_ts_1', ctypes.c_double),
                ('av_1', ctypes.c_double),
                # motor 2
                ('counter_2', ctypes.c_double),
                ('revolutions_2', ctypes.c_double),
                ('recent_ts_2', ctypes.c_double),
                ('av_2', ctypes.c_double),
                # motor3
                ('counter_3', ctypes.c_double),
                ('revolutions_3', ctypes.c_double),
                ('recent_ts_3', ctypes.c_double),
                ('av_3', ctypes.c_double),
                # motor 4
                ('counter_4', ctypes.c_double),
                ('revolutions_4', ctypes.c_double),
                ('recent_ts_4', ctypes.c_double),
                ('av_4', ctypes.c_double),
                #motor 5
                ('counter_5', ctypes.c_double),
                ('revolutions_5', ctypes.c_double),
                ('recent_ts_5', ctypes.c_double),
                ('av_5', ctypes.c_double),
                #motor 6
                ('counter_6', ctypes.c_double),
                ('revolutions_6', ctypes.c_double),
                ('recent_ts_6', ctypes.c_double),
                ('av_6', ctypes.c_double)
                ]
    
    # statistics
    msgs_received_count = 0

    # Define the IP address and port to listen on
    #192.168.1.6 when at drone lab
    #192.168.0.74 when at home
    ip_address = '192.168.1.6'  

    # Ensure this matches with port set on ESP32 via Wireshark
    port = 2222  

    # Create a UDP socket
    udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    # Bind the socket to the IP address and port
    udp_socket.bind((ip_address, port))
  
    #main loop
    try:
        while True:
            
            data, addr = udp_socket.recvfrom(4096) # Get the data and address from sending machine
            msgs_received_count = msgs_received_count + 1
            result = ExampleStruct.from_buffer_copy(data) # Unpack the data into the struct ExampleStruct
            
            #The custom struct works!!!, only lowercase vars 
            #Cannot divide and store message at the same time
            msg.timestamp = result.timestamp #/ 1000000.0
            msg.index = result.index
            msg.garbage_data = result.extremely_important_data
            #most recent interrupt timestamp
            msg.recent_ts_1 = result.recent_ts_1
            msg.recent_ts_2 = result.recent_ts_2
            msg.recent_ts_3 = result.recent_ts_3
            msg.recent_ts_4 = result.recent_ts_4
            msg.recent_ts_5 = result.recent_ts_5
            msg.recent_ts_6 = result.recent_ts_6
            #motor angular velocity
            msg.angular_velocity_1 = result.av_1
            msg.angular_velocity_2 = result.av_2
            msg.angular_velocity_3 = result.av_3
            msg.angular_velocity_4 = result.av_4
            msg.angular_velocity_5 = result.av_5
            msg.angular_velocity_6 = result.av_6
            
            
            current_time = time.time()
            #It works!!!
            #msg.data = str(result.av_1)
            minimal_publisher.publisher_.publish(msg)

            #node will always be running
            time.sleep(0.1)
    
    except KeyboardInterrupt:
        msg.data = 'UDP listener stopped.'
    
    finally:
        udp_socket.close()
        rclpy.spin(minimal_publisher)
        # Destroy the node explicitly
        # (optional - otherwise it will be done automatically
        # when the garbage collector destroys the node object)
        minimal_publisher.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
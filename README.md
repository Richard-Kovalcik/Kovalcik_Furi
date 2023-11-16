# Kovalcik_Furi

**Real-Time Force and Moment Feedback of a Hexarotor Drone**  

**poster** 
![image](https://github.com/Richard-Kovalcik/Kovalcik_Furi/assets/113212733/0463e30a-f391-4bee-8d32-4f3a26d5d2aa)  

**ESP32**  
To program the ESP32 with the provided main.c you will need to have the VScode extension Platform.io installed. The ESP32 used was :upesy_wroom.   
*A brief explanation of the main.c*
I begin by creating 4  arrays for storing the number of pulses triggered, the converted data in radians, the time data, and the velocity data. I then create a packed struct for storing the data that I want to transmit over UDP. I then configure my IP adddress and the password of the desired router. **Make sure the port number is the same in this code and in the ros2 publisher** I then create a function for convertiong the gathered values into a velocity in radians per second. After this, I create 6 interrupt service routines that handle the data collection. **Make sure to add IRAM_ATTR to designate a memory location for storing the data**. In the setup I attact all of the interrupt routines to the rising edge of each pin. I initiate a wifi connection. In my main loop I print the data for debugging purposes and write the data over UDP using the Arduino UDP library. 
**data_processing**  

**ros2**  

**test_data**


# SmartParkArea

![one-pager](https://github.com/bounIoT/SmartParkArea/blob/master/Images/iot-one-pager.png)

Car parks are not automated for pricing, billing and payment processes. For billing and payment procedures, parks rely on people, counters and receipts. In a car park, paying the bill manually cost time and effort for one. Car park have to deal with payment, time keeping and counter operation for 24-hour with a number of employees on different shifts.

We, Kemal Tulum and Batuhan Enes Çağlayan, have a solution for this. We can detect the plate of the car in a slot with image recognition technologies and bill the car for the duration spent in the slot. The bill and payment options can be provided over a smartphone application or web servers. 

## About the System

| Component  | Action |
| ------------- | ------------- |
| Ultrasonic Sensor  | Measures the distance to car to detect slot status |
| OV7670 Camera | Captures images of car plates for plate recognition purposes |
| Arduino Yun | Provides power for circuit and communicates with cloud |
| Arduino IDE | Used to configure Arduino Yun |
| IBM Cloud | Used as a cloud platform to receive, store and send data |
| Node-RED | Used as a cloud server to visualize data |


## IoT Architecture

![architecture](https://i.ibb.co/C6ggK9n/iotottt.jpg)

## Circuit Diagram

![circuit](https://i.ibb.co/XLhBz5L/iotproject-bb.png)

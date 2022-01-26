This project is meant to turn any Arduino or clone into a powerful I2C motor controller with dual encoder support. It supports a variety of H-Bridge chips and modules as long as they use they use two direction pins and one PWM pin per channel. Most analog motor driver chips you can find work like that. 

Here are my recommendations:

* **L298** You can usually find breakout boards for these dual 2A motor drivers at around 2-5$ everywhere around the world. Here's an [example](https://www.aliexpress.com/item/1005001621936295.html) from Aliexpress. They make a great pair with TT style motors, and other low-current options. However these chips are very sensitive to loads over the 2A limit.

* **TB6612FNG**
	At arount the same price as the L298, [these modules](https://shop.pimoroni.com/products/tb6612fng-dual-motor-driver-carrier
) have a peak current of 3.2A. ... making them a great to pair with Pololu Micro Motors / N20 Motors. 
	
* **VNH30x Series**
	Originally designed for powersteering and other automotive uses, the is a great option for large(r) motors. Each chip can support up to 15A and 30A peak loads on one channel. Additionally these chips have an analog output pin called **CS**, or **C**urrent **S**ense. You can read the voltage on this pin to see how much current each motor uses. This will be supported in later versions of the firmware. There are a few breakout boards on the market, such as [this one](https://www.pololu.com/product/707) by Pololu.

Motorhead is also available as a package for robot-block, offering a plug-and-play ROS experience.

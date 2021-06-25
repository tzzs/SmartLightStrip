# Smart Light Strip

[TOC]

> ESP8266 (LiLon NodeMcu V3)



The port diagram below is for reference. Picture from this [site](https://www.instructables.com/Getting-Started-With-ESP8266LiLon-NodeMCU-V3Flashi/).

![img](https://content.instructables.com/ORIG/FCL/V0OG/IVO7W206/FCLV0OGIVO7W206.jpg?auto=webp&frame=1&width=678&fit=bounds&md=14cee154242999ffcbd64652017d4a3a)



## HOW TO USE BLINKER TO CREATE A SMART LIGHT STRIP

### 1. Install Arduino

You cloud download the Arduino IDE from this website [Software | Arduino](https://www.arduino.cc/en/software).

![image-20210625004937487](https://cos5-1255991898.cos.ap-chongqing.myqcloud.com/tk/image-20210625004937487.png)

### 2. Download library

Click Project -> Load Library -> Manage Library, then enter Blinker to download the library.

Use the same method to download Adafruit_NeoPixel  and ArduinoJson library.



​		At this step, the environment setup has been completed. And it will be more interesting next.



### 3. Download code for the project

```shell
git clone https://github.com/tzzs/SmartLightStrip.git
```

​		Then you have to use Arduino to open this folder or project.

### 4. Download blinker application

Android APK: http://ccdn.diandeng.tech/apk/blinker-2.5.2-beta.apk

IOS APP: https://apps.apple.com/cn/app/id1498805902 

![Screenshot_2021-06-25-01-07-52-454_iot.clz.me](https://cos5-1255991898.cos.ap-chongqing.myqcloud.com/tk/Screenshot_2021-06-25-01-07-52-454_iot.clz.me.jpg)

​		

### 5. Get Secret Key from application

Click the plus sign in the upper right corner, then select standalone device, select network access, and select a communication service provider. A secret key will be given at this time(may need to wait a few seconds), remember it.



### 6. Configure WIFI

Fill in the obtained information into the code.

```c
char auth[] = ""; 		// app secret key
char ssid[] = "";    	// wifi ssid/name
char pswd[] = ""; 		// wifi password
```



### 7. Installing with Boards Manager

Starting with 1.6.4, Arduino allows installation of third-party platform packages using Boards Manager. We have packages available for Windows, Mac OS, and Linux (32 and 64 bit).

- Install the current upstream Arduino IDE at the 1.8.9 level or later. The current version is on the [Arduino website](https://www.arduino.cc/en/main/software).
- Start Arduino and open the Preferences window.
- Enter `https://arduino.esp8266.com/stable/package_esp8266com_index.json` into the *File>Preferences>Additional Boards Manager URLs* field of the Arduino IDE. You can add multiple URLs, separating them with commas.
- Open Boards Manager from Tools > Board menu and install *esp8266* platform (and don't forget to select your ESP8266 board from Tools > Board menu after installation).

Boards manager link: `https://arduino.esp8266.com/stable/package_esp8266com_index.json`

Documentation: https://arduino-esp8266.readthedocs.io/en/3.0.0/



### 8. Flash the code to ESP8266.

Click Tools > Boards > ESP8266 Boards (3.0) > NodeMcu  1.0(ESP-12E)

Then click verify  and upload.



### 9. Connecting components

This is also a very important step, otherwise it may not work properly.

![Schematic_SmartLightStrip_2021-06-26](https://cos5-1255991898.cos.ap-chongqing.myqcloud.com/tk/Schematic_SmartLightStrip_2021-06-26.png)



### 10. Connecting board with Blinker APP

First of all you have to add some components  corresponding to the code function.

The switch corresponds to the Switch1 in the code.

The Brightness slider corresponds to the Slider1 in the code.

The Color picker corresponds to the WS2812 in the code.

![app](https://cos5-1255991898.cos.ap-chongqing.myqcloud.com/tk/app.jpg)



### 11. In the end

**It's over here. Enjoy it.**



### 12. Post-credits scene

The voice control function of Xiaoai based on Blinker to the code, waiting for you to explore! 
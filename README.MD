#The Project

For this project I am making a mailbox alert IoT device so my roommates (girlfriends parents) can see when the mail arrived without having to walk up their driveway. To start this project I will be using a NodeMCU V2, a tilt sensor, a raspberry pi 1/2/3, a D1 Mini, and of course the other various prototyping kit. I ended up bying everything on Amazon since the shipping was fast, and because I have prime and I don't want it to go to waste. 

The main workhorse for prototyping is the NodeMCU. Mine was just under $9 but can be found for about half the price with rediculously long shipping. The majority of this project can be completed with just this device and a sensor, but I am going to use a server and second client device to handle and display the sensor data. 

#Foreseen Obsticles

I will need a way to minimize power consumption of the sensor node at the mailbox so that battery life is as long as possible. To do this I will use a deep sleep state that is adjusted to fit the typical time it takes for a mailman to open and close the mail. This may be hard to configure correctly as I don't want to miss events due to the node sleeping. I will look into ways the wifi could be shut off when waiting for events to occur.

https://openhomeautomation.net/esp8266-battery/

#Starting Off

I quickly made prototype that activates an led by using digital pin D7 and a tilt switch. This is digitally activated after a time of 500ms to remove bounce.

Great. Now i need to start a REST endpoint and HTTP server to host the data i want to store. I am using the aRest.h library which allows me to watch the tilt state and number of tilt events from the IP address of the device. I also initialize the http server on the device with my local network credentials. For effeciency and stability sake I set the node to have a static IP. Some research showed that wifi will connect faster with a static IP over waiting for DHCP to assign from the router. Batter life spared.

NOTE: REST may not be a great way about this. I have found a way to turn off WiFi so maybe a push using MQTT to a raspberry pi server running thingsboard.io

Now when I point to the correct IP on a web browser I can see a JSON file of my device variables as well as other useful details:
{"variables": {"tilt_state": 0, "event_count": 4, "time": 1234567890}, "id": "01", "name": "mailbox", "hardware": "esp8266", "connected": true}

It seems the tilt function is working as tilt_state = 0 when the breadboard is flat on the table. Sweet. I also made the event counter only count when the tilt state changes to the 'tilted' position so that one event is counted as the door opening and closing, just like when the mailman comes by.

#Problem

Battery usage on the nodemcu is very high. Having a constant REST endpoint running over wifi will be very ineffecient. Instead I will be sending data directly to a raspberry pi server using thingboard.io as a server (open source, looks promising). ...


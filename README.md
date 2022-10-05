# Real Time Kinematics Rover Manager
### RTKRoverManager

<img align="right" src="./screenshots/RTKRoverManager.png" width="240"/>

This is a submodule of the [RTKRover](https://github.com/audio-communication-group/RTKRover).

Configure ESP WiFi credentials and other default values for a RTK rover. 
The device runs as an access point if no credentials are saved, (SSID: 
"rtkrover"+chipID, IP: 192.168.4.1). 
Fill the web form with your data, save and reboot. 
After restart the device is reachable as a client in your local network 
("rtkrover"+chipID.local or local IP).

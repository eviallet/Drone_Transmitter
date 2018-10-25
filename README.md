# Drone
Controlling a drone with Qt. Real time view of the drone's sensors with QCharts

<img src="PlotsCapture.PNG" height="450" width="800">

Receiver : https://github.com/EViallet/Drone_Receiver

Demo videos of the drone's feedback :

[![Youtube video 1](https://img.youtube.com/vi/ZbeCxRpi09s/0.jpg)](https://www.youtube.com/watch?v=ZbeCxRpi09s)

[![Youtube video 2](https://img.youtube.com/vi/V7GaL_eG6UQ/0.jpg)](https://www.youtube.com/watch?v=V7GaL_eG6UQ)

# Working
* Control the drone with a Xbox gamepad. (@see preprocessor.cpp)
* Remotely change the drone's PIDs values and save/load them from computer.
* Real time plotting of the drone's sensors. (@see realtimeplot.cpp)
* 3D vizualisation of the drone's angles (@see vizualisation.cpp)

# TODO
* Infrared sonar for altitude

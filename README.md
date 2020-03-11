# EasyMouse
## INTRODUCTION
### ABOUT
* Individuals who have disabilities that do not allow them to have fine motor function might experience difficulty operating a computer mouse precisely. Having an alternative to a computer mouse that can work accurately, easily, and in a manner not awkward to use in public is needed.

* EasyMouse is software that runs on a smart watch and computer that allows a user to control the mouse using the rotation of their wrist. The sensitivity of the mouse can be adjusted to accommodate all levels of control.

* EasyMouse was tested for accuracy with the cursor, and with clicking motions. Trials concluded with 100% accuracy with mouse movement detections, while  producing 62.75% accuracy with clicking the mouse.
## INSTRUCTIONS
### SETTING UP
##### WATCH
* Change the IP address in ``basicuieasymouse.c`` in ``...\CS528 Project Report.zip\BasicUIEasyMouse\src`` to your computers IP address. (Located on line 52)
* Using TizenStudio, upload the "BasicUIEasyMouse" application into the smart watch.
* Application should be ready to go in the "apps" menu of the smart watch.

##### COMPUTER
* Make sure Java  11+ is installed
* Run ``compile.bat`` then ``run.bat`` 
 * OR you can just run ``update.bat``
* A console should be waiting for a connection now.

### RUNNING THE APPLICATION
Once the computer is waiting for a connection, make sure the watch and computer are connected to the same Wifi network. If they are, then simply launch the application from the smart watch, and it should connect to the computer and manipulate the mouse. 

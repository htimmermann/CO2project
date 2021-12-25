# CO2project

A CO2 sensor designed to keep schools open during the COVID-19 pandemic.

<p align="center">
  <img src="https://user-images.githubusercontent.com/66896884/146073631-59923889-1681-4199-98d6-f4f7e7ee6264.png" alt="Henry's custom image"/>
</p>

## Description 

This is a guide to build a CO2 sensor for classrooms and other uses. The schematic is attached, a guide on how to build/hook up the CO2 sensor with an Aurdino Uno, along with the Python 3 code used to analyze, plot, and sort the data. There are two versions of the code, one which sorts out outliers (GITHUBVERSION.py) and one that does not (GITHUBNOTSORTED.py). Regardless of the code, points are sorted into three categories: green (0-800 PPM), yellow (800-1000 PPM), and red (1000+). Green indicates that the environment is safe to be in, yellow being a warning sign, and red indicating to leave the room.

> The schematic includes an Arduino Uno, a breadboard, a CO2 sensor, an SD card reader/writer, and a Real-time-block (RTC) 
> - Links to purchase will be in the requirements file

<p align="center">
  
  <img src="https://user-images.githubusercontent.com/66896884/147379428-e50c582e-ae83-48a4-82e6-87e3c320a853.png" width="400" height="350" /> 
  <img src="https://user-images.githubusercontent.com/66896884/147379430-aaebaf7b-8e9c-467b-88eb-7acc02c270cd.png" width="400" height="350" />
</p>


## License

Feel free to use this project for personal and commercial projects.


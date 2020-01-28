# Pitch Perfector
A microphone application which performs real-time pitch shifting

## System Diagram
![System Diagram](Documentation/Images/Schematic/Schematic.PNG)

The **ECM microphone** converts the vocal (from ~100Hz up to ~3.5kHz) input into an analoge signal  
  
The **I2S ADC** converts this to I2S format which is understood by the Raspberry Pi with a datarate of 240kbps   
  
**Digital filtering** is then performed on the input using an IIR filter implemented in C++  
  
**Frequency analysis** is performed by applying a fourier transform and identifying the base frequency    
  
**Pitch shifting** then shifts the frequency components of the signal using phase vocoding so they match the note input by the GUI or to a predetermined scale value  
  
This new shifted value is then output to a **speaker**  

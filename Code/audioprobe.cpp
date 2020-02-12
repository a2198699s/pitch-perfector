// audioprobe.cpp
#include <iostream>
#include "RtAudio.h"
int main()
{
  RtAudio audio;
  // Determine the number of devices available
  unsigned int devices = audio.getDeviceCount();
  // Scan through devices for various capabilities
  RtAudio::DeviceInfo info;
  for ( unsigned int i=0; i<devices; i++ ) {
    info = audio.getDeviceInfo( i );
    if ( info.probed == true ) {
      // Print, for example, the maximum number of output channels for each device
      std::cout << "device = " << i;
      std::cout << ": maximum output channels = " << info.inputChannels << "\n";
    }
  }
  return 0;
}

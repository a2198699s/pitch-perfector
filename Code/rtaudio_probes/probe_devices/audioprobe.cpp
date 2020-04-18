// audioprobe.cpp
#include <iostream>
#include <RtAudio.h>
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
      std::cout << "device = " << i;
      std::cout << ": Input channels = " << info.inputChannels <<  " Output channels = " << info.outputChannels << " Duplex Channels: " << info.duplexChannels << "\n";
    }
  }
  return 0;
}

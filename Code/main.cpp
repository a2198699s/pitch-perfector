#include <iostream>
#include <SFML/Audio.hpp>

using namespace std;
int main() 
{
    cout << "Welcome to Pitch Perfector!\n";
    cout << "This code has been over-written on the Raspberry Pi\n";
    
    sf::Music music;
    if (!music.openFromFile("music.wav")) 
        return -1; // error
    music.play();
    return 0;
}

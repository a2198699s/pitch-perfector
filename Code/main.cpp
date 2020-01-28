#include <iostream>
#include <SFML/Audio/Music.hpp>
 
//#include <>



int main() 
{
    std::cout << "Welcome to Pitch Perfector!!\n";
    std::cout << "This code has been over-written on the Raspberry Pi\n";
    
    //SoundBuffer buffer;
    sf::Music music;
    std::cout << "HERE\n";
    if (!music.openFromFile("TSwift.ogg")){
	std::cout << "ERROR\n";
        return -1; // error
    }
    music.openFromFile("TSwift.ogg");
    std::cout << "Playing\n";
    music.play();
    std::cin.ignore();
    return 0;
}

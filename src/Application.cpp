#include "Application.h"
#include "SFML/Graphics.hpp"

namespace TC
{
    Application::Application(const ApplicationDef& def)
        : window{sf::VideoMode{def.width, def.height}, def.name, def.style}    
    {
        
    }

    void Application::Run()
    {
         while (window.isOpen())
        {
            // check all the window's events that were triggered since the last iteration of the loop
            sf::Event event;
            while (window.pollEvent(event))
            {
                // "close requested" event: we close the window
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            
        }
    }
}
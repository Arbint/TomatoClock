#pragma once
#include "SFML/Graphics.hpp"
#include <string>
namespace TC
{
    struct ApplicationDef
    {
    public:
        unsigned int width;
        unsigned int height;
        std::string name;
        unsigned int style;
        ApplicationDef()
            : width{600}, height{800}, name{"Untitled"}, style {0}
        {

        }

        ApplicationDef(unsigned int width, unsigned int height, const std::string& name, unsigned int style)
            : width{width}, height{height}, name{name}, style{style}
        {

        }
    };

    class Application
    {
    public:
        Application(const ApplicationDef& def);
        void Run();
    private:
        void InitializeComponents();
        void Tick();
        void NativeRender();
        void Render();
        std::string GetClockTime();
    private:

        sf::RenderWindow window;
        
        sf::Clock clock;
        sf::Clock tickClock;

        sf::Sprite tomatoSprite;
        sf::Text clockText;

        float tickInterval;
    };
}
#pragma once
#include "SFML/Graphics.hpp"
namespace TC
{
    struct ApplicationDef
    {

    };

    class Application
    {
    public:
        Application(ApplicationDef def);
        void Run();
    private:
        sf::RenderWindow window;
    };
}
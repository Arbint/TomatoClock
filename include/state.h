#pragma once
#include <SFML/Graphics.hpp>
namespace TC
{
    class Application;
    class State
    {
    public:
        virtual void OnStateSet(Application* app) {};
        virtual void OnStateOff(Application* app) {};
        virtual void TickState(Application* app) {};
        virtual void Draw(sf::RenderWindow& window) {};
        virtual void HandleEvent(const sf::Event& event, Application* app){};
        ~State(){}
    protected:
        State(){};
    };
}
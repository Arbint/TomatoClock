#pragma once
#include "SFML/Graphics.hpp"
#include <string>
#include "core.h"

namespace TC
{
    class State;

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
        ~Application();
        void Run();
        bool IsMouseOnSprite(const sf::Sprite& sprite) const;
        bool IsSpriteClicked(const sf::Sprite& sprite) const;
        
        bool IncrementHour(int amt);
        bool IncrementMinute(int amt);
        bool IncrementSec(int amt);
        bool IsMouseOnButton() const;
        void SetButtonText(const std::string& newText);
        
        //start a new counting using the displayed time as the duration.
        void StartNewCounting();

        //restart the couting using the previously set counting duration.
        void ReStartCounting();

        void SwithToSettingState();
        void SwithToAleartState();

        float GetDeltaTime() const {return deltaTime; }
        
        //get a copy of current time
        std::tm GetCurrentCountingTime() const {return *countingTime;}
        void SetCurrentCountingTime(const std::tm& newTime);
        
    private:
        void SwithToCountingState();
        void InitializeComponents();
        void Tick();
        void NativeRender();
        void Render();
        void SetState(const Shared<State>& newState);
        std::string CountingTimeAsString();
        void HandleEvent(const sf::Event& event);
        
    private:
        sf::RenderWindow window;
        
        sf::Clock clock;
        sf::Clock tickClock;

        sf::Sprite tomatoSprite;
        sf::Text clockText;

        float tickInterval;

        Shared<State> settingState;
        Shared<State> countingState;
        Shared<State> alertState;
        WeakRef<State> currentState;

        sf::Sprite Button;
        sf::Text ButtonText;

        Shared<std::tm> countingTime;
        std::tm timeSetting;

        float deltaTime;
    };
}
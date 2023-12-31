#include "Application.h"
#include <format>
#include "SFML/Graphics.hpp"
#include "assetManager.h"
#include "utilities.h"
#include <time.h>
#include "state.h"
#include "settingState.h"
#include "countingState.h"
#include "alertState.h"

namespace TC
{
    Application::Application(const ApplicationDef& def)
        : window{sf::VideoMode{def.width, def.height}, def.name, def.style},
         clock{},
         tickClock{},
         tickInterval{1.f/60.f},
         tomatoSprite{},
         clockText{},
         settingState{nullptr},
         countingState{nullptr},
         alertState{nullptr},
         currentState{},
         Button{},
         ButtonText{},
         countingTime{new std::tm{}},
         timeSetting{},
         deltaTime{0.f}
    {
        InitializeComponents();
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
        clock.restart();
        tickClock.restart();
         while (window.isOpen())
        {
            // check all the window's events that were triggered since the last iteration of the loop
            sf::Event event;
            while (window.pollEvent(event))
            {
                HandleEvent(event);
            }

            if(tickClock.getElapsedTime().asSeconds() > tickInterval)
            {
                deltaTime = tickClock.restart().asSeconds();
                Tick();
                NativeRender();
            }
        }
    }

    bool Application::IsMouseOnSprite(const sf::Sprite &sprite) const
    {
        auto bound = sprite.getGlobalBounds();
        auto mousePos = sf::Mouse::getPosition(window);
        auto translatePos = window.mapPixelToCoords(mousePos);
        return bound.contains(translatePos);
    }

    bool Application::IsSpriteClicked(const sf::Sprite &sprite) const
    {
        bool isMouseOnSprite = IsMouseOnSprite(sprite);
        bool isMouseClicked = sf::Mouse::isButtonPressed(sf::Mouse::Left);

        return isMouseOnSprite && isMouseClicked;
    }

    bool Application::IncrementHour(int amt)
    {
        countingTime->tm_hour+=amt;
        if(countingTime->tm_hour<0)
        {
            countingTime->tm_hour = 0;
            return false;
        }
        return true;
    }

    bool Application::IncrementMinute(int amt)
    {
        countingTime->tm_min += amt;
        while (countingTime->tm_min>=60)
        {
            countingTime->tm_min -= 60;
            IncrementHour(1);
        }

        while(countingTime->tm_min<0)
        {
            if(IncrementHour(-1))
            {
                countingTime->tm_min+=60;
            }else
            {
                countingTime->tm_min = 0;
                return false;
            }
        }
        return true;
    }

    bool Application::IncrementSec(int amt)
    {
        countingTime->tm_sec += amt;
        while (countingTime->tm_sec>=60)
        {
            countingTime->tm_sec -= 60;
            IncrementMinute(1);
        }

        while(countingTime->tm_sec<0)
        {
            if(IncrementMinute(-1))
            {
                 countingTime->tm_sec+=60;
            }else
            {
                countingTime->tm_sec = 0;
                return false;
            }
        }

        return true;
    }

    bool Application::IsMouseOnButton() const
    {
        return IsMouseOnSprite(Button);
    }

    void Application::SetButtonText(const std::string &newText)
    {
        ButtonText.setString(newText);
        ButtonText.setOrigin(ButtonText.getGlobalBounds().width/2.f, ButtonText.getGlobalBounds().height/2.f);
        ButtonText.setPosition(Button.getPosition()-sf::Vector2f{0.f, ButtonText.getGlobalBounds().height/2.f});
    }

    void Application::StartNewCounting()
    {
        timeSetting = *countingTime;
        SwithToCountingState();
    }

    void Application::ReStartCounting()
    {
        *countingTime = timeSetting;
        SwithToCountingState();
    }

    void Application::SwithToSettingState()
    {
        SetState(settingState);
    }

    void Application::SwithToCountingState()
    {
        SetState(countingState);
    }

    void Application::SwithToAleartState()
    {
        SetState(alertState);
    }

    void Application::SetCurrentCountingTime(const std::tm &newTime)
    {
        *countingTime = newTime;
    }

    void Application::InitializeComponents()
    {
        //tomato texture
        Shared<sf::Texture> tomatoTexture = AssetManager::Get().LoadTexture("Tomato.png");
        tomatoTexture->setSmooth(true);
        tomatoSprite.setOrigin(tomatoTexture->getSize().x/2, tomatoTexture->getSize().y/2);
    
        tomatoSprite.setScale(0.5f, 0.5f);
        tomatoSprite.setPosition(window.getSize().x/2, window.getSize().y/2 - 200.f);
        tomatoSprite.setTexture(*tomatoTexture);

        //clock Font
        Shared<sf::Font> Font = AssetManager::Get().LoadFont("kenvector_future.ttf");
        clockText.setFont(*Font);
        clockText.setStyle(sf::Text::Style::Bold);
        clockText.setCharacterSize(80);
        clockText.setString("00:00:00");
        clockText.setOrigin(clockText.getGlobalBounds().width/2.f, clockText.getGlobalBounds().height/2.f);
        clockText.setPosition(window.getSize().x/2, window.getSize().y - 400.f);

        //start stop btn
        Shared<sf::Texture> btnTexture = AssetManager::Get().LoadTexture("Btn.png");
        Button.setTexture(*btnTexture);

        Button.setOrigin(btnTexture->getSize().x/2.f, btnTexture->getSize().y/2.f);
        Button.setPosition(window.getSize().x/2, window.getSize().y - 150.f);
        
        ButtonText.setFont(*Font);
        ButtonText.setStyle(sf::Text::Style::Bold);
        ButtonText.setCharacterSize(50);
        SetButtonText("Start");

        //states
        settingState = std::make_shared<SettingState>();
        SetState(settingState);

        countingState = std::make_shared<CountingState>();
        alertState = std::make_shared<AlertState>();
    }

    void Application::Tick()
    {
        if(!currentState.expired())
        {
            currentState.lock()->TickState(this);
        }

        clockText.setString(CountingTimeAsString());
    }

    void Application::NativeRender()
    {
        window.clear(sf::Color{80,109,55,255});
        Render();
        window.display();
    }

    void Application::Render()
    {
        window.draw(tomatoSprite);
        window.draw(clockText);
        window.draw(Button);
        window.draw(ButtonText);

        if(!currentState.expired())
        {
            currentState.lock()->Draw(window);
        }
    }

    void Application::SetState(const Shared<State> &newState)
    {
        if(!currentState.expired())
        {
            if(currentState.lock() == newState)
            {
                return;
            }

            currentState.lock()->OnStateOff(this);
        }

        currentState = newState;
        
        if(!currentState.expired())
        {
            currentState.lock()->OnStateSet(this);
        }
    }

    std::string Application::CountingTimeAsString()
    {
        char buffer[9];
        std::strftime(buffer, 9, "%H:%M:%S", countingTime.get());
        return std::string(buffer, buffer + 8);
    }
    
    void Application::HandleEvent(const sf::Event &event)
    {
        // "close requested" event: we close the window
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }
        
        if(!currentState.expired())
        {
            currentState.lock()->HandleEvent(event, this);
        }
    }
}
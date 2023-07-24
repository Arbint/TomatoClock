#include "Application.h"
#include <format>
#include "SFML/Graphics.hpp"
#include "assetManager.h"
#include "utilities.h"
#include <time.h>
#include "state.h"
#include "settingState.h"
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
         timeSetting{new std::tm{}}
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
        timeSetting->tm_hour+=amt;
        if(timeSetting->tm_hour<0)
        {
            timeSetting->tm_hour = 0;
            return false;
        }
        return true;
    }

    bool Application::IncrementMinute(int amt)
    {
        timeSetting->tm_min += amt;
        while (timeSetting->tm_min>=60)
        {
            timeSetting->tm_min -= 60;
            IncrementHour(1);
        }

        while(timeSetting->tm_min<0)
        {
            if(IncrementHour(-1))
            {
                timeSetting->tm_min+=60;
            }else
            {
                timeSetting->tm_min = 0;
                return false;
            }
        }
        return true;
    }

    bool Application::IncrementSec(int amt)
    {
        timeSetting->tm_sec += amt;
        while (timeSetting->tm_sec>=60)
        {
            timeSetting->tm_sec -= 60;
            IncrementMinute(1);
        }

        while(timeSetting->tm_sec<0)
        {
            if(IncrementMinute(-1))
            {
                 timeSetting->tm_sec+=60;
            }else
            {
                timeSetting->tm_sec = 0;
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
    }

    void Application::Tick()
    {
        if(!currentState.expired())
        {
            currentState.lock()->TickState(this);
        }

        clockText.setString(TimeSettingAsString());
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

    std::string Application::TimeSettingAsString()
    {
        char buffer[9];
        std::strftime(buffer, 9, "%H:%M:%S", timeSetting.get());
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
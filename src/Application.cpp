#include "Application.h"
#include <format>
#include "SFML/Graphics.hpp"
#include "assetManager.h"
#include "config.h"
#include "utilities.h"
#include <time.h>
namespace TC
{
    Application::Application(const ApplicationDef& def)
        : window{sf::VideoMode{def.width, def.height}, def.name, def.style},
         clock{},
         tickClock{},
         tickInterval{1.f/60.f},
         tomatoSprite{},
         clockText{}
    {
        
        AssetManager::Get().Init(GetResouceDir());
        InitializeComponents();
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
                // "close requested" event: we close the window
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            if(tickClock.getElapsedTime().asSeconds() > tickInterval)
            {
                Tick();
                NativeRender();
            }
        }
    }

    void Application::InitializeComponents()
    {
        //tomato texture
        Shared<sf::Texture> tomatoTexture = AssetManager::Get().LoadTexture("Tomato.png");
        tomatoTexture->setSmooth(true);
        tomatoSprite.setOrigin(tomatoTexture->getSize().x/2, tomatoTexture->getSize().y/2);
    
        tomatoSprite.setScale(0.5f, 0.5f);
        tomatoSprite.setPosition(window.getSize().x/2, window.getSize().y/2 - 100.f);
        tomatoSprite.setTexture(*tomatoTexture);

        //clock Font
        Shared<sf::Font> clockFont = AssetManager::Get().LoadFont("kenvector_future.ttf");
        clockText.setFont(*clockFont);
        clockText.setStyle(sf::Text::Style::Bold);
        clockText.setCharacterSize(80);
        clockText.setString("00:00:00");
        clockText.setOrigin(clockText.getGlobalBounds().width/2.f, clockText.getGlobalBounds().height/2.f);
        clockText.setPosition(window.getSize().x/2, window.getSize().y - 200.f);
    }

    void Application::Tick()
    {
        clockText.setString(GetClockTime());
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
    }
    std::string Application::GetClockTime()
    {
        int secondsRemaining = (int)(clock.getElapsedTime().asSeconds());
        char buffer[9];
        Shared<std::tm> timeInfo = std::make_shared<std::tm>();
        timeInfo->tm_hour = 0;
        timeInfo->tm_min = 0;
        timeInfo->tm_sec = 0;

        while(secondsRemaining - 3600 > 0)
        {
            secondsRemaining-=3600;
            timeInfo->tm_hour += 1;
        }

        while(secondsRemaining - 60 > 0)
        {
            secondsRemaining-=60;
            timeInfo->tm_min += 1;
        }

        timeInfo->tm_sec = secondsRemaining;

        
        std::strftime(buffer, 9, "%H:%M:%S", timeInfo.get());
        return std::string(buffer, buffer + 8);
    }
}
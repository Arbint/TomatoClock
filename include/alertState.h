#pragma once
#include "state.h"
#include <SFML/Audio.hpp>
namespace TC
{
    class AlertState : public State
    {
    public:
        AlertState();
        virtual void OnStateSet(Application* app) override; 
        virtual void OnStateOff(Application* app) override;
        virtual void HandleEvent(const sf::Event& event, Application* app) override;

        virtual void TickState(Application* app) override;
    private:
        sf::Sound alertSound;
        float repeatInterval;
        float repeatTimer;
    };
}
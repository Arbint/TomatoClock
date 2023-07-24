#pragma once
#include "state.h"

namespace TC
{
    class CountingState : public State
    {
    public:
        CountingState();
        virtual void HandleEvent(const sf::Event& event, Application* app) override;
        virtual void OnStateSet(Application* app) override;
        virtual void TickState(Application* app) override;
    private:
        float timeElapsed;
        std::tm timeSetting;
    };
}
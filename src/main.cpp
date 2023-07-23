#include "Application.h"
#include <memory>
int main()
{
    TC::ApplicationDef appDef{800,800, "Tomato Clock", sf::Style::Close|sf::Style::Titlebar};
    std::unique_ptr<TC::Application> app{new TC::Application{appDef}};
    app->Run();
}
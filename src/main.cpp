#include "Application.h"
#include <memory>
int main()
{
    TC::ApplicationDef appDef{600,800, "Tomato Clock", sf::Style::Default};
    std::unique_ptr<TC::Application> app{new TC::Application{appDef}};
    app->Run();
}
#include "Application.h"
#include "Engine.h"

// TODO: test
#include "Event.h"
#include "BattleMap.h"
#include "Save.h"

int main(int argc, char* argv[])
{
    // Application app;
    // return app.run();

    // auto e = Event::getInstance();
    Save::getInstance()->load(1);

    return 0;
}


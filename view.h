#pragma once

#include <SFML/Graphics.hpp>

using namespace sf;

View view;

void setPlayerCoordinateForView(float x, float y)
{
    if (x < 260) x = 260;
    if (y < 260) y = 260;
    if (x > 980) x = 980;
    if (y > 600) y = 600;

    view.setCenter(x, y);
}

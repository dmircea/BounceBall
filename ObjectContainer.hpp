#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Object.hpp"

class ObjectContainer
{
private:
    std::vector<Object> all;

public:
    ObjectContainer() {}
    ~ObjectContainer() {}

    void add(shapeType type, sf::Color color, float x_mov, float y_mov, const sf::Vector2f & pos)
    {
        Object temp(type, color, x_mov, y_mov);
        temp.getShape()->setPosition(pos);
        add(temp);
    }

    void add(shapeType type, sf::Color color, float x_mov, float y_mov, const Window_Area & play_area)
    {
        Object temp(type, color, x_mov, y_mov);
        temp.setPositionCenteredOn(play_area);
        add(temp);
    }

    void add(Object newObject)
    {
        all.push_back(newObject);
    }

    void pop_last()
    {
        all.pop_back();
    }

    void pop_first()
    {
        all.erase(all.begin());
    }

    //  Check all objects. For the first intersection that happens, a reference to the object is returned.
    //  If no intersection happens, then nullptr.
    Object * intersects(sf::Vector2i point)
    {
        for (int i = 0; i < all.size(); ++i)
        {
            if (all[i].intersects(point))
            {
                return & all[i];
            }
        }
        return nullptr;
    }
};

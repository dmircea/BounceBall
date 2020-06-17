#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Object.hpp"

class ObjectContainer
{
private:
    std::vector<Object> all;
    int m_maximum;
    //  TODO -- all.size stays the same even after using pop or erase
    //  Must create a private member variable to define the size of the container
    int m_current_size;

public:
    ObjectContainer() : ObjectContainer(30) {}
    ObjectContainer(int max) : m_maximum(max), m_current_size(0) {}
    ~ObjectContainer() {}

    int maximum() const { return m_maximum; }
    int size() const { return m_current_size; }

    void add(shapeType type, sf::Color color, float x_mov, float y_mov, const sf::Vector2f & pos)
    {
        Object temp(type, color, x_mov, y_mov);
        temp.setPosition(pos);
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
        if(m_current_size < m_maximum)
        {
            all.push_back(newObject);
            m_current_size++;
        }
    }

    void pop_last()
    {
        if(m_current_size > 0)
        {
            all.pop_back();
            m_current_size--;
        }
    }

    void pop_first()
    {
        if(m_current_size > 0)
        {
            all.erase(all.begin());
            m_current_size--;
        }
    }

    void pop_random()
    {
        if(m_current_size > 0)
        {
            std::cout << "Inside the remove random function\n";
            int index = getRandomInteger(0, m_current_size);
            all.erase(all.begin() + index);
            std::cout << "After the remove random function\n";
            m_current_size--;
        }
    }

    //  Check all objects. For the first intersection that happens, a reference to the object is returned.
    //  If no intersection happens, then nullptr.
    Object * intersects(sf::Vector2i point)
    {
        for (int i = 0; i < m_current_size; ++i)
        {
            if (all[i].intersects(point))
            {
                return & all[i];
            }
        }
        return nullptr;
    }

    void draw_all(sf::RenderWindow & window)
    {
        std::cout << "Inside the draw all function\n";
        std::cout << "Number of elements: " << m_current_size << '\n';
        for (int i = 0; i < m_current_size; ++i)
        {
            std::cout << "Drawing number " << i << '\n';
            all[i].draw(window);
        }
    }

    void move_all(const Window_Area &play_area, const sf::Time & dt)
    {
        std::cout << "Inside the move all function\n";
        for (int i = 0; i < m_current_size; ++i)
        {
            all[i].move(play_area, dt);
        }
    }

    void print_all()
    {
        for (int i = 0; i < m_current_size; ++i)
        {
            all[i].print();
            std::cout << '\n';
        }
    }
};

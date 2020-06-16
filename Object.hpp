#pragma once

#include <SFML/Graphics.hpp>
#include "Utilities.hpp"

class Object
{
private:
    sf::Shape * m_shape;
    float m_move_x;
    float m_move_y;
    // bool m_isMoving;
    shapeType m_shape_type;

    int hor_dir;
    int ver_dir;

    int m_size;

    // bool m_being_moved;

    state m_status;

public:
    Object(shapeType type, sf::Color color, float x_mov, float y_mov, int size = 50)
    : m_move_x(x_mov), m_move_y(y_mov), m_status(state::NOT_MOVING), m_shape_type(type),
    hor_dir(1), ver_dir(1), m_size(size)
    {
        if(type == shapeType::CIRCLE)
        {
            m_shape = new sf::CircleShape(size);
        }
        else if(type == shapeType::CONVEX)
        {
            m_shape = new sf::ConvexShape(4);
        }
        else if(type == shapeType::RECTANGLE)
        {
            m_shape = new sf::RectangleShape(sf::Vector2f(size,size));
        }
        m_shape->setFillColor(color);
    }

    Object(const Object & other)
    {
        m_shape_type = other.shape_type();
        if(m_shape_type == shapeType::CIRCLE)
        {
            m_shape = new sf::CircleShape(50);
        }
        else if(m_shape_type == shapeType::CONVEX)
        {
            m_shape = new sf::ConvexShape(4);
        }
        else if(m_shape_type == shapeType::RECTANGLE)
        {
            m_shape = new sf::RectangleShape(sf::Vector2f(50,50));
        }
        m_move_x = other.move_x();
        m_move_y = other.move_y();
        m_status = other.status();
        m_shape->setFillColor(other.getShape()->getFillColor());
        m_shape->setPosition(other.getShape()->getPosition());
        directions(other.directions());
        // std::cout << "Original x_move is " << other.move_x() << "  as opposed to " << m_move_x << '\n';
        // std::cout << "Original y_move is " << other.move_y() << "  as opposed to " << m_move_y << '\n';
    }

    ~Object()
    {
        delete m_shape;
    }

    int size() const { return m_size; }
    void move_x(const float new_move_x) { m_move_x = new_move_x; }
    void move_y(const float new_move_y) { m_move_y = new_move_y; }
    bool isMoving() const { return m_status == state::MOVING; }
    bool isNotMoving() const { return m_status == state::NOT_MOVING; }
    bool beingMoved() const { return m_status == state::BEING_MOVED; }
    bool justStopped() const { return m_status == state::STOPPED; }
    void status(const state new_state) { m_status = new_state; }
    state status() const { return m_status; }
    shapeType shape_type() const { return m_shape_type; }
    float move_x() const { return m_move_x; }
    float move_y() const { return m_move_y; }
    void directions(const sf::Vector2i & pair) { hor_dir = pair.x; ver_dir = pair.y; }
    sf::Vector2i directions() const { return sf::Vector2i(hor_dir, ver_dir); }
    sf::Shape * getShape() const { return m_shape; }

    sf::Vector2f getCenterPosition() const
    {
        sf::Rect<float> rect = m_shape->getGlobalBounds();
        return sf::Vector2f((rect.left + rect.width / 2.0f), (rect.top + rect.height / 2.0f));
    }

    void setPositionCenteredOn(const Window_Area & play_area)
    {
        m_shape->setPosition(
            (play_area.PLAY_AREA_RIGHT - play_area.PLAY_AREA_LEFT) / 2.0f,
            (play_area.PLAY_AREA_BOTTOM - play_area.PLAY_AREA_TOP) / 2.0f);
    }

    void move(const Window_Area & play_area, const sf::Time & delta)
    {
        float distX = m_move_x * delta.asSeconds();
        float distY = m_move_y * delta.asSeconds();

        auto rect = m_shape->getGlobalBounds();

        if(rect.top - distY < play_area.PLAY_AREA_TOP || rect.top + rect.height + distY > play_area.PLAY_AREA_BOTTOM)
        {
            ver_dir *= -1;
            distY += 0.001;
            std::cout << "changed vertical bounds at ( " << rect.left << ", " << rect.top << " )" << '\n';
        }
        if(rect.left - distX < play_area.PLAY_AREA_LEFT || rect.left + rect.width + distX > play_area.PLAY_AREA_RIGHT)
        {
            hor_dir *= -1;
            distX += 0.001;
            std::cout << "changed horizontal bounds at ( " << rect.left << ", " << rect.top << " )" << '\n';
        }

        distX *= hor_dir;
        distY *= ver_dir;

        m_shape->move(distX, distY);

        // std::cout << "Just moved: " << distX << " x_axis,   " << distY << " y_axis\n";

    }

    void stop(sf::Color color = sf::Color::Blue)
    {
        m_move_x = 0;
        m_move_y = 0;
        m_status = state::NOT_MOVING;
        m_shape->setFillColor(color);
    }

    void start(int move_x, int move_y, sf::Color color = sf::Color::Red)
    {
        m_move_x = move_x;
        m_move_y = move_y;
        ver_dir = 1;
        hor_dir = 1;
        m_status = state::MOVING;
        m_shape->setFillColor(color);
    }

    void draw(sf::RenderWindow & window)
    {
        if(m_shape != nullptr)
        {
            window.draw(*m_shape);
        }
    }

    bool intersects(sf::Vector2i point)
    {
        auto rect = m_shape->getGlobalBounds();

        if(rect.contains(point.x, point.y))
        {
            return true;
        }

        return false;
    }

    void print()
    {
        std::cout << "Sphere at ( " << m_shape->getPosition().x << ", " << m_shape->getPosition().y << " ).\n"
                << "move_x = " << m_move_x << ",   move_y = " << m_move_y << ",   \n";
    }

};

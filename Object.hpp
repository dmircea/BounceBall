#pragma once

#include <SFML/Graphics.hpp>
#include "Utilities.hpp"

class Object
{
private:
    sf::CircleShape m_shape;
    float m_move_x;
    float m_move_y;
    // bool m_isMoving;

    int hor_dir;
    int ver_dir;

    int m_size;

    // bool m_being_moved;

    state m_status;

public:
    Object(shapeType type, sf::Color color, float x_mov, float y_mov, int size = 50)
    : m_move_x(x_mov), m_move_y(y_mov), m_status(state::NOT_MOVING), m_size(size)
    {
        m_shape = sf::CircleShape(size);
        m_shape.setFillColor(color);
        int random = getRandomInteger(0,2);
        int random2 = getRandomInteger(0,2);
        hor_dir = initializeDirection(random);
        ver_dir = initializeDirection(random2);

    }

    int initializeDirection(int random)
    {
        if(random == 0)
        {
            return -1;
        }
        else
        {
            return 1;
        }
    }

    Object(const Object & other)
    {

        m_shape = sf::CircleShape(50);

        m_move_x = other.move_x();
        m_move_y = other.move_y();
        m_status = other.status();
        m_shape.setFillColor(other.getShape().getFillColor());
        m_shape.setPosition(other.getShape().getPosition());
        // directions(other.directions());
        int random = getRandomInteger(0,2);
        int random2 = getRandomInteger(0,2);
        hor_dir = initializeDirection(random);
        ver_dir = initializeDirection(random2);
        // std::cout << "Original x_move is " << other.move_x() << "  as opposed to " << m_move_x << '\n';
        // std::cout << "Original y_move is " << other.move_y() << "  as opposed to " << m_move_y << '\n';
    }

    void operator= (const Object & other)
    {

        m_shape = sf::CircleShape(50);

        m_move_x = other.move_x();
        m_move_y = other.move_y();
        m_status = other.status();
        m_shape.setFillColor(other.getShape().getFillColor());
        m_shape.setPosition(other.getShape().getPosition());
        directions(other.directions());
    }

    ~Object() { }

    int size() const { return m_size; }
    void move_x(const float new_move_x) { m_move_x = new_move_x; }
    void move_y(const float new_move_y) { m_move_y = new_move_y; }
    bool isMoving() const { return m_status == state::MOVING; }
    bool isNotMoving() const { return m_status == state::NOT_MOVING; }
    bool beingMoved() const { return m_status == state::BEING_MOVED; }
    bool justStopped() const { return m_status == state::STOPPED; }
    void status(const state new_state) { m_status = new_state; }
    state status() const { return m_status; }
    // shapeType shape_type() const { return m_shape_type; }
    float move_x() const { return m_move_x; }
    float move_y() const { return m_move_y; }
    void directions(const sf::Vector2i & pair) { hor_dir = pair.x; ver_dir = pair.y; }
    sf::Vector2i directions() const { return sf::Vector2i(hor_dir, ver_dir); }
    sf::CircleShape getShape() const { return m_shape; }


    void setPosition(const sf::Vector2f & point)
    {
        m_shape.setPosition(point);
    }

    sf::Vector2f getCenterPosition() const
    {
        sf::Rect<float> rect = m_shape.getGlobalBounds();
        return sf::Vector2f((rect.left + rect.width / 2.0f), (rect.top + rect.height / 2.0f));
    }

    void setPositionCenteredOn(const Window_Area & play_area)
    {
        m_shape.setPosition(
            (play_area.PLAY_AREA_RIGHT - play_area.PLAY_AREA_LEFT) / 2.0f,
            (play_area.PLAY_AREA_BOTTOM - play_area.PLAY_AREA_TOP) / 2.0f);
    }

    void move(const Window_Area & play_area, const sf::Time & delta)
    {
        float distX = m_move_x * delta.asSeconds();
        float distY = m_move_y * delta.asSeconds();

        auto rect = m_shape.getGlobalBounds();

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

        m_shape.move(distX, distY);

        // std::cout << "Just moved: " << distX << " x_axis,   " << distY << " y_axis\n";

    }

    void stop(sf::Color color = sf::Color::Blue)
    {
        m_move_x = 0;
        m_move_y = 0;
        m_status = state::NOT_MOVING;
        m_shape.setFillColor(color);
    }

    void start(int move_x, int move_y, sf::Color color = sf::Color::Red)
    {
        m_move_x = move_x;
        m_move_y = move_y;
        ver_dir = 1;
        hor_dir = 1;
        m_status = state::MOVING;
        m_shape.setFillColor(color);
    }

    void draw(sf::RenderWindow & window)
    {
        window.draw(m_shape);
    }

    bool intersects(sf::Vector2i point)
    {
        auto rect = m_shape.getGlobalBounds();

        if(rect.contains(point.x, point.y))
        {
            return true;
        }

        return false;
    }

    void print()
    {
        std::cout << "Sphere at ( " << m_shape.getPosition().x << ", " << m_shape.getPosition().y << " ).\n"
                << "move_x = " << m_move_x << ",   move_y = " << m_move_y << ",   \n";
    }

};

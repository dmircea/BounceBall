
#include <SFML/Graphics.hpp>

struct Window_Area
{
    int PLAY_AREA_TOP;
    int PLAY_AREA_LEFT;
    int PLAY_AREA_RIGHT;
    int PLAY_AREA_BOTTOM;

    Window_Area(int top, int left, int right, int bottom)
    : PLAY_AREA_TOP(top), PLAY_AREA_LEFT(left), PLAY_AREA_RIGHT(right),
    PLAY_AREA_BOTTOM(bottom) { }
};

enum class shapeType { CIRCLE, CONVEX, RECTANGLE};

class Object
{
private:
    sf::Shape * m_shape;
    float m_move_x;
    float m_move_y;
    bool m_isMoving;
    shapeType m_shape_type;

    int hor_dir;
    int ver_dir;


public:
    Object(shapeType type, sf::Color color, float x_mov, float y_mov)
    : m_move_x(x_mov), m_move_y(y_mov), m_isMoving(false), m_shape_type(type),
    hor_dir(1), ver_dir(1)
    {
        if(type == shapeType::CIRCLE)
        {
            m_shape = new sf::CircleShape(50);
        }
        if(type == shapeType::CONVEX)
        {
            m_shape = new sf::ConvexShape(4);
        }
        if(type == shapeType::RECTANGLE)
        {
            m_shape = new sf::RectangleShape(sf::Vector2f(50,50));
        }
        m_shape->setFillColor(color);
    }

    ~Object()
    {
        delete m_shape;
    }

    void move_x(const float new_move_x) { m_move_x = new_move_x; }
    void move_y(const float new_move_y) { m_move_y = new_move_y; }
    void isMoving(const bool move) { m_isMoving = move; }
    bool isMoving() const { return m_isMoving; }
    float move_x() const { return m_move_x; }
    float move_y() const { return m_move_y; }
    sf::Shape * getShape() const { return m_shape; }

    void setPositionCentered(Window_Area & play_area)
    {
        m_shape->setPosition(
            (play_area.PLAY_AREA_RIGHT - play_area.PLAY_AREA_LEFT) / 2.0f,
            (play_area.PLAY_AREA_BOTTOM - play_area.PLAY_AREA_TOP) / 2.0f);
    }

    void move(Window_Area & play_area, sf::Time & delta)
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
    }

    void draw(sf::RenderWindow & window)
    {
        window.draw(*m_shape);
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

};

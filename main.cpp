#include <SFML/Graphics.hpp>
// #include <X11/Xlib.h>
#include <random>
#include <functional>

#include <iostream>

#include "Utilities.hpp"
#include "FPSCounter.hpp"
#include "Object.hpp"
#include "ObjectContainer.hpp"

//	Constants for Movement
const float MIN_MOVE = 100;
const float MAX_MOVE = 1000;

//	TODO : was meant to be used for framerate limitation (until setFramerateLimit was found), may be used for something more later
const int TIME_FRAME = 60;

//	Constants for the width and height of the actual window.
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 700;
//56
const int OBJECT_SIZE = 50;
const int MAXIMUM_OBJECTS = (WINDOW_WIDTH * WINDOW_HEIGHT) / (2 * OBJECT_SIZE * 2 * OBJECT_SIZE) - 6;

//	Function signatures
void setUpText(sf::Text & text, const sf::Vector2f & position, bool centered = false);
void drawText(sf::RenderWindow & window, const sf::Text & text);
void shapeInteraction(sf::Vector2i mousePos, sf::Shape * object);

int main()
{
	// XInitThreads();
	srand(time(0));
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH,WINDOW_HEIGHT), "Bounce Ball");
	//	Those lines are neccessary to avoid issues.
	window.setFramerateLimit(TIME_FRAME + 1);
	window.setActive(false);

	Window_Area play_area(50, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	//	Variables that deal with the speed of the shape (initialized as random)
	float move_x = getRandomFloat(MIN_MOVE, MAX_MOVE);
	float move_y = getRandomFloat(MIN_MOVE, MAX_MOVE);

	//	Main Object on the screen (centered inside the play area)
	Object shape(shapeType::CIRCLE, sf::Color::Red, move_x, move_y, OBJECT_SIZE);
	shape.setPositionCenteredOn(play_area);
	shape.status(state::MOVING);

	shape.print();

	std::cout << "Before creating container\n";

	//	Create an object container that takes care of all objects!
	ObjectContainer circles(MAXIMUM_OBJECTS);

	std::cout << "After creating container\n";

	circles.print_all();

	Object * activated_shape = nullptr;

	//	TODO : This line is for future uses (it may be taken out)
	sf::Rect<float> screen(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	//	Clock used for delta time later in the code. May be used to keep track of time.
	sf::Clock clock;

	//	FPSCounter that will be used for the message.
	FPSCounter fps;

	//	Fonts used for any text that appears in game!
	sf::Font font;
	font.loadFromFile("Fonts/8bitOperatorPlus8-Regular.ttf");

	//	Texts that appear in the game.
	sf::Text fps_message;
	sf::Text number_message;

	sf::RectangleShape add_button_outline;
	sf::Text add_button;

	sf::RectangleShape remove_button_outline;
	sf::Text remove_button;

	fps_message.setFont(font);
	number_message.setFont(font);
	add_button.setFont(font);
	remove_button.setFont(font);

	fps_message.setString("FPS");
	number_message.setString("Number of balls: 1");
	add_button.setString("Add Circle!");
	remove_button.setString("Remove Random Circle!");

	fps_message.setCharacterSize(15);
	number_message.setCharacterSize(15);
	add_button.setCharacterSize(15);
	remove_button.setCharacterSize(15);

	fps_message.setFillColor(sf::Color::White);
	number_message.setFillColor(sf::Color::White);
	add_button.setFillColor(sf::Color::White);
	remove_button.setFillColor(sf::Color::White);

	setUpText(fps_message, sf::Vector2f(WINDOW_WIDTH - 50, 10));
	setUpText(number_message, sf::Vector2f(10, 10));
	setUpText(add_button, sf::Vector2f(number_message.getGlobalBounds().left + number_message.getGlobalBounds().width + 30, 10));
	setUpText(remove_button, sf::Vector2f(add_button.getGlobalBounds().left + add_button.getGlobalBounds().width + 30, 10));

	//	Set up the buttons to look like buttons by changing the shape outline
	add_button_outline.setPosition(add_button.getPosition());
	add_button_outline.setSize(sf::Vector2f(add_button.getGlobalBounds().width + 5, add_button.getGlobalBounds().height + 10));
	add_button_outline.setOutlineColor(sf::Color::White);
	add_button_outline.setFillColor(sf::Color::Black);
	add_button_outline.setOutlineThickness(1);

	remove_button_outline.setPosition(remove_button.getPosition());
	remove_button_outline.setSize(sf::Vector2f(remove_button.getGlobalBounds().width + 5, remove_button.getGlobalBounds().height + 10));
	remove_button_outline.setOutlineColor(sf::Color::White);
	remove_button_outline.setFillColor(sf::Color::Black);
	remove_button_outline.setOutlineThickness(1);

	window.setActive(true);

	bool hasActivationBeenClicked = false;
	bool isActivationButtonHeldDown = false;
	bool hasActivationBeenReleased = false;
	sf::Vector2i mousePosition;

	//	Main loop starts here
	while (window.isOpen())
	{

		//	Get the appropriate delta time to use for movement.
		sf::Time dt = clock.restart();

		//	FPS checking and outputing
		int framerate = fps.getFPS();
		if(framerate != -1)
		{
			fps_message.setString(std::to_string(framerate));
		}

		//	Checks if the window is focused. If not make it so that the game is paused.
		if(!window.hasFocus())
		{
			continue;
		}

		//	Check for user input here
		//	TODO : create a release key functionality for mouse button later.
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			if (event.type == sf::Event::KeyPressed)
			{
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				{
					window.close();
				}
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
				{
					if(!isActivationButtonHeldDown)
					{
						hasActivationBeenClicked = true;
						isActivationButtonHeldDown = true;
						mousePosition = sf::Mouse::getPosition(window);
						std::cout << "the mouse is currently at ( " << mousePosition.x << ", " << mousePosition.y << " )\n";

						if(add_button_outline.getGlobalBounds().contains(sf::Vector2f(mousePosition)))
						{
							activated_shape = nullptr;
							shape.move_x(getRandomFloat(MIN_MOVE, MAX_MOVE));
							shape.move_y(getRandomFloat(MIN_MOVE, MAX_MOVE));
							circles.add(shape);
						}
						if(remove_button_outline.getGlobalBounds().contains(sf::Vector2f(mousePosition)))
						{
							activated_shape = nullptr;
							circles.pop_random();
						}
						else
						{
							activated_shape = circles.intersects(mousePosition);
						}

					}
				}
			}
			//	This if statement is the same as "is button released"
			if(event.type == sf::Event::MouseButtonReleased && isActivationButtonHeldDown)
			{
				isActivationButtonHeldDown = false;
				hasActivationBeenReleased = true;
				mousePosition = sf::Mouse::getPosition(window);
				std::cout << "Mouse released!\n";
			}
		}



		// if mouse button is clicked, check where the mouse clicked.
		if(activated_shape != nullptr)
		{
			if(isActivationButtonHeldDown)
			{
				if(activated_shape->isMoving() && hasActivationBeenClicked)
				{
					activated_shape->stop();
					activated_shape = nullptr;
				}
				else if(activated_shape->isNotMoving() && hasActivationBeenClicked)
				{
					activated_shape->status(state::BEING_MOVED);
					activated_shape->getShape()->setFillColor(sf::Color::Green);
				}
				hasActivationBeenClicked = false;
			}
			if(hasActivationBeenReleased)
			{
				if(activated_shape->beingMoved())
				{
					sf::Vector2f pos = activated_shape->getCenterPosition();
					int movement_x = pos.x - mousePosition.x;
					int movement_y = pos.y - mousePosition.y;
					movement_x = (1.5f * movement_x * MAX_MOVE) / WINDOW_WIDTH;
					movement_y = (1.5f * movement_y * MAX_MOVE) / WINDOW_HEIGHT;
					activated_shape->start(movement_x, movement_y);
					activated_shape = nullptr;
				}

				hasActivationBeenReleased = false;
			}
		}
		else
		{
			isActivationButtonHeldDown = false;
			hasActivationBeenClicked = false;
			hasActivationBeenReleased = false;
		}

		number_message.setString("Number of balls: " + std::to_string(circles.size()));

		window.clear();

		//	Use move function from the object's class to move around in a play area
		circles.move_all(play_area, dt);

		drawText(window, fps_message);
		drawText(window, number_message);
		window.draw(add_button_outline);
		drawText(window, add_button);
		window.draw(remove_button_outline);
		drawText(window, remove_button);

		circles.draw_all(window);
		std::cout << "After the draw all function\n";
		window.display();
		std::cout << "After the display of all drawn graphics\n";

	}

	return 0;
}

void setUpText(sf::Text & text, const sf::Vector2f & position, bool centered)
{
	if(!centered)
	{
		text.setPosition(position);
	}
	else
	{
		sf::Rect<float> textRect = text.getLocalBounds();
		text.setOrigin((textRect.left + textRect.width) / 2.0f,
						(textRect.top + textRect.height) / 2.0f);
		text.setPosition(position);
	}
}
void drawText(sf::RenderWindow & window, const sf::Text & text)
{
	window.draw(text);
}

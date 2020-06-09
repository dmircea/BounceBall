#include <SFML/Graphics.hpp>
// #include <X11/Xlib.h>
#include <random>
#include <functional>

#include <iostream>

#include "FPSCounter.hpp"
#include "Object.hpp"

//	Constants for Movement
const float MIN_MOVE = 100;
const float MAX_MOVE = 1000;

//	TODO : was meant to be used for framerate limitation (until setFramerateLimit was found), may be used for something more later
const int TIME_FRAME = 60;

//	Constants for the width and height of the actual window.
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 700;

//	Function signatures
void setUpText(sf::Text & text, const sf::Vector2f & position, bool centered = false);
void drawText(sf::RenderWindow & window, const sf::Text & text);
void shapeInteraction(sf::Vector2i mousePos, sf::Shape * object);

int main()
{
	// XInitThreads();
	srand(time(0));
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH,WINDOW_HEIGHT), "Moving Circle");
	//	Those lines are neccessary to avoid issues.
	window.setFramerateLimit(TIME_FRAME + 1);
	window.setActive(false);

	Window_Area play_area(50, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	//	Variables that deal with the speed of the shape (initialized as random)
	float move_x = rand() / (RAND_MAX / (MAX_MOVE - MIN_MOVE)) + MIN_MOVE;
	float move_y = rand() / (RAND_MAX / (MAX_MOVE - MIN_MOVE)) + MIN_MOVE;

	//	Main Object on the screen (centered inside the play area)
	Object shape(shapeType::CIRCLE, sf::Color::Red, move_x, move_y);
	shape.setPositionCentered(play_area);
	shape.isMoving(true);

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

	fps_message.setFont(font);
	number_message.setFont(font);

	fps_message.setString("FPS");
	number_message.setString("Number of balls: 1");

	fps_message.setCharacterSize(15);
	number_message.setCharacterSize(15);

	fps_message.setFillColor(sf::Color::White);
	number_message.setFillColor(sf::Color::White);

	setUpText(fps_message, sf::Vector2f(WINDOW_WIDTH - 50, 10));
	setUpText(number_message, sf::Vector2f(10, 10));

	window.setActive(true);

	bool hasMouseBeenClicked = false;
	sf::Vector2i mousePosition;

	//	Main loop starts here
	while (window.isOpen())
	{
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
					hasMouseBeenClicked = true;
					mousePosition = sf::Mouse::getPosition(window);
					std::cout << "the mouse is currently at ( " << mousePosition.x << ", " << mousePosition.y << " )\n";
				}
			}
		}

		//	TODO : add an if statement to check if window has focus.

		// if mouse button is clicked, check where the mouse clicked.
		if(hasMouseBeenClicked)
		{
			hasMouseBeenClicked = false;

			if(shape.intersects(mousePosition))
			{
				if(shape.isMoving() == true)
				{
					shape.move_x(0);
					shape.move_y(0);
					shape.isMoving(false);
				}
				else
				{
					shape.move_x(100);
					shape.move_y(100);
					shape.isMoving(true);
				}

			}
			//	TODO : 	Check if mousePosition is inside the rectangle of the shape.
			//			if so then the shape movement is changed to zero.
			//			The is moving variable is set to false.
			//	EXTRA : It may be a good idea to see whether or not the ball is moving.
			//			if it is moving stop it,
			//			otherwise make it move using a hold and release mechanism
		}

		window.clear();

		//	Get the appropriate delta time to use for movement.
		sf::Time dt = clock.restart();

		//	Use move function from the object's class to move around in a play area
		shape.move(play_area, dt);

		//	FPS checking and outputing
		int framerate = fps.getFPS();
		if(framerate != -1)
		{
			fps_message.setString(std::to_string(framerate));
		}

		drawText(window, fps_message);
		drawText(window, number_message);

		shape.draw(window);
		window.display();

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

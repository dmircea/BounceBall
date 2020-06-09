# BounceBall
A simple game written in C++ using the SFML library.

Contains only simple functionality for stopping and starting the ball.
The ball will bounce against the walls.

This project requires SFML to be built. 
- Please download SFML here: https://www.sfml-dev.org/download/sfml/2.5.1/
- You may also use your repository to download SFML.
  - This is especially recommended for Linux users.


Assuming SFML is properly installed and this project is cloned use the following command to compile the source code:

```
clang++ main.cpp -o main -lsfml-graphics -lsfml-system -lsfml window
```

If you would like to make use of the make.py script that comes with the repository first make sure that the latest version of python3 is installed.
Then simply use ```./make.py``` or ```python3 make.py``` to use the compilation script.

The fonts used in this game are downloaded from: https://www.1001freefonts.com/

Thank you and happy coding!

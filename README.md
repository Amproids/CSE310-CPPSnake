# Overview

This project is a simple implementation of the classic Snake game using C++. As someone diving into software development, I wanted to learn C++ basics and explore how game development works. This project gave me a great opportunity to work with graphics programming and learn useful libraries like SDL2.

The game has all the classic Snake features - you control a snake that grows when it eats apples, while trying to avoid hitting walls or itself. I also added a high score system that saves your best scores between games.

[Software Demo Video](http://youtube.link.goes.here)

# Development Environment

* Clang++ compiler (C++14 standard)
* SDL2 (Simple DirectMedia Layer 2.0)
* Command line compilation:
clang++ -std=c++14 main.cpp -o main -I/usr/local/include -L/usr/local/lib -lSDL2

The game uses several C++ features that I learned about:
* Vectors for storing the snake's body
* Classes and objects
* File handling for the high score system
* Enum classes for directions
* Memory management

# Useful Websites

- [SDL2 Documentation](https://documentation.help/SDL/index.html)
- [C++ Reference](https://en.cppreference.com/)
- [LazyFoo's SDL Tutorials](https://lazyfoo.net/tutorials/SDL/)
- [Stack Overflow](https://stackoverflow.com/)

# Future Work

- Add a pause feature
- Add sound effects and background music
- Create a menu system
- Implement obstacles or walls in the play area
- Add different visual themes
- Create a multiplayer mode

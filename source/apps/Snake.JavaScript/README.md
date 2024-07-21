# Snake in JavaScript

![screenshot](https://github.com/lostjared/Snake.JavaScript/blob/main/snake.jpg)

Play at http://lostsidedead.biz/snake

This is a basic Snake game implemented using HTML5 and JavaScript with a canvas element. The game was originally written in Rust and SDL2 and has been converted to run in a web browser.

## Getting Started

To run the game, simply open the `index.html` file in a web browser.

### Prerequisites

- A modern web browser that supports HTML5 and JavaScript.

## Game Controls

- Use the arrow keys to control the direction of the snake:
  - Left Arrow: Move Left
  - Right Arrow: Move Right
  - Down Arrow: Move Down
  - Up Arrow: Move Up

## Game Rules

- The objective is to eat as many apples as possible.
- Each apple consumed increases the score by 1.
- The snake grows longer with each apple consumed.
- The game ends if the snake collides with the walls or itself.
- The player starts with 4 lives. The game resets when all lives are lost.

## Files

- `index.html`: The main HTML file that sets up the canvas and includes the game script.
- `snake.js`: The JavaScript file containing the game logic and rendering code.

## Development

### Converting from Rust to JavaScript

This project was originally implemented in Rust using SDL2. It has been converted to JavaScript to run in a web browser.


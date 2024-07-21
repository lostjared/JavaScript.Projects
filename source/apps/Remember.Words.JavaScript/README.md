# Word Recall Game

This is a simple word recall game implemented in JavaScript. The 
objective is 
to correctly type the displayed words within a limited time. The game will display a word, and the player has to type it correctly to move to the next word. If the player makes a mistake, the game is over.

## Features

- Randomly shuffles words for each game session
- Displays a message when the words are correctly typed or when the game is over
- Simple and intuitive keyboard controls

## How to Play

1. Open the game in your browser.
2. Press `Space` to start the game. The words are displayed
3. Press `Enter` to start the countdown.
4. Type the displayed words and press `Enter` to submit.
5. If you type the word correctly, you can continue to the next words.
6. If you type the words incorrectly, the game is over.
7. Press `Enter` to start a new game after game over.

## Controls

- `Space`: Start the game
- `Enter`: Start the countdown / Submit typed words / Start a new game after game over
- `Backspace`: Delete the last character while typing

## Code Overview

### Main Components

- **Words Class**: Handles the word list, shuffling, and providing the next word.
- **Game Class**: Manages the game state, handles user input, and renders the game on the canvas.

### Key Methods

- `Words.shuffle()`: Randomly shuffles the word list.
- `Words.next()`: Returns the next word in the list.
- `Game.newGame()`: Initializes a new game session.
- `Game.init()`: Sets up event listeners and starts the game loop.
- `Game.handleEvent(e)`: Handles keyboard events.
- `Game.addWord()`: Adds a new word to the current word text.
- `Game.checkInput()`: Checks if the typed input matches the displayed word.
- `Game.wrapText(ctx, text, x, y, maxWidth, lineHeight)`: Wraps text within a specified width for display on the canvas.
- `Game.draw()`: Renders the game elements on the canvas.
- `Game.loop()`: The game loop that keeps the game running.

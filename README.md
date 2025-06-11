## Chrome Dino Game Clone (C++ & SFML)

A simple and fun clone of the classic Chrome Dino game — built using C++ and SFML. Run, jump, and dodge dynamically colored cacti and flying birds as the game gets faster and harder over time!

## Features

Jump and run mechanics

Ground and flying obstacles

Random colors for obstacles

Increasing difficulty over time

Score tracking and restart option


## Requirements

SFML 2.6 (install via Homebrew or official site)

C++17 compatible compiler

On macOS (via Homebrew):

brew install sfml@2

## Run Locally

Clone the repo:

git clone https://github.com/your-username/chrome-dino-clone.git
cd chrome-dino-clone

Make sure arial.ttf is present in the folder.

Compile the game:

g++ -std=c++17 dino-game.cpp -o dino-game \
-I/opt/homebrew/opt/sfml@2/include \
-L/opt/homebrew/opt/sfml@2/lib \
-lsfml-graphics -lsfml-window -lsfml-system

Run it:

./dino-game


## Controls

Key     Action

Space   Jump / Start

Enter   Restart (after Game Over)

Esc     Exit


## Folder Contents

chrome-dino-clone/
├── dino-game.cpp
├── dino-game
├── arial.ttf
├── screenshot.png
└── README.md

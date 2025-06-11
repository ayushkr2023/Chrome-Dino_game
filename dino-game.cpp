#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>
#include <algorithm>

// Window and gameplay constants
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 400;
const int GROUND_Y = 360;
const float GRAVITY = 0.6f;
const float JUMP_VELOCITY = -12.0f;
const float BASE_SPEED = 6.0f;
const float BASE_SPAWN_DELAY = 1.5f;
const float MIN_SPAWN_DELAY = 0.5f;

// Ground obstacle (cactus)
struct Obstacle {
    sf::RectangleShape shape;
    Obstacle(float x, float y, float width, float height) {
        shape.setSize(sf::Vector2f(width, height));
        shape.setFillColor(getRandomColor());
        shape.setPosition(x, y - height);
    }

    sf::Color getRandomColor() {
        static std::vector<sf::Color> colors = {
            sf::Color(255, 50, 50),    // red
            sf::Color(50, 200, 50),    // green
            sf::Color(50, 150, 255),   // blue
            sf::Color(255, 200, 0),    // yellow
            sf::Color(150, 0, 255)     // purple
        };
        return colors[rand() % colors.size()];
    }
};

// Flying obstacle (bird)
struct FlyingObstacle {
    sf::RectangleShape shape;
    FlyingObstacle(float x, float y) {
        shape.setSize(sf::Vector2f(40, 20));
        shape.setFillColor(sf::Color(100, 100, 255)); // bird color
        shape.setPosition(x, y);
    }
};

int main() {
    srand(static_cast<unsigned>(time(0)));
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Chrome Dino Clone");
    window.setFramerateLimit(60);

    // Dino setup
    sf::RectangleShape dino(sf::Vector2f(44, 44));
    dino.setFillColor(sf::Color(50, 200, 100));
    dino.setPosition(100, GROUND_Y - 44);

    // Ground setup
    sf::RectangleShape ground(sf::Vector2f(WINDOW_WIDTH * 2, 5));
    ground.setFillColor(sf::Color(100, 100, 100));
    ground.setPosition(0, GROUND_Y);

    // Load font
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        return -1;
    }

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 10);

    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(28);
    gameOverText.setFillColor(sf::Color::Yellow);
    gameOverText.setString("Game Over!\nPress Enter to Restart");
    gameOverText.setPosition(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 - 50);

    // Game state
    std::vector<Obstacle> obstacles;
    std::vector<FlyingObstacle> birds;
    sf::Clock clock;
    float spawnTimer = 0;
    float velocityY = 0;
    bool isJumping = false;
    bool gameOver = false;
    bool started = false;
    int score = 0;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (!gameOver && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                if (!started) {
                    started = true;
                    clock.restart();
                }
                if (!isJumping) {
                    velocityY = JUMP_VELOCITY;
                    isJumping = true;
                }
            }

            if (gameOver && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                gameOver = false;
                started = false;
                obstacles.clear();
                birds.clear();
                dino.setPosition(100, GROUND_Y - 44);
                velocityY = 0;
                isJumping = false;
                score = 0;
                ground.setPosition(0, GROUND_Y);
                spawnTimer = 0;
                clock.restart();
            }
        }

        window.clear();

        if (started && !gameOver) {
            // Gravity and ground collision
            velocityY += GRAVITY;
            dino.move(0, velocityY);
            if (dino.getPosition().y >= GROUND_Y - 44) {
                dino.setPosition(dino.getPosition().x, GROUND_Y - 44);
                isJumping = false;
            }

            // Speed and spawn delay scale with score
            float speed = BASE_SPEED + score / 20.0f;
            float spawnDelay = std::max(MIN_SPAWN_DELAY, BASE_SPAWN_DELAY - score * 0.02f);

            ground.move(-speed, 0);
            if (ground.getPosition().x <= -WINDOW_WIDTH)
                ground.setPosition(0, GROUND_Y);

            spawnTimer += clock.restart().asSeconds();
            if (spawnTimer > spawnDelay) {
                int r = rand() % 10;
                if (r < 2 && score >= 5) {
                    // 20% chance to spawn bird after score 5+
                    float birdY = GROUND_Y - 100 - (rand() % 40);
                    birds.emplace_back(WINDOW_WIDTH, birdY);
                } else {
                    float width = 20 + rand() % 20;
                    float height = 40 + rand() % 20;
                    obstacles.emplace_back(WINDOW_WIDTH, GROUND_Y, width, height);
                }
                spawnTimer = 0;
            }

            // Move obstacles
            for (size_t i = 0; i < obstacles.size();) {
                obstacles[i].shape.move(-speed, 0);
                if (obstacles[i].shape.getPosition().x < -50) {
                    obstacles.erase(obstacles.begin() + i);
                    score++;
                } else if (dino.getGlobalBounds().intersects(obstacles[i].shape.getGlobalBounds())) {
                    gameOver = true;
                    break;
                } else {
                    ++i;
                }
            }

            // Move birds
            for (size_t i = 0; i < birds.size();) {
                birds[i].shape.move(-speed, 0);
                if (birds[i].shape.getPosition().x < -50) {
                    birds.erase(birds.begin() + i);
                    score++;
                } else if (dino.getGlobalBounds().intersects(birds[i].shape.getGlobalBounds())) {
                    gameOver = true;
                    break;
                } else {
                    ++i;
                }
            }

            scoreText.setString("Score: " + std::to_string(score));
        }

        // Draw everything
        window.draw(ground);
        window.draw(dino);
        for (const auto& obs : obstacles) window.draw(obs.shape);
        for (const auto& bird : birds) window.draw(bird.shape);
        window.draw(scoreText);
        if (gameOver) window.draw(gameOverText);

        window.display();
    }

    return 0;
}
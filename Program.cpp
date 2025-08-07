#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>

using namespace sf;
using namespace std;

void changeLabel(Text& label, string start, int endvar) {
    label.setString(start + to_string(endvar));
}

int main() {

    vector<Color> colors = {
        Color::Red,
        Color(255, 165,0),
        Color::Yellow,
        Color::Green,
        Color::Cyan,
        Color::Blue,
        Color(139,0,255),
        Color::White,
        Color::Black
    };

    srand(time(NULL));

    RenderWindow app(VideoMode(900,600), "Arkanoid");
    app.setFramerateLimit(60);
    vector<Sprite> blocks;

    Texture block, bg;
    block.loadFromFile("images/block.png");
    bg.loadFromFile("images/bg.png");

    Sprite bgs(bg);
    bgs.move(-10,0);

    Sprite paddle(block);
    paddle.setScale(0.8, 0.2);
    paddle.setColor(Color(0, 150, 255));
    paddle.setPosition(300,500);

    CircleShape ball(7);
    ball.setFillColor(Color::White);
    ball.setPosition(450, 400);
    float vel_x = 2, vel_y = 3;

    Font font_Arial;
    font_Arial.loadFromFile("fonts/arial.ttf");

    int curLives = 3;
    Text lives("Lives: " + to_string(curLives), font_Arial);
    lives.move(5, 0);
    
    int curScore = 0;
    Text score("Score: " + to_string(curScore), font_Arial);
    score.move(3, 35);

    Text endScreen("Press R to play again", font_Arial);
    endScreen.setPosition(300,250);
    bool gameEnd = false;

    float disY = 50;
    float disX = 80;
    float offsetX = 50;
    float offsetY = 50;
    for (int x = 0; x < 10; x++) {
        for (int y = 0; y < 6; y++) {
            Sprite plat(block);
            plat.setScale(0.2, 0.2);
            plat.setColor(colors[y]);
            plat.setPosition(offsetX + disX * x, offsetY + disY * y);
            blocks.push_back(plat);
        }
    }

    while (app.isOpen()) {
        Event app_event;
        while (app.pollEvent(app_event)) {
            if (app_event.type == Event::EventType::Closed) {
                app.close();
            }

        }

        if (Keyboard::isKeyPressed(Keyboard::A) && !gameEnd) {
            if (paddle.getPosition().x >= 0) {
                paddle.move(-6, 0);
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::D) && !gameEnd) {
            if (paddle.getPosition().x <= 600) {
                paddle.move(6, 0);
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::R) && gameEnd) {
            for (int x = 0; x < 10; x++) {
                for (int y = 0; y < 6; y++) {
                    Sprite plat(block);
                    plat.setScale(0.2, 0.2);
                    plat.setColor(colors[y]);
                    plat.setPosition(offsetX + disX * x, offsetY + disY * y);
                    blocks.push_back(plat);
                }
            }
            ball.setPosition(450, 400);
            paddle.setPosition(300, 500);
            curLives = 3;
            curScore = 0;
            changeLabel(lives, "Lives: ", curLives);
            changeLabel(score, "Score: ", curScore);
            gameEnd = false;
        }

        if (ball.getGlobalBounds().intersects(paddle.getGlobalBounds())) {
            vel_y = -vel_y;
        }
        if (!gameEnd) ball.move(vel_x, vel_y);

        app.clear(Color::Blue);
        app.draw(bgs);
        for (int i = 0; i < blocks.size(); i++) {
            app.draw(blocks[i]);

            if (ball.getGlobalBounds().intersects(blocks[i].getGlobalBounds())) {
                vel_y = -vel_y;
                if (rand() % 3 == 0) {
                    vel_x = -vel_x;
                }

                blocks[i].setPosition(-1000, -1000);
                curScore += 10;
                changeLabel(score, "Score: ", curScore);
            }
        }

        if (ball.getPosition().y > 750) {
            ball.setPosition(450, 400);
            curLives--;
            changeLabel(lives, "Lives: ", curLives);
        }

        if (ball.getPosition().y < 0) {
            vel_y = -vel_y;
        }

        if (ball.getPosition().x >= 875 || ball.getPosition().x <= 0) {
            vel_x = -vel_x;
        }

        if (curLives <= 0 || curScore >= 600) {
            gameEnd = true;
            blocks.clear();
        }

        app.draw(paddle);
        app.draw(ball);
        app.draw(lives);
        app.draw(score);
        if (gameEnd) app.draw(endScreen);
        app.display();
    }

    return 0;
}
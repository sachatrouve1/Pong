#ifndef MAIN
#define MAIN

#define SCREEN_X (float)sf::VideoMode::getDesktopMode().width
#define SCREEN_Y (float)sf::VideoMode::getDesktopMode().height

#include <SFML/Graphics.hpp>

void newRound(sf::CircleShape& ball, sf::Vector2f& ballSpeed, sf::Vector2f& ballPosition, sf::RectangleShape& player1, sf::Vector2f& player1Position, sf::RectangleShape& player2, sf::Vector2f& player2Position, int winner);
void newGame(sf::CircleShape& ball, sf::Vector2f& ballSpeed, sf::Vector2f& ballPosition, sf::RectangleShape& player1, sf::Vector2f& player1Position, sf::RectangleShape& player2, sf::Vector2f& player2Position, int winner, int& score1, sf::Text& textScore1, int& score2, sf::Text& textScore2);
std::string setScoreString(int score);
float setSpeedX(int frames, sf::Vector2f& ballSpeed);

#endif

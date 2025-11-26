#include <unistd.h>
#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../header/main.hpp"

using namespace std;

sf::Vector2f player1Dimensions = {10.f, 100.f};
sf::Vector2f player2Dimensions = {10.f, 100.f};
const float ballDimensions = 10.f;
const int playerSpeed = 10;
const float maxSpeed = 20.f;
const float minSpeed = 5.f;

int main(int argc, char const *argv[])
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(SCREEN_X, SCREEN_Y), "PONG", sf::Style::Fullscreen, settings);
    window.setVerticalSyncEnabled(true);

    sf::Color background = sf::Color::Black;

    sf::VertexArray startingBackground(sf::Quads, 4);
    startingBackground[0].position = sf::Vector2f(0.f, 0.f);
    startingBackground[1].position = sf::Vector2f(SCREEN_X, 0.f);
    startingBackground[2].position = sf::Vector2f(SCREEN_X, SCREEN_Y);
    startingBackground[3].position = sf::Vector2f(0.f, SCREEN_Y);
    startingBackground[0].color = sf::Color::Red;
    startingBackground[1].color = sf::Color::Black;
    startingBackground[2].color = sf::Color::Blue;
    startingBackground[3].color = sf::Color::Black;

    sf::VertexArray leftBackground(sf::Quads, 4);
    leftBackground[0].position = sf::Vector2f(0.f, 0.f);
    leftBackground[1].position = sf::Vector2f(SCREEN_X/2, 0.f);
    leftBackground[2].position = sf::Vector2f(SCREEN_X/2, SCREEN_Y);
    leftBackground[3].position = sf::Vector2f(0.f, SCREEN_Y);
    leftBackground[0].color = {255, 0, 0, 75};
    leftBackground[1].color = {  0, 0, 0, 75};
    leftBackground[2].color = {  0, 0, 0, 75};
    leftBackground[3].color = {255, 0, 0, 75};

    sf::VertexArray rightBackground(sf::Quads, 4);
    rightBackground[0].position = sf::Vector2f(SCREEN_X/2, 0.f);
    rightBackground[1].position = sf::Vector2f(SCREEN_X, 0.f);
    rightBackground[2].position = sf::Vector2f(SCREEN_X, SCREEN_Y);
    rightBackground[3].position = sf::Vector2f(SCREEN_X/2, SCREEN_Y);
    rightBackground[0].color = {0, 0,   0, 50};
    rightBackground[1].color = {0, 0, 255, 50};
    rightBackground[2].color = {0, 0, 255, 50};
    rightBackground[3].color = {0, 0,   0, 50};

    sf::Font font;
    if (!font.loadFromFile("../ressources/font/ChargeVectorBold-mLyD9.ttf"))
        cout << "ERROR : Cannot load font";

    sf::Text pong;
    pong.setFont(font);
    pong.setString("PONG");
    pong.setCharacterSize(100);
    pong.setFillColor(sf::Color::White);
    pong.setPosition(
        SCREEN_X/2 - pong.getGlobalBounds().width/2,
        SCREEN_Y/2 - pong.getGlobalBounds().height/2
    );
    pong.setStyle(sf::Text::Italic);

    sf::Text textPif;
    textPif.setFont(font);
    textPif.setString("PIF");
    textPif.setCharacterSize(30);
    textPif.setFillColor(sf::Color::Black);
    textPif.setPosition(20, 20);

    sf::Text textPaf;
    textPaf.setFont(font);
    textPaf.setString("PAF");
    textPaf.setCharacterSize(30);
    textPaf.setFillColor(sf::Color::Black);
    textPaf.setPosition(SCREEN_X - textPaf.getGlobalBounds().width - 20, SCREEN_Y - textPaf.getGlobalBounds().height - 20);

    sf::Color leftSideColor = {255, 150, 150};
    sf::Color rightSideColor = {150, 150, 255};

    sf::RectangleShape midLine(sf::Vector2f(5, SCREEN_Y-20));
    midLine.setPosition(SCREEN_X/2 - 2.5, 20);
    midLine.setFillColor(sf::Color{100, 100, 100});
    
    sf::RectangleShape player1(player1Dimensions);
    player1.setPosition(40, SCREEN_Y/2 - player1Dimensions.y/2);
    player1.setFillColor(sf::Color::White);

    sf::RectangleShape player2(player2Dimensions);
    player2.setPosition(SCREEN_X - 40 - player2Dimensions.x, SCREEN_Y/2 - player2Dimensions.y/2);
    player2.setFillColor(sf::Color::White);

    sf::CircleShape ball(5.f);

    sf::Text textScore1;
    textScore1.setFont(font);
    textScore1.setString("00");
    textScore1.setCharacterSize(30);
    textScore1.setFillColor(sf::Color{255, 255, 255});
    float charSize = textScore1.getGlobalBounds().width / 2;
    textScore1.setPosition(SCREEN_X/4 - charSize, 20);

    sf::Text textScore2;
    textScore2.setFont(font);
    textScore2.setString("00");
    textScore2.setCharacterSize(30);
    textScore2.setFillColor(sf::Color{255, 255, 255});
    textScore2.setPosition(3 * SCREEN_X/4 - charSize, 20);

    sf::Texture textureSoundOn;
    textureSoundOn.setSmooth(true);
    if (!textureSoundOn.loadFromFile("../ressources/images/volumeOn.png"))
        cout << "ERROR: cannot load volumeOn.png" << endl;
    sf::Sprite soundOn;
    soundOn.setTexture(textureSoundOn);
    soundOn.setPosition((SCREEN_X/2)-(textureSoundOn.getSize().x/2), 2);

    sf::Texture textureSoundOff;
    textureSoundOff.setSmooth(true);
    if (!textureSoundOff.loadFromFile("../ressources/images/volumeOff.png"))
        cout << "ERROR: cannot load volumeOff.png" << endl;
    sf::Sprite soundOff;
    soundOff.setTexture(textureSoundOff);
    soundOff.setPosition((SCREEN_X/2)-(textureSoundOn.getSize().x/2), 2);

    sf::Texture flute;
    if (!flute.loadFromFile("../ressources/images/flute.png"))
        cout << "ERROR: cannot load flute.png" << endl;
    sf::Texture baguette;
    if (!baguette.loadFromFile("../ressources/images/baguette.png"))
        cout << "ERROR: cannot load baguette.png" << endl;
    sf::Texture sword;
    if (!sword.loadFromFile("../ressources/images/sword.png"))
        cout << "ERROR: cannot load sword.png" << endl;

    int player1Texture;
    int player2Texture;
    
    sf::SoundBuffer fight;
    if (!fight.loadFromFile("../ressources/sounds/fight.wav"))
        cout << "ERROR: cannot load fight.wav";
    sf::SoundBuffer niceShot;
    if (!niceShot.loadFromFile("../ressources/sounds/nice_shot.wav"))
        cout << "ERROR: cannot load nice_shot.wav";
    sf::SoundBuffer huugh;
    if (!huugh.loadFromFile("../ressources/sounds/huugh.wav"))
        cout << "ERROR: cannot load huugh.wav";
    sf::SoundBuffer tic;
    if (!tic.loadFromFile("../ressources/sounds/tic.wav"))
        cout << "ERROR: cannot load tic.wav";
    sf::SoundBuffer tac;
    if (!tac.loadFromFile("../ressources/sounds/tac.wav"))
        cout << "ERROR: cannot load tac.wav";
    sf::SoundBuffer houuah;
    if (!houuah.loadFromFile("../ressources/sounds/houuah.wav"))
        cout << "ERROR: cannot load houuah.wav";
    sf::SoundBuffer fuh;
    if (!fuh.loadFromFile("../ressources/sounds/flute.wav"))
        cout << "ERROR: cannot load flute.wav";
    sf::SoundBuffer ting;
    if (!ting.loadFromFile("../ressources/sounds/anvil.wav"))
        cout << "ERROR: cannot load anvil.wav";

    sf::Sound startSound;
    startSound.setBuffer(fight);
    sf::Sound noise1;
    noise1.setBuffer(tic);
    sf::Sound noise2;
    noise2.setBuffer(tac);
    sf::Sound noise3;
    noise3.setBuffer(niceShot);
    sf::Sound noise4;
    noise4.setBuffer(huugh);
    sf::Sound noise5;
    noise5.setBuffer(houuah);
    sf::Sound noise6;
    noise6.setBuffer(fuh);
    sf::Sound noise7;
    noise7.setBuffer(ting);
   
    // sf::Music music1;
    // music1.openFromFile("../ressources/plaza_music.ogg");

    bool launch = true;
    bool F1Pressed = true;
    bool F3Pressed = false;
    bool spacePressed = false;
    bool APressed = false;
    bool QPressed = false;
    bool PPressed = false;
    bool MPressed = false;

    sf::Vector2f player1Position = player1.getPosition();
    sf::Vector2f player2Position = player2.getPosition();

    srand(time(NULL));
    sf::Vector2f ballPosition;
    int speedSign;

    for (int i = 0; i < 10; i++)
    {
        ballPosition = {
            SCREEN_X / 2 - ballDimensions / 2,
            (float)(rand() % (int)(SCREEN_Y - ballDimensions))
        };

        speedSign = rand() % 2;
        if (speedSign == 0)
        {
            speedSign = -1;
        }
        else
        {
            speedSign = 1;
        }
    }

    sf::Vector2f ballSpeed = {speedSign * minSpeed, 0};

    ball.setPosition(ballPosition);

    int bounce[100] = {};
    int j = -50;
    for (int i = 0; i < player1Dimensions.y; i++)
    {
        if (j < 0)
        {
            bounce[i] = -sqrt(-j);
        }
        else
        {
            bounce[i] = sqrt(j);
        }
        j += 1;
    }

    int score1 = 0;
    int score2 = 0;
    int winner;
    int frames = 0;

    int srinkLevel[2] = {0, 0};

    bool MaxSpeed = true;

    float pitch[17];
    float p = 0.8;
    for (int i = 0; i <= 16; i++)
    {
        pitch[i] = p;
        p += 0.05;
    }

    while (window.isOpen())
    {
        sf::Event event;
     
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::KeyPressed:
                switch (event.key.code)
                {
                case sf::Keyboard::Escape:
                    window.close();
                    break;

                case sf::Keyboard::F1:
                    F1Pressed = !F1Pressed;
                    break;
                    
                case sf::Keyboard::F2:
                    newGame(ball, ballSpeed, ballPosition, player1, player1Position, player2, player2Position, winner, score1, textScore1, score2, textScore2);
                    break;

                case sf::Keyboard::F3:
                    F3Pressed = !F3Pressed;
                    if (F3Pressed)
                    {
                        midLine.setFillColor(sf::Color(100, 0, 100));
                    }
                    else
                    {
                        midLine.setFillColor(sf::Color(100, 100, 100));
                    }
                    break;

                case sf::Keyboard::Space:
                    spacePressed = !spacePressed;
                    if (spacePressed)
                    {
                        if (F3Pressed)
                        {
                            midLine.setFillColor(sf::Color{200, 0, 200});
                        }
                        else
                        {
                            midLine.setFillColor(sf::Color{200, 200, 200});
                        }
                        background = sf::Color(100, 100, 100);
                    }
                    else
                    {
                        if (F3Pressed)
                        {
                            midLine.setFillColor(sf::Color{100, 0, 100});
                        }
                        else
                        {
                            midLine.setFillColor(sf::Color{100, 100, 100});
                        }
                        background = sf::Color::Black;
                    }
                    break;

                case sf::Keyboard::A:
                    APressed = true;
                    break;

                case sf::Keyboard::Q:
                    QPressed = true;
                    break;

                case sf::Keyboard::P:
                    PPressed = true;
                    break;

                case sf::Keyboard::M:
                    MPressed = true;
                    break;

                default:
                    break;
                }
                break;

            case sf::Event::KeyReleased:
                switch (event.key.code)
                {
                case sf::Keyboard::A:
                    APressed = false;
                    break;

                case sf::Keyboard::Q:
                    QPressed = false;
                    break;

                case sf::Keyboard::P:
                    PPressed = false;
                    break;

                case sf::Keyboard::M:
                    MPressed = false;
                    break;

                default:
                    break;
                }
                break;

            default:
                break;
            }
        }

        if (!spacePressed)
        {
            //? GET TIME KEY DOWN --> DEFINE BALL SPEED
            if (APressed || QPressed || PPressed || MPressed)
            {
                frames++;
            }
            else if (!APressed || !QPressed || !PPressed || !MPressed)
            {
                frames = 0;
            }

            //? MOVE PLAYERS
            if (APressed)
            {
                if (player1Position.y >= 0)
                {
                    player1.setPosition(player1Position.x, player1Position.y - playerSpeed);
                }
                else
                {
                    player1.setPosition(player1Position.x, 0.f);
                }
            }
            if (QPressed)
            {
                if (player1Position.y + player1Dimensions.y <= SCREEN_Y)
                {
                    player1.setPosition(player1Position.x, player1Position.y + playerSpeed);
                }
                else
                {
                    player1.setPosition(player1Position.x, SCREEN_Y - player1Dimensions.y);
                }
            }
            if (PPressed)
            {
                if (player2Position.y >= 0)
                {
                    player2.setPosition(player2Position.x, player2Position.y - playerSpeed);
                }
                else
                {
                    player2.setPosition(player2Position.x, 0.f);
                }
            }
            if (MPressed)
            {
                if (player2Position.y + player2Dimensions.y <= SCREEN_Y)
                {
                    player2.setPosition(player2Position.x, player2Position.y + playerSpeed);
                }
                else
                {
                    player2.setPosition(player2Position.x, SCREEN_Y - player2Dimensions.y);
                }
            }

            //? BALL COLLISION ON WALLS
            if (ballPosition.x + ballDimensions >= SCREEN_X)
            {
                if (F1Pressed)
                    noise3.play();

                score1 += 1;
                winner = 1;
                newRound(ball, ballSpeed, ballPosition, player1, player1Position, player2, player2Position, winner);
                textScore1.setString(setScoreString(score1));
            }
            else if (ballPosition.x <= 0)
            {
                if (F1Pressed)
                    noise3.play();
                
                score2 += 1;
                winner = 2;
                newRound(ball, ballSpeed, ballPosition, player1, player1Position, player2, player2Position, winner);
                textScore2.setString(setScoreString(score2));
            }
            else if (ballPosition.y + ballDimensions >= SCREEN_Y)
            {
                if (F1Pressed)
                {
                    if (ballSpeed.x <= maxSpeed - 3)
                    {
                        noise4.play();
                    }
                    else
                    {
                        noise5.play();
                    }
                }
                
                ballSpeed.y = -ballSpeed.y;
                ballPosition.y = SCREEN_Y - ballDimensions;
            }
            else if (ballPosition.y <= 0)
            {
                if (F1Pressed)
                {
                    if (ballSpeed.x <= maxSpeed - 3)
                    {
                        noise4.play();
                    }
                    else
                    {
                        noise5.play();
                    }
                }
                
                ballSpeed.y = -ballSpeed.y;
                ballPosition.y = 0.f;
            }

            //? BALL COLLISION ON PLAYER
            float diffPosition1 = player1Position.x + player1Dimensions.x - ballSpeed.x;
            float diffPosition2 = player2Position.x - ballSpeed.x;

            //? PLAYER 1
            if (ballPosition.x < diffPosition1 && ballPosition.x+ballDimensions >= player1Position.x && ballPosition.y+ballDimensions > player1Position.y && ballPosition.y <= player1Position.y+player1Dimensions.y)
            {
                if (F1Pressed)
                {
                    if (player1Texture == 1)
                    {
                        noise6.setPitch(pitch[(int)abs(ballSpeed.x) - 4]);
                        noise6.play();
                    }
                    else if (player1Texture == 2)
                    {
                        noise7.setPitch(1);
                        noise7.play();
                    }
                    else
                    {
                        noise1.play();
                    }
                }

                float ballYMid = ballPosition.y + ballDimensions/2;
                int ballPlayerPosition = ballYMid - player1.getPosition().y;

                if (ballPlayerPosition < 0)
                {
                    ballSpeed.y = -7.f;
                }
                else if (ballPlayerPosition > 100)
                {
                    ballSpeed.y = 7.f;
                }
                else
                {
                    ballSpeed.y = bounce[ballPlayerPosition];
                }

                ballSpeed.x = -ballSpeed.x;
                setSpeedX(frames, ballSpeed);
            }
            //? PLAYER 2
            if (ballPosition.x+ballDimensions > diffPosition2 && ballPosition.x <= player2Position.x+player2Dimensions.x && ballPosition.y+ballDimensions > player2Position.y && ballPosition.y <= player2Position.y+player2Dimensions.y)
            {
                if (F1Pressed)
                {
                    if (player2Texture == 1)
                    {
                        noise6.setPitch(pitch[(int)abs(ballSpeed.x) - 4]);
                        noise6.play();
                    }
                    else if (player2Texture == 2)
                    {
                        noise7.setPitch(0.8);
                        noise7.play();
                    }
                    else
                    {
                        noise2.play();
                    }
                }

                float ballYMid = ballPosition.y + ballDimensions/2;
                int ballPlayerPosition = ballYMid - player2.getPosition().y;

                if (ballPlayerPosition < 0)
                {
                    ballSpeed.y = -7.f;
                }
                else if( ballPlayerPosition > 100)
                {
                    ballSpeed.y = 7.f;
                }
                else
                {
                    ballSpeed.y = bounce[ballPlayerPosition];
                }

                ballSpeed.x = -ballSpeed.x;
                setSpeedX(frames, ballSpeed);
            }
        
            ball.setPosition(ballPosition.x + ballSpeed.x, ballPosition.y + ballSpeed.y);

            player1Position = player1.getPosition();
            player2Position = player2.getPosition();
            ballPosition = ball.getPosition();

             if (ballSpeed.x > 0)
            {
                ball.setFillColor(leftSideColor);
            }
            else
            {
                ball.setFillColor(rightSideColor);
            }

            if (score1 > score2)
            {
                textScore1.setStyle(sf::Text::Underlined);
                textScore2.setStyle(sf::Text::Regular);
            }
            else if (score2 > score1)
            {
                textScore2.setStyle(sf::Text::Underlined);
                textScore1.setStyle(sf::Text::Regular);
            }

            // if (score1 % 2 != 0)
            // {
            //     player1Dimensions.y -= score1+1; 
            // }
            // else
            // {
            //     player1Dimensions.y -= score1; 
            // }
            // if (score2 % 2 != 0)
            // {
            //     player2Dimensions.y -= score2+1;
            // }
            // else
            // {
            //     player2Dimensions.y -= score2;
            // }

            //? F3 FANCY MODE
            if (F3Pressed)
            {
                if (score1 >= 50)
                {
                    player1.setTexture(&sword, true);
                    player1Texture = 2;
                }
                else if (score1 >= 20)
                {
                    player1.setTexture(&flute);
                    player1Texture = 1;
                }
                else if (score1 >= 10)
                {
                    player1.setTexture(&baguette);
                    player1Texture = 0;
                }

                if (score2 >= 50)
                {
                    player2.setTexture(&sword, true);
                    player2Texture = 2;
                }
                else if (score2 >= 20)
                {
                    player2.setTexture(&flute);
                    player2Texture = 1;
                }
                else if (score2 >= 10)
                {
                    player2.setTexture(&baguette);
                    player2Texture = 0;
                }
            }
            else
            {
                if (player1Texture == 2)
                    player1.setSize(player1Dimensions);
                if (player2Texture == 2)
                    player2.setSize(player1Dimensions);
                player1Texture = 0;
                player2Texture = 0;
                player1.setTexture(NULL);
                player2.setTexture(NULL);
            }
        }


        window.clear(background);
        if (launch)
        {
            launch = false;
            window.draw(startingBackground);
            window.draw(pong);
            window.draw(textPif);
            window.draw(textPaf);
            window.display();
            sleep(3);
            startSound.play();
        }
        else
        {
            window.draw(leftBackground);
            window.draw(rightBackground);
            if (F1Pressed)
                window.draw(soundOn);
            else
                window.draw(soundOff);
            window.draw(midLine);
            window.draw(player1);
            window.draw(player2);
            window.draw(ball);
            window.draw(textScore1);
            window.draw(textScore2);                
            window.display();
        }
    }

    return 0;
}

void newRound(sf::CircleShape& ball, sf::Vector2f& ballSpeed, sf::Vector2f& ballPosition, sf::RectangleShape& player1, sf::Vector2f& player1Position, sf::RectangleShape& player2, sf::Vector2f& player2Position, int winner)
{
    srand(time(0));
    for (int i = 0; i < 10; i++)
    {
        ballPosition = {
            SCREEN_X / 2 - ballDimensions / 2,
            (float)(rand() % (int)(SCREEN_Y - ballDimensions))
        };
    }

    if (winner == 1)
    {
        ballSpeed = {-minSpeed, 0.f};
    }
    else if (winner == 2)
    {
        ballSpeed = {minSpeed, 0.f};
    }
    player1Position = {40, SCREEN_Y/2 - player1Dimensions.y/2};
    player2Position = {SCREEN_X - 40 - player2Dimensions.x, SCREEN_Y/2 - player2Dimensions.y/2};

    ball.setPosition(ballPosition);
    player1.setPosition(player1Position);
    player2.setPosition(player2Position);
    
    usleep(500000);
}

void newGame(sf::CircleShape& ball, sf::Vector2f& ballSpeed, sf::Vector2f& ballPosition, sf::RectangleShape& player1, sf::Vector2f& player1Position, sf::RectangleShape& player2, sf::Vector2f& player2Position, int winner, int& score1, sf::Text& textScore1, int& score2, sf::Text& textScore2)
{
    newRound(ball, ballSpeed, ballPosition, player1, player1Position, player2, player2Position, winner);
    score1 = 0;
    score2 = 0;
    textScore1.setString("00");
    textScore2.setString("00");
}

std::string setScoreString(int score)
{
    string zero = "0";
    string string = "";

    if (score < 10)
    {
        string = zero + to_string(score);
    }
    else
    {
        string = to_string(score);
    }

    return string;
}

float setSpeedX(int frames, sf::Vector2f& ballSpeed)
{
    int framesSpeedConversion;
    frames /= 2;

    if (frames < minSpeed)
    {
        framesSpeedConversion = minSpeed;
    }
    else if (frames > maxSpeed)
    {
        framesSpeedConversion = maxSpeed;
    }
    else
    {
        framesSpeedConversion = frames;
    }

    if (ballSpeed.x > 0)
    {
        ballSpeed.x = framesSpeedConversion;
    }
    else
    {
        ballSpeed.x = -framesSpeedConversion;
    }

    return ballSpeed.x;
}


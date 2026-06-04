// Copyright 2026 Pan Yanyang
// SPDX-License-Identifier: Apache-2.0

#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <random>

class Game {
private:
    int score = 0;
    int hp = 3;
    bool is_menu = true;
    bool init_menu = false;
    bool init_game = false;
    bool is_over = false;
    bool init_over = false;

    std::random_device seed;
    std::mt19937 rand_gen;

    sf::RenderWindow window;
    sf::Clock clock;
    sf::Texture init_menu_back;
    sf::Texture eater;
    sf::Music bgm;
    sf::Music menu_bgm;
    sf::SoundBuffer eat_buffer;
    sf::Sound eat;
    sf::SoundBuffer hurt_buffer;
    sf::Sound hurt;
    sf::SoundBuffer death_buffer;
    sf::Sound death;
    sf::SoundBuffer bean_buffer;
    sf::Sound bean_move;

    sf::Sprite pacman;
    sf::CircleShape bean;
    sf::RectangleShape menu_back;
    sf::RectangleShape table;

    sf::Font font;
    sf::Text menu_text;
    sf::Text print_score;
    sf::Text print_hp;
    sf::Text over_title;
    sf::Text over_title_2;
    sf::Text over_score;
    sf::Text over_text;

public:
    Game();
    void run();

private:
    void process_event();
    void handle_event(const sf::Event::Closed&);
    void handle_event(const sf::Event::KeyPressed& key);
    void handle_event(const auto&) { }

    void update();
    void control(float delta_time, float speed = 300.f);
    bool outEdge(const sf::Sprite& sprite);

    void render();

    void initText();
};
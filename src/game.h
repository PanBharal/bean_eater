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
    bool init_game = false;
    bool game_over = false;

    std::random_device seed;
    std::mt19937 rand_gen;

    sf::RenderWindow window;
    sf::Clock clock;
    sf::Texture menu_back;
    sf::Texture eater;
    sf::Font font;
    sf::Music bgm;
    sf::SoundBuffer eat_buffer;
    sf::Sound eat;
    sf::SoundBuffer hurt_buffer;
    sf::Sound hurt;
    sf::SoundBuffer death_buffer;
    sf::Sound death;

    sf::Sprite sprite;
    sf::CircleShape bean;
    sf::RectangleShape back;
    sf::Text text;
    sf::Text print_score;
    sf::Text print_hp;
    sf::RectangleShape table;

public:
    Game();
    void run();

private:
    void handle_event(const sf::Event::Closed&);
    void handle_event(const sf::Event::KeyPressed& key);
    void handle_event(const auto&) { }

    void process_event();
    void update();
    void control(float delta_time, float speed = 300.f);
    bool out_boundary();
    void render();
};
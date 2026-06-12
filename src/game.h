// Copyright 2026 Pan Yanyang
// SPDX-License-Identifier: Apache-2.0

#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <random>

class Game {
private:
    enum out { NOOUT = 0,
        UP_OUT,
        DN_OUT,
        LT_OUT,
        RT_OUT };
    enum death { ALIVE = 0,
        GHOST,
        EDGE,
        HUNGER };
    enum level { WAIT = 0,
        NORMAL,
        HAVE_GHOST,
        GHOST_ACC };

    bool is_menu = true;
    bool init_menu = false;
    bool init_game = false;
    bool is_over = false;
    bool init_over = false;
    int score = 0;
    int hp = 5;
    death cause_death = ALIVE;
    level current_level = WAIT;

private:
    std::random_device seed;
    std::mt19937 rand_gen;

    sf::RenderWindow window;
    sf::Clock clock;
    sf::Texture init_menu_back;
    sf::Texture eater;
    sf::Texture tex_ghost;
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
    sf::Sprite ghost;
    sf::RectangleShape menu_back;
    sf::RectangleShape table;

    sf::Font simsun;
    sf::Text menu_text;
    sf::Text print_score;
    sf::Text print_hp;
    sf::Text dead_edge;
    sf::Text dead_hunger;
    sf::Text dead_ghost;
    sf::Text final_score;
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

    template <typename type>
    out outEdge(const type& sprite);

    void render();

    void initText();
};

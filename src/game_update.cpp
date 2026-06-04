// Copyright 2026 Pan Yanyang
// SPDX-License-Identifier: Apache-2.0

#include "game.h"
#include <iostream>

float bean_alive_time = 0;

void Game::update()
{
    if (is_menu && !is_over) {
        if (!init_menu) {
            menu_back.setSize({ 1080, 770 });
            menu_back.setTexture(&init_menu_back);
            menu_back.setPosition({ 0, 0 });

            menu_bgm.play();
            init_menu = true;
        }
    }

    if (!is_menu && !is_over) {
        print_score.setString(L"得分：" + std::to_wstring(score));
        print_hp.setString("HP: " + std::to_string(hp));

        if (!init_game) {
            clock.restart();
            menu_bgm.stop();
            bgm.play();
            init_game = true;
        }

        float delta_time = clock.restart().asSeconds();
        bean_alive_time += delta_time;
        control(delta_time, 1000.f);

        if (bean_alive_time >= 1.8f) {
            bean_move.play();
            std::uniform_real_distribution<float> newX(0.f, 1080.f - 2 * bean.getRadius());
            std::uniform_real_distribution<float> newY(80.f, 720.f - 2 * bean.getRadius());
            bean.setPosition({ newX(rand_gen), newY(rand_gen) });
            bean_alive_time = 0;
            score--;
        } else if (pacman.getGlobalBounds().findIntersection(bean.getGlobalBounds())) {
            eat.play();
            std::uniform_real_distribution<float> newX(0.f, 1080.f - 2 * bean.getRadius());
            std::uniform_real_distribution<float> newY(80.f, 720.f - 2 * bean.getRadius());
            bean.setPosition({ newX(rand_gen), newY(rand_gen) });
            bean_alive_time = 0;
            score++;
        }
        if (score < 0) {
            score = 0;
            hurt.play();
            hp--;
        }
        if (outEdge(pacman)) {
            pacman.setPosition({ 540, 360 });
            hurt.play();
            hp--;
        }
        if (hp <= 0) {
            is_over = true;
        }
    }

    if (is_over && !is_menu) {
        if (!init_over) {
            bgm.stop();
            death.play();
            over_score.setString(L"最终得分：" + std::to_wstring(score));
            init_over = true;
        }
    }
}

void Game::control(float delta_time, float speed)
{
    float move_speed = speed * delta_time;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        pacman.setRotation(sf::degrees(180.f));
        pacman.move({ -move_speed, 0.f });
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        pacman.setRotation(sf::degrees(360.f));
        pacman.move({ move_speed, 0.f });
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        pacman.setRotation(sf::degrees(-90.f));
        pacman.move({ 0.f, -move_speed });
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        pacman.setRotation(sf::degrees(90.f));
        pacman.move({ 0.f, move_speed });
    }
}

bool Game::outEdge(const sf::Sprite& sprite)
{
    sf::Vector2f pos = sprite.getPosition();
    float origin_dist = sprite.getGlobalBounds().size.x / 2.f;
    if (pos.x + origin_dist >= 1080) {
        return true;
    } else if (pos.x - origin_dist <= 0) {
        return true;
    } else if (pos.y + origin_dist >= 770) {
        return true;
    } else if (pos.y - origin_dist <= 0) {
        return true;
    } else
        return false;
}

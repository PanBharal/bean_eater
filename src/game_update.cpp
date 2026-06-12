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

            pacman.setPosition({ window.getSize().x / 2.f, 360 });
            pacman.setRotation(sf::degrees(0.f));
            pacman.setScale({ 1.f, 1.f });
            ghost.setPosition({ 100, 500 });

            current_level = NORMAL;
            init_game = true;
        }

        float delta_time = clock.restart().asSeconds();
        bean_alive_time += delta_time;
        control(delta_time, 900.f);

        if (bean_alive_time >= 2.f) {
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

        if (score >= 15 && current_level < HAVE_GHOST) {
            current_level = HAVE_GHOST;
        }

        if (current_level >= HAVE_GHOST) {
            if (pacman.getGlobalBounds().findIntersection(ghost.getGlobalBounds())) {
                hurt.play();
                pacman.setPosition({ window.getSize().x / 2.f, 360 });
                hp--;
                if (hp == 0)
                    cause_death = GHOST;
            }
            // 实现ghost匀速移动，比向量好上手多了
            if ((outEdge(ghost) == UP_OUT && ghost_incre.y < 0)
                || (outEdge(ghost) == DN_OUT && ghost_incre.y > 0)) {
                ghost_incre.y = -ghost_incre.y;
            } else if ((outEdge(ghost) == LT_OUT && ghost_incre.x < 0)
                || (outEdge(ghost) == RT_OUT && ghost_incre.x > 0)) {
                ghost_incre.x = -ghost_incre.x;
            } else if (score >= 30 && current_level != GHOST_ACC) {
                ghost_incre *= 1.5f;
                current_level = GHOST_ACC;
            }
            ghost.move(ghost_incre * delta_time);
        }

        if (score < 0) {
            score = 0;
            hurt.play();
            hp--;
            if (hp == 0)
                cause_death = HUNGER;
        }
        if (outEdge(pacman) != NOOUT) {
            pacman.setPosition({ window.getSize().x / 2.f, 360 });
            hurt.play();
            hp--;
            if (hp == 0)
                cause_death = EDGE;
        }
        if (hp <= 0) {
            is_over = true;
        }
    }

    if (is_over && !is_menu) {
        if (!init_over) {
            bgm.stop();
            death.play();
            final_score.setString(L"最终得分：" + std::to_wstring(score));
            init_over = true;
        }
    }
}

void Game::control(float delta_time, float speed)
{
    float move_speed = speed * delta_time;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf ::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        pacman.setRotation(sf::degrees(0.f));
        pacman.setScale({ -1.f, 1.f });
        pacman.move({ -move_speed, 0.f });
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        pacman.setRotation(sf::degrees(360.f));
        pacman.setScale({ 1.f, 1.f });
        pacman.move({ move_speed, 0.f });
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        pacman.setRotation(sf::degrees(-90.f));
        pacman.setScale({ 1.f, 1.f });
        pacman.move({ 0.f, -move_speed });
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        pacman.setRotation(sf::degrees(90.f));
        pacman.setScale({ 1.f, 1.f });
        pacman.move({ 0.f, move_speed });
    }
}

template <typename type>
Game::out Game::outEdge(const type& sprite)
{
    sf::Vector2f pos_sprite = sprite.getPosition();
    sf::Vector2f origin_dist = sprite.getGlobalBounds().size / 2.f;
    if (pos_sprite.x + origin_dist.x > window.getSize().x) {
        return RT_OUT;
    } else if (pos_sprite.x - origin_dist.x < 0) {
        return LT_OUT;
    } else if (pos_sprite.y + origin_dist.y > window.getSize().y) {
        return DN_OUT;
    } else if (pos_sprite.y - origin_dist.y < 0) {
        return UP_OUT;
    } else
        return NOOUT;
}

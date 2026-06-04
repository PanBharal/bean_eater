// Copyright 2026 Pan Yanyang
// SPDX-License-Identifier: Apache-2.0

#include "game.h"
#include <SFML/Graphics.hpp>
#include <filesystem>

template <typename type>
static type loadAsset(std::filesystem::path path)
{
    type file;
    if (!file.loadFromFile(path)) {
        throw std::runtime_error("Failed to load file!");
    }
    return file;
}

template <typename type>
static type openAsset(std::filesystem::path path)
{
    type file;
    if (!file.openFromFile(path)) {
        throw std::runtime_error("Failed to open file!");
    }
    return file;
}

Game::Game()
    : window(sf::VideoMode({ 1080, 770 }), L"吃豆人 - 我吃故我在！")
    , font(open_asset<sf::Font>("assets/song.ttf"))
    , bgm(open_asset<sf::Music>("assets/rifle.mp3"))
    , eat_buffer(load_asset<sf::SoundBuffer>("assets/eat.ogg"))
    , hurt_buffer(load_asset<sf::SoundBuffer>("assets/hurt.ogg"))
    , death_buffer(load_asset<sf::SoundBuffer>("assets/death.wav"))
    , menu_back(load_asset<sf::Texture>("assets/menu_back.png"))
    , eater(load_asset<sf::Texture>("assets/eater.png"))
    , sprite(eater)
    , text(font)
    , print_score(font)
    , print_hp(font)
    , eat(eat_buffer)
    , hurt(hurt_buffer)
    , death(death_buffer)
    , rand_gen(seed())
{
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);

    back.setSize({ 1080, 770 });
    back.setTexture(&menu_back);
    back.setPosition({ 0, 0 });

    text.setString(L"吃个球！");
    text.setCharacterSize(200);
    text.setOrigin(text.getGlobalBounds().getCenter());
    text.setPosition({ 540, 200 });
    text.setFillColor(sf::Color::Yellow);
    text.setOutlineColor({ 170, 136, 0 });
    text.setOutlineThickness(3.f);

    print_score.setCharacterSize(50);
    print_score.setFillColor(sf::Color::White);
    print_score.setOutlineColor({ 67, 67, 67 });
    print_score.setOutlineThickness(3.f);

    print_hp.setCharacterSize(50);
    print_hp.setPosition({ 540, 0 });
    print_hp.setFillColor(sf::Color::Red);
    print_hp.setOutlineColor(sf::Color::White);
    print_hp.setOutlineThickness(3.f);

    bgm.setVolume(60);
    bgm.setLooping(true);

    eat.setVolume(800);
    hurt.setVolume(800);
    death.setVolume(1000);

    sprite.setOrigin(sprite.getGlobalBounds().getCenter());
    sprite.setPosition({ 540, 360 });
    sprite.setRotation(sf::degrees(0.f));

    bean.setRadius(15.f);
    std::uniform_real_distribution<float> distX(0.f, 1080.f - 2 * bean.getRadius());
    std::uniform_real_distribution<float> distY(0.f, 720.f - 2 * bean.getRadius());
    bean.setPosition({ distX(rand_gen), distY(rand_gen) });
    bean.setFillColor({ 108, 209, 46 });
    bean.setOutlineThickness(-5.f);
    bean.setOutlineColor({ 181, 230, 29 });
}

void Game::run()
{
    while (window.isOpen()) {
        process_event();
        update();
        render();
    }
}

void Game::process_event()
{
    while (const auto event = window.pollEvent()) {
        event->visit([this](const auto& type) { this->handle_event(type); });
    }
}

void Game::handle_event(const sf::Event::Closed&)
{
    window.close();
}

void Game::handle_event(const sf::Event::KeyPressed& key)
{
    if (key.code == sf::Keyboard::Key::Escape) {
        window.close();
    } else if (key.code == sf::Keyboard::Key::Space) {
        is_menu = false;
    }
}

void Game::render()
{
    if (is_menu) {
        window.clear(sf::Color::White);
        window.draw(back);
        window.draw(text);
    } else if (!is_menu && !game_over) {
        window.clear({ 136, 171, 218 });
        window.draw(bean);
        window.draw(sprite);
        window.draw(print_score);
        window.draw(print_hp);
    } else if (game_over) {
        window.clear(sf::Color::Black);
        window.draw(print_hp);
        window.draw(print_score);
    }
    window.display();
}

void Game::update()
{

    if (!is_menu && !game_over) {
        if (!init_game) {
            bgm.play();
            init_game = true;
        }

        float delta_time = clock.restart().asSeconds();
        control(delta_time, 1000.f);

        print_score.setString(L"得分：" + std::to_wstring(score));
        print_hp.setString("HP: " + std::to_string(hp));

        if (sprite.getGlobalBounds().findIntersection(bean.getGlobalBounds())) {
            eat.play();
            std::uniform_real_distribution<float> newX(0.f, 1080.f - 2 * bean.getRadius());
            std::uniform_real_distribution<float> newY(0.f, 720.f - 2 * bean.getRadius());
            bean.setPosition({ newX(rand_gen), newY(rand_gen) });
            score++;
        }

        if (out_boundary()) {
            sprite.setPosition({ 540, 360 });
            hurt.play();
            hp--;
        }
    }
    if (hp <= 0 && !game_over) {
        game_over = true;
        bgm.stop();
        death.play();
        print_hp.setString(L"民以食为天！！\n由于你太爱吃墙\n被娇羞的熟石灰\n腐蚀了脑子......");
        print_hp.setCharacterSize(100);
        print_hp.setOrigin(print_hp.getGlobalBounds().getCenter());
        print_hp.setPosition({ 1080, 240 });
        print_score.setString(L"最终得分：" + std::to_wstring(score) + L"\n\n按ESC以退出");
        print_score.setOrigin(print_score.getGlobalBounds().getCenter());
        print_score.setPosition({ 540, 590 });
    }
}

void Game::control(float delta_time, float speed)
{
    float move_speed = speed * delta_time;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        sprite.setRotation(sf::degrees(180.f));
        sprite.move({ -move_speed, 0.f });
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        sprite.setRotation(sf::degrees(360.f));
        sprite.move({ move_speed, 0.f });
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        sprite.setRotation(sf::degrees(-90.f));
        sprite.move({ 0.f, -move_speed });
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        sprite.setRotation(sf::degrees(90.f));
        sprite.move({ 0.f, move_speed });
    }
}

bool Game::out_boundary()
{
    sf::Vector2f pos = sprite.getPosition();
    float eater_origin = sprite.getGlobalBounds().size.x / 2.f;
    if (pos.x + eater_origin >= 1080) {
        return true;
    } else if (pos.x - eater_origin <= 0) {
        return true;
    } else if (pos.y + eater_origin >= 770) {
        return true;
    } else if (pos.y - eater_origin <= 0) {
        return true;
    } else
        return false;
}

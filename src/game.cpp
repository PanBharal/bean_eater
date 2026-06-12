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
    , simsun(openAsset<sf::Font>("assets/song.ttf"))
    , menu_bgm(openAsset<sf::Music>("assets/I eat so I'm here.mp3"))
    , bgm(openAsset<sf::Music>("assets/rifle.mp3"))
    , eat_buffer(loadAsset<sf::SoundBuffer>("assets/eat.ogg"))
    , hurt_buffer(loadAsset<sf::SoundBuffer>("assets/hurt.ogg"))
    , death_buffer(loadAsset<sf::SoundBuffer>("assets/death.wav"))
    , bean_buffer(loadAsset<sf::SoundBuffer>("assets/pong.wav"))
    , init_menu_back(loadAsset<sf::Texture>("assets/menu_back.png"))
    , eater(loadAsset<sf::Texture>("assets/eater.png"))
    , tex_ghost(loadAsset<sf::Texture>("assets/ghost.png"))
    , pacman(eater)
    , ghost(tex_ghost)
    , menu_text(simsun)
    , dead_edge(simsun)
    , dead_hunger(simsun)
    , dead_ghost(simsun)
    , over_text(simsun)
    , print_score(simsun)
    , print_hp(simsun)
    , final_score(simsun)
    , eat(eat_buffer)
    , hurt(hurt_buffer)
    , death(death_buffer)
    , bean_move(bean_buffer)
    , rand_gen(seed())
{
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);

    initText();

    menu_bgm.setVolume(10);
    menu_bgm.setLooping(true);

    bgm.setVolume(50);
    bgm.setLooping(true);

    eat.setVolume(700);
    hurt.setVolume(700);
    death.setVolume(800);
    bean_move.setVolume(800);

    pacman.setOrigin(pacman.getGlobalBounds().getCenter());
    ghost.setOrigin(ghost.getGlobalBounds().getCenter());

    bean.setRadius(15.f);
    std::uniform_real_distribution<float> distX(0.f, 1080.f - 2 * bean.getRadius());
    std::uniform_real_distribution<float> distY(80.f, 720.f - 2 * bean.getRadius());
    bean.setPosition({ distX(rand_gen), distY(rand_gen) });
    bean.setFillColor({ 108, 209, 46 });
    bean.setOutlineThickness(-5.f);
    bean.setOutlineColor({ 181, 230, 29 });
}

void Game::initText()
{
    menu_text.setString(L"吃个球！");
    menu_text.setCharacterSize(200);
    menu_text.setOrigin(menu_text.getGlobalBounds().getCenter());
    menu_text.setPosition({ 540, 160 });
    menu_text.setFillColor({ 67, 67, 67 });
    menu_text.setOutlineColor(sf::Color::White);
    menu_text.setOutlineThickness(2.f);

    print_hp.setString("HP: " + std::to_string(hp));
    print_hp.setCharacterSize(50);
    print_hp.setOrigin(print_hp.getGlobalBounds().getCenter());
    print_hp.setPosition({ 1080 - print_hp.getGlobalBounds().size.x - 60, print_hp.getGlobalBounds().size.y / 2 + 10 });
    print_hp.setFillColor(sf::Color::Red);
    print_hp.setOutlineColor(sf::Color::White);
    print_hp.setOutlineThickness(2.f);

    print_score.setString(L"得分：" + std::to_wstring(score));
    print_score.setCharacterSize(50);
    print_score.setOrigin(print_score.getGlobalBounds().getCenter());
    print_score.setPosition({ print_score.getGlobalBounds().size.x + 10, print_score.getGlobalBounds().size.y / 2 + 10 });
    print_score.setFillColor(sf::Color::White);
    print_score.setOutlineColor({ 67, 67, 67 });
    print_score.setOutlineThickness(2.f);

    dead_edge.setString(L"由于你太爱吃墙\n被娇羞的熟石灰\n腐蚀了脑子......");
    dead_edge.setCharacterSize(100);
    dead_edge.setOrigin(dead_edge.getGlobalBounds().getCenter());
    dead_edge.setPosition({ 540, 230 });
    dead_edge.setFillColor(sf::Color::Red);

    dead_hunger.setString(L"民以食为天！！\nbro怎么连食物都追不上...");
    dead_hunger.setCharacterSize(80);
    dead_hunger.setOrigin(dead_hunger.getGlobalBounds().getCenter());
    dead_hunger.setPosition({ 540, 300 });
    dead_hunger.setFillColor(sf::Color::Red);

    dead_ghost.setString(L"见着你幽灵叔叔还不递支烟？");
    dead_ghost.setCharacterSize(80);
    dead_ghost.setOrigin(dead_ghost.getGlobalBounds().getCenter());
    dead_ghost.setPosition({ 540, 300 });
    dead_ghost.setFillColor(sf::Color::Red);

    final_score.setString(L"最终得分：" + std::to_wstring(score));
    final_score.setCharacterSize(50);
    final_score.setOrigin(final_score.getGlobalBounds().getCenter());
    final_score.setPosition({ 540, 560 });
    final_score.setFillColor(sf::Color::White);
    final_score.setOutlineColor({ 67, 67, 67 });
    final_score.setOutlineThickness(2.f);

    over_text.setString(L"<N> 菜单  <R> 继续  <ESC> 退出");
    over_text.setCharacterSize(30);
    over_text.setOrigin(over_text.getGlobalBounds().getCenter());
    over_text.setPosition({ 540, 690 });
    over_text.setFillColor(sf::Color::White);
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
    } else if (key.code == sf::Keyboard::Key::R) {
        init_game = false;
        is_over = false;
        init_over = false;
        score = 0;
        hp = 5;
        cause_death = ALIVE;
        current_level = NORMAL;
    } else if (key.code == sf::Keyboard::Key::N) {
        is_menu = true;
        init_menu = false;
        init_game = false;
        is_over = false;
        init_over = false;
        score = 0;
        hp = 5;
        cause_death = ALIVE;
        current_level = WAIT;
    }
}

void Game::render()
{
    if (is_menu) {
        window.clear(sf::Color::White);
        window.draw(menu_back);
        window.draw(menu_text);
    } else if (!is_menu && !is_over) {
        window.clear({ 136, 171, 218 });
        window.draw(bean);
        window.draw(pacman);
        if (current_level >= HAVE_GHOST) {
            window.draw(ghost);
        }
        window.draw(print_score);
        window.draw(print_hp);
    } else if (is_over) {
        window.clear(sf::Color::Black);
        if (cause_death == HUNGER)
            window.draw(dead_hunger);
        else if (cause_death == EDGE)
            window.draw(dead_edge);
        else if (cause_death == GHOST)
            window.draw(dead_ghost);
        window.draw(final_score);
        window.draw(over_text);
    }
    window.display();
}

#include "Menu.hpp"

Menu::Menu() {}
Menu::~Menu() {}

void Menu::init(int screen_width, int menu_height, SDL_Color color)
{
    rect_.x = rect_.y = 0;
    rect_.w = screen_width;
    rect_.h = menu_height;
    color_ = color;
}

void Menu::addButton(SDL_Renderer *renderer, int width, std::string message, SDL_Color button_color, SDL_Color text_color, void (*func)())
{
    SDL_Rect d;
    d.w = width;
    d.h = rect_.h;
    d.y = 0;
    if (buttons_.size() == 0)
    {
        d.x = 0;
        buttons_.push_back(std::make_unique<Button>());
        buttons_[buttons_.size() - 1]->init(renderer, d, message, button_color, text_color, func);
    }
    else if ((buttons_[buttons_.size() - 1]->getX() + buttons_[buttons_.size() - 1]->getW() + width) < rect_.w)
    {
        d.x = buttons_[buttons_.size() - 1]->getX() + buttons_[buttons_.size() - 1]->getW();
        buttons_.push_back(std::make_unique<Button>());
        buttons_[buttons_.size() - 1]->init(renderer, d, message, button_color, text_color, func);
    }
}

void Menu::render(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, color_.r, color_.g, color_.b, color_.a);
    SDL_RenderFillRect(renderer, &rect_);
    for (int i = 0; i < buttons_.size(); i++)
    {
        buttons_[i]->render(renderer);
    }
}

void Menu::update(int x, int y)
{
    for (int i = 0; i < buttons_.size(); i++)
    {
        buttons_[i]->update(x, y);
    }
}

#ifndef TP4_TOWERDEFENSE_LTEXTURE_H
#define TP4_TOWERDEFENSE_LTEXTURE_H

#include <string>
#include <SDL2/SDL_system.h>
#include <SDL2/SDL_ttf.h>

class Texture {
public:
    Texture();
    ~Texture();

    bool generateFromText(const std::string &text, SDL_Renderer *renderer,
            TTF_Font *font, SDL_Color text_color = { 0xC5, 0xC8, 0xC6 },
            SDL_Color background_color = { 0x1D, 0x1F, 0x21 });

    bool loadFromFile(const std::string &path,
                      SDL_Renderer *gRenderer,
                      int r = 0xFF,
                      int g = 0,
                      int b = 0xFF);

    //Deallocates texture
    void free();

    void setColor(Uint8 red, Uint8 green, Uint8 blue);
    void setBlendMode(SDL_BlendMode blending);
    void setAlpha(Uint8 alpha);

    void render(SDL_Renderer *renderer,
                int x,
                int y,
                SDL_Rect *clip = NULL,
                double angle = 0.0,
                SDL_Point *center = NULL,
                SDL_RendererFlip flip = SDL_FLIP_NONE);
    void renderSprite(SDL_Renderer *renderer, int x, int y, SDL_Rect *clip);

    int getWidth() const;
    int getHeight() const;

private:
    SDL_Texture *mTexture;
    int mWidth;
    int mHeight;
};


#endif //TP4_TOWERDEFENSE_LTEXTURE_H

#ifndef TP4_TOWERDEFENSE_LTEXTURE_H
#define TP4_TOWERDEFENSE_LTEXTURE_H

#include <string>
#include <SDL_system.h>
#include <SDL_ttf.h>

//Texture wrapper class
class LTexture {
public:
    //Initializes variables
    LTexture();

    //Deallocates memory
    ~LTexture();

    bool generateFromText(const std::string &text, SDL_Renderer *renderer,
            TTF_Font *font, SDL_Color text_color = { 0xC5, 0xC8, 0xC6, 0xCC },
            SDL_Color background_color = { 0x1D, 0x1F, 0x21, 0xCC});

    //Loads image at specified path
    bool loadFromFile(const std::string &path, SDL_Renderer *gRenderer);

    //Deallocates texture
    void free();

    //Set color modulation
    void setColor(Uint8 red, Uint8 green, Uint8 blue);

    //Set blending
    void setBlendMode(SDL_BlendMode blending);

    //Set alpha modulation
    void setAlpha(Uint8 alpha);

    //Renders texture at given point
    void render(SDL_Renderer *gRenderer,
                int x,
                int y,
                SDL_Rect *clip = NULL,
                double angle = 0.0,
                SDL_Point *center = NULL,
                SDL_RendererFlip flip = SDL_FLIP_NONE);

    void render_sprite(SDL_Renderer *gRenderer, int x, int y, SDL_Rect* clip);

    //Gets image dimensions
    int getWidth();

    int getHeight();

private:
    //The actual hardware texture
    SDL_Texture *mTexture;

    //Image dimensions
    int mWidth;
    int mHeight;
};


#endif //TP4_TOWERDEFENSE_LTEXTURE_H

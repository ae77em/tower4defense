#include <SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "LTexture.h"

LTexture::LTexture() {
    //Initialize
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture() {
    //Deallocate
    free();
}
bool LTexture::generateFromText(const std::string &text, SDL_Renderer *renderer,
        TTF_Font *font, SDL_Color text_color, SDL_Color background_color) {
    //Get rid of preexisting texture
    free();

    /* WARNING: A text_surface will fail to load if the string
       to generate is empty. Setting the string to a single space
       is a workaround for this behavior. */
    SDL_Surface* text_surface = TTF_RenderText_Shaded(font,
            (text == "") ? " " : text.c_str(),
            text_color, background_color);
    if (! text_surface) return false;

    mTexture = SDL_CreateTextureFromSurface(renderer, text_surface);
    if (! mTexture) {
        SDL_FreeSurface(text_surface);
        return false;
    }

    mWidth = text_surface->w;
    mHeight = text_surface->h;

    SDL_FreeSurface(text_surface);
    return true;
}

bool LTexture::loadFromFile(const std::string &path, SDL_Renderer *gRenderer) {
    //Get rid of preexisting texture
    free();

    //The final texture
    SDL_Texture *newTexture = NULL;

    //Load image at specified path
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(),
               IMG_GetError());
    } else {
        //Color key image
        SDL_SetColorKey(loadedSurface,
                        SDL_TRUE,
                        SDL_MapRGB(loadedSurface->format, 0xFF, 0, 0xFF));

        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n",
                   path.c_str(), SDL_GetError());
        } else {
            //Get image dimensions
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    //Return success
    mTexture = newTexture;
    return mTexture != NULL;
}

void LTexture::free() {
    //Free texture if it exists
    if (mTexture != NULL) {
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
    //Modulate texture rgb
    SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending) {
    //Set blending function
    SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha) {
    //Modulate texture alpha
    SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(SDL_Renderer *gRenderer,
                      int x,
                      int y,
                      SDL_Rect *clip,
                      double angle,
                      SDL_Point *center,
                      SDL_RendererFlip flip) {
    //Set rendering space and render to screen
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};

    //Set clip rendering dimensions
    if (clip != NULL) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    //Render to screen
    SDL_RenderCopyEx(gRenderer,
                     mTexture,
                     clip,
                     &renderQuad,
                     angle,
                     center,
                     flip);
}


void LTexture::render_sprite(SDL_Renderer *gRenderer, int x, int y, SDL_Rect* clip) {
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };

    //Set clip rendering dimensions
    if(clip != NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    //Render to screen
    SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);
}

int LTexture::getWidth() {
    return mWidth;
}

int LTexture::getHeight() {
    return mHeight;
}

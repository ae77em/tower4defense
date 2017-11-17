#include "Animation.h"
#include <SDL2/SDL.h>
#include "Texture.h"
#include <stdexcept>
#include <utility>

CompositeAnimation::CompositeAnimation() {}

void CompositeAnimation::memberPushBack(Animation *m, int x, int y) {
    members.push_back(Record(std::unique_ptr<Animation>(m), Point(x, y)));
}

void CompositeAnimation::renderFrame(int frame, int x, int y) {
    for (auto& pair : members) {
        auto& member(pair.first);
        auto& offset(pair.second);
        member->renderFrame(frame, x + offset.x, y + offset.y);
    }
}

int CompositeAnimation::getHeight() const {
    int max = 0;
    for (auto& pair : members) {
        auto& member(pair.first);
        auto& offset(pair.second);
        int curr = offset.y + member->getHeight();
        max = (curr < max) ? max : curr;
    }
    return max;
}

int CompositeAnimation::getWidth() const {
    int max = 0;
    for (auto& pair : members) {
        auto& member(pair.first);
        auto& offset(pair.second);
        int curr = offset.x + member->getWidth();
        max = (curr < max) ? max : curr;
    }
    return max;
}

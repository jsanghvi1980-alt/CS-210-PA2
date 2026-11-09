#pragma once
#include "Scene.h"

// The whole story. It knows the first scene and nothing else.
//
// RULES (do not change):
//   - Keep every name and signature exactly as written.
//   - The private helpers must be recursive. No loops that walk down the tree.
//   - This class must work for ANY T. Never mention Moment in here.
template <typename T>
class StoryTree {
private:
    Scene<T>* root_;

    // TODO 6: how many scenes are in the story that starts at scene?
    // An empty story has 0 scenes.
    int countScenes(Scene<T>* scene) const {
        if (scene == nullptr) {
            return 0;
        }

        int total = 1;

        for (int i = 0; i < scene->getNumChoices(); i++) {
            total += countScenes(scene->getChoice(i));
        }

        return total;
    }

    // TODO 7: how many endings are in the story that starts at scene?
    int countEndings(Scene<T>* scene) const {
        if (scene == nullptr) {
            return 0;
        }

        if (scene->isEnding()) {
            return 1;
        }

        int total = 0;

        for (int i = 0; i < scene->getNumChoices(); i++) {
            total += countEndings(scene->getChoice(i));
        }

        return total;
    }

    // TODO 8: the most choices a player can make on the way to an ending,
    // starting at scene. A single ending scene has height 0.
    // An empty story has height -1.
    int height(Scene<T>* scene) const {
        if (scene == nullptr) {
            return -1;
        }

        if (scene->isEnding()) {
            return 0;
        }

        int tallest = -1;

        for (int i = 0; i < scene->getNumChoices(); i++) {
            int childHeight = height(scene->getChoice(i));

            if (childHeight > tallest) {
                tallest = childHeight;
            }
        }

        return tallest + 1;
    }

public:
    StoryTree() : root_(nullptr) {}

    ~StoryTree() {
        clear();
    }

    void setRoot(Scene<T>* root) {
        clear();
        root_ = root;
    }

    Scene<T>* getRoot() const {
        return root_;
    }

    bool isEmpty() const {
        return root_ == nullptr;
    }

    // TODO 9: free every scene in the story and leave the tree empty.
    // Hint: what does deleting a Scene already do for you?
    void clear() {
        delete root_;
        root_ = nullptr;
    }

    int countScenes() const {
        return countScenes(root_);
    }

    int countEndings() const {
        return countEndings(root_);
    }

    int height() const {
        return height(root_);
    }
};
#pragma once
#include <vector>

struct SceneNode {
    SceneNode* parent = nullptr;
    std::vector<SceneNode*> children;

    void addChild(SceneNode* child) {
        child->parent = this;
        children.push_back(child);
    }
};

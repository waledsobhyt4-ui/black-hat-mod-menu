#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class FloatingButton : public CCNode, public CCTouchDelegate {
protected:
    CCSprite* m_sprite = nullptr;
    bool m_dragging = false;
    CCPoint m_offset;
    std::function<void()> m_callback;

public:
    static FloatingButton* create(const char* spriteName, std::function<void()> cb) {
        auto ret = new FloatingButton();
        if (ret && ret->init(spriteName, cb)) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }

    bool init(const char* spriteName, std::function<void()> cb) {
        if (!CCNode::init()) return false;

        m_callback = cb;

        m_sprite = CCSprite::createWithSpriteFrameName(spriteName);
        if (!m_sprite) m_sprite = CCSprite::create(spriteName);

        auto bg = CCScale9Sprite::create("GJ_square01.png");
        bg->setContentSize({50, 50});
        bg->setColor({20, 20, 20});
        bg->setOpacity(230);
        this->addChild(bg);

        m_sprite->setScale(0.75f);
        m_sprite->setColor({255, 215, 0});
        this->addChild(m_sprite);

        this->setContentSize({50, 50});
        this->setAnchorPoint({0.5f, 0.5f});

        this->setTouchEnabled(true);
        this->setTouchMode(kCCTouchesOneByOne);
        this->setTouchPriority(-500);

        return true;
    }

    bool ccTouchBegan(CCTouch* touch, CCEvent*) override {
        auto local = this->convertToNodeSpace(touch->getLocation());
        CCRect rect(-25, -25, 50, 50);

        if (rect.containsPoint(local)) {
            m_dragging = false;
            m_offset = touch->getLocation() - this->getPosition();
            this->setScale(1.1f);
            return true;
        }
        return false;
    }

    void ccTouchMoved(CCTouch* touch, CCEvent*) override {
        m_dragging = true;
        this->setPosition(touch->getLocation() - m_offset);
    }

    void ccTouchEnded(CCTouch*, CCEvent*) override {
        this->setScale(1.0f);
        if (!m_dragging && m_callback) m_callback();
    }

    void ccTouchCancelled(CCTouch*, CCEvent*) override {
        this->setScale(1.0f);
    }
};

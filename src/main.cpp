#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include "FloatingButton.hpp"

using namespace geode::prelude;

// === GLOBAL STATE ===
namespace BlackHat {
    bool noclip = false;
    bool godmode = false;
    bool speedhack = false;
    float speed = 1.0f;
}

// === MOD MENU POPUP ===
class BlackHatMenu : public FLAlertLayer {
public:
    static BlackHatMenu* create() {
        auto ret = new BlackHatMenu();
        if (ret && ret->init()) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }

    bool init() {
        if (!FLAlertLayer::init(nullptr, "Black Hat", "Close", nullptr, 320.f)) {
            return false;
        }

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        if (m_mainLayer) {
            m_mainLayer->setColor({15, 15, 15});
        }

        auto title = CCLabelBMFont::create("Black Hat Mod Menu", "bigFont.fnt");
        title->setPosition({ winSize.width / 2, winSize.height / 2 + 110 });
        title->setScale(0.7f);
        title->setColor({255, 215, 0});
        this->addChild(title);

        auto menu = CCMenu::create();
        menu->setPosition({0, 0});
        this->addChild(menu);

        // Noclip
        auto noclipLabel = CCLabelBMFont::create("Noclip", "goldFont.fnt");
        noclipLabel->setScale(0.55f);
        noclipLabel->setPosition({ winSize.width / 2 - 70, winSize.height / 2 + 55 });
        this->addChild(noclipLabel);

        auto noclipToggle = CCMenuItemToggler::createWithStandardSprites(
            this, menu_selector(BlackHatMenu::onNoclip), 0.7f
        );
        noclipToggle->setPosition({ winSize.width / 2 + 70, winSize.height / 2 + 55 });
        noclipToggle->toggle(BlackHat::noclip);
        menu->addChild(noclipToggle);

        // Godmode
        auto godLabel = CCLabelBMFont::create("Godmode", "goldFont.fnt");
        godLabel->setScale(0.55f);
        godLabel->setPosition({ winSize.width / 2 - 70, winSize.height / 2 + 15 });
        this->addChild(godLabel);

        auto godToggle = CCMenuItemToggler::createWithStandardSprites(
            this, menu_selector(BlackHatMenu::onGodmode), 0.7f
        );
        godToggle->setPosition({ winSize.width / 2 + 70, winSize.height / 2 + 15 });
        godToggle->toggle(BlackHat::godmode);
        menu->addChild(godToggle);

        // Speedhack
        auto speedLabel = CCLabelBMFont::create("Speedhack x2", "goldFont.fnt");
        speedLabel->setScale(0.55f);
        speedLabel->setPosition({ winSize.width / 2 - 70, winSize.height / 2 - 25 });
        this->addChild(speedLabel);

        auto speedToggle = CCMenuItemToggler::createWithStandardSprites(
            this, menu_selector(BlackHatMenu::onSpeedhack), 0.7f
        );
        speedToggle->setPosition({ winSize.width / 2 + 70, winSize.height / 2 - 25 });
        speedToggle->toggle(BlackHat::speedhack);
        menu->addChild(speedToggle);

        return true;
    }

    void onNoclip(CCObject* sender) {
        auto t = static_cast<CCMenuItemToggler*>(sender);
        BlackHat::noclip = !t->isToggled();
        Notification::create(
            BlackHat::noclip ? "Noclip ON" : "Noclip OFF",
            NotificationIcon::Success
        )->show();
    }

    void onGodmode(CCObject* sender) {
        auto t = static_cast<CCMenuItemToggler*>(sender);
        BlackHat::godmode = !t->isToggled();
        Notification::create(
            BlackHat::godmode ? "Godmode ON" : "Godmode OFF",
            NotificationIcon::Success
        )->show();
    }

    void onSpeedhack(CCObject* sender) {
        auto t = static_cast<CCMenuItemToggler*>(sender);
        BlackHat::speedhack = !t->isToggled();
        BlackHat::speed = BlackHat::speedhack ? 2.0f : 1.0f;
        Notification::create(
            BlackHat::speedhack ? "Speedhack ON" : "Speedhack OFF",
            NotificationIcon::Success
        )->show();
    }
};

// === MENU LAYER ===
class $modify(BHMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        auto btn = FloatingButton::create("GJ_optionsBtn_001.png", []() {
            BlackHatMenu::create()->show();
        });
        btn->setPosition({ winSize.width - 35.f, winSize.height - 35.f });
        btn->setID("blackhat-menu-btn"_spr);
        this->addChild(btn, 100);

        return true;
    }
};

// === PLAY LAYER ===
class $modify(BHPlayLayer, PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        auto btn = FloatingButton::create("GJ_optionsBtn_001.png", []() {
            BlackHatMenu::create()->show();
        });
        btn->setPosition({ winSize.width - 35.f, winSize.height - 60.f });
        btn->setID("blackhat-menu-btn-play"_spr);
        this->addChild(btn, 100);

        return true;
    }

    void destroyPlayer(PlayerObject* player, GameObject* obj) {
        if (BlackHat::noclip || BlackHat::godmode) return;
        PlayLayer::destroyPlayer(player, obj);
    }
};

// === PLAYER (Speedhack) ===
class $modify(BHPlayer, PlayerObject) {
    void update(float dt) {
        if (BlackHat::speedhack && BlackHat::speed != 1.0f) {
            dt *= BlackHat::speed;
        }
        PlayerObject::update(dt);
    }
};

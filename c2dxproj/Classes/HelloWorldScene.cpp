/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"

#include <unistd.h>
#include <limits>
#include <memory>
#include <string>
#include <stdexcept>

USING_NS_CC;

HelloWorld::~HelloWorld() {
    mpz_clear(m_numAbsValue);
    mpz_clear(m_numDenominator);
    mpz_clear(m_numCurrentValue);
}

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    // https://machinecognitis.github.io/Math.Gmp.Native/html/89d8c501-6c12-9104-7e2d-be411f443b62.htm
    mpz_init(m_numCurrentValue);
    mpz_init(m_numDenominator);
    mpz_init(m_numAbsValue);

    char buffer[100];
    memset(buffer, 0, sizeof(buffer));
    std::snprintf(buffer, sizeof(buffer), "Init TID %d %d", gettid(), std::numeric_limits<double>::digits10);
    m_pLabelInitTid = Label::createWithTTF(buffer, "fonts/Marker Felt.ttf", 24);
    if (m_pLabelInitTid == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        m_pLabelInitTid->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - m_pLabelInitTid->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(m_pLabelInitTid, 1);
    }

    m_iIteration = 0;
    m_dCurrentValue = 0.0;

    m_pLabelUpdateTid = Label::createWithTTF(buffer, "fonts/Marker Felt.ttf", 21);
    if (m_pLabelUpdateTid == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        m_pLabelUpdateTid->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - 20 - m_pLabelUpdateTid->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(m_pLabelUpdateTid, 2);
    }

    m_pLabelRenderTid = Label::createWithTTF(buffer, "fonts/Marker Felt.ttf", 18);
    if (m_pLabelUpdateTid == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        m_pLabelRenderTid->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - 40 - m_pLabelRenderTid->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(m_pLabelRenderTid, 3);
    }

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");
    if (sprite == nullptr)
    {
        problemLoading("'HelloWorld.png'");
    }
    else
    {
        // position the sprite on the center of the screen
        sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

        // add the sprite as a child to this layer
        this->addChild(sprite, 0);
    }

    this->scheduleUpdate();

    return true;
}

double HelloWorld::calculateCurrentFraction(int iteration)
{
    int denominator = (2 * iteration) + 1;
    double absValue = ((double)4.0) / ((double)denominator);
    if ( (iteration % 2) == 0 ) {
        return absValue;
    }
//    CCLOG("calculateCurrentFraction %d %Lf", iteration, absValue);
    // CCLOG("calculateCurrentFraction %d %0.15f", iteration, absValue);
    return -absValue;
}

void HelloWorld::update(float f)
{
    double currentFraction = 0.0;
    int iteration = 1000;
    for ( int i = 0; i < iteration; i++ ) {
        for ( int j = 0; j < iteration; j++ ) {
            currentFraction = calculateCurrentFraction(j);
        }
        currentFraction = calculateCurrentFraction(m_iIteration);
        m_dCurrentValue += currentFraction;
        m_iIteration++;
    }
    // CCLOG("HelloWorld update currentValue: %Lf", m_dCurrentValue);
    CCLOG("HelloWorld update currentValue: %0.15f", m_dCurrentValue);

    char buffer[100];
    memset(buffer, 0, sizeof(buffer));
    std::snprintf(buffer, sizeof(buffer), "Update TID %d %0.15f", gettid(), m_dCurrentValue);
    m_sCurrentText = buffer;
    // CCLOG("The current tid: %d text: %s", gettid(), m_sCurrentText.c_str());
    m_pLabelUpdateTid->setString(m_sCurrentText);
    // m_pLabel->setString(m_sCurrentText);
//    m_pLabel->setString("What the hell?");
}

void HelloWorld::render(cocos2d::Renderer* renderer, const cocos2d::Mat4& eyeTransform, const cocos2d::Mat4* eyeProjection)
{
    char buffer[100];
    memset(buffer, 0, sizeof(buffer));
    std::snprintf(buffer, sizeof(buffer), "Render TID %d %d", gettid(), m_iIteration);
    std::string currentText = buffer;
    m_pLabelRenderTid->setString(currentText);
    Scene::render(renderer, eyeTransform, eyeProjection);
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}

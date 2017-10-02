#include "Base.h"
#include <stdio.h>
#include <stdarg.h>
#include "UI.h"

std::vector<Base*> Base::root_;
std::set<Base*> Base::collector_;

Base::~Base()
{
    for (auto c : childs_)
    {
        delete c;
    }
}

void Base::LOG(const char* format, ...)
{
    char s[1000];
    va_list arg_ptr;
    va_start(arg_ptr, format);
    vsprintf(s, format, arg_ptr);
    va_end(arg_ptr);
    fprintf(stderr, s);
}

void Base::drawAll()
{
    //从最后一个独占屏幕的场景开始画
    int begin_base = 0;
    for (int i = 0; i < Base::root_.size(); i++)    //记录最后一个全屏的层
    {
        root_[i]->backRun();
        if (Base::root_[i]->full_window_)
        {
            begin_base = i;
        }
    }
    for (int i = begin_base; i < Base::root_.size(); i++)  //从最后一个全屏层开始画
    {
        auto b = Base::root_[i];
        if (b->visible_)
        {
            b->drawSelfAndChilds();
        }
    }
}

void Base::setPosition(int x, int y)
{
    for (auto c : childs_)
    {
        c->setPosition(c->x_ + x - x_, c->y_ + y - y_);
    }
    x_ = x; y_ = y;
}

//运行
//参数为是否在root中运行，为真则参与绘制，为假则不会被画出
int Base::run(bool in_root /*= true*/)
{
    BP_Event e;
    auto engine = Engine::getInstance();
    loop_ = true;
    result_ = -1;
    if (in_root) { addOnRootTop(this); }
    entrance();
    while (loop_ && engine->pollEvent(e) >= 0)
    {
        int t0 = engine->getTicks();
        if (Base::root_.size() == 0) { break; }
        Base::drawAll();
        checkStateAndEvent(e);
        switch (e.type)
        {
        case BP_QUIT:
            //if (engine->showMessage("Quit"))
            //loop_ = false;
            break;
        default:
            break;
        }
        engine->renderPresent();
        int t1 = engine->getTicks();
        int t = 25 - (t1 - t0);
        if (t <= 0) { t = 1; }
        engine->delay(t);
        //LOG("one frame\n");
    }
    //while (engine->pollEvent(e) > 0);
    //engine->flushEvent();
    exit();
    if (in_root) { removeFromRoot(this); }
    return result_;
}

Base* Base::removeFromRoot(Base* b)
{
    if (b == nullptr)
    {
        if (root_.size() > 0)
        {
            b = root_.back();
            root_.pop_back();
        }
    }
    else
    {
        for (int i = 0; i < root_.size(); i++)
        {
            if (root_[i] == b)
            {
                root_.erase(root_.begin() + i);
                break;
            }
        }
    }
    //collector_.insert(b);
    return b;
}

void Base::addChild(Base* b)
{
    childs_.push_back(b);
}

void Base::addChild(Base* b, int x, int y)
{
    addChild(b);
    b->setPosition(x_ + x, y_ + y);
}

void Base::removeChild(Base* b)
{
    for (int i = 0; i < childs_.size(); i++)
    {
        if (childs_[i] == b)
        {
            childs_.erase(childs_.begin() + i);
            break;
        }
    }
    collector_.insert(b);
}

void Base::clearChilds()
{
    for (auto c : childs_)
    {
        delete c;
    }
    childs_.clear();
}

void Base::drawSelfAndChilds()
{
    if (visible_)
    {
        draw();
        for (auto c : childs_)
        {
            if (c->visible_) { c->drawSelfAndChilds(); }
        }
    }
}

//只处理当前的节点和当前节点的子节点，检测鼠标是否在范围内
void Base::checkStateAndEvent(BP_Event& e)
{
    if (visible_)
    {
        //注意这里是反向
        for (int i = childs_.size() - 1; i >= 0; i--)
        {
            childs_[i]->checkStateAndEvent(e);
        }
        //setState(Normal);

        if (e.type == BP_MOUSEMOTION)
        {
            if (inSide(e.motion.x, e.motion.y))
            {
                state_ = Pass;
            }
            else
            {
                state_ = Normal;
            }
        }
        if (e.type == BP_MOUSEBUTTONDOWN)
        {
            if (inSide(e.motion.x, e.motion.y))
            {
                state_ = Press;
            }
        }
        //注意下个时序才会画，所以可以在dealEvent中改变原有状态
        dealEvent(e);
    }
}

//这个可以在自己的循环中使用，避免卡死
void Base::pollEvent()
{
    BP_Event e;
    Engine::getInstance()->pollEvent(e);
    switch (e.type)
    {
    case BP_QUIT:
        /*if (Engine::getInstance()->showMessage("Quit"))
        {
            Engine::getInstance()->destroy();
        }*/
        break;
    default:
        break;
    }
}

void Base::setAllChildState(State s)
{
    for (auto c : childs_)
    {
        c->state_ = s;;
    }
}

void Base::setChildState(int i, State s)
{
    if (i >= 0 && i < childs_.size())
    {
        childs_[i]->state_ = s;
    }
}

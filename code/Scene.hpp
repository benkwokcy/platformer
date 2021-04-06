enum Event {
    MOVE_LEFT,
    MOVE_RIGHT
};

class Scene {
public:
    virtual void paint() = 0;
    virtual void event() = 0;
    virtual void update() = 0;
};
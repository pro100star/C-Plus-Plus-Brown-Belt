#include "geo2d.h"
#include "game_object.h"

#include "test_runner.h"

#include <vector>
#include <memory>

using namespace std;

class Unit : public GameObject {
public:
    ~Unit() override = default;

    explicit Unit(geo2d::Point position) : pos(position) {}

    bool Collide(const GameObject& that) const override;

    geo2d::Point GetObject() const {
        return pos;
    }

    bool CollideWith(const Unit& that) const override;
    bool CollideWith(const Building& that) const override ;
    bool CollideWith(const Tower& that) const override;
    bool CollideWith(const Fence& that) const override;

private:
    geo2d::Point pos;
};

class Building : public GameObject {
public:
    virtual ~Building() = default;

    explicit Building(geo2d::Rectangle geometry) : geom(geometry) {}

    bool Collide(const GameObject& that) const override;
    bool CollideWith(const Unit& that) const override;
    bool CollideWith(const Building& that) const override;
    bool CollideWith(const Tower& that) const override;
    bool CollideWith(const Fence& that) const override;

    geo2d::Rectangle GetObject() const {
        return geom;
    }

private:
    geo2d::Rectangle geom;
};

class Tower : public GameObject {
public:
    explicit Tower(geo2d::Circle geometry) : geom(geometry) {}

    bool Collide(const GameObject& that) const override;
    bool CollideWith(const Unit& that) const override;
    bool CollideWith(const Building& that) const override;
    bool CollideWith(const Tower& that) const override;
    bool CollideWith(const Fence& that) const override;

    geo2d::Circle GetObject() const {
        return geom;
    }

private:
    geo2d::Circle geom;
};

class Fence : public GameObject {
public:
    explicit Fence(geo2d::Segment geometry) {
        geom = geometry;
    }

    bool Collide(const GameObject& that) const override;
    bool CollideWith(const Unit& that) const override;
    bool CollideWith(const Building& that) const override;
    bool CollideWith(const Tower& that) const override;
    bool CollideWith(const Fence& that) const override;

    geo2d::Segment GetObject() const {
        return geom;
    }

private:
    geo2d::Segment geom;
};

bool Unit::Collide(const GameObject &that) const {
    return that.CollideWith(*this);
}

bool Unit::CollideWith(const Unit &that) const {
    return geo2d::Collide(pos, that.GetObject());
}

bool Unit::CollideWith(const Building &that) const {
    return geo2d::Collide(pos, that.GetObject());
}

bool Unit::CollideWith(const Tower &that) const {
    return geo2d::Collide(pos, that.GetObject());
}

bool Unit::CollideWith(const Fence &that) const {
    return geo2d::Collide(pos, that.GetObject());
}

bool Building::Collide(const GameObject &that) const {
    return that.CollideWith(*this);
}

bool Building::CollideWith(const Unit &that) const {
    return geo2d::Collide(geom, that.GetObject());
}

bool Building::CollideWith(const Building &that) const {
    return geo2d::Collide(geom, that.GetObject());
}

bool Building::CollideWith(const Tower &that) const {
    return geo2d::Collide(geom, that.GetObject());
}

bool Building::CollideWith(const Fence &that) const {
    return geo2d::Collide(geom, that.GetObject());
}

bool Tower::Collide(const GameObject &that) const {
    return that.CollideWith(*this);
}

bool Tower::CollideWith(const Unit &that) const {
    return geo2d::Collide(geom, that.GetObject());
}

bool Tower::CollideWith(const Building &that) const {
    return geo2d::Collide(geom, that.GetObject());
}

bool Tower::CollideWith(const Tower &that) const {
    return geo2d::Collide(geom, that.GetObject());
}

bool Tower::CollideWith(const Fence &that) const {
    return geo2d::Collide(geom, that.GetObject());
}

bool Fence::Collide(const GameObject &that) const {
    return that.CollideWith(*this);
}

bool Fence::CollideWith(const Unit &that) const {
    return geo2d::Collide(geom, that.GetObject());
}

bool Fence::CollideWith(const Building &that) const {
    return geo2d::Collide(geom, that.GetObject());
}

bool Fence::CollideWith(const Tower &that) const {
    return geo2d::Collide(geom, that.GetObject());
}

bool Fence::CollideWith(const Fence &that) const {
    return geo2d::Collide(geom, that.GetObject());
}

bool Collide(const GameObject& first, const GameObject& second) {
    return first.Collide(second);
}

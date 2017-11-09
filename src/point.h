#pragma once

class Point {
public:
  Point() : xp(0), yp(0), zp(0){};
  Point(const Point &p) : xp(p.xp), yp(p.yp), zp(p.zp){};
  Point(int xp, int yp) : xp(xp), yp(yp), zp(0){};
  Point(int xp, int yp, int zp) : xp(xp), yp(yp), zp(zp){};
  Point(Point &&) = default;
  Point &operator=(const Point &) = default;
  ~Point(){};

  Point &operator+(int number) {
    xp += number;
    yp += number;
    zp += number;

    return *this;
  }

  Point &operator-(int number) {
    xp -= number;
    yp -= number;
    zp -= number;

    return *this;
  }

  Point &operator+=(Point point) {
    xp += point.x();
    yp += point.y();
    zp += point.z();

    return *this;
  }

  Point &operator-=(Point point) {
    xp -= point.x();
    yp -= point.y();
    zp -= point.z();

    return *this;
  }

  Point &operator+(Point point) {
    xp += point.x();
    yp += point.y();
    zp += point.z();

    return *this;
  }

  Point &operator-(Point point) {
    xp -= point.x();
    yp -= point.y();
    zp -= point.z();

    return *this;
  }

  Point &increase_all(int x, int y, int z) {
    xp += x;
    yp += y;
    zp += z;

    return *this;
  }

  Point &decrease_all(int x, int y, int z) {
    xp -= x;
    yp -= y;
    zp -= z;

    return *this;
  }

  virtual int x() const final { return xp; };
  virtual int y() const final { return yp; };
  virtual int z() const final { return zp; };

  virtual void x(const int val) final { xp = val; };
  virtual void y(const int val) final { yp = val; };
  virtual void z(const int val) final { zp = val; };

private:
  int xp;
  int yp;
  int zp;
};

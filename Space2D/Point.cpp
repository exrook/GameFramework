#include "Point.h"
namespace Space2D {

Point::Point (long double const &x, long double const &y): X(x), Y(y) {
}
Point::Point (Point const &p2) {
  X = p2.x();
  Y = p2.y();
}
Point::Point (): X(0), Y(0) {
}
const long double Point::x() const{
  return X;
}
const long double Point::x(long double const &x) {
  X = x;
  return X;
}
const long double Point::y() const{
  return Y;
}
const long double Point::y(long double const &y) {
  Y = y;
  return Y;
}

Point const Point::operator+ (Point const &p2) const{
  return Point(*this) += p2;
}
Point const Point::operator- (Point const &p2) const{
  return Point(*this) -= p2;
}
Point const Point::operator * (long double const &scale) const{
  return Point(*this) *= scale;
}
Point const Point::operator / (long double const &scale) const{
  return Point(*this) /= scale;
}
Point& Point::operator = (Point const &p2) {
  X = p2.x();
  Y = p2.y();
  return *this;
}
Point& Point::operator += (Point const &p2) {
  X += p2.x();
  Y += p2.y();
  return *this;
}
Point& Point::operator -= (Point const &p2) {
  X -= p2.x();
  Y -= p2.y();
  return *this;
}
Point& Point::operator *= (long double const &scale) {
  X *= scale;
  Y *= scale;
  return *this;
}
Point& Point::operator /= (long double const &scale) {
  X /= scale == 0? 1:scale;
  Y /= scale == 0? 1:scale;
  return *this;
}
const bool Point::operator == (Point const &p2) const{
  return (X == p2.x())&&(Y == p2.y());
}
const bool Point::operator != (Point const &p2) const{
  return (X != p2.x())||(Y != p2.y());
}

}
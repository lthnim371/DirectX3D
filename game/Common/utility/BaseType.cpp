#include "stdafx.h"
#include "BaseType.h"

////////////////////////////////////////////////////////////////
// Point
Point::Point(const LONG& _x/*=0*/, const LONG& _y/*=0*/)
{
	x = _x;
	y = _y;
}
Point Point::operator - (const Point& o)
{
	return Point(x-o.x, y-o.y);
}
Point Point::operator + (const Point& o)
{
	return Point(x+o.x, y+o.y);
}
Point Point::operator + (const Size& cs)
{
	return Point(x+cs.cx, y+cs.cy);
}
float Point::operator ^ (const Point& o)
{
	return sqrt(float((x-o.x)*(x-o.x) + (y-o.y)*(y-o.y)));
}
Point Point::ToScreen(HWND hWnd)
{
	Point tmp(*this);
	::ClientToScreen(hWnd, &tmp);
	return tmp;
}
Point Point::ToClient(HWND hWnd)
{
	Point tmp(*this);
	::ScreenToClient(hWnd, &tmp);
	return tmp;
}

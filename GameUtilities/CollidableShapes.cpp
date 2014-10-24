#include "CollidableShapes.h"

namespace jer
{
    const bool RectangleCollidable::canCollide(const int t) const
    {
        return t == RECTANGLE || t == POINT;
    }
    
    const bool RectangleCollidable::collidesPoint(const Point<double> p) const
    {
        return p.x() < bottomRight.x() && p.x() > topLeft.x() && p.y() < bottomRight.y() && p.y() > topLeft.y();
    }
    
    const bool RectangleCollidable::collides(const Collidable &other, Radians * const angle) const
    {
		Point<double> topLeft(getTopLeft());
		Point<double> bottomRight(getBottomRight());
		
        switch(other.getType())
        {
        case POINT:
            if(angle)  // if an address was passed in
                *angle = getTheta(getPosition() - other.getPosition());
            return collidesPoint(other.getPosition());
		case RECTANGLE:
			{
				Point<double> otherTopLeft(other.getOffset()+other.getPosition());
				Point<double> otherBottomRight(otherTopLeft+other.getDimensions());
                
				const bool ret = otherTopLeft.x() < bottomRight.x() && otherBottomRight.x() > topLeft.x() &&
					otherTopLeft.y() < bottomRight.y() && otherBottomRight.y() > topLeft.y();
                
                if(ret == false) return false;
                
                Uint8 collisions = 0;
                if(collidesPoint(otherTopLeft)) collisions |= 1;
                if(collidesPoint(otherTopLeft+other.getDimensions().x())) collisions |= 2;
                if(collidesPoint(otherTopLeft+other.getDimensions().y())) collisions |= 4;
                if(collidesPoint(otherBottomRight)) collisions |= 8;
                
                if(other.collidesPoint(getTopLeft())) collisions |= 16;
                if(other.collidesPoint(getTopRight())) collisions |= 32;
                if(other.collidesPoint(getBottomLeft())) collisions |= 64;
                if(other.collidesPoint(getBottomRight())) collisions |= 128;
                
                switch(collisions)
                {
                case 5:
				case 160:
					*angle = Radians::ANGLE_RIGHT;
					break;
				case 10:
				case 80:
					*angle = Radians::ANGLE_LEFT;
					break;
				case 20:
					{
						Point center(getPosition()+getOffset());
						Radians threshold(getTheta(getTopRight()-center));
						Radians actual(getTheta(otherBottomLeft-center));
						if(actual < threshold)
							*angle = Radians::ANGLE_RIGHT;
						else
							*angle = Radians::ANGLE_UP;
						break;
					}
				case 129:
					{
						Point center(getPosition()+getOffset());
						Radians threshold(getTheta(getBottomRight()-center));
						Radians actual(getTheta(otherTopLeft-center));
						if(actual < threshold)
							*angle = Radians::ANGLE_DOWN;
						else
							*angle = Radians::ANGLE_RIGHT;
						break;
					}
				case 66:
					{
						Point center(getPosition()+getOffset());
						Radians threshold(getTheta(getBottomLeft()-center));
						Radians actual(getTheta(otherTopRight-center));
						if(actual < threshold)
							*angle = Radians::ANGLE_LEFT;
						else
							*angle = Radians::ANGLE_DOWN;
						break;
					}
				case 24:
					{
						Point center(getPosition()+getOffset());
						Radians threshold(getTheta(getTopLeft()-center));
						Radians actual(getTheta(otherBottomRight-center));
						if(actual < threshold)
							*angle = Radians::ANGLE_UP;
						else
							*angle = Radians::ANGLE_LEFT;
						break;
					}
				case 3:
				case 192:
					*angle = Radians::ANGLE_DOWN;
					break;
				case 12:
				case 48:
					*angle = Radians::ANGLE_UP;
					 break;
                }
                
			}
			/*
		case CIRCLE:
			{
				See CircleCollidable::collides
			}
			*/
		default:
			if(other.canCollide(getType()))
				return other.collides(*this);
			return false;
        }
    }
	
	const bool CircleCollidable::canCollide(const int t) const
	{
		return t == RECTANGLE || t == POINT || t == CIRCLE;
	}
	
	const bool CircleCollidable::collides(const Collidable& other) const
	{
		switch(other.getType())
		{
		case POINT:
			return pythagoras(getCenter()-other.getPosition()) < radius;
			
		case RECTANGLE:
			{
				Point<double> temp(other.getOffset() + other.getPosition());
				Point<double> corners[4] = {other.getOffset()+other.getPosition(), temp+other.getDimensions(), temp+other.getDimensions().x(), temp+other.getDimensions().y()};
				Point<double> cardinals[4] = {getCenter() + X_t<double>(radius), getCenter() - X_t<double>(radius), getCenter() + Y_t<double>(radius), getCenter() - Y_t<double>(radius)};
				
				for(int i = 0; i < 4; i++)
				{
					if(collides(Collidable(POINT, corners+i)))
						return true;
					else if(other.collides(Collidable(POINT, cardinals+i)))
						return true;
				}
				
				return false;
			}
		
		case CIRCLE:
			return pythagoras(getCenter() - (other.getPosition() + other.getOffset())) < other.getDimensions().x()+radius;
			
		default:
			if(other.canCollide(getType()))
				return other.collides(*this);
			return false;
		}
	}
    
}
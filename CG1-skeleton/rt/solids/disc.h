#ifndef CG1RAYTRACER_SOLIDS_DISC_HEADER
#define CG1RAYTRACER_SOLIDS_DISC_HEADER

#include <rt/solids/solid.h>
#include <core/point.h>
#include <rt/bbox.h>

namespace rt {

class Disc : public Solid {
public:
	Vector normal;
	Point origin;
	float radius;
    Disc() {}
    Disc(const Point& center, const Vector& normal, float radius, CoordMapper* texMapper, Material* material);

	virtual Point getCenter() const;
    virtual BBox getBounds() const;
    virtual Intersection intersect(const Ray& ray, float previousBestDistance=FLT_MAX) const;
    virtual Point sample() const;
    virtual float getArea() const;

};

}


#endif

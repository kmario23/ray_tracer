#ifndef CG1RAYTRACER_GROUPS_SIMPLEGROUP_HEADER
#define CG1RAYTRACER_GROUPS_SIMPLEGROUP_HEADER

#include <vector>
#include <rt/groups/group.h>
#include <rt/primitive.h>
#include <rt/ray.h>
#include <rt/bbox.h>

namespace rt {

class SimpleGroup : public Group {
public:
    virtual BBox getBounds() const;
    virtual Intersection intersect(const Ray& ray, float previousBestDistance) const;
    virtual void rebuildIndex();
    virtual void add(Primitive* p);
	virtual Point getCenter() const;
    virtual void setMaterial(Material* m);
    virtual void setCoordMapper(CoordMapper* cm);
	std::vector<Primitive*> primitives;
};

}

#endif

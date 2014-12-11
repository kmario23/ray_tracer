#include <rt/primmod/instance.h>
#include <iostream>
namespace rt{

	Instance::Instance(Primitive* content): content_(content)
	{
		transformation = Matrix::identity();

	}
	
	Primitive* Instance::content()
	{
		return content_;
	}

	Point Instance::getCenter() const
	{
		BBox transformed_bbox = content_->getBounds();
		Point min_p = transformed_bbox.min;
		Point max_p = transformed_bbox.max;

		min_p = transformation*min_p;
		max_p = transformation*max_p;


		return Point(
			(min_p.x + max_p.x) / 2,
			(min_p.y + max_p.y) / 2,
			(min_p.z + max_p.z) / 2
		);
	}

    BBox  Instance::getBounds() const
	{
		BBox transformed_bbox = content_->getBounds();
		Point min_p = transformed_bbox.min;
		Point max_p = transformed_bbox.max;

		min_p = transformation*min_p;
		max_p = transformation*max_p;

		transformed_bbox.min = min_p;
		transformed_bbox.max = max_p;

		return transformed_bbox;
	}

    Intersection  Instance::intersect(const Ray& ray, float previousBestDistance) const
	{
		//1.inverse transformation applied to the ray (point origin + direction)
		//2.check the archetype for intersection
		//3.transform the intersection point with the transformation matrix
		//4.transform the intersection normal with the transposed of the inverse transformation matrix

		float previousBestDistance_;
		Ray inverse_ray;
		inverse_ray.o = transformation.invert() * ray.o;
		inverse_ray.d = transformation.invert() * ray.d;
		

		Point p = ray.getPoint(previousBestDistance);
		p = transformation.invert() * p;
		previousBestDistance_ = (p-inverse_ray.o).length();

		Intersection intersect;
		intersect = content_->intersect(inverse_ray,previousBestDistance_);
		if(intersect)
		{
			Point transformed_intersection;
			Vector transformed_normal;
			transformed_normal = transformation.invert().transpose()*intersect.normal();
			transformed_intersection = transformation*intersect.local();
			float distance = (transformed_intersection -inverse_ray.o).length();
			/*intersect._normal = transformed_normal.normalize();
			intersect._local = transformed_intersection;*/
			//return intersect;
			return Intersection(distance,inverse_ray,(Solid*)content_,transformed_normal.normalize(),transformed_intersection);
		}
		return Intersection::failure();
	}

    void  Instance::setMaterial(Material* m)
	{
		content_->setMaterial(m);
	}
	
    void  Instance::setCoordMapper(CoordMapper* cm)
	{
		content_->setCoordMapper(cm);
	}

	//reset transformation back to identity
	void Instance::reset()
	{
		transformation = transformation.identity();
	}

    void Instance::translate(const Vector& t)
	{
		Matrix translate;
		translate = translate.identity();
		translate[0][3] = t.x;
		translate[1][3] = t.y;
		translate[2][3] = t.z;

		transformation = product(transformation,translate);
	}

    void Instance::rotate(const Vector& axis, float angle)
	{
		Vector axis_(axis.x,axis.y,axis.z);
		axis_ = axis_ / axis_.length();
		
		Matrix rotation_matrix = Matrix::identity();
		//taken from http://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_from_axis_and_angle
		rotation_matrix[0][0] = cos(angle) + axis_.x*axis_.x*(1-cos(angle));
		rotation_matrix[1][0] = axis_.y*axis_.x*(1-cos(angle)) + axis_.z*sin(angle);
		rotation_matrix[2][0] = axis_.z*axis_.x*(1-cos(angle)) - axis_.y*sin(angle);
		rotation_matrix[0][1] = axis_.x*axis_.y*(1-cos(angle)) - axis_.z*sin(angle);
		rotation_matrix[1][1] = cos(angle)+axis_.y*axis_.y*(1-cos(angle));
		rotation_matrix[2][1] = axis_.z*axis_.y*(1-cos(angle)) + axis_.x*sin(angle);
		rotation_matrix[0][2] = axis_.x*axis_.z*(1-cos(angle)) + axis_.y*sin(angle);
		rotation_matrix[1][2] = axis_.y*axis_.z*(1-cos(angle)) - axis_.x*sin(angle);
		rotation_matrix[2][2] = cos(angle) + axis_.z*axis_.z*(1-cos(angle));

		transformation = product(transformation,rotation_matrix);
	}

    void Instance::scale(float scale)
	{
		Matrix scale_matrix;
		scale_matrix = scale_matrix.identity();
		scale_matrix[0][0] = scale;
		scale_matrix[1][1] = scale;
		scale_matrix[2][2] = scale;

		transformation = product(transformation,scale_matrix);
	}

    void Instance::scale(const Vector& scale)
	{
		Matrix scale_matrix;
		scale_matrix = scale_matrix.identity();
		scale_matrix[0][0] = scale.x;
		scale_matrix[1][1] = scale.y;
		scale_matrix[2][2] = scale.z;
		transformation = product(transformation,scale_matrix);
	}

}
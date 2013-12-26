#ifndef		__TRACKBALL_COMPONENT_HH_
#define		__TRACKBALL_COMPONENT_HH_

#include "Component.hh"
#include "Utils/SmartPointer.hh"
#include <glm/glm.hpp>

namespace Component
{
	class TrackBall : public Component::ComponentBase<TrackBall>
	{
	public:
		TrackBall(Engine &engine, Handle &entity) :
			Component::ComponentBase<Component::TrackBall>(engine, entity, "TrackBall"),
			dist(0.0f),
			rotateSpeed(0.0f),
			zoomSpeed(0.0f),
			angles(0, 0),
			toLook(entity)
		{}
		virtual ~TrackBall(void)
		{}
		void init(Handle _toLook, float _dist, float rotatingSpeed, float zoomingSpeed)
		{
			toLook = _toLook;
			dist = _dist;
			rotateSpeed = rotatingSpeed;
			zoomSpeed = zoomingSpeed;
		}
		void reset()
		{}
public:
	float					dist;
	float					rotateSpeed, zoomSpeed;
	glm::vec2				angles;
	Handle                  toLook;
	private:
		TrackBall(TrackBall const &);
		TrackBall &operator=(TrackBall const &);
	};

}

#endif //!__TRACKBALL_COMPONENT_HH_
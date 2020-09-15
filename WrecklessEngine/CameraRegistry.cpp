#include "CameraRegistry.h"

namespace CameraSystem
{
	std::unordered_map<std::string, Ref<PerspectiveCamera>> CameraRegistry::m_PerspectiveCameras;
	std::unordered_map<std::string, Ref<OrthogonalCamera>>  CameraRegistry::m_OrthogonalCameras;

	void CameraRegistry::RegisterPerspectiveCamera(const std::string& name, Ref<PerspectiveCamera> camera)
	{
		auto it = m_PerspectiveCameras.find(name);
		if (it != m_PerspectiveCameras.end())
			WRECK_ASSERT(false, "Perspective camera already exists");

		m_PerspectiveCameras[name] = std::move(camera);
	}
	void CameraRegistry::RegisterOrthogonalCamera(const std::string& name, Ref<OrthogonalCamera> camera)
	{
		auto it = m_OrthogonalCameras.find(name);
		if (it != m_OrthogonalCameras.end())
			WRECK_ASSERT(false, "Perspective camera already exists");

		m_OrthogonalCameras[name] = std::move(camera);
	}
	Ref<PerspectiveCamera> CameraRegistry::QueryPerspectiveCamera(const std::string& name)
	{
		auto it = m_PerspectiveCameras.find(name);
		if (it != m_PerspectiveCameras.end())
			return it->second;

		WRECK_ASSERT(false, "Couldn't find Perspective camera");
	}
	Ref<OrthogonalCamera> CameraRegistry::QueryOrthogonalCamera(const std::string& name)
	{
		auto it = m_OrthogonalCameras.find(name);
		if (it != m_OrthogonalCameras.end())
			return it->second;

		WRECK_ASSERT(false, "Couldn't find Orthogonal camera");
	}
}

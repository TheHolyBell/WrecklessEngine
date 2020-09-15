#pragma once
#include "CommonInclude.h"
#include "PerspectiveCamera.h"
#include "OrthogonalCamera.h"

namespace CameraSystem
{
	// Class serves as container for "global" cameras. So if you need to keep track or query camera you can register it here
	class CameraRegistry
	{
	public:
		static void RegisterPerspectiveCamera(const std::string& name, Ref<PerspectiveCamera> camera);
		static void RegisterOrthogonalCamera(const std::string& name, Ref<OrthogonalCamera> camera);

		static Ref<PerspectiveCamera> QueryPerspectiveCamera(const std::string& name);
		static Ref<OrthogonalCamera> QueryOrthogonalCamera(const std::string& name);
	private:
		static std::unordered_map<std::string, Ref<PerspectiveCamera>> m_PerspectiveCameras;
		static std::unordered_map<std::string, Ref<OrthogonalCamera>> m_OrthogonalCameras;
	};
}
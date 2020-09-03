#pragma once
// HRESULT hr should exist in the local scope for these macros to work

#define GFX_EXCEPT_NOINFO(hr) Graphics::Renderer::HrException( __FILE__, __LINE__,(hr) )
#define GFX_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw Graphics::Renderer::HrException( __FILE__,__LINE__,hr )

#ifndef NDEBUG
#define GFX_EXCEPT(hr) Graphics::Renderer::HrException( __FILE__,__LINE__,(hr),infoManager->GetMessages() )
#define GFX_THROW_INFO(hrcall) infoManager->Set(); if( FAILED( hr = (hrcall) ) ) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::Renderer::DeviceRemovedException( __FILE__,__LINE__,(hr),infoManager->GetMessages() )
#define GFX_THROW_INFO_ONLY(call) infoManager->Set(); (call); {auto v = infoManager->GetMessages(); if(!v.empty()) {throw Graphics::Renderer::InfoException( __FILE__,__LINE__,v);}}
#else
#define GFX_EXCEPT(hr) Graphics::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO_ONLY(call) (call)
#endif

// macro for importing infomanager into local scope
// this.GetInfoManager(Graphics& gfx) must exist
#ifdef NDEBUG
#define INFOMAN(gfx) HRESULT hr
#else
#define INFOMAN(gfx) HRESULT hr; std::shared_ptr<InfoManager> infoManager = GetInfoManager((gfx))
#endif
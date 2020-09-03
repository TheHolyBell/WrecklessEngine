#pragma once

#define WRECK_EXCEPT( hr )  Exceptions::HrException( __FILE__,__LINE__,(hr) )
#define WRECK_LAST_EXCEPT() Exceptions::HrException( __FILE__,__LINE__,GetLastError() )

//#define WRECK_HR( hr ) if(FAILED(HRESULT hrcall = (hr))) 

#define WRECK_HR(hrcall) if( FAILED(hr = (hrcall) ) ) Exceptions::HrException( __FILE__,__LINE__,(hrcall) )
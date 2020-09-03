#pragma once

#define WRECK_EXCEPT( hr )  Exceptions::HrException( __FILE__,__LINE__,(hr) )
#define WRECK_LAST_EXCEPT() Exceptions::HrException( __FILE__,__LINE__,GetLastError() )
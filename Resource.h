#pragma once

#include "Header.h"

inline String GetResource(String file) {
#ifdef UseResource
	return Resource(file);
#endif // UseResource

#ifndef UseResource
	return file;
#endif // !UseResource
}
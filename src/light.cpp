/*
 * light.cpp
 *
 *  Created on: Feb 2, 2015
 *      Author: li-zh
 */

#include "light.h"

LightResource& LightResource::GetInstance() {
	static LightResource instance; // Guaranteed to be destroyed.
									 // Instantiated on first use.
	return instance;
}

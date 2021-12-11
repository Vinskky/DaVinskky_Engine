#ifndef _C_CAMERA_H_
#define _C_CAMERA_H_

#include "Component.h"

class C_Camera : public Component
{
public:
	C_Camera(GameObject* owner, bool debugTextEnabled = false);
	~C_Camera();

private:


};

#endif // !_C_CAMERA_H_


// ***************************************************************************
//
//   Generated automatically by genwrapper.
//   Please DO NOT EDIT this file!
//
// ***************************************************************************

#include <osgIntrospection/ReflectionMacros>
#include <osgIntrospection/TypedMethodInfo>
#include <osgIntrospection/StaticMethodInfo>
#include <osgIntrospection/Attributes>

#include <osgAnimation/Animation>
#include <osgAnimation/AnimationUpdateCallback>
#include <osgAnimation/Channel>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

BEGIN_ABSTRACT_OBJECT_REFLECTOR(osgAnimation::AnimationUpdateCallbackBase)
	I_DeclaringFile("osgAnimation/AnimationUpdateCallback");
	I_VirtualBaseType(osg::Object);
	I_Constructor0(____AnimationUpdateCallbackBase,
	               "",
	               "");
	I_Method1(bool, link, IN, osgAnimation::Channel *, channel,
	          Properties::PURE_VIRTUAL,
	          __bool__link__Channel_P1,
	          "",
	          "");
	I_Method1(int, link, IN, osgAnimation::Animation *, animation,
	          Properties::PURE_VIRTUAL,
	          __int__link__Animation_P1,
	          "",
	          "");
END_REFLECTOR


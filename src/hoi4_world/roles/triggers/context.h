#pragma once




#include "src/hoi4_world/roles/triggers/scope.h"



namespace hoi4
{

struct Context
{
	const Scope& root;
	const Scope& this_scope;
	const Scope& prev;
	const Scope& from;
};

}  // namespace hoi4
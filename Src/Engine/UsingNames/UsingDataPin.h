/**
* @file UsingDataPin.h
*/
#ifndef POKARINENGINE_USINGDATAPIN_H_INCLUDED
#define POKARINENGINE_USINGDATAPIN_H_INCLUDED

#include <memory>

namespace PokarinEngine
{
	// ---------------------
	// ëOï˚êÈåæ
	// ---------------------

	template <class T>
	class DataPin;

	// ---------------------------
	// å^ÇÃï ñºÇíËã`
	// ---------------------------

	template <class T>
	using DataPinPtr = std::shared_ptr<DataPin<T>>;
}

#endif // !POKARINENGINE_USINGDATAPIN_H_INCLUDED

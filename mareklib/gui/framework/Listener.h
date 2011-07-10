/**
 * Listener.h
 * emptyExample
 *
 * Created by Marek Bereza on 27/06/2011.
 *
 */
#pragma once
#include "Control.h"

namespace gui {
	class Listener {
	public:
		virtual void controlChanged(Control *control) = 0;
	};
};
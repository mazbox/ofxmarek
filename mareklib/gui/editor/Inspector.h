/**
 * Inspector.h
 * emptyExample
 *
 * Created by Marek Bereza on 01/07/2011.
 *
 */
#include "Container.h"
#include "Title.h"
namespace gui {
		
	class Inspector: public Container, public Listener {
	public:
		Inspector();
		void setControl(Control *currentControl);
		void draw();
		void controlChanged(Control *ctrl);

	private:
		Control *currentControl;
		void clearControls();
		void addControl(string name, string type, void *name);
		int reloadButtonValue;
	};
	

};
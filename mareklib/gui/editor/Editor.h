/**
 * Editor.h
 * emptyExample
 *
 * Created by Marek Bereza on 16/06/2011.
 *
 */
#include "Container.h"
#include "ofGuiEventDispatcher.h"
#include "MenuBar.h"
#include "Instantiator.h"
#include "Inspector.h"
namespace gui {
	class Editor: public Container, public Listener {
	public:
		Editor();
		void controlChanged(Control *c);
		void setup(Container *root);

		void setupMenus();
		void touchOver(int x, int y, int id);
		bool touchDown(int x, int y, int id);
		bool touchMoved(int x, int y, int id);
		bool touchUp(int x, int y, int id);
		
		void draw();
		void setEnabled(bool enable);
		bool isEnabled();
	private:
		bool enabled;
		bool resizing;
		Container *root;
		Control *rolledOverControl;
		Control *focusedControl;
		ofVec2f lastMousePosition;
		ofGuiEventDispatcher eventDispatcher;
		MenuBar *menuBar;
		Inspector inspector;
	};
};
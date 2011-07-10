/**
 * Settings.h
 * emptyExample
 *
 * Created by Marek Bereza on 24/06/2011.
 *
 */
#include "Properties.h"

class Settings {
private:
	enum SettingsType {
		SettingsIntType,
		SettingsFloatType,
		SettingsBoolType
	};
	struct Setting {
		string name;
		void *value;
		SettingsType type;
	};
public:

	Settings() {
		settingsLoaded = false;
		path = "";
	}
	
	void load(string path = "settings.xml") {
		this->path = path;
		settingsLoaded = p.load(path);
		
	}
	void add(string name, int &value, int defaultValue) {
		settings.push_back(Setting());
		settings.back().name = name;
		settings.back().value = &value;
		settings.back().type = SettingsIntType;
		if(settingsLoaded) {
			value = p.getInt(name, defaultValue);

		} else {
			value = defaultValue;
		}
		
	}
	
	void add(string name, bool &value, bool defaultValue) {
		settings.push_back(Setting());
		settings.back().name = name;
		settings.back().value = &value;
		settings.back().type = SettingsBoolType;
		if(settingsLoaded) {
			value = p.getInt(name, defaultValue);
		} else {
			value = defaultValue;
		}
		
	}
	
	void add(string name, float &value, float defaultValue) {
		settings.push_back(Setting());
		settings.back().name = name;
		settings.back().value = &value;
		settings.back().type = SettingsFloatType;
		if(settingsLoaded) {
			value = p.getFloat(name, defaultValue);
		} else {
			value = defaultValue;
		}
	}
	
	void save() {

		for(int i = 0; i < settings.size(); i++) {
			if(settings[i].type==SettingsFloatType) {
				p.set(settings[i].name, *((float*)settings[i].value));
			} else if(settings[i].type==SettingsIntType) {
				p.set(settings[i].name, *((int*)settings[i].value));				
			} else if(settings[i].type==SettingsBoolType) {
				p.set(settings[i].name, *((bool*)settings[i].value));				
			} else {
				printf("Ach, nooo. We can't work with other things\n");
			}
		}
		
		if(path=="") path = "settings.xml";
		p.save(path);
	}
	
private:
	vector<Setting> settings;
	string path;
	Properties p;
	bool settingsLoaded;
};
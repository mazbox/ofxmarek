/*
 *  Slots.h
 *  emptyExample
 *
 *  Created by Marek Bereza on 19/12/2010.
 *  Copyright 2010 Marek Bereza. All rights reserved.
 *
 */
#include <vector>
#pragma once
/** 
 * provides a set of slots that can be occupied or not.
 * So you can fill up slots, release slots etc.
 * You can also cap the number of slots available
 * by setMax()
 */
class Slots {
public:
	
	Slots() {
		setMax(-1);
		
	}
	
	void setMax(int max) {
		this->max = max;
	}
	
	
	/**
	 * Returns the available slot id, or -1 if no more slots are available.
	 */
	int getSlot() {
		for(int i = 0; i < slots.size(); i++) {
			if(slots[i]==0) {
				slots[i] = 1;
				return i;
			}
		}
		if(max==-1 || slots.size()<max) {

			slots.push_back(1);
			return slots.size()-1;
		}
		return -1;
	}
	
	void releaseSlot(int slotId) {
		slots[slotId] = 0;
	}
	
	void print() {
		printf("SLOTS\n=====\n");
		for(int i = 0; i < slots.size(); i++) {
			int v = slots[i];
			printf("Slot %d  = %d\n",i, v);
		}
		printf("=====\n");
	}
private:
	int max;
	vector<int> slots;
};
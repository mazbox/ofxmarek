template<typename T>
class Particles {
public:

	Particles() {
		pos = 0;
		maxAge = 50;
		numParticles = 1000;
		positions = new T[numParticles];
		velocities = new T[numParticles];
		ages = new int[numParticles];
		
		for(int i = 0; i < numParticles; i++) {
			positions[i] = T(-10, -10);
			velocities[i] = T(0, 0);
			ages[i] = maxAge;
		}
	}
	~Particles() {
		delete [] positions;
		delete [] velocities;
		delete [] ages;
	}
	void spawn(float x, float y, float dx, float dy) {
		ages[pos] = 0;
		positions[pos] = T(x, y);
		velocities[pos] = T(dx, dy);
		
		pos++;
		pos %= numParticles;
	}
	
	void update() {
		for(int i = 0; i < numParticles; i++) {
			ages[i]++;
			positions[i] += velocities[i];
		}
	}
	void draw() {

		for(int i = 0; i < numParticles; i++) {
			if(ages[i]<maxAge) {
				float size = ofMap(ages[i], 0, maxAge, 10, 0);
				ofSetColor(100, 0,0, ofMap(ages[i], 0, maxAge, 255, 0));
				ofRect(positions[i].x - size/2, positions[i].y - size/2, size, size);
			}
		}
	}
private:
	int maxAge;
	int pos;
	int numParticles;
	
	T *positions;
	T *velocities;
	int *ages;
};

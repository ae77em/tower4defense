#ifndef TP4_TOWERDEFENSE_SERIALIZABLE_H
#define TP4_TOWERDEFENSE_SERIALIZABLE_H

#include <string>

/* "Interface" (kind of) to implement a serializable class. */
class Serializable {
	public:
		Serializable() {}
		virtual ~Serializable() {}

		virtual std::string serialize() = 0;
		virtual bool deserialize(std::string obj_serial) = 0;
};

#endif

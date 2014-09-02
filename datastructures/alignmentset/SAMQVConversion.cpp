#include <cstddef>
#include <assert.h>
#include "FASTQSequence.hpp"
#include "SAMQVConversion.hpp"

void QualityVectorToPrintable(unsigned char *data, int length) {
	if (data == NULL) {
		return;
	}
	for (int i = 0; i < length; i++) {
		data[i] = (((unsigned char)data[i]) == MAX_STORED_QUALITY) ? MAX_PRINTED_QUALITY : (unsigned char)data[i];
		data[i] = (((unsigned char)data[i]) == SENTINAL) ? MAP_SENTINAL : (unsigned char)data[i];
		assert(data[i] != 255);
	}
}

void QualityStringToStored(unsigned char *data, int length) {
	if (data == NULL) {
		return;
	}
	for (int i = 0; i < length; i++) {
		data[i] = data[i] - FASTQSequence::charToQuality;
		data[i] = ((unsigned char)data[i]) == MAX_PRINTED_QUALITY ? MAX_STORED_QUALITY : (unsigned char)data[i];
		data[i] = ((unsigned char)data[i]) == MAP_SENTINAL ? SENTINAL : (unsigned char)data[i];
	}
}


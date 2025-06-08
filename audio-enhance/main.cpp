#include "audio.h"

int main(int const argc, char const* const* const argv) noexcept {
	audio audio;

	audio.initialize_shared_audio_stream();
	return 0;
}
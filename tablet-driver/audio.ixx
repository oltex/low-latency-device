module;
#pragma comment(lib, "ole32.lib")
#include <Windows.h>
#include <mmdeviceapi.h>
#include <Audioclient.h>
export module audio;
import <stdio.h>;

export class audio final {
	IAudioClient3* _client;
public:
	inline audio(void) noexcept {
		::CoInitialize(nullptr);

		IMMDeviceEnumerator* enumerator;
		::CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), reinterpret_cast<void**>(&enumerator));

		IMMDevice* device;
		enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &device);
		enumerator->Release();

		device->Activate(__uuidof(IAudioClient3), CLSCTX_ALL, NULL, reinterpret_cast<void**>(&_client));
		device->Release();
	}
	inline ~audio(void) noexcept {
		_client->Release();
		::CoUninitialize();
	}

	inline auto initialize(void) const noexcept {
		WAVEFORMATEX* format;
		unsigned int current_period;
		_client->GetCurrentSharedModeEnginePeriod(&format, &current_period);

		unsigned int default_period, fundamental_period, minimum_period, maximum_period;
		_client->GetSharedModeEnginePeriod(format, &default_period, &fundamental_period, &minimum_period, &maximum_period);
		_client->InitializeSharedAudioStream(0, minimum_period, format, nullptr);
		::printf("[AUDIO]   shared-mode engine period:  %u frames / %.1fms\n", minimum_period, minimum_period * 1000.0 / format->nSamplesPerSec);
		::CoTaskMemFree(format);
	}
};

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
		CoInitialize(nullptr);

		IMMDeviceEnumerator* enumerator;
		CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), reinterpret_cast<void**>(&enumerator));

		IMMDevice* device;
		enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &device);
		enumerator->Release();

		device->Activate(__uuidof(IAudioClient3), CLSCTX_ALL, NULL, reinterpret_cast<void**>(&_client));
		device->Release();
	}
	inline ~audio(void) noexcept {
		_client->Release();
		CoUninitialize();
	}

	inline auto initialize(void) const noexcept {
		WAVEFORMATEX* format;
		unsigned int current_period;
		_client->GetCurrentSharedModeEnginePeriod(&format, &current_period);

		unsigned int default_period, fundamental_period, minimum_period, maximum_period;
		_client->GetSharedModeEnginePeriod(format, &default_period, &fundamental_period, &minimum_period, &maximum_period);
		_client->InitializeSharedAudioStream(0, minimum_period, format, nullptr);
		printf("Shared Mode Engine Period\n"\
			" Current Period: %d -> %d frame\n"\
			" Minimum Period: %d frame\n"\
			" Maximum Period: %d frame\n\n",
			current_period, minimum_period, minimum_period, maximum_period);
		CoTaskMemFree(format);
	}
};

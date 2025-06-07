#pragma once
#include <Windows.h>
#include <mmdeviceapi.h>
#include <Audioclient.h>

class audio final {
public:
	inline explicit audio(void) noexcept {
		CoInitialize(nullptr);

		IMMDeviceEnumerator* enumerator;
		CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), reinterpret_cast<void**>(&enumerator));

		IMMDevice* device;
		enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &device);
		enumerator->Release();

		IAudioClient3* client;
		device->Activate(__uuidof(IAudioClient3), CLSCTX_ALL, NULL, reinterpret_cast<void**>(&client));
		device->Release();


		//BOOL isOffloadCapable = FALSE;
		//client->IsOffloadCapable(AudioCategory_Media, &isOffloadCapable);
		//AudioClientProperties audioProps{};
		//audioProps.cbSize = sizeof(AudioClientProperties);
		//audioProps.eCategory = AudioCategory_Media;
		//audioProps.Options = AUDCLNT_STREAMOPTIONS_RAW | AUDCLNT_STREAMOPTIONS_MATCH_FORMAT;
		//client->SetClientProperties(&audioProps);

		WAVEFORMATEX* format;
		client->GetMixFormat(&format);

		UINT32 default_period, fundamental_period, min_period, max_period;
		client->GetSharedModeEnginePeriod(format, &default_period, &fundamental_period, &min_period, &max_period);

		client->InitializeSharedAudioStream(0, min_period, format, nullptr);

		CoTaskMemFree(format);
		client->Release();
		CoUninitialize();
	}
};
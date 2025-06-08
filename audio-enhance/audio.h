#pragma once
#include <Windows.h>
#include <mmdeviceapi.h>
#include <Audioclient.h>
#include <stdio.h>

class audio final {
public:
	inline explicit audio(void) noexcept {
		CoInitialize(nullptr);

		IMMDeviceEnumerator* enumerator;
		CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), reinterpret_cast<void**>(&enumerator));

		IMMDevice* device;
		enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &device);
		enumerator->Release();

		device->Activate(__uuidof(IAudioClient3), CLSCTX_ALL, NULL, reinterpret_cast<void**>(&_client));
		device->Release();
	}

	inline auto initialize(void) noexcept {
		WAVEFORMATEX* format;
		UINT32 current_period;
		_client->GetCurrentSharedModeEnginePeriod(&format, &current_period);

		UINT32 default_period, fundamental_period, minimum_period, maximum_period;
		_client->GetSharedModeEnginePeriod(format, &default_period, &fundamental_period, &minimum_period, &maximum_period);
		printf("Shared Mode Engine Period\n"\
			"  Current Period: %d ¡æ 128 frame\n"\
			"  Minimum Period: %d frame\n"\
			"  Maximum Period: %d frame\n\n",
			current_period, minimum_period, maximum_period);
		_client->InitializeSharedAudioStream(0, minimum_period, format, nullptr);
		CoTaskMemFree(format);
	}
private:
	IAudioClient3* _client;
};

//inline ~audio(void) noexcept {
//	//client->Release();
//	//CoUninitialize();
//}

//REFERENCE_TIME rt1, rt2;
//client->GetDevicePeriod(&rt1, &rt2);

//BOOL isOffloadCapable = FALSE;
//client->IsOffloadCapable(AudioCategory_Media, &isOffloadCapable);
//AudioClientProperties audioProps{};
//audioProps.cbSize = sizeof(AudioClientProperties);
//audioProps.eCategory = AudioCategory_Media;
//audioProps.Options = AUDCLNT_STREAMOPTIONS_RAW | AUDCLNT_STREAMOPTIONS_MATCH_FORMAT;
//client->SetClientProperties(&audioProps);

//WAVEFORMATEX* format;
//_client->GetMixFormat(&format);
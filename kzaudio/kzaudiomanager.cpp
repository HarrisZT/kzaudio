/*****************************************************************************\ 
| Copyright(C) 2019-2024 KZGAMES. All Rights Reserved.                        |
| Author: Zachary T Harris                                                    |
| 																			  |
| File: audiomanager.cpp											          |
| Desc: interface for game audio system service     			              | 
|       the contents of this file are intended to be a possible example		  |
|       of how one might implement an audio manager using kz audio            |
|																			  |
|																			  |
|     																		  |
| This program is free software: you can redistribute it and/or modify		  |
| it under the terms of the GNU General Public License as published by		  |
| the Free Software Foundation, either version 3 of the License, or			  |
| (at your option) any later version.										  |
| 																			  |
| This program is distributed in the hope that it will be useful,			  |
| but WITHOUT ANY WARRANTY; without even the implied warranty of			  |
| MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				  |
| GNU General Public License for more details.								  |
| 																			  |
| You should have received a copy of the GNU General Public License			  |
| along with this program.  If not, see <http://www.gnu.org/licenses/>.		  |
******************************************************************************/ 
#include "kzaudiomanager.h"




AudioManager::AudioManager() {
	m_currentMusic = MUSICID_UNDEFINED; 
	m_initialized  = false;
    m_soundEnabled = true;
    m_musicEnabled = true;
    m_audioDevice  = nullptr;
    m_music        = nullptr; 
	m_globalVolume = 100;

	for (int i = 0; i < SOUNDID_UNDEFINED; ++i) {
		m_sounds[i] = nullptr;
	}
}



AudioManager::~AudioManager() {
	UnloadAllSounds();  
	if (m_music) {
		delete m_music;
		m_music = NULL;
	}
	if (m_audioDevice) {
		delete m_audioDevice;
		m_audioDevice = NULL;
	} 
}



bool AudioManager::Initialize(const std::string& directory) {
	m_directory = directory;

	if (m_initialized) {
		return true;
	}
	//create and initialize OpenAL device:
	m_audioDevice = new kz::AudioDevice();
	if (!m_audioDevice ||
		!m_audioDevice->Initialize()) {
		return false;
	}  
	//test that all sound files will load:
	for (int i = 0; i < SOUNDID_UNDEFINED; ++i) {
		if (!LoadSound((SOUNDID)i))
			return false;
	} 
	UnloadAllSounds(); 

	//test that all music files will load:
	for (int i = 0; i < MUSICID_UNDEFINED; ++i) {
		if (!LoadMusic((MUSICID)i))
			return false;
	} 
	//we're good
	m_initialized = true;
	return m_initialized;
}



void AudioManager::Update() { 
	 if (m_music) 
		 m_music->Update();  
}


 
void AudioManager::SetGlobalVolume(int volume) {
	m_globalVolume = volume;
	m_audioDevice->SetGlobalVolume(volume); 
} 



int AudioManager::GetGlobalVolume() const {
	return m_audioDevice->GetGlobalVolume();
}



void AudioManager::SetMuted(bool mute) { 
	if (mute) 
		SetGlobalVolume(0);
	else SetGlobalVolume(m_globalVolume);
} 



bool AudioManager::LoadSound(SOUNDID id) {
	std::string pathToFile;

	if (id >= SOUNDID_UNDEFINED) {
		return false;
	}
	//check if sound is already loaded:
	if (m_sounds[id]) {
		return true;
	} 
	m_sounds[id] = new SoundEffect;
	if (!m_sounds[id]) {
		return false;
	}
	pathToFile = m_directory + GetSoundFileName(id);
	if (!m_sounds[id]->buffer.Load(pathToFile)) {
		UnloadSound(id);
		return false;
	} 
	m_sounds[id]->sound.SetBuffer(&m_sounds[id]->buffer);
	m_sounds[id]->sound.SetInitialVolume(100);
	m_sounds[id]->sound.SetVolume(100);

	return true;
}



void AudioManager::UnloadSound(SOUNDID id) {
	if (id < SOUNDID_UNDEFINED) {  
		if (m_sounds[id]) {
			delete m_sounds[id];
			m_sounds[id] = NULL;
		}
	}
}



void AudioManager::PlaySound(SOUNDID id, bool repeat) { 
	if (!m_soundEnabled || !m_sounds[id]) {
		return;
	}
	if (repeat || !m_sounds[id]->sound.IsPlaying()) {
		m_sounds[id]->sound.Play();
	}
}



void AudioManager::StopAllSounds() {
	for (int i = 0; i < SOUNDID_UNDEFINED; ++i) {
		if (m_sounds[i] != NULL)
			m_sounds[i]->sound.Stop();
	} 
} 



void AudioManager::UnloadAllSounds() {
	for (int i = 0; i < SOUNDID_UNDEFINED; ++i) {
		UnloadSound((SOUNDID)i);
	}
}



bool AudioManager::SoundIsPlaying(SOUNDID id) const { 
	return m_sounds[id]->sound.IsPlaying();
}

bool AudioManager::SoundIsPaused(SOUNDID id) const { 
	return m_sounds[id]->sound.IsPaused();
}

bool AudioManager::SoundIsStopped(SOUNDID id) const { 
	return m_sounds[id]->sound.IsStopped();
}



bool AudioManager::SoundIsEnabled() const { 
	return m_soundEnabled;
} 

void AudioManager::SetSoundEnabled(bool enable) {
	m_soundEnabled = enable;
}



kz::Sound* const AudioManager::GetSound(SOUNDID id) const {
	return &m_sounds[id]->sound; 
}



bool AudioManager::LoadMusic(MUSICID mus) { 
	if (m_currentMusic != mus) {
		m_currentMusic = mus;

		if (!m_music) {
			m_music = new kz::MusicStream();
			if (!m_music) {
				return false;
			}
		}
		m_music->Stop(); 
		 
		std::string pathToFile = m_directory + GetMusicFileName(mus);  
		if (!m_music->Load(pathToFile)) {
			return false;
		} 
	}
	return true;
}

 

bool AudioManager::MusicIsPlaying() const {
	return m_music ? m_music->IsPlaying() : false; 
}

bool AudioManager::MusicIsPaused() const {
	return m_music ? m_music->IsPaused() : false; 
}

bool AudioManager::MusicIsStopped() const {
	return m_music ? !m_music->IsPlaying() : false; 
} 

 

MUSICID AudioManager::GetMusicId() const {
	return m_currentMusic;
} 



bool AudioManager::MusicIsEnabled() const {
	return m_musicEnabled;
} 



void AudioManager::SetMusicEnabled(bool enable) {  
	m_musicEnabled = enable;
	if (m_musicEnabled)  
		PlayMusic(); 
	else {
		if (m_music) {
			delete m_music;
			m_music = NULL;
		}
	}
}



void AudioManager::PlayMusic() {
	if (m_music)
		m_music->Play();
}



void AudioManager::PauseMusic() {
	if (m_music)
		m_music->Pause();
}



void AudioManager::StopMusic() {
	if (m_music)
		m_music->Stop();
}



void AudioManager::ResumeMusic() {
	if (m_music)
		m_music->Resume(); 
}



std::string AudioManager::GetSoundFileName(SOUNDID id) {
	switch (id) {
	case SOUNDID_JUMP:
		return "jump.wav";
	case SOUNDID_BREAK_BLOCK:
		return "breakblock.wav";
	}
	return std::string();
} 



std::string AudioManager::GetMusicFileName(MUSICID id) { 
	switch (id) {
	case MUSICID_TITLESCREEN:
		return "titlescreen.ogg";
	case MUSICID_LEVEL_1:
		return "level1.ogg";
	}
	return std::string();
} 
/*****************************************************************************/  
//EOF                                                                         |
/*****************************************************************************/ 
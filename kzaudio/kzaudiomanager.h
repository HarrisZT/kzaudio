/*****************************************************************************\ 
| Copyright(C) 2019-2024 KZGAMES. All Rights Reserved.                        |
| Author: Zachary T Harris                                                    |
| 																			  |
| File: audiomanager.h 												          |
| Desc: interface for game audio system service     			              | 
|       the contents of this file are intended to be a possible example		  |
|       of how one might implement an audio manager using kz audio            |
|                                                                             |
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
#ifndef __AUDIOMANAGER_H__
#define __AUDIOMANAGER_H__

#include "kzglobalinstance.h" 
#include "kzmusicstream.h"
#include "kzaudiodevice.h" 
#include "kzsoundbuffer.h"
#include "kzsound.h" 



/**
enum values to identify a music file*/
typedef enum { 
	MUSICID_TITLESCREEN,
	MUSICID_LEVEL_1,
	MUSICID_UNDEFINED
} MUSICID;

/**
enum values to identify a sound effect*/
typedef enum { 
	SOUNDID_JUMP,
	SOUNDID_BREAK_BLOCK,
	SOUNDID_UNDEFINED
} SOUNDID;





/**
interface for game audio system service*/
class AudioManager final : public kz::GlobalInstance<AudioManager> {
public:

	/** construct audio system instance*/
	AudioManager();
	~AudioManager();


	/** initialize the audio system.
		this function must be called once before using audio.
		@param directory: the directory containing audio files*/
	bool Initialize(const std::string& directory);


	/** updates the audio, this function must be called
		once during the main program loop*/
	void Update();


	/**	set the global volume (affects all sounds and music).
		@param volume: value between [0 - 100]*/
	void SetGlobalVolume(int volume);

	/** returns the global volume, a value between [0 - 100]*/
	int GetGlobalVolume() const;

	/**	mute or unmute the audio (affects sounds and music)
		@param mute: true to mute the audio, false to unmute*/
	void SetMuted(bool mute);


	/** load a sound effect into memory
		@param id: enum value identifying the sound
		@return: true on success, false on failure*/
	bool LoadSound(SOUNDID id);

	/** unload a sound effect from memory
		@param id: enum value identifying the sound*/
	void UnloadSound(SOUNDID id);


	/** unload all sound effects from memory*/
	void UnloadAllSounds();

	/** stops playback of all sounds*/
	void StopAllSounds();


	/**	play a sound effect (must be currently loaded)
		@param id: enum value identifying the sound
		@param repeat: if false, this function does nothing if its already playing.*/
	void PlaySound(SOUNDID id, bool repeat = true);


	/** returns true if the specified sound if currently playing*/
	bool SoundIsPlaying(SOUNDID id) const;

	/** returns true if the specified sound if currently paused*/
	bool SoundIsPaused(SOUNDID id) const;

	/** returns true if the specified sound if currently stopped*/
	bool SoundIsStopped(SOUNDID id) const;


	/** returns a pointer to the specified sound.
		Pointer may be NULL if sound has not been loaded*/
	kz::Sound* const GetSound(SOUNDID id) const;


	/**	load a new music stream.
		@param id: enum value identifying the music
		@return: true on success, else false on failure*/
	bool LoadMusic(MUSICID id);

	/** returns the enum id of currently loaded music*/
	MUSICID GetMusicId() const;


	/** start playing the currently loaded music*/
	void PlayMusic();

	/** pause the currently loaded music*/
	void PauseMusic();

	/** stop the currently loaded music*/
	void StopMusic();

	/** start playing the currently loaded music*/
	void ResumeMusic();


	/** returns true if the currently loaded music is playing*/
	bool MusicIsPlaying() const;

	/** returns true if the currently loaded music is paused*/
	bool MusicIsPaused() const;

	/** returns true if the currently loaded music is stopped*/
	bool MusicIsStopped() const;


	/** returns true if sound is enabled, else false*/
	bool SoundIsEnabled() const;

	/**	returns true if music is enabled, else false*/
	bool MusicIsEnabled() const;

	/**	set sound playback to enabled or disabled*/
	void SetSoundEnabled(bool set);

	/**	set sound playback to enabled or disabled*/
	void SetMusicEnabled(bool set);


private:
	std::string GetSoundFileName(SOUNDID id);
	std::string GetMusicFileName(MUSICID id);

	struct SoundEffect {
		kz::SoundBuffer buffer;
		kz::Sound       sound;
	};
	bool             m_initialized;
	int              m_globalVolume;
	std::string      m_directory;
	MUSICID          m_currentMusic;
	bool             m_soundEnabled;
	bool             m_musicEnabled;
	kz::AudioDevice* m_audioDevice;
	kz::MusicStream* m_music;
	SoundEffect*     m_sounds[SOUNDID_UNDEFINED];
}; 
/*****************************************************************************/  
#endif//EOF                                                                   |
/*****************************************************************************/
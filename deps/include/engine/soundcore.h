#include "engine.h"

#ifndef __SOUNDCORE_H__
#define __SOUNDCORE_H__ 1

/**
 * @class Source
 *
 * @brief Represents an audio source for OpenAL.
 *
 * The Source class encapsulates properties and methods related to audio playback using OpenAL.
 */
class Source
{
public:
  typedef uint32_t Id; ///< Type definition for the identifier.

  /**
   * @brief Constructor for the Source class.
   */
  Source();

  /**
   * @brief Destructor for the Source class.
   */
  ~Source();

  /**
   * @brief Returns the elapsed time in milliseconds.
   *
   * @return Elapsed time in milliseconds.
   */
  u32 getTranscurredTime();

  u32 source_, buffer_; ///< Identifiers for OpenAL source and buffer.

  boolean loop_; ///< Flag indicating whether the source should loop playback.

  s32 state_, size_;                ///< State and size information for the source.
  s32 sample_offset_, sample_rate_; ///< Sample offset and sample rate parameters.

  f32 pitch_, gain_;              ///< Pitch and gain parameters for audio playback.
  f32 position_[3], velocity_[3]; ///< 3D positional and velocity parameters for the audio source.
};

/**
 * @class SoundCore
 *
 * @brief Manages audio playback using OpenAL for a set of sounds.
 *
 * The SoundCore class provides functionality to manage audio playback using OpenAL for a set of sounds.
 * It handles sound branching, updates, uploading, and manipulation of various audio properties.
 */
class SoundCore
{
public:
  static const u32 k_max_sounds = 50; ///< Maximum number of sounds supported by the SoundCore.

  /**
   * @brief Gets the instance of the SoundCore.
   *
   * @return Pointer to the singleton instance of the SoundCore.
   */
  static SoundCore *Instance();

  /**
   * @brief Free all resources
   */
  void free();

  /**
   * @brief Updates the internal state of the SoundCore.
   */
  void update();

  /**
   * @brief Uploads a sound with specified properties.
   *
   * @param src Pointer to the sound data.
   * @param pitch Pitch parameter for audio playback.
   * @param gain Gain parameter for audio playback.
   * @param position Array representing the 3D position of the sound.
   * @param velocity Array representing the 3D velocity of the sound.
   * @param loop Flag indicating whether the sound should loop playback.
   *
   * @return Identifier of the uploaded sound.
   */
  Source::Id uploadSound(const byte *src, f32 pitch, f32 gain, f32 position[3], f32 velocity[3], boolean loop = false);

  /**
   * @brief Gets the state of a sound.
   *
   * @param sound Identifier of the sound.
   *
   * @return State of the specified sound.
   */
  s32 getState(Source::Id sound);

  /**
   * @brief Sets the gain parameter for a sound.
   *
   * @param sound Identifier of the sound.
   * @param gain New gain value for the sound.
   */
  void setGain(Source::Id sound, f32 gain);

  /**
   * @brief Gets the gain parameter of a sound.
   *
   * @param sound Identifier of the sound.
   *
   * @return Gain value of the specified sound.
   */
  f32 getGain(Source::Id sound);

  /**
   * @brief Sets the pitch parameter for a sound.
   *
   * @param sound Identifier of the sound.
   * @param pitch New pitch value for the sound.
   */
  void setPitch(Source::Id sound, f32 pitch);

  /**
   * @brief Gets the pitch parameter of a sound.
   *
   * @param sound Identifier of the sound.
   *
   * @return Pitch value of the specified sound.
   */
  f32 getPitch(Source::Id sound);

  /**
   * @brief Sets the position parameter for a sound.
   *
   * @param sound Identifier of the sound.
   * @param position New position array for the sound.
   */
  void setPosition(Source::Id sound, f32 position[3]);

  /**
   * @brief Gets the position parameter of a sound.
   *
   * @param sound Identifier of the sound.
   * @param ret_position Array to store the position of the specified sound.
   */
  void getPosition(Source::Id sound, f32 ret_position[3]);

  /**
   * @brief Sets the velocity parameter for a sound.
   *
   * @param sound Identifier of the sound.
   * @param velocity New velocity array for the sound.
   */
  void setVelocity(Source::Id sound, f32 velocity[3]);

  /**
   * @brief Gets the velocity parameter of a sound.
   *
   * @param sound Identifier of the sound.
   * @param ret_velocity Array to store the velocity of the specified sound.
   */
  void getVelocity(Source::Id sound, f32 ret_velocity[3]);

  /**
   * @brief Sets the loop parameter for a sound.
   *
   * @param sound Identifier of the sound.
   * @param loop New loop value for the sound.
   */
  void setLoop(Source::Id sound, boolean loop);

  /**
   * @brief Gets the loop parameter of a sound.
   *
   * @param sound Identifier of the sound.
   *
   * @return Loop value of the specified sound.
   */
  boolean getLoop(Source::Id sound);

  /**
   * @brief Initiates playback of a sound.
   *
   * @param sound Identifier of the sound to play.
   */
  void play(Source::Id sound);

  /**
   * @brief Pauses playback of a sound.
   *
   * @param sound Identifier of the sound to pause.
   */
  void pause(Source::Id sound);

  /**
   * @brief Stops playback of a sound.
   *
   * @param sound Identifier of the sound to stop.
   */
  void stop(Source::Id sound);

  /**
   * @brief Rewinds a sound to its starting position.
   *
   * @param sound Identifier of the sound to rewind.
   */
  void rewind(Source::Id sound);

private:
  /**
   * @brief Constructor for the SoundCore class.
   */
  SoundCore();

  /**
   * @brief Destructor for the SoundCore class.
   */
  ~SoundCore();

  /**
   * @brief ImGUI function for managing sounds.
   *
   * Add a droped filepath
   */
  void ImGUI_SoundManager(const char *filepath);

  boolean initialized_; ///< Flag indicating whether the SoundCore is initialized.

  void *device_ = nullptr;      ///< Pointer to the OpenAL device.
  void *context_ = nullptr;     ///< Pointer to the OpenAL context.
  Source audios_[k_max_sounds]; ///< Array of sound sources managed by the SoundCore.
};

/**
 * @brief Macro for accessing the singleton instance of the SoundCore.
 */
#define SC (SoundCore::Instance())

#endif /* __SOUNDCORE_H__ */

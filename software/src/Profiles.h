#ifndef __PROFILES_H__
#define __PROFILES_H__

#include "Logger.h"
#include "Profile.h"
#include <Arduino.h>
#include <optional>
#include <vector>

/**
 * Handle soldering profiles.
 */
class Profiles {
public:
  /**
   * @brief Construct a new Profiles instance, containing all available profiles.
   */
  Profiles(Logger &logger);

public:
  /**
   * @brief Get a profile given its name. Will return std::nullopt if the profile cannot be found.
   */
  std::optional<Profile> getProfile(String &name);

  /**
   * @brief Get the total number of profiles.
   */
  uint8_t getNumberOfProfiles() { return _profiles.size(); }

  /**
   * @brief Get name of available profiles.
   */
  std::vector<String> availableProfiles() const;

private:
  Logger &_logger;
  std::vector<Profile> _profiles;
};

#endif //__PROFILES_H__

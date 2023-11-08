#ifndef __PROFILE_BUILDER_H__
#define __PROFILE_BUILDER_H__

#include "Profile.h"

class ProfileBuilder {
public:
  ProfileBuilder(String name) : _profile({name, {}}) {}

  ProfileBuilder &addStep(Profile::Step step) {
    _profile.steps.push_back(step);
    return *this;
  }

  Profile build() const { return _profile; }

private:
  Profile _profile;
};

#endif // __PROFILE_BUILDER_H__
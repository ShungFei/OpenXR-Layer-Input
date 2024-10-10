#pragma once

#include "pch.h"

namespace toolkit {
    namespace input {
        enum class Hand : uint32_t { Left, Right };
        struct IHandTracker {
            virtual ~IHandTracker() = default;

            virtual XrPath getInteractionProfile() const = 0;

            virtual void registerAction(XrAction action, XrActionSet actionSet) = 0;
            virtual void unregisterAction(XrAction action) = 0;
            virtual void registerActionSpace(XrSpace space,
                                             const std::string& path,
                                             const XrPosef& poseInActionSpace) = 0;
            virtual void unregisterActionSpace(XrSpace space) = 0;

            virtual void registerBindings(const XrInteractionProfileSuggestedBinding& bindings) = 0;

            virtual const std::string getFullPath(XrAction action, XrPath subactionPath) = 0;

            virtual void beginSession(XrSession session,
                                      std::shared_ptr<toolkit::graphics::IDevice> graphicsDevice) = 0;
            virtual void endSession() = 0;

            virtual void sync(XrTime frameTime, XrTime now, const XrActionsSyncInfo& syncInfo) = 0;
            virtual bool
            locate(XrSpace space, XrSpace baseSpace, XrTime time, XrTime now, XrSpaceLocation& location) const = 0;
            virtual void render(const XrPosef& pose,
                                XrSpace baseSpace,
                                XrTime time,
                                std::shared_ptr<graphics::ITexture> renderTarget) const = 0;

            virtual bool getActionState(const XrActionStateGetInfo& getInfo, XrActionStateBoolean& state) const = 0;
            virtual bool getActionState(const XrActionStateGetInfo& getInfo, XrActionStateFloat& state) const = 0;
            virtual bool isHandEnabled(Hand hand) const = 0;
            virtual bool isTrackedRecently(Hand hand) const = 0;

            virtual void handleOutput(Hand hand, float frequency, XrDuration duration) = 0;

            virtual const GesturesState& getGesturesState() const = 0;
        };
    };
};
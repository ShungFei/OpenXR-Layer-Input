#include "pch.h"

enum class Hand : uint32_t { Left, Right };

class StateController {
public:
    StateController() {
    }

    bool getActionState(const XrActionStateGetInfo& getInfo, XrActionStateBoolean& state) const {
    const auto& actionIt = m_actions.find(getInfo.action);
    if (actionIt == m_actions.cend()) {
        return false;
    }
    const auto& action = actionIt->second;

    auto subActionIt = action.subActions.find(getInfo.subactionPath);
    if (subActionIt == actionIt->second.subActions.cend()) {
        if (actionIt->second.subActions.empty()) {
            return false;
        }
        subActionIt = actionIt->second.subActions.begin();
    }
    const auto& subAction = subActionIt->second;

    if ((subAction.hand == Hand::Left && !m_leftHandEnabled) ||
        (subAction.hand == Hand::Right && !m_rightHandEnabled)) {
        return false;
    }

    state.isActive = XR_TRUE;
    state.currentState = subAction.boolValue;
    state.changedSinceLastSync = subAction.boolValueChanged;
    state.lastChangeTime = subAction.timeBoolValueChanged;

    return true;
}
}
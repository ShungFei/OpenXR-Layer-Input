# The list of OpenXR functions our layer will override.
override_functions = [
    "xrGetSystem",
    "xrEnumerateViewConfigurationViews",
    "xrCreateSession",
    "xrBeginSession",
    "xrEndSession",
    "xrDestroySession",
    "xrCreateSwapchain",
    "xrDestroySwapchain",
    "xrSuggestInteractionProfileBindings",
    "xrCreateAction",
    "xrDestroyAction",
    "xrCreateActionSpace",
    "xrDestroySpace",
    "xrAttachSessionActionSets",
    "xrEnumerateSwapchainImages",
    "xrWaitSwapchainImage",
    "xrAcquireSwapchainImage",
    "xrReleaseSwapchainImage",
    "xrPollEvent",
    "xrGetCurrentInteractionProfile",
    "xrLocateViews",
    "xrLocateSpace",
    "xrSyncActions",
    "xrGetActionStateBoolean",
    "xrGetActionStateFloat",
    "xrGetActionStatePose",
    "xrGetActionStateVector2f",
    "xrApplyHapticFeedback",
    "xrStopHapticFeedback",
    "xrWaitFrame",
    "xrBeginFrame",
    "xrEndFrame",
    "xrGetVisibilityMaskKHR",
]

# The list of OpenXR functions our layer will use from the runtime.
# Might repeat entries from override_functions above.
requested_functions = [
    "xrGetSystem",
    "xrGetInstanceProperties",
    "xrGetSystemProperties",
    "xrEnumerateViewConfigurationViews",
    "xrEnumerateSwapchainFormats",
    "xrCreateSwapchain",
    "xrEnumerateSwapchainImages",
    "xrWaitSwapchainImage",
    "xrAcquireSwapchainImage",
    "xrReleaseSwapchainImage",
    "xrCreateActionSet",
    "xrDestroyActionSet",
    "xrCreateAction",
    "xrDestroyAction",
    "xrCreateActionSpace",
    "xrCreateReferenceSpace",
    "xrDestroySpace",
    "xrLocateSpace",
    "xrSyncActions",
    "xrGetActionStatePose",
    "xrPathToString",
    "xrStringToPath",
    "xrConvertWin32PerformanceCounterToTimeKHR",
    "xrCreateHandTrackerEXT",
    "xrDestroyHandTrackerEXT",
    "xrLocateHandJointsEXT",
    "xrGetVisibilityMaskKHR",
]

extensions_to_search = ["XR_KHR_visibility_mask", "XR_KHR_win32_convert_performance_counter_time", "XR_EXT_hand_tracking"]

# OpenXR Layer Input

This is an OpenXR API Layer that overrides the rotation of a connected Meta Quest Pro headset and the thumbstick positions of the paired Oculus Touch controllers.

## Using

When the API Layer is active, a [ZeroMQ](https://zeromq.org/) TCP socket is opened on port 5555 and messages are continuously pushed to it that contain an image of the current view from the left eye. You can then feed this image to an AI agent, and respond to the socket with a JSON object containing at least the following keys:

- "left_thumbstick_x"
- "left_thumbstick_y"
- "right_thumbstick_x"
- "right_thumbstick_y"
- "head_pos_x"
- "head_pos_y"
- "head_pos_z"
- "head_dir_w"
- "head_dir_x"
- "head_dir_y"
- "head_dir_z"

Note that the head position is not currently overridden.

## Building

1. Install Visual Studio 2022.
2. Install cppzmq and OpenCV. I used vcpkg for this.
3. Click the 'Build' button in Visual Studio or run `msbuild OpenXR-Toolkit.sln`

## Acknowledgements

This software is based on Matthieu Bucchianeri's [OpenXR Toolkit](https://github.com/mbucchia/OpenXR-Toolkit), which serves as a useful template for developing an OpenXR API Layer.

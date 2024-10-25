#include "pch.h"
#include "interfaces.h"
#include "factories.h"
#include "layer.h"
#include "log.h"

#include "image.pb.h"
#include <zmq.hpp>
#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>

namespace {

    using namespace toolkit;
    using namespace toolkit::config;
    using namespace toolkit::graphics;
    using namespace toolkit::input;
    using namespace toolkit::log;

    class InputController : public IInputController {
        public:
        InputController(OpenXrApi& openXR, std::shared_ptr<IConfigManager> configManager)
            : m_openXR(openXR), m_configManager(configManager) {
            using namespace std::chrono_literals;

            // connect socket to machine learning agent
            socket.connect("tcp://localhost:5555");
            DebugLog("Connected to machine learning server.\n");
        }

        ~InputController() override {
        }

        // Get action from machine learning agent by sending an image
        int getActionFromAgent(std::shared_ptr<graphics::ITexture> texture) override {
            // Convert texture to image and save it
            auto path = localAppData / "texture.png";
            DebugLog("Saving image to file %s...\n", path.string().c_str());
            texture->saveToFile(path);

            // Read image from path with OpenCV
            DebugLog("Reading image from file...\n");
            cv::Mat image = cv::imread(path.string(), cv::IMREAD_COLOR);
            if (!image.data) {
                return -1;
            }
            cv::resize(image, image, cv::Size(224, 224));

            // Serialise image with protobuf
            Image imageProto;
            imageProto.set_data(image.data, image.total() * image.elemSize());
            imageProto.set_width(image.cols);
            imageProto.set_height(image.rows);
            imageProto.set_channels(image.channels());

            std::string imageProtoString;
            imageProto.SerializeToString(&imageProtoString);

            // Send image to machine learning agent
            DebugLog("Sending image to machine learning server...\n");
            zmq::message_t message(imageProtoString.size());
            memcpy(message.data(), imageProtoString.data(), imageProtoString.size());
            socket.send(message, zmq::send_flags::none);
            DebugLog("Sent image to machine learning server.\n");

            // Wait for reply
            zmq::message_t reply{};
            socket.recv(reply, zmq::recv_flags::none);
            std::string replyString{static_cast<char*>(reply.data()), reply.size()};
            DebugLog("Received reply: %s\n", replyString.c_str());

            // Parse reply string as json
            nlohmann::json json = nlohmann::json::parse(replyString);

            // Get actions from json and store them
            XrVector2f leftThumbstickPosition{json["left_thumbstick_x"].get<float>(), json["left_thumbstick_y"].get<float>()};
            m_thumbstickPositions[Hand::Left] = leftThumbstickPosition;
            XrVector2f rightThumbstickPosition{json["right_thumbstick_x"].get<float>(), json["right_thumbstick_y"].get<float>()};
            m_thumbstickPositions[Hand::Right] = rightThumbstickPosition;
            
            m_headPose.position.x = json["head_pos_x"].get<float>();
            m_headPose.position.y = json["head_pos_y"].get<float>();
            m_headPose.position.z = json["head_pos_z"].get<float>();
            m_headPose.orientation.w = json["head_dir_w"].get<float>();
            m_headPose.orientation.x = json["head_dir_x"].get<float>();
            m_headPose.orientation.y = json["head_dir_y"].get<float>();
            m_headPose.orientation.z = json["head_dir_z"].get<float>();

            m_hasAction = true;
            return 0;
        }

        XrQuaternionf getHeadOrientation() const override {
            return m_headPose.orientation;
        }

        bool getThumbstickActionState(const Hand hand, XrActionStateFloat& state) const override {
            if (m_thumbstickPositions.find(hand) == m_thumbstickPositions.end()) {
                return false;
            }

            if (hand == Hand::Left) {
                state.currentState = m_thumbstickPositions.at(hand).x;
            } else if (hand == Hand::Right) {
                state.currentState = m_thumbstickPositions.at(hand).y;
            }

            return true;
        }

        bool isEnabled() const override {
            // TODO: Currently, this controller is enabled when the "record statistics" setting is enabled
            return m_configManager->getValue(config::SettingRecordStats);
        }

        bool hasAction() const override {
            return m_hasAction;
        }

        private:
        OpenXrApi& m_openXR;
        const std::shared_ptr<IConfigManager> m_configManager;

        // initialize with a single IO thread
        zmq::context_t context{1};
        zmq::socket_t socket{context, zmq::socket_type::req};

        std::map<Hand, XrVector2f> m_thumbstickPositions;
        XrPosef m_headPose;

        bool m_hasAction{false};
    };
}

namespace toolkit::input {
    std::shared_ptr<IInputController> CreateInputController(toolkit::OpenXrApi& openXR,
                                                    std::shared_ptr<toolkit::config::IConfigManager> configManager) {
        return std::make_shared<InputController>(openXR, configManager);
    }
}
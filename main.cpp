#include <iostream>
#include <vector>
#include <string>

#include <opencv2/opencv.hpp>

#include "vehicles/multirotor/api/MultirotorRpcLibClient.hpp"
#include "common/Common.hpp"

int main() {

    msr::airlib::MultirotorRpcLibClient client;

    client.confirmConnection();

    std::vector<std::string> vehicles = client.listVehicles();
    std::cout << "Active number of vehicles: " << vehicles.size() << std::endl;

    for (const std::string& name : vehicles) {
        std::cout << "Active vehicle names: '" << name << "'" << std::endl;
    }

    std::vector<msr::airlib::ImageCaptureBase::ImageRequest> requests;
    // NOTE: I have no idea but camera_name field for ImageRequest has to be 0. Some values will crash the app.
    requests.push_back(msr::airlib::ImageCaptureBase::ImageRequest(msr::airlib::ImageCaptureBase::ImageRequest("0", msr::airlib::ImageCaptureBase::ImageType::Scene, false)));

    std::string vehicle_name = "SimpleFlight";

    while (true)
    {
        const std::vector<msr::airlib::ImageCaptureBase::ImageResponse>& responses = client.simGetImages(requests, vehicle_name);

        if (responses.size() == 0) {
            std::cout << "No response from the camera." << std::endl;
            continue;
        }

        const msr::airlib::ImageCaptureBase::ImageResponse& response = responses[0];

        if (response.image_data_uint8.size() == 0) continue;

        cv::Mat frame;
        try {
            frame = cv::imdecode(cv::Mat(response.image_data_uint8), cv::IMREAD_COLOR);
        } catch (const cv::Exception& e) {
            std::cerr << "OpenCV Decode Fail: " << e.what() << std::endl;
            continue;
        }

        if (frame.empty()) {
            std::cout << "OpenCV frame is empty." << std::endl;
            continue;
        }

        cv::imshow("AirSim C++ Segmentation View", frame);

        if (cv::waitKey(1) == 'q') {
            break;
        }
    }

    cv::destroyAllWindows();

    return 0;
}
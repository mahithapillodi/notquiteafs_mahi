#include "../notquiteafs.grpc.pb.h"
#include <grpc++/grpc++.h>
#include <memory>
#include <iostream>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using notquiteafs::ServerRequest;
using notquiteafs::ServerResponse;
using notquiteafs::ServerService;

class ServerClient {
public:
    ServerClient(std::shared_ptr<Channel> channel) : _stub{ServerService::NewStub(channel)} {}

    std::string ServerMethod(const std::string& request_server_field) {
        // Prepare request
        ServerRequest request;
        request.set_request_server_field(request_server_field);

        // Send request
        ServerResponse response;
        ClientContext context;
        Status status;
        status = _stub->ServerMethod(&context, request, &response);

        // Handle response
        if (status.ok()) {
            return response.response_server_field();
        } else {
            std::cerr << status.error_code() << ": " << status.error_message() << std::endl;
            return "RPC failed";
        }
    }

private:
    std::unique_ptr<ServerService::Stub> _stub;
};

int main(int argc, char** argv) {
    std::string server_address{"localhost:2510"};
    ServerClient client{grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials())};
    std::string request_server_field{"world"};
    std::string response_server_field = client.ServerMethod(request_server_field);
    std::cout << "Client received: " << response_server_field << std::endl;
    return 0;
}
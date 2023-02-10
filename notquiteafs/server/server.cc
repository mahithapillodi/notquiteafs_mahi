
#include "../notquiteafs.grpc.pb.h"
#include <grpc++/grpc++.h>
#include <memory>
#include <iostream>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using notquiteafs::ServerRequest;
using notquiteafs::ServerResponse;
using notquiteafs::ServerService;

class ServerServiceImpl final : public ServerService::Service {
    Status ServerMethod(ServerContext* context, const ServerRequest* request, ServerResponse* response) override {
        response->set_response_server_field("Hello " + request->request_server_field());
        return Status::OK;
    }
};

void RunServer() {
    std::string server_address{"localhost:2510"};
    ServerServiceImpl service;

    // Build Server
    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server{builder.BuildAndStart()};

    // Run Server
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}

int main(int argc, char** argv) {
    RunServer();
    return 0;
}
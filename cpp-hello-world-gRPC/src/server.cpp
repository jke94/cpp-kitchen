#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "greeter.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using greeter::Greeter;
using greeter::HelloRequest;
using greeter::HelloReply;

/**
 * @brief Implementación del servicio Greeter
 */
class GreeterServiceImpl final : public Greeter::Service 
{
public:
    GreeterServiceImpl();

    Status SayHello(
        ServerContext* context, 
        const HelloRequest* request,
        HelloReply* reply
    ) override;

private:
    int responseCounter_;
};

GreeterServiceImpl::GreeterServiceImpl() 
    : responseCounter_(0) 
{
}

Status GreeterServiceImpl::SayHello(
    ServerContext* context, 
    const HelloRequest* request,
    HelloReply* reply)
{
    responseCounter_++;
    std::string response = "[Request " + std::to_string(responseCounter_) + "] " + "Hola, " + request->name();
    reply->set_message(response);

    return Status::OK;    
}


void runServer()
{
    std::string server_address("localhost:50051");
    GreeterServiceImpl service;

    ServerBuilder serviceBuilder;
    serviceBuilder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    serviceBuilder.RegisterService(&service);

    std::unique_ptr<Server> server(serviceBuilder.BuildAndStart());

    std::cout << "Servidor escuchando en " << server_address << std::endl;
    server->Wait();
}

int main() 
{
    runServer();
    return 0;
}

#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "greeter.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using greeter::Greeter;
using greeter::HelloRequest;
using greeter::HelloReply;

class GreeterClient 
{
public:
    GreeterClient(std::shared_ptr<Channel> channel);
    std::string SayHello(const std::string& name);

private:
    std::unique_ptr<Greeter::Stub> stub_;
};

GreeterClient::GreeterClient(std::shared_ptr<Channel> channel)
    : stub_(Greeter::NewStub(channel)) {}

std::string GreeterClient::SayHello(const std::string& name) 
{
    HelloRequest request;
    request.set_name(name);

    HelloReply reply;
    ClientContext context;

    Status status = stub_->SayHello(&context, request, &reply);
    
    if (!status.ok()) 
    {
        return "RPC failed";
    } 

    return reply.message();
}

int main() 
{
    GreeterClient greeter(grpc::CreateChannel(
        "localhost:50051", 
        grpc::InsecureChannelCredentials())
    );
    std::string user("Mike");
    std::string reply = greeter.SayHello(user);
    std::cout << "Greeter recibió: " << reply << std::endl;

  return 0;
}

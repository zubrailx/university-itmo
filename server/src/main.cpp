#include <dbpb/address-book.grpc.pb.h>
#include <dbpb/address.pb.h>

#include <grpc/grpc.h>
#include <grpcpp/server_builder.h>

#include <iostream>

class AddressBookService final : public AddressBook::Service {
public:
  virtual grpc::Status GetAddress(grpc::ServerContext *context,
                                    const NameQuerry *request,
                                    Address *response) {
    std::cout << "Server: GetAddress for \"" << request->name() << "\"."
              << std::endl;

    response->set_name("Peter Peterson");
    response->set_zip("12345");
    response->set_country("Superland");

    return grpc::Status::OK;
  }
};

int main(int argc, char *argv[]) {
  grpc::ServerBuilder builder;
  builder.AddListeningPort("0.0.0.0:50051", grpc::InsecureServerCredentials());

  AddressBookService my_service;
  builder.RegisterService(&my_service);

  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  server->Wait();

  return 0;
}

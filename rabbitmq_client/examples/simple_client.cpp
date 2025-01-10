// Copyright 2025 TeiaCare
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <teiacare/rabbitmq_client/rabbitmq_client.hpp>

#include <iostream>
#include <memory>

int main(int argc, char** argv)
{
    tc::rmq::rabbitmq_client client{"localhost", 5672, "guest", "guest"};
    std::shared_ptr<tc::rmq::channel> channel = client.create_channel("Channel_1");

    channel->declare_exchange("Exchange_1", "direct", true, false);
    channel->declare_queue("Queue_1", true, false, false);
    channel->bind_queue("Queue_1", "Exchange_1", "routing_key");

    channel->publish("Exchange_1", "routing_key", "HELLO WORLD!");
    const std::string msg = channel->consume("Queue_1");
    std::cout << msg << std::endl;

    std::cout << "OK" << std::endl;

    return 0;
}

#pragma once

#include <teiacare/rabbitmq_client/channel.hpp>

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

// #include <rabbitmq-c/amqp.h>
// #include <rabbitmq-c/tcp_socket.h>

namespace tc::rmq
{
/**
 * @brief Client for interacting with RabbitMQ.
 */
class rabbitmq_client
{
public:
    /**
     * @brief Constructs a RabbitMQ client.
     * @param host The hostname of the RabbitMQ server.
     * @param port The port of the RabbitMQ server.
     * @param user The username for authentication.
     * @param password The password for authentication.
     */
    explicit rabbitmq_client(const std::string& address, int port, const std::string& username, const std::string& password);

    /**
     * @brief Destructor for the RabbitMQ client.
     */
    ~rabbitmq_client() = default;

    /**
     * @brief Creates a new channel with the specified name.
     * @param name The name of the channel.
     * @return A shared pointer to the created channel.
     */
    std::shared_ptr<tc::rmq::channel> create_channel(const std::string& name);

    /**
     * @brief Retrieves an existing channel by name.
     * @param name The name of the channel.
     * @return A shared pointer to the channel.
     */
    std::shared_ptr<tc::rmq::channel> get_channel(const std::string& name) const;

private:
    std::shared_ptr<amqp_connection_state_t_> connection;
    std::unique_ptr<amqp_socket_t_, void (*)(amqp_socket_t_*)> socket;
    std::unordered_map<std::string, std::shared_ptr<channel>> channels;
    static uint16_t next_channel_id;

    auto init_connection() -> decltype(connection);
    auto init_socket() -> decltype(socket);
    void check_amqp_reply(const std::string& context) const;
};

}

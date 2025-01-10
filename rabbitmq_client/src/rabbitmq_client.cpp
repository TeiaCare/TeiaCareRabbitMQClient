#include <teiacare/rabbitmq_client/channel.hpp>
#include <teiacare/rabbitmq_client/rabbitmq_client.hpp>

#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>
#include <stdexcept>

namespace tc::rmq
{
uint16_t rabbitmq_client::next_channel_id = 0;

auto rabbitmq_client::init_connection() -> decltype(connection)
{
    return {
        amqp_new_connection(),
        [](amqp_connection_state_t_* c) {
            amqp_connection_close(c, AMQP_REPLY_SUCCESS);
            amqp_destroy_connection(c);
        }};
}

auto rabbitmq_client::init_socket() -> decltype(socket)
{
    return {
        amqp_tcp_socket_new(connection.get()),
        [](amqp_socket_t_*) {}};
}

rabbitmq_client::rabbitmq_client(const std::string& address, int port, const std::string& username, const std::string& password)
    : connection{init_connection()}
    , socket{init_socket()}
{
    int socket_status = amqp_socket_open(socket.get(), address.c_str(), port);
    if (socket_status != amqp_status_enum_::AMQP_STATUS_OK)
    {
        const auto error_msg = amqp_error_string2(socket_status);
        throw std::runtime_error(error_msg);
    }

    amqp_login(connection.get(), "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, username.c_str(), password.c_str());
}

std::shared_ptr<tc::rmq::channel> rabbitmq_client::create_channel(const std::string& name)
{
    if (channels.contains(name))
    {
        throw std::runtime_error("Channel with name " + name + " already exists");
    }

    auto channel_ptr = std::make_shared<tc::rmq::channel>(connection, ++next_channel_id, name);
    channels[name] = channel_ptr;
    return channel_ptr;
}

std::shared_ptr<tc::rmq::channel> rabbitmq_client::get_channel(const std::string& name) const
{
    if (!channels.contains(name))
    {
        throw std::runtime_error("Channel with name " + name + " not found");
    }

    return channels.at(name);
}

void rabbitmq_client::check_amqp_reply(const std::string& context) const
{
    amqp_rpc_reply_t reply = amqp_get_rpc_reply(connection.get());
    if (reply.reply_type != AMQP_RESPONSE_NORMAL)
    {
        throw std::runtime_error(context + " failed: " + amqp_error_string2(reply.library_error));
    }
}

}

#include <teiacare/rabbitmq_client/channel.hpp>

#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>
#include <stdexcept>

namespace tc::rmq
{
channel::channel(std::weak_ptr<amqp_connection_state_t_> connection, uint16_t channel_id, const std::string& name)
    : _connection{connection}
    , _channel_id{channel_id}
    , _name{name}
{
    amqp_channel_open(get_connection(), _channel_id);
    check_amqp_reply("Opening channel");
}

channel::~channel()
{
    amqp_channel_close(get_connection(), _channel_id, AMQP_REPLY_SUCCESS);
}

void channel::declare_exchange(const std::string& exchange, const std::string& type, bool durable, bool auto_delete)
{
    amqp_exchange_declare(
        get_connection(),
        _channel_id,
        amqp_cstring_bytes(exchange.c_str()),
        amqp_cstring_bytes(type.c_str()),
        0,
        durable,
        auto_delete,
        0,
        amqp_empty_table);

    check_amqp_reply("Declaring exchange");
}

void channel::declare_queue(const std::string& queue, bool durable, bool exclusive, bool auto_delete)
{
    amqp_queue_declare(
        get_connection(),
        _channel_id,
        amqp_cstring_bytes(queue.c_str()),
        0,
        durable,
        exclusive,
        auto_delete,
        amqp_empty_table);

    check_amqp_reply("Declaring queue");
}

void channel::bind_queue(const std::string& queue, const std::string& exchange, const std::string& routing_key)
{
    amqp_queue_bind(
        get_connection(),
        _channel_id,
        amqp_cstring_bytes(queue.c_str()),
        amqp_cstring_bytes(exchange.c_str()),
        amqp_cstring_bytes(routing_key.c_str()),
        amqp_empty_table);

    check_amqp_reply("Binding queue");
}

void channel::unbind_queue(const std::string& queue, const std::string& exchange, const std::string& routing_key)
{
    amqp_queue_unbind(
        get_connection(),
        _channel_id,
        amqp_cstring_bytes(queue.c_str()),
        amqp_cstring_bytes(exchange.c_str()),
        amqp_cstring_bytes(routing_key.c_str()),
        amqp_empty_table);

    check_amqp_reply("Unbinding queue");
}

void channel::publish(const std::string& exchange, const std::string& routing_key, const std::string& message)
{
    amqp_basic_properties_t props;
    props._flags = AMQP_BASIC_CONTENT_TYPE_FLAG | AMQP_BASIC_DELIVERY_MODE_FLAG;
    props.content_type = amqp_cstring_bytes("text/plain");
    props.delivery_mode = 2; // persistent delivery mode

    amqp_basic_publish(
        get_connection(),
        _channel_id,
        amqp_cstring_bytes(exchange.c_str()),
        amqp_cstring_bytes(routing_key.c_str()),
        0,
        0,
        &props,
        amqp_cstring_bytes(message.c_str()));

    check_amqp_reply("Publishing");
}

std::string channel::consume(const std::string& queue)
{
    amqp_basic_consume(
        get_connection(),
        _channel_id,
        amqp_cstring_bytes(queue.c_str()),
        amqp_empty_bytes,
        0,
        1,
        0,
        amqp_empty_table);

    check_amqp_reply("Consuming");

    amqp_envelope_t envelope;
    amqp_maybe_release_buffers(get_connection());

    amqp_rpc_reply_t ret = amqp_consume_message(
        get_connection(),
        &envelope,
        nullptr,
        0);

    if (ret.reply_type != AMQP_RESPONSE_NORMAL)
    {
        throw std::runtime_error("Failed to consume message");
    }

    std::string message(static_cast<char*>(envelope.message.body.bytes), envelope.message.body.len);
    amqp_destroy_envelope(&envelope);
    return message;
}

void channel::check_amqp_reply(const std::string& context) const
{
    amqp_rpc_reply_t reply = amqp_get_rpc_reply(get_connection());
    if (reply.reply_type != AMQP_RESPONSE_NORMAL)
    {
        throw std::runtime_error(context + " failed: " + amqp_error_string2(reply.library_error));
    }
}

amqp_connection_state_t_* channel::get_connection() const
{
    if (auto c = _connection.lock())
    {
        return c.get();
    }

    throw std::runtime_error("Connection is no longer available");
}

}

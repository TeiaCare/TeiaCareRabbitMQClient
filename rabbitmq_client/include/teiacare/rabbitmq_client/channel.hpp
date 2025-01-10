#pragma once

#include <cstdint>
#include <memory>
#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>
#include <string>

namespace tc::rmq
{
/**
 * @brief Represents a RabbitMQ channel.
 */
class channel
{
public:
    /**
     * @brief Constructs a RabbitMQ channel.
     * @param conn A weak pointer to the RabbitMQ connection.
     * @param channel_id The ID of the channel.
     * @param name The name of the channel.
     */
    explicit channel(std::weak_ptr<struct amqp_connection_state_t_> conn, uint16_t channel_id, const std::string& name);

    /**
     * @brief Destructor for the RabbitMQ channel.
     */
    ~channel();

    /**
     * @brief Declares an exchange.
     * @param exchange The name of the exchange.
     * @param type The type of the exchange (e.g., "direct").
     * @param durable Whether the exchange is durable.
     * @param auto_delete Whether the exchange should auto-delete.
     */
    void declare_exchange(const std::string& exchange, const std::string& type, bool durable, bool auto_delete);

    /**
     * @brief Declares a queue.
     * @param queue The name of the queue.
     * @param durable Whether the queue is durable.
     * @param exclusive Whether the queue is exclusive.
     * @param auto_delete Whether the queue should auto-delete.
     */
    void declare_queue(const std::string& queue, bool durable, bool exclusive, bool auto_delete);

    /**
     * @brief Binds a queue to an exchange.
     * @param queue The name of the queue.
     * @param exchange The name of the exchange.
     * @param routing_key The routing key for the binding.
     */
    void bind_queue(const std::string& queue, const std::string& exchange, const std::string& routing_key);

    /**
     * @brief Unbinds a queue from an exchange.
     * @param queue The name of the queue.
     * @param exchange The name of the exchange.
     * @param routing_key The routing key for the binding.
     */
    void unbind_queue(const std::string& queue, const std::string& exchange, const std::string& routing_key);

    /**
     * @brief Publishes a message to an exchange.
     * @param exchange The name of the exchange.
     * @param routing_key The routing key for the message.
     * @param message The message to publish.
     */
    void publish(const std::string& exchange, const std::string& routing_key, const std::string& message);

    /**
     * @brief Consumes a message from a queue.
     * @param queue The name of the queue.
     * @return The consumed message.
     */
    std::string consume(const std::string& queue);

    /**
     * @brief Gets the name of the channel.
     * @return The name of the channel.
     */
    const std::string& get_name() const
    {
        return _name;
    }

private:
    std::weak_ptr<amqp_connection_state_t_> _connection;
    uint16_t _channel_id;
    const std::string _name;

    void check_amqp_reply(const std::string& context) const;
    amqp_connection_state_t_* get_connection() const;
};

}

#  Message Queues (Point-to-Point)

### Technical Working
![](https://raw.githubusercontent.com/karanpratapsingh/portfolio/master/public/static/courses/system-design/chapter-III/message-queues/message-queue.png)
This is service-to-sevice communication, in this case, one service (a producer) wants to communicate to one other service. They do this by sending their message in a queue that the consumer consumes from. So the message is stored in the queue until it's processed and deleted. Each message is processed only once by our consumer. 

Here's the workflow in full:
1. User makes a request to the producer. 
2. Producer publishes a job to the queue. Then they notify the user of the job's status e.g. "Your job was been submittted/started"
3. The consumer picks up the job from the queue, processes it, then sends a signal that the job is complete somehow.

---
### Advantages and Benefits
- **Scalability:**  You can also scale wiht it. If you're getting 1000+ requests, instead of processing them all at once immediately, we can put them in a queue to let the service process them at its own rate.
- **Decoupling:** Remove dependencies betwee ncomponetns and simplify the implementation of decoupled applications. We'll see an example of this soon.
- **Performance:** Asynchronous communication; producers and consumers don't interact with each other, but the queue itself. You can continue adding requests to the queue without waiting for the previous ones to be finished.
- **Queues:** Persist our data a bit. Messages are stored until the consumer pulls them and acknowledges that they've been processed. Can app could be offline, and go back online and all those orders are stored in the queue. There's also retries, but that's not the default.

---
### Features Of A Message Queue
- **Push or Pull Delivery:** Pulling means continuously querying the queue for new messages, whilst pushing means notifying a consumer when a new message is available.
- **FIFO Queues:** The first/oldest message/entry is processed first.
- **Schedule or Delay Delivery:** Commonly ou should be able to set a specific delivery time for a message.
- **At-Least-once Delivery:** Message queues may store multiple copies of a message and resend them in the case of failures/errors to ensure that they're delivered at least once.
- **Exactly Once Delivery:** Don't have duplicate messages are dleivered exactly once.
- **Dead letter queues**: A queue to which other queues send messages that can't be processed successfully. Almost like a logging technqiue as you'll store these for inspection later to see why things failed.
- **Ordering:** Most message queues ensure that messages are delivered to the consumer in the same order as they were received from the producer, and that we deliver a message to the consumer at least once.

---
### Examples in action
- First let's see a standard setup and example for message queues in an application, why it's used in this situation, etc. Why use message queues? What's the requirements of the application that necessitates this. Let's also explain how this actually "decouples" the 2 services because we haven't gotten an idea of why that is yet. Talk about what happens if our service goes offline, and how it can come back online to the messages. Talk about the various benefits and whatnot. Code snippets from each service, how the data travels because I think it travels from producer, to the message queue (which is essentially just your message broker using the message queue pattern), then to the consumer. In producer and consumer they probably have some code conecting. Finally why this works over a regular request-response.

### Example 1: E-commerce Order Processing
Let's see a real world example of how message queues work. We'll build an e-commerce order processing system that demonstrates why message queues are essential and how they provide decoupling

#### Requirement and Scenario
- Process thousands of orders per day (with spikes during sales)
- Send confirmation emails immediately
- Update inventory counts
- Process payments 
- Generate shipping labels
- Update analytics and recommendations 
- Handle failures gracefully without losing orders.

However we have some technical challenges:
- Different processing speeds (email is fast, payment processing is slow)
- External dependencies (payment gateway, shipping API)
- Variable load (Black Friday vs. a regular Tuesday)
- Service maintenance windows 
- Failure recovery

#### Without Message Queues: The Problematic Approach
```Python
# Order Service - A synchronous approach

import requests
import time

def process_order_synchronously(order_data):
  try:
    # Step 1: Save order to database
    save_order(order_data)

    # Step 2: Process payment (can take 2-5 seconds)
    payment_response = requests.post(
      "http://paymnet-service/process",
      json=order_data,
      timeout=10
    )

    # Step 3: Update inventory (fast, but could fail)
    inventory_response = requests.post(
      "http://inventory-service/update",
      json={"product_id": order_data["product_id"], "quantity":-1}
    )

    # Step 4: Send email (usually fast)
    email_response = requests.post(
      "http://email-service/send",
      json={"to":order_data["email"], "template": "order_confirmation"}
    )

    # Step 5: Create shipping label (slow, uses an external API)
    shipping_response = request.post(
      "http://shipping-service/create-label",
      json=order_data
    )

    return {"status":"success", "order_id", order_data["id"]}
  except Exception as e:
    # If any service fails, the entire order fails
    # Customer sees error, adn order might be in an inconsistent state. 
    return {"status":"error", "message": str(e)}

######## *** The Inventory, Shipping, and the rest of the services *** ########
# ...
```
Here are the issues with this approach:
- **Blocking:** Customer waits for all services to complete (could be 10+ seconds).
- **Failure cascade:** If any service fails, the entire thing fails, leaving things in an inconsistent state.
- **Tight coupling:** Order service must know about and connect to every other service. Look here, we have to create logic for making requests to every other service.

#### With Message Queues: The Decoupled Approach
Now let's see how using message queues can solve these problems:
```Python
########## *** Order Service - Producer *** ##########
import json
import pika
from datetime import datetime

class OrderService:
  def __init__(self):
    # Connect to RabbitMQ
    self.connection = pika.BlockingConnection(
      pika.ConnectionParameter("localhost")
    )
    self.channel = self.connection.channel()


    '''
    The line exchange="orders" refers to a RabbitMQ exchange called "orders". This is the core of the messaging system as it receives messages from producers and routes them to queues under the "orders" exchange. Think of it like this, imagine a mailroom:

    - OrderService is an employee that places a message in the "orders" inbox. 
    - Exchange: The mailrooms inbox, and we'll decide where that message should go.
    - Queues: Like payment_processing are like mailboxes that the mailroom can route the mail to.
    - Payment and Email services are like employees who check their mailboxes and act on the messages.
    
    With fanout, we broadcast messages to all queues bound to it.
    '''
    self.channel.exchange_declare(exchange="orders", exchange_type="fanout")

    # Create a listener for payment_completed 
    self.channel.queue_declare(queue="order_updates", durable=True)
    self.channel.queue_bind(exchange="orders", queue="order_updates")
    self.channel.basic_consume(queue="order_updates", on_message_callback=self.handle_payment_update)



  def create_order(self, order_data):
    try:
      # Step 1: Save order to the database (critical step)
      order_id = self.save_order(order_data)

      # Step 2: Publish ORDER_CREATED message to queue
      message = {
        'order_id': order_id,
        'customer_email': order_data['email'],
        'product_id': order_data['product_id'],
        'quantity': order_data['quantity'],
        'timestamp': datetime.utcnow().isoformat(),
        'event_type': 'ORDER_CREATED' 
      }

      self.channel.basic_publish(
        exchange='orders',
        routing_key='',
        body=json.dumps(message),
        properties=pika.BasicProperties(
          delivery_mode=2,  # Make message persistent, which means what?
        )
      )

      return {
        "status": "success",
        "order_id" order_id,
        "message": "Order received and being processed"
      }
    
    except Exception as e:
      return {"status": "error", "message": "Failed to create order"}

  def save_order(self, order_data):
    # Save to database and return order_id
    return "order_12345"

  def handle_payment_update(self, channel, method, properties, body):
     message = json.loads(body)
    if message["event_type"] == "PAYMENT_COMPLETED":
      order_id = message["order_id"]
      self.update_order_status(order_id, "paid")
    channel.basic_ack(delivery_tag=method.delivery_tag
  
  def update_order_status(self, order_id, status):
    # Have your order updating logic to makr an order as complete rather than pending.
    pass

########## *** Payment Service *** ##########
class PaymentService:
  def __init__(self):
    connection = pika.BlockingConnection(
      pika.ConnectionParameters("localhost")
    )
    self.channel = connection.channel()

    # 1. Create/declare a queue called "payment_processing" for this service
    # 2. "Bind" the queue to "orders" which means something
    self.channel.queue_declare(queue="payment_processing", durable=True)
    self.channel.queue_bind(
      exchange="orders",
      queue="payment_processing"
    )

    # Set up a consumer that consumes one message at a time. When a message is 
    # found, we'll process it with a callback function.
    self.channel.basic_qos(prefetch_count=1)
    self.channel.basic_consume(
      queue="payment_processing",
      on_message_callback=self.process_payment
    )
  
  def process_payment(self, channel, method, properties, body):
    try: 
      message = json.loads(body)
      order_id = message["order_id"]

      # Simulate payment processing (2-5 seconds)
      payment_result = self.charge_customer(message)
      if payment_result["success"]:
        # Payment was successful so publish a PAYMENT_COMPLETED event 
        # that will be found by others. The else implicitly says it failed.
        success_message = {
          'order_id': order_id,
          'event_type': 'PAYMENT_COMPLETED',
          'amount': payment_result['amount']
        }
        
        channel.basic_publish(
          exchange='orders',
          routing_key='',
          body=json.dumps(success_message)
        )
      
      # Regardless of success or failure, we'll acknowledge that we at least processed 
      # the message. By acknowledging the message, it'll be removed from the message queue.
      channel.basic_ack(delivery_tag=method.delivery_tag)
    except Exception as e:
      # Reject the message, send it to a dead-letter queue or retry
      channel.basic_nack(
        delivery_tag=method.delivery_tag,
        requeue=False
      )
  
  def charge_customer(self, order_data):
    time.sleep(3)
    return {"success": True, "amount": 99.99}
  
  def start_consuming(self):
    print("Payment service waiting for messages...")
    self.channel.start_consuming()


######## *** Email Service *** ######## 
import smtplib
class EmailService:
  def __init__(self):
    connection = pika.BlockingConnection(
      pika.ConnectionParameters("localhost")
    )
    
    # Create queue for email service called "email_notifications"; we don't really specifically target 
    # the email notifications queue though. We're doing fanout so we're broadcasting.
    self.channel = connection.channel()
    self.channel.queue_declare(queue="email_notifications", durable=True)
    self.channel.queue_bind(
      exchange="orders",
      queue="email_notifications"
    )

    # Setup a consumer (listening) for email service 
    self.channel.basic_qos(prefetch_count=10) # Handle 10 messages at once
    self.channel.basic_consume(
      queue="email_notifications",
      on_message_callback=self.send_email
    )
  
  def send_email(self, channel, method, properties, body):
    # Note that the email service is listening for both events 
    # created by the Order and Payment services. 
    try:
      message = json.loads(body)
      if message['event_type'] == 'ORDER_CREATED':
        self.send_order_confirmation(message)
      elif message['event_type'] == 'PAYMENT_COMPLETED':
        self.send_payment_confirmation(message)

      channel.basic_ack(delivery_tag=method.delivery_tag)
    except Exception as e:
      print(f"Email failed: {e}")
      channel.basic_nack(delivery_tag=method.delivery_tag, requeue=True)
  
  # You'd have your logic here
  def send_order_confirmation(self, order_data):
    pass

  def send_payment_confirmation(self, payment_data):
    pass
  
  def start_consuming(self):
    print("Email service waiting for messages...")
    self.channel.start_consuming()
```

#### How the Data Flows 
Here's the workflow for an order:
1. Customer places an order in the Order Service.
2. Order Service saves order to the database and publishes `ORDER_CREATED`.
3. Message Broker (RabbitMQ in this case) receives message and routes it to all queues:
  - `payment_processing` queue
  - `email_notifications` queue
  - `inventory_updates` queue
  - `shipping_labels` queue
4. Each Consumer Service independently processes the message:
  - Payment Service charges customer and publishes `PAYMENT_COMPLETED`
  - Email Service sends confirmation email
  - Inventory Service updates stock counts.
  - Shipping Service creates label when payment completes
  - Order service probably marks the order was completed rather than pending.
5. Customer gets an immediate response whilst processing happens in the background. 

#### How This Achieves Decoupling and Benefits
Service Independence:
- Order Service doesn't know or care about Payment, Email, or Shipping services.
- Each service can be deployed, updated, or scaled independently
- Adding new services (Analytics, Recommendations) requires no changes to existing services.

Failure Isolation:
- If Payment Service goes down, orders will still get created and emails still send. 
```Python
# Before crash: Queue is empty, messages are flowing normally
Payment Queue: []

# During crash, Messages will accumulate in the payment_processing queue
Payment Queue: [order_123, order_124, order_125, order_126]

# After restart: the service starts processing the backlog of messages 
# - No data loss: Messages are persisted in queue
# - Automatic recovery: Serviec picks up where it left off
# - Other services unaffected: Email and inventory keep working
# - Graceful degradation: System operates at reduced capacity vs. complete failure
Payment Service comes online → processes order_123, order_124, order_125...
```
- Messages wait in `payment_processing` queue until service returns. There are no cascading failures. 

**Note:** Obviously there are many ways to do this. Here we're talking about a theoretical situation where we're fully asynchronous. However maybe in a real implementation there's a mix of synchronous and asynchronous communication.

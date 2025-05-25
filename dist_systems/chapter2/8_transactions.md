# Transactions 

## Introduction to Transactions
### What are transactions? Payment Example!
At dinner Alice offers to pay the bill, and the Bob will just transfer money Alice's account to compensate Alice for paying the bill. In this case, it'd be bad if one of the operations didn't work such as, Alice pays the bill, but Bob's account doesn't send money to Alice's due to some error. We need a way to make sure either both of these operations happen, or the whole thing is cancelled. This is a transaction, either all operations happen or none of them do. More formally, a **transaction** is a series of operations that are executed as a single, atomic unit to ensure data integrity and consistency. 

---
### ACID Consistency Model
Properties that should be maintained during transaction processing:
1. **Atomic:** All operations will either succeed or fail together.
2. **Consistency:** All changes are consistent with database constraints. For example, if an account balance can't go below zero, then a transaction will fail if one of its operations violates this condition.
3. **Isolation:** Multiple transactions can happen simultaneously without affecting the outcomes of other transactions.
4. **Durability:** All changes made by operations will persist (written to disk), and will remain in the system even in the event of a system failure.

---
### Base Consistency Model
The BASE consistency model for NoSQL databases. Okay for some domains and situations, ACID is a lot more pessimistic and overprotective of the data, meaning it's a lot more worried about the data's safety than that's really required. NoSQL often uses BASE, which is less strict, but it aligns with horizontal scaling for performance.
- **Basic Availability:** Database works most of the time.
- **Soft State:** Stores don't have to be write-consistent. Replicas don't need to be mutually consistent all the time.
- **Eventual Consistency:** The data is all the same/consistent not immediately, but over time.

#### When is BASE used?
It's evident in NoSQL database like DynamoDB, Cassandra, Hadoop, etc. Remember the idea is that we don't offer guaranteed consistency, but the data will become consistent in the future or when someone does a read and forces it to become consistent.  

In the real world, BASE-compliant databases are typically used by large companies that are storing and processing several terabytes of data every day. It implies that at this point, there's so much data that trying to enforce ACID compliance creates so much overhead that it actually hurts our performance noticeably.

### Examples of ACID Transactions
Should be used in a scenario where you have multiple operations that need to happen together. Examples are transferring money from two accounts, the idea of buying an item which also reduces stock, etc. You don't use a transaction when doing reads, when it's not mission critical, or if it's one operation.


```sql
-- Important SQL Operation; if a step fails, all of them fail.
BEGIN;
UPDATE accounts SET balance = balance - 100 WHERE id = 1;
UPDATE accounts SET balance = balance + 100 WHERE id = 2;
COMMIT;
```

## Distributed Transactions
A distributed transaction is a transaction that spans across two or more separate databases. This is typically the situation where the databases are on different servers, but may also span different databases on one server. Usually the former though. The reason we're talking about distributed transactions in the first place is that, they're going to get a lot more common when you're scaling up and dealing with relational databases. More importantly, dealing with a distributed transaction involves altering data on multiple databases. All nodes must coordinate whether to abort or commit their separate portions of the transaction.

In any case, increased complexity and latency is going to be inevitable. To deal with this, we have created popular solutions for distributed transaction. You may also hear these solutions be called **consensus protocols**, which hare rules that distributed systems use to ensure all nodes agree on the same data. I'll try to show some real world examples like indicate which pattern could be used in what situation

---
### Two-Phase Commit (2PC)
![](https://raw.githubusercontent.com/karanpratapsingh/portfolio/master/public/static/courses/system-design/chapter-II/distributed-transactions/two-phase-commit.png)
We carry out an update in two commits/stages. We require a coordinator node which coordinates and oversees the transaction across different participant nodes:

#### Phase 1: Prepare Phase 
Coordinator asks each node if it can carry out the update, whether the nodes are prepared:
1. Coordinator sends "prepare" message to all participants.
2. Each participant prepares the transaction on their end (locks resources, etc).
3. Participants respond with something like "vote-commit" or "vote-abort" depending on whether they were able to prepare the database transaction.

#### Phase 2: Commit Phase
Coordinator watches and sees the responses:
1. If all participants voted commit, the coordinator sends "commit" to all participants.
2. If any participant voted abort, the coordinator sends "abort" to all participants.
3. Participants either commit their portions of the transaction to the database or rollback depending on what they get back from the coordinator.

#### Implementing 2PC Example

```Python
# You'd probably have this code in a library that you can use across your services?
# But I can imagine just having this in your coordinator at first.

class TwoPhaseCommitCoordinator:
    def __init__(self, participants):
        self.participants = participants
        self.transaction_id = None
    
    def begin_transaction(self, transaction_data):
        self.transaction_id = generate_transaction_id()
        
        # Phase 1: Prepare, collect the votes from other participants
        votes = []
        for participant in self.participants:
            try:
                # Likely a network request to send to a participant
                vote = participant.prepare(self.transaction_id, transaction_data)
                votes.append(vote)
            except Exception as e:
                # Errors result in abort
                votes.append("abort")
        
        # Phase 2: Commit or Abort
        if all(vote == "commit" for vote in votes):
            return self._commit_phase()
        else:
            return self._abort_phase()
    
    def _commit_phase(self):
        for participant in self.participants:
            try: 
                # Likely send a network request to tell the participant to commit their portion of the transaction.
                participant.commit(self.transaction_id)
            except Exception as e:
                # Handle partial commit scenario
                self._handle_commit_failure(participant, e)
        return "committed"
    
    def _abort_phase(self):
        for participant in self.participants:
            try:
                # Send a network request to tell the participant to rollback their portion of the transaction.
                participant.abort(self.transaction_id)
            except Exception as e:
                # Log failure but continue, notice that if this happens, we're leaving a participant
                # waiting indefinitely for an answer. This can happens if we crash mid 2PC protocol.
                # Note: 3PC will handle this.
                pass
        return "aborted"

# Represents a participant
class Participant:
    def __init__(self, name):
        self.name = name
        self.prepared_transactions = {}
    
    def prepare(self, transaction_id, data):
        # Phase 1: Simulate business logic and resource locking
        # This happens in phase 1, and the participant returns whether they can commit or they failed.
        if self._can_commit(data):
            self.prepared_transactions[transaction_id] = data
            return "commit"
        return "abort"
    
    def commit(self, transaction_id):
        if transaction_id in self.prepared_transactions:
            # Apply changes permanently; committing the transaction
            data = self.prepared_transactions[transaction_id]
            self._apply_changes(data)
            del self.prepared_transactions[transaction_id]
    
    def abort(self, transaction_id):
        if transaction_id in self.prepared_transactions:
            # Release locks and cleanup; rolling our transaction back
            del self.prepared_transactions[transaction_id]
```

#### Drawbacks of 2PC
If the coordinator crashes after participants vote "Yes" but before sending "Commit", they’re left hanging, unsure what to do. This is a blocking problem. Other than this, this is synchronous in nature and causes high latency, and also the resources are locked during the entire protocol duration.

---
### Three-Phase Commit (3PC)
![](https://raw.githubusercontent.com/karanpratapsingh/portfolio/master/public/static/courses/system-design/chapter-II/distributed-transactions/three-phase-commit.png)

3PC is an enhanced version of 2PC that's designed to address the limitations of 2PC, specifically the risk of blocking. 3PC introduces an additional phase between Prepare and Commit, known as the **Pre-Commit** phase. It sets an upper-bound on the amount of time required before a transaction either commits or aborts. Here are the [docs for it](https://tikv.org/deep-dive/distributed-transaction/distributed-algorithms/#:~:text=Unlike%20the%20two%2Dphase%20commit,transaction%20either%20commits%20or%20aborts.)

1. **Phase 1 (Can-Commit):** Coordinator asks all participants if they can commit. Participants respond "yes" or "no" without preparing resources
2. **Phase 2 (Pre-Commit):** If all participants said "yes", coordinator sends "preCommit" message to all of them. 
  1. Upon receiving the pre-commit, each participant prepares the resources, queries, and logs their intention to commit. They send an acknowledgment back to the coordinator.
  2. Each participant sets a timeout:
    1. If the coordinator crashes after sending the pre-commit, but before sending the final do-commit, participants may be left waiting. That's the issue we found with 2PC.
    2. However with this timeout, things are different. After the timeout, if a participant receives a pre-commit but not a do-commit/abort, it can safely reason that all others were ready to commit because that's what was required for pre-commit to even happen. As a result, it can decide to either commit or abort on its own.
3. **Phase 3 (Commit or Abort):** Coordinator sends a final "doCommit" or "abort" message. Then all participants execute the final decision. 

The timeout mechanism is what differentiates 3PC from 2PC as it allows participants to reach a decision even without the coordinator, avoiding indefinite blocking and improving fault tolerance.

#### 3PC Drawbacks and Considerations
Though 3PC isn't perfect. It makes some assumptions:
1. There are no network partitions
2. Bounded message delays, meaning messages take a predictable maximum amount of time to arrive (say 10 seconds).
3. And reliable timeouts meaning if a participant waits longer than the bound and hears nothing from the coordinator it can assume something went wrong.

But here's the thing, if a coordinator crashes that does handle the case. However if there's a network partition, the participant won't be able to hear from the coordinator, even though the coordinator could be alive and well on the other side of the partition. So the participant may not be able to communicate with the coordinator, it commits in its own, whilst the coordinator  (or other participants) may have aborted. This creates that data inconsistency. Of course, we're thinking about edge cases, but these things can happen, and you don't want that happening in a serious application, and it causes people to lose money. 

---
### SAGA Pattern
![](https://raw.githubusercontent.com/karanpratapsingh/portfolio/master/public/static/courses/system-design/chapter-II/distributed-transactions/sagas.png)
A saga is a sequence of local transactions, where each local transactions updates the database and publishes a message/event to trigger the next local transaction in the saga. If a local transaction fails, then the saga executes a series of compensating transactions that undo the changes that were made by the preceding local transactions. However there are two ways of coordination with saga:
- **Choreography:** Each local transaction publishes an event that triggers local transactions in other services.
- **Orchestration:** An orchestrator tels the participants what local transactions to executes.

Assume we're building an e-commerce store where customers have a credit limit. The app must ensure a new order doesn't exceed the customer's credit limit. Since Orders and Customers are in different databases owned by different services, the application cannot simply use a single local ACID transaction. 

#### Example: Choreography Based Saga
![](https://microservices.io/i/sagas/Create_Order_Saga.png)
1. The `Order Service` receives a `POST /orders` request, which creates an `Order` in the `PENDING` state.
2. Emit a an order created event, which the `Customer Service` receives.
3. The `Customer Service` does stuff like making sure the order doesn't exceed the customer's credit limit or something.
4. `Customer Service` sends back an event indicating the outcome and the `Order Service` either approves or rejects the order based on the event.

In this pattern, each service listens for and reacts to events from other services, managing its own part of the transaction. It's decentralized, decoupled, and services react to events and publish their own. There are drawbacks of course, as this can get hard to track if the number of steps grow. This works with for simpler workflows where there are fewer steps. Here's what the implementation could look like:

```Java

// In your Order Service
public class OrderService {
    public void createOrder(OrderRequest request) {
        // Step 1: Create and save order as pending and send an event 
        Order order = new Order(request.getCustomerId(), request.getAmount());
        order.setStatus("PENDING");
        orderRepository.save(order);
        // Emit event
        eventBus.publish(new OrderCreatedEvent(order));
    }

    public void handleCreditReserved(CreditReservedEvent event) {
        // Step 3: We have an event listener that accepts a CreditReservedEvent. This represents 
        // when the user has credit, so we'll approve the event. Notice how in this, it's very 
        // event-based, or messages based and asynchronous.
        Order order = orderRepository.findById(event.getOrderId());
        order.setStatus("APPROVED");
        orderRepository.save(order);
    }

    public void handleCreditLimitExceeded(CreditLimitExceededEvent event) {
        // Step 3: Have another event listener for the other scenario when the credit limit is exceeded.
        Order order = orderRepository.findById(event.getOrderId());
        order.setStatus("REJECTED");
        orderRepository.save(order);
    }
}

// Then in the Customer Service
public class CustomerService {
    public void handleOrderCreated(OrderCreatedEvent event) {
        // Step 2: Listen for that OrderCreatedEvent and publish a message 
        // based on whether the credit was reserved or the limit was exceeded.
        boolean approved = checkCredit(event.getCustomerId(), event.getAmount());
        if (approved) {
            eventBus.publish(new CreditReservedEvent(event.getOrderId()));
        } else {
            eventBus.publish(new CreditLimitExceededEvent(event.getOrderId()));
        }
    }

    private boolean checkCredit(String customerId, double amount) {
        // handles business logic for checking the credit
        Customer customer = customerRepository.findById(customerId);
        return customer.getCreditLimit() >= amount;
    }
}
```

#### Example: Orchestration Based Saga
![](https://microservices.io/i/sagas/Create_Order_Saga_Orchestration.png)
1. The `Order Service` receives `POST /orders` and create a saga orchestrator for that order, call it a `Create Order` saga orchestrator.
2. The saga orchestrator creates the `Order` in an `PENDING` state.
3. The orchestrator sends a `Reserve Credit` command/message to the `Customer Service`.
4. The `Customer Service` attempts to reserve credit and then sends back a reply message to indicate hte outcome.
5. After seeing the outcome, the saga orchestrator either approves or rejects the `Order`.

This is more centralized, as you have one orchestrator that's coordinating the saga. It's deciding the next steps, things are tightly coupled with the orchestrator. Of course there are pros as this is easier with complex flows and you can more easily trace things. One more thing before we actually look into the implementation. In the example we described above, we talk about working with events and responses. But there still is a distinction between "commands" and "events":
- **Command:** Placed in a specific queue like "reserve-credits-commands" and only one service (Customer Service) is expected to handle this.
- **Event:** Placed on a queue like "credit-events" and multiple services can listen or react to it (Order service, audit service, etc.)

```Java
/*
In your order service, you'll create an orchestrator for the Order Saga/Workflow.
Then this orchestrator is just here to actually list out the steps for the workflow. 

*/
public class CreateOrderSagaOrchestrator {

    private final MessageBus messageBus;
    private final OrderRepository orderRepository;
    public CreateOrderSagaOrchestrator(MessageBus messageBus, OrderRepository orderRepository) {
        this.messageBus = messageBus;
        this.orderRepository = orderRepository;
        // Register to listen for replies from Customer Service, listen for the "ReserveCreditResult"
        this.messageBus.registerHandler("ReserveCreditResult", this::handleReserveCreditResult);
    }

    public void createOrder(OrderRequest request) {
        // Step 1: Create pending order 
        Order order = new Order(request.getCustomerId(), request.getAmount());
        order.setStatus("PENDING");
        orderRepository.save(order);

        // Send command to customer service to check the reserve credit through a message
        ReserveCreditCommand cmd = new ReserveCreditCommand();
        cmd.orderId = order.getId();
        cmd.customerId = request.getCustomerId();
        cmd.amount = request.getAmount();

        messageBus.send("ReserveCreditCommand", cmd);
    }

    private void handleReserveCreditResult(Object message) {
        // Based on the ReserveCreditCredit from the customer service, update the order. 
        ReserveCreditResult result = (ReserveCreditResult) message;
        Order order = orderRepository.findById(result.orderId);

        if (result.success) {
            order.setStatus("APPROVED");
        } else {
            order.setStatus("REJECTED");
        }

        orderRepository.save(order);
    }
}


// Then in your Customer Service
public class CustomerService {

    private final MessageBus messageBus;
    private final CustomerRepository customerRepository;

    public CustomerService(MessageBus messageBus, CustomerRepository customerRepository) {
        this.messageBus = messageBus;
        this.customerRepository = customerRepository;

        // Register to handle credit reserve command
        this.messageBus.registerHandler("ReserveCreditCommand", this::handleReserveCreditCommand);
    }

    private void handleReserveCreditCommand(Object message) {
        // Based on the command, find the customer by extracting customer id in the Reserve credit command
        ReserveCreditCommand command = (ReserveCreditCommand) message;
        Customer customer = customerRepository.findById(command.customerId);
        boolean success = false;
        if (customer.getCreditLimit() >= command.amount) {
            customer.reserve(command.amount);
            customerRepository.save(customer);
            success = true;
        }

        // Send a message containing the result which will be listened to by anyone, but we're expecting 
        // the OrderService to get it and actually do stuff with it.
        ReserveCreditResult result = new ReserveCreditResult();
        result.orderId = command.orderId;
        result.success = success;
        messageBus.send("ReserveCreditResult", result);
    }
}
```

#### Comparing Orchestration, Saga, and other considerations
Choreography uses event-driven communication as services publish events ("Order Created", "Credit Reserved", etc.) and other services listen for the events they actually care about, and send back messages. However orchestration uses command-driven communication, sending commands like ("Reserve Credit", "Process Payment") and waits for replies.

However there's no automatic rollbacks, so we must create our own logic for undoing changes made earlier in the saga. There's also a lack of isolation, as hey we could be running sagas concurrently and cause data anomalies. For reliability, make sure your service updates its database and then publishes a message/event. Also the client that initiates the saga (places the order), which is an asynchronous workflow started by a synchronous request (e.g. HTTP `POST /orders`) needs to be able to determine the outcome of the saga. Here are the options for that:
- The service sends a response once the saga completes, e.g. once we receive in `OrderApproved` or `OrderRejected` event.
- The service sends back a response after initiating the saga and the client periodically polls (e.g. `GET /orders/{:orderID}`) to get the outcome. 
- The service sends a response after initiating the saga, and then sends an event (e.g. websocket, web hook, etc.) to the client once the sata completes.


### TLDR and takeaway
I mean we haven't even covered Paxos and whatnot. But of course the best way to understand this stuff is to actually build small applications that use these and whatnot. Also one more thing, it's kind of unfortunate that this write-up uses ideas like message brokers, publishing, events, etc. We haven't even covered that type of stuff yet, but trust that it's covered in the next chapter.

## Credits
- [Saga Explained](https://microservices.io/patterns/data/saga.html)
- [Raft Explained](https://raft.github.io/)
- [Paxos Explained](https://www.scylladb.com/glossary/paxos-consensus-algorithm/)
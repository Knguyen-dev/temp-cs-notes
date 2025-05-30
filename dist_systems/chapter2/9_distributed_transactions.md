# Distributed Transactions
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


#### Example Orchestration
```bash

# HTTP and more synchronous example
Order Service (Orchestrator):
1. Receive order request
2. Call Payment Service → Reserve payment
3. Call Inventory Service → Reserve items  
4. Call Shipping Service → Schedule delivery
5. Call Notification Service → Send confirmation

If any step fails:
- Order Service handles rollback
- Calls compensation actions in reverse order
- Updates order status to "failed"

# Somewhat more asynchronous example with message buses
Order Service coordinates but doesn't block:
1. Send "ReservePayment" command to Payment Service
2. Listen for "PaymentReserved" event
3. When received, send "ReserveInventory" command
4. Listen for "InventoryReserved" event
5. Continue the sequence...
```
The Order Service knows about all other services and directly invokes them. In sequence it'll go through everything. This can be synchronous (HTTP) or asynchronous (using message bus), the orchestrator will still wait for each step to complete before sending the next request or message.


#### Example Choreography
In choreogrpahy, services communicate through **events** without a central controller.
```
1. Order Service publishes "OrderCreated" event
2. Payment Service listens, processes payment, publishes "PaymentReserved" 
3. Inventory Service listens, reserves items, publishes "ItemsReserved"
4. Shipping Service listens, schedules delivery, publishes "ShippingScheduled"
5. Notification Service listens, sends confirmation

If Payment fails:
- Payment Service publishes "PaymentFailed" event
- Order Service listens and updates order to "failed"
- No inventory or shipping actions occur
```
Each service only knows about the events it cares about and publishes its own events when work completes. This is more loosely coupled, more asynchronous and event-driven in nature.


#### Comparing Orchestration, Saga, and other considerations
Choreography uses event-driven communication as services publish events and other services listen for the events they actually care about. Orchestration could be both, but in the end it's still technically synchronous in nature. It's tightly toupled as we have the logic for invoking all components in the orchehstrator.

However there's no automatic rollbacks, so we must create our own logic for undoing changes made earlier in the saga. There's also a lack of isolation, as hey we could be running sagas concurrently and cause data anomalies. For reliability, make sure your service updates its database and then publishes a message/event. Also the client that initiates the saga (places the order), which is an asynchronous workflow started by a synchronous request (e.g. HTTP `POST /orders`) needs to be able to determine the outcome of the saga. Here are the options for that:
- The service sends a response once the saga completes, e.g. once we receive in `OrderApproved` or `OrderRejected` event.
- The service sends back a response after initiating the saga and the client periodically polls (e.g. `GET /orders/{:orderID}`) to get the outcome. 
- The service sends a response after initiating the saga, and then sends an event (e.g. websocket, web hook, etc.) to the client once the sata completes.

---
### TLDR and takeaway
I mean we haven't even covered Paxos and whatnot. But of course the best way to understand this stuff is to actually build small applications that use these and whatnot. Also one more thing, it's kind of unfortunate that this write-up uses ideas like message brokers, publishing, events, etc. We haven't even covered that type of stuff yet, but trust that it's covered in the next chapter.

## Credits
- [Saga Explained](https://microservices.io/patterns/data/saga.html)
- [Raft Explained](https://raft.github.io/)
- [Paxos Explained](https://www.scylladb.com/glossary/paxos-consensus-algorithm/)
# OWASP: Authorization


### Least Privilege
Only assign users the minimum amount of privileges needed to do their job. If we don't, then the confidentality of sensitive resources is at risk. For every combination of user type and resource, what operations can the user perform on it?

### Access Control Techniques

#### Role Based Access Control (RBAC)
Access is granted or denied based on the roles assigned to a user. The permissions themselves e.g. "write" isn't directly assigned to an entity, but rather the permissions are associated with a role. If the entity has the role, they have the permissions for that role. It's simple, but not very fine grained.
| Role   | Permissions                         |
| ------ | ----------------------------------- |
| Doctor | View/edit patient records           |
| Nurse  | View (but not edit) patient records |
| Admin  | Add/remove users                    |

#### Attribute-Based Access Control (ABAC):
Access is granted based on the assigned attributes on the subject, object being accessed, or the environment. 

| Attribute Type | Examples                                |
| -------------- | --------------------------------------- |
| User           | clearance="TopSecret", department="CIA" |
| Resource       | classification="Secret"                 |
| Environment    | time < 6PM, location="HQ"               |

Then have policy: "Allow access if user's clearance >= documentation classification AND location is HQ and it's during working hours. 

- Agent Jane (TopSecret, CIA, HQ, 4pm) can access a Secret doc.
- Agent Bob (Secret, FBI, home, 10pm) cannot access a Secret doc.

ABAC is highly flexible but it can get quite complex. It's really good when you want to incorporate environmental and other dynamic attributes such as the time of day, type of device, etc.

#### Relationship-Based Access Control (ReBAC)
Here the access depends on the relationship between the entities (users, resources, and groups). So like "Only friends can view my private posts". Or in a collaborative platform like Google Docs "Only users explicitly shared on this doc can edit it". 

Relationships:
- Alice is a **friend** of Bob $\rightarrow$ can see Bob's posts.
- Alice is in the **"Editors" group** on a doc $\rightarrow$ can edit it.

More advanced examples:
- Alice is a manager of Bob $\rightarrow$ can approve his timesheets.
- A teacher is assigned to a class $\rightarrow$ can grade student submissions.

ReBAC is good for graph-style relationships and are necessary in social networking applications like Twitter or Facebook. Its implementation is based on graph theory:
- Nodes: users and resources (e.g. Docs, Teams, Projects).
- Edges: Relationships like owner, viewer, editor, etc.

Whilst RBAC is the most popular, in general you shoud prefer something like ABAC or ReBAC which will save you when your system grows in complexity.

### Protect Lookup IDS
Ideally you don't want to show the id of a resource. But of course we sometimes have to, so just make sure that a user can't tamper with the ID of a resource to somehow escalate their privileges. They shouldn't be able to access other people's posts just because they have the post ID!

### Extra stuff:
- Implement logging: Log accesses, whether they succeed or fail. 

## Credits
- [Authorization](https://cheatsheetseries.owasp.org/cheatsheets/Authorization_Cheat_Sheet.html)
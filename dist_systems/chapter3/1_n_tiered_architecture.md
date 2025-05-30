# N-Tier Architecture
A way of organizing your application into separate logical and physical layers. Each layer has a specific purpose/responsibility, with a higher layer being able to use the services in a lower layer but not the other way around.

![](https://raw.githubusercontent.com/karanpratapsingh/portfolio/master/public/static/courses/system-design/chapter-III/n-tier-architecture/n-tier-architecture.png)

In the diagram above, we have **tiers**, which are physically separated machines/deployments. Here each layer is hosted on its own tier, however that's not required. Physically separating things like this can improve the scalability and resiliency, but you will get latency and it's a lot more complexity. There are two types of styles:
- **Closed layer architecture:** A layer can only call the next layer immediately down. So here the web layer can only access the business layer, not the data layer. This is more common I think.
- **Open layer architecture:** A layer can call any layer below it. So now web can access business and data.

### 3-Tier Architecture
To speak more about the logical separation, you can think of this as a programming design pattern that reinforces separation of concerns, and the common common form of this is called **3-tier** architecture. It emphasizes that each layer has a single purpose, that there's a clear data flow, and more importantly that if you change can above layer you won't need to change the layer below it. Here are the three layers from top to bottom:
- **Presentation:** This is what the user sees or interacts with. It's here for displaying data, collecting user input. It doesn't contain your business logic or how your system processes data and stores stuff, it just renders info and passes any requests to the next layer. 
- **Service/Logic:** Contains the core logic of the application, it's where the business and application features are present. For example if you were creating a social media app, here you find logic for logging in, publishing posts, creating comments, messaging other users, liking posts, etc. This layer interacts with the database/persistence layer to make sure the user's actions are stored e.g. a user's post is stored in our system. This is commonly implemented as your API. Anyways notice that if you change your frontend, you're still going to have the same API endpoints, showing how changing an upper layer ideally shouldn't result in a change in a lower layer.
- **Data/Persistence:** Contains the logic for interacting with your data store. Extending on our social media example, here we'll have the logic for storing posts, comments, or even helper functions that query user accounts during the login process. Anything that directly interacts with the database goes here.

### Idea of Contracts/Interfaces, DTOs, and Java
Each layer should interact with the adjacent layers through well-defined interfaces, APIs in this case. Meaning you should define standards when implementing a piece of a layer. If you're defining user services, you should have defined interfaces that show what kind of data those interfaces accept and return. This promotes the idea that changes in one layer do not affect others as long as you follow the interfaces. Also it just keeps things clearer. 

If you're doing this in Java, you're probably going to hear about **DTOs (Data Transfer Objects)**. It's just the idea of having a simple object that holds data and is used to transfer it between layers, typically from the client to the backend or backend to the client. E.g. knowing the JSON object coming from an API, or the structure of the request body in a post request, or any type of data sent between. Just a good thing to know if you see people creating stuff like `LoginRequestDTO` or `LoginResponseDTO`.

In Java, the presentation layer is often implemented as **Controllers (e.g. `@RestController`)**, which define API endpoints. Even though API endpoints aren't the UI, they're still the entry point into the system, so sometimes we treat them like the presentation layer. Here's an example of everything packed together:
```Java
@RestController
@RequestMapping("/api/users")
public class UserController {

    // Presentation layer only uses the service/logic layer
    private final UserService userService;

    public UserController(UserService userService) {
        this.userService = userService;
    }

    // DTOs that structure what object we get when creating a user and what to resopnd
    @PostMapping
    public ResponseEntity<UserDTO> createUser(@RequestBody CreateUserDTO dto) {
        UserDTO createdUser = userService.createUser(dto);
        return ResponseEntity.ok(createdUser);
    }
}
```
Even though 3-tier architecture is about logical separation, in Java you’ll often see it reflected as a consistent folder/package structure, with `controller` (presentation), `service` (logic), and `repository` (data) packages, each working through DTOs and interfaces to keep things clean and decoupled.

## Credits
1. [Intro to N-Tier Architecture - Teddy Smith](https://www.youtube.com/watch?v=0jZvOGnVcJ0)
2. [More about N-Tier Architecture - AmigosCode](https://www.youtube.com/watch?v=xJC7ItRoEbw)
3. [DTOs vs Entities - Baeldung](https://www.baeldung.com/java-entity-vs-dto)
4. [Returning DTOs, domain models, or both? - StackOverflow](https://stackoverflow.com/questions/21554977/should-services-always-return-dtos-or-can-they-also-return-domain-models)
5. [Testing the service layer - Java Guides](https://www.javaguides.net/2022/03/spring-boot-unit-testing-service-layer.html)
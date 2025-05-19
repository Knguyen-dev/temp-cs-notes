

### How to get onboarded? (Practical Approach)
Honestly you won't have a lot of time. An organization is not going to allow your full time job to be a study hall where you study the codebase all day. If your plan is to learn about 60% or more of a large codebase quickly, it probably won't work. You're essentially making everything urgent. What they want is for you to be able to start contributing and working quickly. So here's a typical and smart approach:

1. **Talk to experienced person in charge:** Talk to the lead and learn about the SOPs (standard operating procedures) such as styling guides, documentation requirements, release procedures, etc. 
2. **Talk to an experienced developer:** Here you should ask them about where you can find the stuff that impacts your task and the scope of your work. Ask them to explain it. If you ever get stuck on something, ask the people, you are a new person, everyone gets it.
3. **Take advantage of developer documentation:** If you have questions on what you're looking at, then developer documentation is very useful. You may even want to look at the documentation first.

Having domain knowledge or previous work experience in that area helps as well, but this is the main thing you need to do. Taking notes is probably good as well. Then you would continue to expand your knowledge from there.

---
### Idealistic Approach:
Okay let's say you had more prep-time, and maybe the codebase is somewhat smaller and more manageable. So there's a good chance that you can learn most of it or at least get deeply familiar with a component.

#### Context
First, the general user context/background info. What does the software do? What is it solving and why are people using it? Analyze stuff like user journeys, how do they navigate and use the application, and how does that interaction work.

Second, I recommend learning the technologies used. What framework, programming languages, databases, are they using? If you're able to separate the main components out, what kind of packages and libraries are being used? What's the build tool, dependency manager, etc. In this stage, you're probably going to be in that file system, looking around, here are some tips:


Going around in your top level files, and figuring out some quick stuff.

Of course asking questions along the way is definitely going to be the best thing to complement this stage with.

---
#### System Architecture
Use a top down approach. What is the top-level architecture, what components are talking to each other? How does the data flow? Then go deeper, what routes are in this API, what data is accepted, what data is returned? How does the frontend handle its routes? How does authentication work, etc. So for web systems we need to locate:

- **Client applications:** Front end, mobile, desktop, etc. Identify you react apps, swift apps, etc.
- **Backend applications:** The APIs (aka services) that are communicating with our front-ends and each other. What types of APIs are they (RESTful, GraphQL), what purpose do they serve? Are they backed by a database and if so what type of database (SQL, NoSQL). 
- **Other app-related components:** Any other components? Any caches (Redis)? Any CDNs (AWS Cloudfront)? What external APIs are being spoken to (e.g. Firebase, AWS S3, Auth0, etc)? 
- **Deployment Process:** How is the application deployed? What technologies are used for that?
- **Containerization:** Are we running containers, if so that means deployment can be consistent across environments? Are we also using Kubernetes for production-level orchestration?
- **Automation and CI/CD:**
  - CI automates testing and building the app whenever the code is pushed. 
  - CD ensures that after successful builds, we deploy the code
  - This is usually handled by GitHub Actions, GitLab CI/CD, Jenkins, or CircleCI

 
Here we should also learn about how the app is deployed, what technology is used for that? 





#### Small tips
1. 
2. Pick a component of the program you think you would best understand. Such as the backend, the database, the authentication system, the frontend, a specific page on the frontend etc. Learn a specific thing and expand from there. If you get stuck on a particular component/step, treat it like a black box and move on. Don't get stuck on a component, the main idea is getting familiar with most of it.



- What libraries or packages are being used?
- Start at the entrypoints. A main program, an `__init__.py` or some place where your routes are defined in the application, or something similar. 
can help each other understand what's happening. Speeds up analysis.
- Draw diagrams of the databases, architectural design, data flow.
- How does a user navigate the site and use it? What's the user journey?


https://www.reddit.com/r/learnprogramming/comments/89pjdy/how_to_learn_a_new_codebase_quickly/
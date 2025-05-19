# GitHub Actions

### What is CI/CD?
- **Continuous Integration (CI):** Idea of frequently integrating code changes into a shared repo. Then automated tests are run to ensure that the new code works with existing codebase. So this should happen when you try to add your code to the main branch of your repo, through commits or merges. Immediately the 'CI server' it'll start a 'build' to see if your software is good, passing it if no tests fail. So commit -> repo -> CI Server -> Developers
  - **Benefits:** This allows us to automate the detection nof bugs, reduced time in finding integration issues such tests are run, and gives us confident that the software we're running is always in a working state.
- **Continuous Delivery (CD):** Ensures that the codebase is always in a deployable state. This automates the release process so that after changes are applied, those changes are ready for us to deploy on the production software. Note that deployment may still require manual approval.
  - **Benefits:** The changes that pass the CI phase are prepared to be deployed. However actual deployment to production is a manual step. This makes our releases more reliable, and reduces the risk of deployment issues.
  - **What about Continuous Deployment?:** The idea of 'Continuous Deployment' makes it so that once changes are applied from the CI phase, the application is automatically deployed to production! This is a lot faster, but of course please be careful.
- **CI/CD Pipeline:** A process that automates and organizes the integration, testing, and deployment stages. CI/CD helps teams deliver software faster and with higher quality through automation and continuous feedback loops. We'd be able to release new features and fixes more frequently, and it allows us to quickly hear from users and stakeholders.

---
### How does GitHub Actions come into play?
Github Actions is a CI/CD platform integrated into Github. It allows us to integrate, automatically test, and deploy or deliver your code based on events such as pushes, pulls, requests, or even schedule-based triggers.

- **Workflow:** An automated process that you define in your Github repository. It consists of one or more jobs that run in response to events like pushing code, creating pull requests, or manually triggering a workflow. 
- **Triggers:** Events that start a workflow 
- **Jobs:** Series of steps that run on Github-hosted runners, or self-hosted runners. Each job does something like building your code, running tests, and deploying.
- **Steps:** Individual actions within a job. Such as installing dependencies, running tests, deploying to server, etc.
- **Runner:** The environment where your workflows are executed. Github provides cloud-hosted runners that are already pre-configured for Linux, Windows, etc. Of course you can use your own.

### How does it work? How does this benefit me?
Create a directory `.github.workflows/myCIFile.yml` and put some `.yml` files in there, each yml file being a workflow.





#### Example: 1_example.yml and 2_action.yml
- Every job must have the `runs-on` property
- An action, such as `.github/workflows/myAction.yml` can be thought of as a reusable piece of code. Actions and popular workflows are all over the market place.
- You can also restrict what kinds of actions that can be added. That's the more administrative side of things though.

- You should know when we're running things, by default all jobs are going to run in parallel. However since 




## Credits

https://docs.github.com/en/actions/writing-workflows/quickstart

https://docs.github.com/en/actions/writing-workflows/choosing-what-your-workflow-does/caching-dependencies-to-speed-up-workflows


# Continuous Integration, Continuous Deployment
A practice where developers frequently merge code changes in a central repo. Then the repo will automatically build the project, check if everything works, run some test. Then CD could mean continuous delivery or deployment.

## Introduction

### Continuous Integration
Developers who practice CI will merge their changes back to the main branch as often as possble. Then their changes are validated by some tools automatically building the project and running tests against the project. If everything goes well the changes can be applied and we avoid issues when merging to main. If something breaks, the changes won't be integrated into the main branch.  There's a lot of emphasis on tests being run on our changes and making sure that everytime a developer merges into main, they're leaving the application in a good state. In general the idea is to avoid a lot of merge conflicts, or everyone waiting until the end to merge their commits and realizing that there's a lot of merge conflicts as a result of that. Also it's the fact that with CI, you're expected to write your own tests and so having a good test suite is going to be a big deal.

There are five steps to setup CI:
1. Start writing tests for the critical parts of your codebase. Start small with your tests and then you can increase the difficulty or complexity.
2. Use a CI service to run those tests automatically on every push to the main repo.
3. Make sure that your team integrates frequently. 
4. Fix the build if it breaks.
5. Write tests for every new story that you implement.

When picking a CI tool you'll want features:
- Version control management
- Automated testing 
- Automated builds 
- Automated Deployments


### Continuous Delivery
It's the idea that we're going to automatically deploy the code changes into a testing, or even a production environment after a successful build. So after the continuous integration build step goes well, we'll deploy the changes. Of course it's not always automatic deployment, you typically have the final say on whether to deploy the current build, or maybe your team will wait a bit for more commits so that your feature is a little more complete when deploying.

### Continuous Deployment
Similar to continuous delivery, but the deployment is fully automatic, there's no need for human intervention to check if that's the build you want. It's a little faster as now there's no need for a release day, but I guess a little riskier.

### Last Considerations
- **CI/CD Pipeline:** An automated sequence of steps that tkae code from commit to production. Such as Build → Test → Package → Deploy. Each stage can include multiple tasks (e.g. static code analysis, security scans, integration tests).
- **Environmental Separation:** CI/CD usually involves multiple environments such as development, staging, and production. Continuous Delivery typically deploys to staging, whilst continuous deploymnet pushes straight to production.
- **Rollback Mechanisms:** Rollback strategies (e.g. versioned deployments, feature toggles, etc.) are critical.
- **Monitoring and Observability:** You should have logging, metrics, and alerts. Tools like Prometheus, Grafana, Sentry, and others help with this and help you catch issues early into production.
- **Security Checks:** CI/CD include oftens security scans like dependency vulnerability checks, static code analysis, secret detection. 
- **Challenges and Trade-offs:** Not everything is easy to test (e.g. UI behavior). Also long running tests can slow down the pipeline. Strike a balance between speed and confidence.

Note: CI/CD is one of those things where you learn by doing. It's a lot of practice.
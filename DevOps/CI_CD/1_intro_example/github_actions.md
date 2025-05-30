# GitHub Actions: Introduction

### What is GitHub Actions? Why is it important?
Platform for automating developer workflows. Creating CI/CD Pipelines are just one of those workflows. Let's say you're running an open source project. When a new contributor joins that project you manually give them the links to the documentation, you also give them some videos to watch, etc. That's one workflow. Another workflow could be when someone creates a new issue. You want to be able to sort this issue, assign it someone, see if the issue is reproducible, etc. When pushing changes, you want to make sure to run tests on the code, you want to build it, and in general get ready for deployment. That's another workflow. As a developer, you don't want to manually do these workflows. Imagine that everytime a new contributor is added, you have eto debrief them manually, or if a new issue is added it's not properly assigned, labeled, etc. We want to automate as muhc of these workflows as possible, to maximize time working on features and fixing software issues. GitHub Actions is a platform that allows us to automate these workflows.

Everytime something happens IN or TO your repository, you can have automatic actions/steps that are executed in response. Things that happen in or to your repo are called **events** or **triggers**. Examples of events are things like a PR being created, a contributor joining, an issue being created, etc. So the flow would look like:
1. Listen to some event. This could be an issue being created.
2. Trigger a workflow that's executed in response to that event. This could be sorting the issue, labeling it, assigning to to a contributor, etc.

Not everyone has an open source repo so you probably don't have that many workflows. The most common workflow that you'll use is a CI/CD Pipeline. Here the event is could be when someone pushes to a specific branch like `main` or `dev`. Then we'll run the steps for testing, building, packaging, and then deploying. Before continuing let's talk about what those terms actually mean. GitHub actions makes things a little easier to integrate with other technologies.

- node app, build docker image, push to nexus repo, deploy to digital ocean
- java app with maven, integration tests with linux and windows, build odcker image, push to aws repo, deploy to aws eks

Who cares about installing any of these tools and configuring all them to work with each other. I just want to be able to specify a node and docker with certain versions and connect to a target and deploy.

#### Explaining various stages
1. **Source:** The trigger of the event that starts the workflow, which is our CI/CD pipeline in this case. So this will be a push, pull request, or even a manual trigger.
2. **Build:** Transform the source code into an executable or just its ready to run format. Like making the build files.
3. **Test:** Run some automated tests on the code. This would be unit and integration tests, but also linting, static code checks, secret detection, and other security scanning stuff.
4. **Package:** Bundle the build output into a "deployable artifact". This could be a `.jar`, `.zip`, a Docker image, or someother deployable file. In any case, you'd add metadata such as the version of your software, timestamp, etc.
  1. **Artifact Store (Optional But Common):** It's common to store the packaged result in some kind of artifact repository. For example, you'd upload that docker image to Docker Hub. This lets you and others keep track of the latest releases of your software. You'd also tag/version the artifact.
5. **Deploy:** Make the application available in an enivronment such as staging, QA, or production.
  1. **Post-deployment:** Have health checks, monitoring, and rollback if deployment fails.


### Using GitHub Actions
When you create a new repository, and go to the actions tab, you're going to see all the already setup workflows for common tech setups. You'll see a lot of types of workflows:
- Deployment: For deploying apps to different places.
- Continuous Integration: Such as building a docker image, running it, pushing it to DockerHub. Other one sis just building the project, and testing the project. 
- Security: a lot of it is static code analysis of vulnerabilities.
- Automation: Labling pull requests based on the files changed, checking stale issues and PRs, greeting first time contributors, etc.

There are a lot of different workflows that you can use for different situations. So people created these pre-defined workflows with all the boilerplate to help you out, and of course you can still use them but make little tweaks to them. In any case, to setup github actions you need to create the path `/.github/workflows/my_workflow.yml`. This is done whether you use a pre-built workflow or if you create your own. Workflows on GitHub Actions get executed on GitHub servers. A given job will run in a GitHub Actions Runner, and the other job will run on another runner. The main point is that by default, your jobs will run in parallel with each other. However there will be times when youwant one job to start after another has already finished. 

### What is GitLab CI/CD
Another tool for CI/CD that offers similar features to GitHub Actions. It's more indepth and has a lot more features than GitHub Actions. GitLab is the thing you'd use for enterprise level or decently complex CI/CD processes. You'd use GitHub actions for somewhat simple CI/CD processes that do a decent job and get things done it seems. 

### GitHub Packages and GHCR
GitHub Packages is a software package hosting service that allows you to host software packages privately or publicy. So here you're able to maintain things such as containers and other dependencies. It's literally your source code and packages all in one place. So it stores stuff like npm packages, Maven stuff, etc. Within GitHub Packages, we have **GHCR (GitHub Container Registry)**. GHCR is a part of GitHub Packages, but it's specifically for hosting Docker and OCI container images. This service is specifically for holding container images. So it's like Docker Hub, but for GitHub. So while it isn't a server that runs your app and hosts it, it stores your images and now those images are availalbe to be pulled from anywhere.

## Credits
- [GitHub Actions Basics - TechWorld with Nana](https://youtu.be/R8_veQiYBjI?si=zxsfLO_Lsv4XJwEI)

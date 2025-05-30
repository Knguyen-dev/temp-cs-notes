# Publishing Container Images
A container registry is a repo or platform where we can store the images for our docker containers, or whatever images we generate. GitHub has a builtin container registry known as GitHub Container Registry (GHCR). It offers a convenient way to publish Docker images directly from your GitHub repository. Then these docker images are stored directly within their GitHub repositories.



### Introduction: How it works 

Here's what you need:
- GitHub Account
- A Dockerfile in your project
- A GitHub repo where this is installed

Enable GHCR for your account or organization. No explicit setup should be needed here. Then you can view pushed packages at `https://github.com/OWNER`. Extra stuff you need to know:
- The container registry stores container images within your org or personal account. You can associate an image with a repo.
- You need an access token to interact with things. GitHub Packages only supports auth. with personal access tokens (classic). When authenticating, you can use the `GITHUB_TOKEN` to publish packages.

#### Working with docker commands
```bash
docker push ghcr.io/NAMESPACE/IMAGE_NAME:latest
docker pull ghcr.io/NAMESPACE/IMAGE_NAME
```
Where namespace is the name of the personal account (like your username) or organization





### Example 1: Yaml file to push to Docker Hub only
```YAML
name: Publish Docker image

on:
  release:
    types: [published]

jobs:
  # Makes sense that it's only a single job
  push_to_registry:
    name: Push Docker image to Docker Hub
    runs-on: ubuntu-latest
    permissions:
      packages: write
      contents: read
      attestations: write
      id-token: write
    steps:

      # Obviously you need to checkout your repo and login to docker hub so that it's 
      # authorized to push
      - name: Check out the repo
        uses: actions/checkout@v4

      - name: Log in to Docker Hub
        uses: docker/login-action@f4ef78c080cd8ba55a85445d5b36e214a81df20a
        with:
          username: ${{ secrets.DOCKER_USERNAME }}
          password: ${{ secrets.DOCKER_PASSWORD }}

      # Log into GHCR
      - name: Log in to the Container registry
        uses: docker/login-action@65b78e6e13532edd9afa3aa52ac7964289d1a9c1
        with:
          registry: ghcr.io
          username: ${{ github.actor }}
          password: ${{ secrets.GITHUB_TOKEN }}

      - name: Extract metadata (tags, labels) for Docker
        id: meta
        uses: docker/metadata-action@9ec57ed1fcdbf14dcef7dfbe97b2010124a938b7
        with:
          images: my-docker-hub-namespace/my-docker-hub-repository

      # Here you actually build and push the docker image
      # But of course, since you've logged into both GHCR and Docker Hub, you're pushing to bohh
      - name: Build and push Docker image
        id: push
        # Probably replace this with whatever version is currently latest
        uses: docker/build-push-action@3b5e8027fcad23fda98b2e3ac259d8d67585f671
        with:
          # Location and name of the docker file
          context: .
          file: ./Dockerfile 

          # Indicates that you actually want ot push to docker hub
          push: true
          tags: ${{ steps.meta.outputs.tags }}
          labels: ${{ steps.meta.outputs.labels }}

      # Generates an artifact attestation for the image. This just increases the security of stuff.
      # probably entirely optional for simpler projects.
      - name: Generate artifact attestation
        uses: actions/attest-build-provenance@v2
        with:
          subject-name: index.docker.io/my-docker-hub-namespace/my-docker-hub-repository
          subject-digest: ${{ steps.push.outputs.digest }}
          push-to-registry: true
```

### Example 2: Pushing to both Docker Hub and GHCR
```YAML

name: Publish Docker image


# Obviously you can change this to pushing to main
on:
  release:
    types: [published]

jobs:
  push_to_registries:
    name: Push Docker image to multiple registries
    runs-on: ubuntu-latest
    permissions:
      packages: write
      contents: read
      attestations: write
      id-token: write
    steps:
      - name: Check out the repo
        uses: actions/checkout@v4

      - name: Log in to Docker Hub
        uses: docker/login-action@f4ef78c080cd8ba55a85445d5b36e214a81df20a
        with:
          username: ${{ secrets.DOCKER_USERNAME }}
          password: ${{ secrets.DOCKER_PASSWORD }}

      - name: Log in to the Container registry
        uses: docker/login-action@65b78e6e13532edd9afa3aa52ac7964289d1a9c1
        with:
          registry: ghcr.io
          username: ${{ github.actor }}
          password: ${{ secrets.GITHUB_TOKEN }}

      - name: Extract metadata (tags, labels) for Docker
        id: meta
        uses: docker/metadata-action@9ec57ed1fcdbf14dcef7dfbe97b2010124a938b7
        with:
          images: |
            my-docker-hub-namespace/my-docker-hub-repository
            ghcr.io/${{ github.repository }}

      - name: Build and push Docker images
        id: push
        uses: docker/build-push-action@3b5e8027fcad23fda98b2e3ac259d8d67585f671
        with:
          context: .
          push: true
          tags: ${{ steps.meta.outputs.tags }}
          labels: ${{ steps.meta.outputs.labels }}

      - name: Generate artifact attestation
        uses: actions/attest-build-provenance@v2
        with:
          subject-name: ghcr.io/${{ github.repository }}
          subject-digest: ${{ steps.push.outputs.digest }}
          push-to-registry: true
```

### Example 3: Simple Yaml to just push to GHCR
```Yaml
name: Publish Docker image to GHCR

on:
  push:
    branches: [main]  # or any other branch

jobs:
  build-and-push:
    runs-on: ubuntu-latest

    permissions:
      contents: read
      packages: write  # Required to push to GHCR

    steps:
      - name: Checkout repository
        uses: actions/checkout@v3

      - name: Log in to GitHub Container Registry
        uses: docker/login-action@v3
        with:
          registry: ghcr.io
          username: ${{ github.actor }}
          password: ${{ secrets.GITHUB_TOKEN }}

      - name: Build and push Docker image
        uses: docker/build-push-action@v5
        with:
          context: .
          push: true
          tags: ghcr.io/${{ github.repository_owner }}/my-app:latest
```

## Credits
- [Publishing Docker Images (CI/CD) - GitHub Docs](https://docs.github.com/en/actions/use-cases-and-examples/publishing-packages/publishing-docker-images)
- [Publishing Docker Images to GitHub Packages](https://docs.github.com/en/actions/use-cases-and-examples/publishing-packages/publishing-docker-images#publishing-images-to-github-packages)
# Orphans in Docker

### Orphaned Container
Refers to a container that's still running or exists even after its parent service has been modified or removed. This often happens when:
- A service is removed or renamed in `docker-compose.yml`, but the old container is not deleted. Running `docker-compose up` again creates a new container whilst the old one remains.
- A container isn't cleaned up after stopping or restarting a service, leading to old unused containers lingering.
- Docker compose doesn't automatically remove containesr from services that no loner exist in the configuration when updates are made. 

I mean `docker-compose down` will stop and remove containers created by `docker-compose up`. However orphaned containers can still happen.
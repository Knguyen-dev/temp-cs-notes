# Ivanti Secure

### Motivation
Essentially if you try to connect to the MySQL database on your local machine, you're going to get an error saying something along the lines of "Service Not Found". The reason being is that the MySQL database is located within IU's internal intranet, rather than the broader internet, so if you look it up, it's not going to appear. Solve this by connecting to IU's VPN. After connecting to IU's VPN, you will be able to access the js2 MySQL database. 

### Setup
1. Download and open the client
2. Create new connection using this server url: `vpn.iu.edu`
3. Enter credentials and do duo mobile. They'll ask you for your "secondary password" but just enter in the same IU password you have.
4. Verify connection with database `ping sasrdsmp01.uits.iu.edu`


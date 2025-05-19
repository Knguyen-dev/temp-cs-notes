# This is going to involve the same thing as before. Inside our container, we're going to have a /app directory where 
# all the project source code is and where all the dependencies are. As well as this, we'll run this application on port 3000 inside the container.
FROM node:17-alpine
WORKDIR /app
COPY package.json .
RUN npm install
COPY . .
CMD ["npm","run","dev"]
EXPOSE 3000
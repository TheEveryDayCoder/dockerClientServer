# clientServer
Client Server socket for EE450


1) create a Dockerfile ... might need to make seperate repos for all of this
2) got "Got permission denied while trying to connect to the Docker daemon socket at unix" -->
   *1 try if docker ps gives off the same error if it does then I need to add the user to the docker group
   using the following command:
         (maybe if group does not exist) --> sudo groupadd docker
         
         sudo usermod -aG docker ${USER}
         newgrp docker
         reboot
         docker ps

Inside docker file
        RUN: commands are commands that run inside the image are for an immage build step (i.e. stacking layers on top of each image) 
        CMD: the command the conatiner will execute  will run on docker run

docker build does not call CMD ( looks like it just verifies its working)
* Greate commands:
  docker image ls
  docker image rm "cotainier name or id "
  docker build -t "tag name " <location of Dockerfile>



         

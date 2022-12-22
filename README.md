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
Status: 
 Need to figure out what is going on... all the ports seem to right and connected and goes to serverM but not into server A... still a port issue not sure where to... need to understand the docker network more maybe... not sure if i need to specify 0.0.0.0 or /udp or /tcp.... still in work mkaing progress.
 figured it out! use --network="host" This will map all ports to localhost no need for port forwarding... becareful when port forwarding there are some tricky things but it seems that --network="host" works well.
 
Inside docker file
        RUN: commands are commands that run inside the image are for an immage build step (i.e. stacking layers on top of each image) 
        CMD: the command the conatiner will execute  will run on docker run

docker build does not call CMD ( looks like it just verifies its working)
* Greate commands:
  docker image ls
  docker image rm "cotainier name or id "
  docker build -t "tag name " <location of Dockerfile>



         

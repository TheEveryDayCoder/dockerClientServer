This code is the EE450 Project.
-Gustavo Salazar

This code block consists of a handful of files:
    -clientA.c ,clientA.h
    -clientB.c, clientB.h
    -serverM.c, serverM.h
    -serverA.c, serverA.h
    -serverB.c, serverB.c
    -serverC.c, serverC.c
    -socketHelper.c, socketHelper.h

A brief discription of each:

clients files consit of command feature as per the project description.
There exist a way for a user within the block files to to check there current ammount (or wallet). There exist a way for generating transactions for user to user if the user exists in the block chain. Lastly, there exist a command to constuct a sorted list through the following command "./clientX TXLIST." Where X is either A or B. Constructing the exist will appear "alicoin.txt." This is true for both A and B clients. In addition, these file call some helper functions located in socketHelper that are used accross all the files.

The main server connects through udp for back end servers and udp servers. The back end servers job duty is to read in the text file and send all of its conents through a udp port, after requested from the main server, where it is than parsed in the main server. In addition, the main server will take its contents and fgure out of a transaction is needed or was just checking the wallet either way main server does the heavy lifting in this program. The main server will disscussed in greater detail below.

The socket Helper file is just a file that stores more helper functions to reduce copy and paste of the same code across multiple locations. The purpose is to just to have easy access to functions.


Potentail Bugs:
 When testing the program there seemed to be no orginal problems except for waiting a slight time between shutting down and starting up servers between each excuttion this might happen while testing. I found that waiting for a couple seconds between each interval allows all sockets and process to appear.

I have performed a series of text never, the less the was not perform with such a large data set. Thus I am unsure that this will work for huge text file. I tried by best to increase the data that could be sent accross but it could be more than I tested. Thus there might be a loss of data or text might be truncated.

Next, to generate the textfile alicoin.txt, it was under the assumpition that nomore than 1000 lines (per piazzaa) will be contained in the blockfiles each.

lastly, the client names will not appear. currently there was a bug when trying to find the client name


Detail description of ServerM:

Server M does most of the work and thus requires greater detail to describe its work. I will break down each case serverM handles in the clock.

       -Wallet Transaction:
               The wallet starts from the client. the client will appened the phrase " WALLET" to message. This will allow the main server to use the function "containsWallet" to see what task to perform. In this case when the key word wallet is in the phrase serverM knows this will be a Wallet task. The server M has been implemented with a Poll and is constant listening to either client A or client B for information. Once the message is recived from the client serverM will than send it to its "perform wallet task" where its is asking all backend servers to send over its information (i.e. the text files content). The main server will parse all these files and see if the user name exist in any one of these files. If it does not exist than it will send a tcp message to the client saying the approaite message on clients window. If the username does exist it will add or subtract up all transactions with that name and call and send the correct information to the client.
               
       - Transaction:
               The transaction portion of this code does simialr steps done in wallet in that it asks the back end servers to send its information over. Once its sent over the code will than add if possible to reciving user and create the transaction statement. Once this statement exist it will send this statement to a random server where the statement will be uploaded to that server's text file. Next, the appropriate information was sent out to its client
       - Sorted List:
                Firstly, serverM will confirm that the message from client contained "TXLIST." Once confirmed the server will grab all the information from from the following files and sort them based on serial id to write to the "alicoin.txt" It is important to note that this might not work for case larger than size of the array. 
        

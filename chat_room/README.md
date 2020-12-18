# Chat-Room-server modified from https://www.geeksforgeeks.org/simple-chat-room-using-python/
## Steps:
* open terminal and run `git clone https://github.com/shengchaohu/shengchao.git`
* go to the shengchao/chat_room
## Example use case:
### first we start the server
on server side, run `python3 chat_server.py YourIPAddress SomePort`
### then clients can run the code to connect to server
on client side, run `python3 client.py YourIPAddress SomePort`
## Advantage:
* Easy to add logic and new games by writing code to deal with recieved messages as you want
* Only some basic python code knowledge required as the network connection framework is already written
* No reliance on third party chat application apis such as WeChat's
* Safe and private
## Disadvantage:
* Only work on local area network, such as for computers connected to the same home router or in the same classroom
* User has to have a computer and python installed to run the script

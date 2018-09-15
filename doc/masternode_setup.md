# Anon Masternode setup guide

> In this guide we will install an ANON masternode through the terminal. 
Let's get started!

**Disclaimer, you will get the best results if you do this in one sitting. Be cautious and always triple check the addresses and private keys you are working with.**

## Contents
-	[Initial node setup](#1-initial-node-setup)
-	[Masternode Preperation](#2-masternode-preperation)
-	[Get a VPS](#3-get-a-vps)
-	[Configure your masternode](#4-configure-your-masternode)
-	[Masternode Config file](#5-masternode-config-file)
- [VPS Config File](#6-vps-config-file)
- [Final Steps](#7-final-steps)
-	[Troubleshooting](#8-troubleshooting)
-	[Security](#9-security)
-	[Questions](#10-questions)


## 1. Initial node setup

1.	Download the [latest version of ANON](https://github.com/anonymousbitcoin/anon).

2.	Navigate to the green button 'clone or download' 

3.	Copy the link and follow the instructions detailed in the readme to build for your local system.

4.	Start the node with the command `./src/anond`

## 2. Masternode Preperation

1.	Generate a private key for your masternode with the command `./src/anon-cli masternode genkey` - copy the generated and store it in a text document for future use.

2.	Get an account address to receive payouts with the `./src/anon-cli getaccountaddress 0`. Copy 
the address in preperation to send it the collateral funds.

3.	Send EXACTLY 500 ANON coins to the address you received.
Note that this has to be sent in **ONE transaction**. 

4.	Get the transactions output by running the command `./src/anon-cli masternode outputs`. You will see the transaction and a number next to it, either `0` or `1`.

5.	Copy the transaction id and output id
Save the output in the text file for future use.

At this point you should have the following informaiton:

-	Masternode Private Key
-	Your indexed account address
-	Transaction received when sending 500 ANON to above address
-	The output id for the transaction

## 3. Get a VPS 
We recommend renting a VPS with www.vultr.com because they are fast and cheap.

1.	Create an account:<br>
![Create account](https://i.imgur.com/ingiJYX.png)<br>

2.	Deploy a new server

3.	Choose a location close to you to have a fast connection

4.	Choose Ubuntu 16.04  x64 as operating system and ensure you take a VPS with a minimum of 2 GB memory.

5.	Move to step 7 and give your masternode VPS a name.<br>
![Masternode name](https://i.imgur.com/QvBBvew.png)<br>

6.	Click "Deploy now"<br>
The server is now being started. Please wait until the status is "Available".

7.	Click the server name and copy the IP-address and password via the copy button.
![Masternode server information](https://i.imgur.com/5wJEIB1.png)<br>
Save them in the text file for future use.

## 4. Configure your masternode

Depending upon which operating system you are using follow the correct section:

### Windows - [PuTTY](https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html)

#### Install PuTTY and run it.
*	You will be greeted with the following page:<br>
![Imgur](https://i.imgur.com/X1k9vXi.png)<br>
*	Go to Connection and adjust the session timer to 30. This is to ensure that you don't get kicked from the server.<br>
![image](https://github.com/zaemliss/installers/raw/master/timeout.png)<br>
*	Go back to Session.
*	Fill the Host name field with the IP address you previously copied and click "Open".<br>
*	You will see a popup asking you if you trust this host. Choose Yes!( this will only be asked once ).
![Imgur](https://i.imgur.com/Y2iEDj8.png)<br>
*	Login as "root". Hit ENTER
*	Copy the password you saved previously and right click in the putty terminal.  ENTER.
*	You are now logged into your server:

### Mac/Linux - Terminal ( preinstalled )
*	You can find Terminal by following the steps: 
*	Go to Finder, Applications then click on utilities, then you'll find the terminal there.
*	Type: ssh root@YourMasternodeIPaddress.  ENTER.
*	You are now logged into your server.

### General steps

Let's update our system to the latest version to make sure we are secure.
*	Type: `sudo apt-get update`  ENTER
*	Wait until this finishes
*	Type: `sudo apt-get upgrade` ENTER
*	Type "y" if the system ask for the confirmation of updating the system.

Now you can refer back to the [Initial node setup](#1-initial-node-setup) to install the full node.

## 5. Masternode config file

1.	Navigate to the folder where you stored you `anon.conf` on your local machine.

2.	Open the masternode.conf file. It should contain the following:

```
# Masternode config file
# Format: alias IP:port masternodeprivkey collateral_output_txid collateral_output_index
# Example: MN01 127.0.0.2:51474 93HaYBVUCYjEMeeH1Y4sBGLALQZE1Yc1K64xiqgX37tGBDQL8Xg 2bcd3c84c84f87eaa86e4e56834c92927a07f9e18718810b92e0d0324456a67c 0
```

3.	Add your own real working node details under it. <br />

4.	Give your masternode an alias.

5.	Put the server IP address ( your vultr ip or other vps/vm ip) followed by the port 33130 <br />

6.	Put the private key generated in step 2.1 <br />

7.	Put the transaction hash and output id from step 2.5 <br />

Example below

```
MN01 124.842.07.0:6250 119cCx5YeA519YkTzun4EptdexAo3RvQXaPdkP 838328ce57cc8b168d932d138001781b77b22470c05cf2235a3284093cb0019db 0
```

8.	Once complete, save the file <br />

The file will look like this:
```
# Masternode config file
# Format: alias IP:port masternodeprivkey collateral_output_txid collateral_output_index
# Example: mn1 127.0.0.2:51474 93HaYBVUCYjEMeeH1Y4sBGLALQZE1Yc1K64xiqgX37tGBDQL8Xg 2bcd3c84c84f87eaa86e4e56834c92927a07f9e18718810b92e0d0324456a67c 0
MN01 124.842.07.0:6250 119cCx5YeA519YkTzun4EptdexAo3RvQXaPdkP 838328ce57cc8b168d932d138001781b77b22470c05cf2235a3284093cb0019db 0
```

9. Restart the node<br>

## 6. VPS Config File

1. On your vps machine, navigate to your `/.anon/` folder where your `anon.conf` is stored.

2. Open the file and add the following lines:

```
masternode=1
masternodeprivkey=[masternode genkey from 2.1]
externalip=this.nodes.ip.address
```

3. Save the file and restart your node.

## 7. Final Steps

1. Start your masternode by changing into the anon folder and entering `./src/anon-cli masternode start-all`

1. Verify the masternode is running on the VPS by running `./anon-cli masternode status`  ENTER

2. The out put should state "Masternode successfully started"

3. Setup SENTINEL for your masternode. Follow instructions in this repo: https://github.com/anonymousbitcoin/sentinel
<br>
If you followed these steps correctly your ANON masternode should be running right now!<br>
If the masternode has not started, restart the mnsync by issuing the command: `./src/anon-cli mnsync reset` and check the status of the sync with `./src/anon-cli mnsync status` you should look for the asset id to be 999 or MASTERNODE_SYNC_FINISHED.

After some time the rewards will be distributed in your wallet.

## 8. Troubleshooting?
Take a look at our FAQ pages [TODO]

## 9. Security

### Wallet

Encrypt your wallet! This prevents other people ( who have access to your computer or get access to your wallet.dat file ) to get in your wallet. Don't lose that password. If you lose it the wallet is locked forever and nobobdy will be able to recover your funds.

Backup your wallet! The beauty of digital files is that you can back them up and store them somewhere safe. After encrypting your wallet make sure you back it up and keep it somewhere safe ( on a usb for example).

## VPS

For the more advanced user I advise making your VPS more secure from all kinds of attacks. We don't want other people stealing our masternode right?
Please look at this guide http://patheyman.com/masternode-secure/ for more info.

## 10. Questions?

If you have a problem or a question you can find us in the #support channel on our Discord.

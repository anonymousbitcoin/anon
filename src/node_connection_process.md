Regular Node Connections
First regular node will try to connect,
    threadGroup.create_thread(boost::bind(&TraceThread<void (*)()>, "opencon", &ThreadOpenConnections));
Then they will run ThreadOpenConnections


Masternode Connections
Thread 16 "anon-masternode" hit Breakpoint 1, ConnectNode (addrConnect=..., pszDest=pszDest@entry=0x0,
    fConnectToMasternode=fConnectToMasternode@entry=true) at net.cpp:367

    MUST CHECK THIS
    CActiveMasternode::ManageState
    CActiveMasternode::GetTypeString

    1       breakpoint     keep y   0x00005555557aa722 in CActiveMasternode::ManageStateInitial() at activemasternode.cpp:131
    3       breakpoint     keep y   0x00005555557abfe8 in CActiveMasternode::ManageState() at activemasternode.cpp:17
    breakpoint already hit 1 time



    FAMOUS LOCK
    2018-06-22 04:29:10 POTENTIAL DEADLOCK DETECTED
    2018-06-22 04:29:10 Previous lock order was:
    2018-06-22 04:29:10  (1) cs  masternodeman.cpp:1596
    2018-06-22 04:29:10  (2) cs_main  masternode.cpp:879
    2018-06-22 04:29:10 Current lock order is:
    2018-06-22 04:29:10  pnode->cs_vRecvMsg  net.cpp:1748 (TRY)
    2018-06-22 04:29:10  (2) cs_main  masternodeman.cpp:1625
    2018-06-22 04:29:10  (1) cs  masternodeman.cpp:1625


    MASTERNODE CONF
    mn1 206.189.118.206:12345 92VV6kUBw9VsDUHof98phCU7LrZk6gq7g6c2GtW2XW3CSzECMLc 0432440cecc8c92c092a43da0d8f104829f8766e93f2e7d4c86549d119d1d6f7 1

➜  anon git:(mn-002) ./src/btcp-cli -testnet masternode start-alias mn1
{
  "alias": "mn1",
  "result": "failed",
  "errorMessage": "Could not allocate txin 0432440cecc8c92c092a43da0d8f104829f8766e93f2e7d4c86549d119d1d6f7:1 for masternode 206.189.118.206:12345"
}


bitcoin@btc-testnet:~/bitcoin-private/anon/anon/src$ ./btcp-cli -testnet masternode status
{
  "vin": "CTxIn(COutPoint(e76dc4cc44, 0), scriptSig=)",
  "service": "206.189.118.206:12345",
  "payee": "n1cCTj1YxK4Xqejk49qQ9BuJJ2B8tR3Fwde",
  "status": "Masternode successfully started"
}
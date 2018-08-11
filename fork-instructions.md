# Instructions to set up an ANON full-node to Mine the Fork
Linux ONLY, 500GB+ recommended

*You will not receive a mining reward if you choose to help mine the fork*

**Anonymous Bitcoin**

March 2, 2018

#

**Install Anonymous Bitcoin**

Follow the instructions [here](https://github.com/anonymousbitcoin/anon).

**Download + Decompress Snapshot Data (ANON UTXOs)**
```
cd ~/.anon/
curl https://s3.amazonaws.com/anon.snapshot/utxo_snapshot.tar.gz | tar xvz
```

You can also generate the utxo_snapshot yourself [here](https://github.com/anonymousbitcoin/UTXO-DUMP).

**Make the config file**
```
mkdir ~/.anon
touch ~/.anon/anon.conf
```

**Run the daemon**
```
cd ~/anon
./src/anond
```

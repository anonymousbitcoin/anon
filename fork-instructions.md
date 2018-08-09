
# Instructions to set up a BitcoinPrivate full-node to Mine the Fork
Linux ONLY, 500GB+ recommended

*You will not receive a mining reward if you choose to help mine the fork*

**Anonymous Bitcoin**

March 2, 2018

#

**Install Anonymous Bitcoin**

Follow the instructions [here]([INSERT_GITHUB_REPO_URL]).

**Download + Decompress Snapshot Data (ANON UTXOs)**
```
cd ~/.anon/
curl https://s3.amazonaws.com/anon.snapshot/utxo_snapshot.tar.gz | tar xvz
```

You can also generate the utxo_snapshot yourself [here]([INSERT_GITHUB_REPO_URL]).

**Make the config file**
```
mkdir ~/.anon
touch ~/.anon/anon.conf
```

**Run the daemon**
```
cd ~/BitcoinPrivate
./src/anond
```

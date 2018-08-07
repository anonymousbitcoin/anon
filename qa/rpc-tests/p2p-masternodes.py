#!/usr/bin/env python3
# Copyright (c) 2018 The Dash Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

# from test_framework.mininode import *
from test_framework.test_framework import BitcoinTestFramework
from test_framework.util import *
from time import *
import os
import shutil

'''
Masternodes Test -- test InstantSend functionality (prevent doublespend for unconfirmed transactions)
'''

MASTERNODE_COLLATERAL = 100


class MasternodeInfo:
    def __init__(self, key, collateral_id, collateral_out):
        self.key = key
        self.collateral_id = collateral_id
        self.collateral_out = collateral_out


class MasternodesTest(BitcoinTestFramework):
    def __init__(self):
        super().__init__()
        # self.mn_count = 10
        # self.num_nodes = self.mn_count + 4
        # self.mninfo = []
        # self.setup_clean_chain = True
        # self.is_network_split = False
        # set sender,  receiver,  isolated nodes
        # self.isolated_idx = self.num_nodes - 1
        # self.receiver_idx = self.num_nodes - 2
        # self.sender_idx = self.num_nodes - 3

    def setup_network(self):
        self.nodes = []
        # self.is_network_split = False 
        self.nodes.append(start_node(0, self.options.tmpdir, ["-debug"]))
        self.nodes.append(start_node(1, self.options.tmpdir, ["-debug"]))
        self.nodes.append(start_node(2, self.options.tmpdir, ["-debug"]))

    def get_mnconf_file(self):
        return os.path.join(self.options.tmpdir, "node0/regtest/masternode.conf")
        

    def run_test(self):
        print('test is running')
        assert_equal(self.nodes[0].getbalance(), 50)


if __name__ == '__main__':
    MasternodesTest().main()
